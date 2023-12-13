#include "threadpool.h"

static tp_job_t *tp_job_create(tp_func_t f, void *arg) {
    tp_job_t *job;

    if (!f) {
        return NULL;
    }

    job = (tp_job_t *)malloc(sizeof(*job));
    if (!job) {
        return NULL;
    }
    job->f = f;
    job->arg = arg;
    job->next = NULL;
    return job;
}

int tp_job_add(tp_main_t *m, tp_func_t f, void *arg) {
    tp_job_t *job;

    if (!m) {
        return -1;
    }

    job = tp_job_create(f, arg);
    if (!job) {
        return -1;
    }

    Pthread_mutex_lock(&m->job_mutex);
#ifdef DEBUG
    dprintf(STDERR_FILENO, "main thread added a job\n");
#endif

    if (!m->head) {
        m->head = job;
    } else {
        m->last->next = job;
    }
    m->last = job;

    // signaling to all threads about the appearance of the task
    Pthread_cond_broadcast(&m->job_cond);
    Pthread_mutex_unlock(&m->job_mutex);
    return 0;
}

// only in crit. section
tp_job_t *tp_job_get(tp_main_t *m) {
    tp_job_t *job;

    if (!m || !m->head) {
        return NULL;
    }

    job = m->head;
    m->head = m->head->next;
    if (!m->head) {
        m->last = NULL;
    }
    return job;
}

int tp_create(tp_main_t *m, int num_threads) {
    int       ret;
    pthread_t thread;

    if (!m || num_threads <= 0) {
        return -1;
    }

    memset(m, 0, sizeof(*m));
    m->num_threads = num_threads;
    Pthread_mutex_init(&m->job_mutex);
    Pthread_cond_init(&m->job_cond);
    Pthread_cond_init(&m->wait_cond);

    for (int i = 0; i < num_threads; i++) {
        ret = pthread_create(&thread, NULL, tp_worker, m);
        if (ret != 0) {
            return -1;
        }

        ret = pthread_detach(thread);
        if (ret != 0) {
            return -1;
        }

#ifdef DEBUG
        dprintf(STDERR_FILENO, "thread number [%d] is created\n", i);
#endif
    }
    return 0;
}

void tp_wait(tp_main_t *m) {
    if (!m) {
        return;
    }

    Pthread_mutex_lock(&(m->job_mutex));
#ifdef DEBUG
    dprintf(STDERR_FILENO, "main thread in tp_wait()\n");
#endif

    while ((m->stop == false && (m->head != NULL || m->num_working_job != 0)) ||
           (m->stop == true && m->num_threads != 0)) {
        Pthread_cond_wait(&(m->wait_cond), &(m->job_mutex));
    }
#ifdef DEBUG
    dprintf(STDERR_FILENO, "main thread leave tp_wait()");
#endif

    Pthread_mutex_unlock(&(m->job_mutex));
}

void tp_destroy(tp_main_t *m) {
    tp_job_t *job, *tmp;

    if (!m) {
        return;
    }

    Pthread_mutex_lock(&(m->job_mutex));
#ifdef DEBUG
    dprintf(STDERR_FILENO, "main thread in tp_destroy()");
#endif

    job = m->head;
    while (job != NULL) {
        tmp = job->next;
        free(job);
        job = tmp;
    }

    // set condition variable to terminate
    m->stop = true;

    // notify the threads to wake up and exit/out of the loop
    Pthread_cond_broadcast(&(m->job_cond));
    Pthread_mutex_unlock(&(m->job_mutex));

    // wait for all threads to finish
    tp_wait(m);

    pthread_mutex_destroy(&(m->job_mutex));
    pthread_cond_destroy(&(m->job_cond));
    pthread_cond_destroy(&(m->wait_cond));
}

void *tp_worker(void *tp_main) {
    tp_main_t *m;
    tp_job_t * job;

#ifdef DEBUG
    pthread_t id = pthread_self();
    size_t    processed = 0;
#endif

    if (!tp_main) {
        return NULL;
    }
    m = (tp_main_t *)tp_main;

    while (1) {
        Pthread_mutex_lock(&(m->job_mutex));

#ifdef DEBUG
        dprintf(STDERR_FILENO, "[%lu] is ready\n", id);
#endif

        while (m->head == NULL && m->stop == false) {
            Pthread_cond_wait(&(m->job_cond), &(m->job_mutex));
        }

#ifdef DEBUG
        dprintf(STDERR_FILENO, "[%lu] started doing job\n", id);
#endif

        if (m->stop == true) {
            break;
        }

        job = tp_job_get(m);

        // be sure to increment the counter until it returns the lock,
        // so other threads will know that someone is working on a task
        m->num_working_job++;

        Pthread_mutex_unlock(&(m->job_mutex));

        if (job) {
            job->f(job->arg);
            free(job);
#ifdef DEBUG
            processed++;
#endif
        }

        Pthread_mutex_lock(&(m->job_mutex));

        m->num_working_job--;

        if (m->stop == false && m->num_working_job == 0 && m->head == NULL) {
            Pthread_cond_signal(&(m->wait_cond));
#ifdef DEBUG
            dprintf(STDERR_FILENO, "[%lu] signalize to main thread\n", id);
#endif
        }

        Pthread_mutex_unlock(&(m->job_mutex));
    }

#ifdef DEBUG
    dprintf(STDERR_FILENO, "[%lu] exiting, did jobs: [%lu]\n", id, processed);
#endif
    m->num_threads--;
    Pthread_cond_signal(&(m->wait_cond));
    Pthread_mutex_unlock(&(m->job_mutex));
    return NULL;
}

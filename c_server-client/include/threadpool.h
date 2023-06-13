#ifndef __threadpool_h__
#define __threadpool_h__

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// #define Pthread_create(thread, attr, start_routine, arg) assert(pthread_create(thread, attr,
// start_routine, arg) == 0); #define Pthread_detach(thread)         assert(pthread_detach(thread)
// == 0);
#define Pthread_cond_init(cond)        assert(pthread_cond_init(cond, NULL) == 0);
#define Pthread_mutex_init(m)          assert(pthread_mutex_init(m, NULL) == 0);
#define Pthread_mutex_lock(m)          assert(pthread_mutex_lock(m) == 0);
#define Pthread_mutex_unlock(m)        assert(pthread_mutex_unlock(m) == 0);
#define Pthread_cond_signal(cond)      assert(pthread_cond_signal(cond) == 0);
#define Pthread_cond_broadcast(cond)   assert(pthread_cond_broadcast(cond) == 0);
#define Pthread_cond_wait(cond, mutex) assert(pthread_cond_wait(cond, mutex) == 0);

typedef void *(*tp_func_t)(void *);

// задача
typedef struct _tp_job_t {
    tp_func_t         f;   // указатель на функцию
    void *            arg; // аргумент для функции
    struct _tp_job_t *next;
} tp_job_t;

// управляющая структура
typedef struct _tp_main_t {
    tp_job_t *head; // список задач
    tp_job_t *last; // список задач
    bool      stop;
    size_t    num_threads;     // количество работающих потоков
    size_t    num_working_job; /* когда поток берет задачу на исполнение - увеличивает эту переменную,
                             когда прекращает обработку - уменьшает соответственно
                             переменная нужна, чтобы понять, что кто-то работает над задачей */
    pthread_mutex_t job_mutex;
    pthread_cond_t  job_cond; // условная переменная для сигнала потоков для обработки задач
    pthread_cond_t  wait_cond; // уславная переменная для сигнала в главный поток
} tp_main_t;

int       tp_create(tp_main_t *m, int num_threads);
void      tp_destroy(tp_main_t *m);
void *    tp_worker(void *tp_main);
int       tp_job_add(tp_main_t *m, tp_func_t f, void *arg);
void      tp_wait(tp_main_t *m);
tp_job_t *tp_job_get(tp_main_t *m);

#endif // __threadpool_h__
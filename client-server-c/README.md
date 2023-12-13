# c_server-client
Simple server-client console app written in C capable of handling multiple clients using epoll and threadpool.

minuses:
- architecture sucks
- accepting new clients can be moved to another process or thread
- there is no limit on the number of tasks in the threadpool
- the result of the task completion is not processed
- there is no time limit on the execution of a task by a thread

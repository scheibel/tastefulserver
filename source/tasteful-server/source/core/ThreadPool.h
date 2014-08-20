#pragma once

#include <QList>

namespace tastefulserver {

class Task;
class TaskThread;

class ThreadPool
{
public:
    ThreadPool(int numThreads = -1);
    ~ThreadPool();

    void setNumThreads(int numThreads);
    bool isStarted() const;
    void start();
    void stop();

    void addTask(Task * task);

protected:
    bool m_started;
    int m_threadCount;
    unsigned m_next;
    QList<TaskThread *> m_threads;
};

} // namespace tastefulserver

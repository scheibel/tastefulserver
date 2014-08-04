#include "ThreadPool.h"

#include <tastefulserver/Task.h>

namespace tastefulserver {

ThreadPool::ThreadPool(int numThreads)
    : m_started(false)
    , m_threadCount(0)
    , m_next(0)
{
    setNumThreads(numThreads);
}

void ThreadPool::setNumThreads(int numThreads)
{
    if (m_started)
    {
        return;
    }

    if (numThreads<=0)
    {
        numThreads = QThread::idealThreadCount();
        if (numThreads<0)
        {
            numThreads = 1;
        }
    }
    m_threadCount = numThreads;
}

bool ThreadPool::isStarted() const
{
    return m_started;
}

void ThreadPool::start()
{
    if (m_started)
    {
        return;
    }

    for (int i = 0;i<m_threadCount;++i)
    {
        TaskThread * thread = new TaskThread();
        m_threads << thread;
        thread->start();
    }
    m_started = true;
}

void ThreadPool::stop()
{
    if (m_started)
    {
        return;
    }

    for (TaskThread * thread : m_threads)
    {
        thread->terminate();
        delete thread;
    }
    m_started = false;
}

ThreadPool::~ThreadPool()
{
    if (m_started)
    {
        stop();
    }
}

void ThreadPool::addTask(Task * task)
{
    m_threads[m_next]->addTask(task);
    m_next = (m_next + 1) % m_threadCount;
}

} // namespace tastefulserver

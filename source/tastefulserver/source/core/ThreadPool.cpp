/**
 * (C) LGPL-3
 *
 * Tasteful Server <https://github.com/scheibel/tasteful-server>
 *
 * Copyright: 2012-2014 Lux, Scheibel
 * Authors:
 *     Roland Lux <rollux2000@googlemail.com>
 *     Willy Scheibel <willyscheibel@gmx.de>
 *
 * This file is part of Tasteful Server.
 *
 * Tasteful Server is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tasteful Server is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Tasteful Server.  If not, see <http://www.gnu.org/licenses/>.
 **/

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

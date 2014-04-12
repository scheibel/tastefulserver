/**
  * (C) LGPL-3
  *
  * Tasteful Server <https://github.com/scheibel/tasteful-server>
  *
  * Copyright: 2012 Lux, Scheibel
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

ThreadPool::ThreadPool(int numThreads) : next(0), started(false) {
	setNumThreads(numThreads);
}

void ThreadPool::setNumThreads(int numThreads) {
	if (started) return;

	if (numThreads<=0) {
		numThreads = QThread::idealThreadCount();
		if (numThreads<0) numThreads=1;
	}
	threadCount = numThreads;
}

bool ThreadPool::isStarted() const {
	return started;
}

void ThreadPool::start() {
	if (started) return;

	for (int i=0; i<threadCount; ++i) {
		TaskThread* thread = new TaskThread();
		threads << thread;
		thread->start();
	}
	started = true;
}

void ThreadPool::stop() {
	if (started) return;

	for (TaskThread* thread: threads) {
		thread->terminate();
		delete thread;
	}
	started = false;
}


ThreadPool::~ThreadPool() {
	if (started) stop();
}

void ThreadPool::addTask(Task* task) {
	threads[next]->addTask(task);
	next = (next+1)%threadCount;
}

} // namespace tastefulserver

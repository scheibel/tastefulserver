#include <internal/ThreadPool>

#include <QDebug>

using namespace internal;

void Task::startUp() {
}

void Task::finish() {
	emit(finished(this));
}


void TaskThread::addTask(Task* task) {
	task->moveToThread(this);
	tasks.insert(task);
	startTask(task);
}

void TaskThread::endTask(Task* task) {
	tasks.remove(task);
	task->deleteLater();
}

void TaskThread::startTask(Task* task) {
	connect(task, SIGNAL(finished(Task*)), this, SLOT(endTask(Task*)));
	QMetaObject::invokeMethod(task, "startUp", Qt::QueuedConnection);
}


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

bool ThreadPool::isStarted() {
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

ThreadPool::~ThreadPool() {
	for (TaskThread* thread: threads) {
		thread->terminate();
		delete thread;
	}
}

void ThreadPool::addTask(Task* task) {
	threads[next]->addTask(task);
	next = (next+1)%threadCount;
}
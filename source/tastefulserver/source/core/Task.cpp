#include <tastefulserver/Task.h>

namespace tastefulserver {

void Task::startUp()
{
}

void Task::finish()
{
    emit(finished(this));
}

void TaskThread::addTask(Task * task)
{
    task->moveToThread(this);
    m_tasks.insert(task);
    startTask(task);
}

void TaskThread::endTask(Task * task)
{
    m_tasks.remove(task);
    task->deleteLater();
}

void TaskThread::startTask(Task * task)
{
    connect(task, &Task::finished, this, &TaskThread::endTask);
    QMetaObject::invokeMethod(task, "startUp", Qt::QueuedConnection);
}

} // namespace tastefulserver

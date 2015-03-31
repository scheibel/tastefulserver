#pragma once

#include <QObject>
#include <QThread>
#include <QSet>

#include <tastefulserver/tastefulserver_api.h>

namespace tastefulserver {

class TASTEFULSERVER_API Task : public QObject
{
    Q_OBJECT

public slots:
    virtual void startUp();

signals:
    void finished(Task * task);

protected:
    void finish();
};

class TASTEFULSERVER_API TaskThread : public QThread
{
    Q_OBJECT

public:
    void addTask(Task * task);

private slots:
    void endTask(Task * task);

private:
    void startTask(Task * task);

protected:
    QSet<Task *> m_tasks;
};

} // namespace tastefulserver

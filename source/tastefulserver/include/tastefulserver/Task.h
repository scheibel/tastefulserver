#pragma once

#include <QObject>
#include <QThread>
#include <QSet>

#include <tastefulserver/tasteful-server_api.h>

namespace tastefulserver {

class TASTEFUL_SERVER_API Task : public QObject
{
    Q_OBJECT

public slots:
    virtual void startUp();

signals:
    void finished(Task * task);

protected:
    void finish();
};

class TASTEFUL_SERVER_API TaskThread : public QThread
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

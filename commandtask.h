#ifndef WASTETIMETASK_H
#define WASTETIMETASK_H

#include <QRunnable>
#include <QObject>

class CommandTask : public QRunnable
{
public:
    explicit CommandTask(
        QObject *receiver_,
        volatile bool *stopped_,
        const QString &cmd_) : receiver(receiver_), stopped(stopped_), cmd(cmd_)  {}

private:
    QObject *receiver;
    volatile bool *stopped;
    const QString cmd;
    void run();
};

#endif // WASTETIMETASK_H

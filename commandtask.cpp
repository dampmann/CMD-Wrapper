#include "commandtask.h"
#include <QThread>
#include <QMetaObject>
#include "pstreambuf.h"

void CommandTask::run() {
    if(*stopped) return;
    pstreambuf b {cmd.toStdString().data()};
    std::string line;
    std::istream is(&b);
    while(std::getline(is, line)) {
      if(*stopped) return;
      QString message = QObject::tr("%1").arg(line.c_str());
      QMetaObject::invokeMethod(
                  receiver,
                  "updateProgress",
                  Qt::QueuedConnection,
                  Q_ARG(QString, message)
      );
    }

    QMetaObject::invokeMethod(
       receiver,
       "is_done",
       Qt::QueuedConnection
    );
}

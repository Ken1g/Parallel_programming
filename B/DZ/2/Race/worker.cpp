#include "worker.h"
#include <QDebug>

#include <iostream>

Worker::Worker(QObject *parent)
    : QObject(parent)
    , Id(-1)
{
}

void Worker::Handle(const int exciterId) {
    std::cout << QString("id:%1 handled signal from %2\n").arg(Id).arg(exciterId).toStdString() << std::endl;

    emit Excite(Id);
}

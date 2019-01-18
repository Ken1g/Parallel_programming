#include <iostream>
#include <QThread>
#include <QCoreApplication>
#include <cstdlib>

#include <QObject>
#include <QDebug>

class Worker : public QObject {
    Q_OBJECT
public:
    expilicit Worker(QObject *parent = nullptr);
    inline void SetID(const int id);
signals:
    void Excite(const int myID);
public slots:
    void Handle(const int exciterId);
private:
    int Id;
};

void Worker::SetId(const int id) {
    Id = id;
}

Worker::Worker(QObject *parent)
    : QObject(parent)
    , Id(-1)
{
}

void Worker::Handle(const int exciterId) {
    std::cout << QString("id:%1 handled signal from %2\n").arg(Id).arg(exciterId).toStdString() << std::endl;

    emit Excite(Id);
}



int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    if (argc != 2)
        return -1;

    const int workersCount = atoi(argv[1]);
    QThread* threads = new QThread[workersCount];
    Worker* workers = new Worker[workersCount];

    for (int i = 0; i < workersCount; ++i) {
        workers[i].SetId(i + 1);
        if (i != workersCOunt - 1) {
            QObject::connect(
                &workers[i], 
                &Worker::Excite,
                &workers[i + 1],
                &Worker::Handle
            );
        }
            workers[i].moveToThread(&threads[i]);
            threads[i].start();
    }
    workers[0].Handle(0);

    for (int i = 0; i < workersCount; ++i) {
        threads[i].wait();
        threads[i].exit();
    }

    std::cout << "LOL" << endl << workersCount;

    return 0;
}


#include <iostream>
#include <QThread>
#include <QCoreApplication>
#include <cstdlib>

#include <QObject>

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


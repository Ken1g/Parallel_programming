#include <QCoreApplication>
#include "worker.h"
#include <iostream>
#include <QThread>
#include <cstdlib>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << "Let the Race begin";

    const int num_of_threads = atoi(argv[1]);
    QThread* threads = new QThread[num_of_threads];
    Worker* workers = new Worker[num_of_threads];

    for (int i = 0; i < num_of_threads; ++i) {
        workers[i].SetId(i + 1);
        if (i != num_of_threads - 1) {
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

    for (int i = 0; i < num_of_threads; ++i) {
        threads[i].wait();
        threads[i].exit();
    }

    std::cout << "LOL" << std::endl << num_of_threads;

    return 0;
}

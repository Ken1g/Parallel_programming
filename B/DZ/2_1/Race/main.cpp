#include <QCoreApplication>
#include <QThread>
#include <QObject>

#include <iostream>
#include <cstdlib>
#include "user.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    const int number_of_users = atoi(argv[1]);

    QThread* threads = new QThread[number_of_users];
    User* users = new User[number_of_users];

    for (int i = 0; i < number_of_users; ++i) {
        users[i].SetId(i + 1);
        if (i != number_of_users - 1) {
            QObject::connect(
                &users[i],
                &User::Send,
                &users[i + 1],
                &User::Recv
            );
        }
        users[i].moveToThread(&threads[i]);
        threads[i].start();
    }
    std::cout << "Let the Race begin";
    users[0].Recv(0);

    for (int i = 0; i < number_of_users; ++i) {
        threads[i].wait();
        threads[i].exit();
    }

    return 0;
}

#include "user.h"
#include <iostream>

User::User(QObject *parent)
    : QObject(parent)
    , Id(-1)
{
}

void User::Recv(const int id) {
    std::cout << id;
    emit Send(Id);
}

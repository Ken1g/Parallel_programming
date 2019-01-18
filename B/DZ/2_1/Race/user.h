#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject {
    Q_OBJECT
public:
    inline void SetId(const int id);
    explicit User(QObject *parent = nullptr);
signals:
    void Send(const int id);
public slots:
    void Recv(const int id);
private:
    int Id;
};

void User::SetId(const int id) {
    Id = id;
}


#endif // USER_H

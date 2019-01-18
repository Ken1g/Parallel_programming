#ifndef THREAD_H
#define THREAD_H

#include <QObject>

class Thread : public QObject
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = nullptr);

    inline void SetId(const int id);
signals:
    void Excite(const int myId);
public slots:
    void Handle(const int exciterId);

private:
    int Id;
};

void Thread::SetId(const int id)
{
    Id = id;
}

#endif // THREAD_H

#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QCoreApplication>
#include <QTcpSocket>
#include <QHostInfo>
#include <QString>
#include <QIODevice>
#include <iostream>
#include <QTextStream>
#include <QThread>
//класс, реализующий клиент

class Client : public QTcpSocket
{

Q_OBJECT

public:

    Client(QString log);

    void send(); // отправить сообщение

    void Recieve(); // прием сообщения

    void abort(); // завершить прием

    QString login;

    QTcpSocket* GetSocket();

private:

    QTcpSocket* qts; // экземпляр сокета

private slots:

    void connecto();

};

class ThreadSender : public QThread
{
public:

    ThreadSender(Client *c);

    ~ThreadSender();

    void run();

    Client *cl;

};

class ThreadReciever : public QThread
{

Q_OBJECT

public:

    ThreadReciever(Client *c);

    ~ThreadReciever();

    void run();

    Client *cl;

private slots:

    void RecieveSlot();

};

#endif // CHATCLIENT_H

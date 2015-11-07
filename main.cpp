#include <QCoreApplication>
#include <QTcpSocket>
#include <QHostInfo>
#include <QString>
#include <QIODevice>
#include <QByteArray>
#include <QTextStream>
#include <QTextCodec>
#include <QAbstractSocket>
#include <QException>
#include <QChar>
#include "chatclient.h"
#include <iostream>

using namespace std;

//конструктор
Client::Client(QString log)
{
    this->login = log;

    QTextStream cout(stdout);

    const QString addr = "31.170.164.127";

    qts = new QTcpSocket(this); //создаем tcp сокет    

    connect(qts, SIGNAL(connected()),SLOT(connecto()));

    qts->connectToHost(addr, 80, QIODevice::ReadWrite); //соединяемся с сервером

}
//слот
void Client::connecto()
{
    QTextStream cout(stdout);

    cout << "Connected with server..." << endl;

    ThreadSender ts(this);

    ts.start();

    ThreadReciever tr(this);

    tr.start();

}
//аксессор сокета
QTcpSocket* Client::GetSocket()
{

    return this->qts;

}

//отправка сообщения
void Client::send()
{

    QString mes = "";

    QTextStream cout(stdout);

    QTextStream cin(stdin);

    cout << this->login << ": " << endl;

    mes = cin.readLine();

    const QChar* qc = mes.constData();

    char c = qc->toLatin1();

    const char* message = &c;

    cout << endl;

    this->qts->write(message);

    cout << "+----------------------------------------+" << endl;

}
//метод приема и печати сообщения

void Client::Recieve()
{

    QTextStream cout(stdout);

    QByteArray mes_b = this->qts->readAll();

    const char* m = mes_b.constData();

    QString message(m);

    cout << message << endl;

    cout << "+----------------------------------------+" << endl;

}

//поток отправки
void ThreadSender::run()
{

    while(true)
    {

        cl->send();

    }

}

ThreadReciever::ThreadReciever(Client *c)
{

    this->cl = c;

    connect(this->cl->GetSocket(), SIGNAL(readyRead()),SLOT(RecieveSlot()));

}

void ThreadReciever::RecieveSlot()
{

    this->start();

}

//поток приема
void ThreadReciever::run()
{

    cl->Recieve();

}

ThreadSender::ThreadSender(Client* c)
{

    this->cl = c;

}

ThreadReciever::~ThreadReciever() {

    while(true)
    {

        this->wait(10000);

    }

}

ThreadSender::~ThreadSender() {

    while(true)
    {

        this->wait(10000);

    }

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream cout(stdout);

    QTextStream cin(stdin);

    QString login = "";

    cout << "Insert login:" << endl;

    login = cin.readLine();

    Client c(login);

    return a.exec();
}

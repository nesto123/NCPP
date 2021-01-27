#include "server.h"
#include "ui_server.h"

#include <QAuthenticator>
#include <QNetworkProxy>
#include <QTcpServer>
#include <QTcpSocket>
#include <algorithm>

Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server), m_server(new QTcpServer(this)), noConnections(0)
{
    ui->setupUi(this);

    // QHostAddress::LocalHost, QHostAddress::Any
    if( !m_server->listen(QHostAddress("127.0.0.1"), 9090) )
    {
        ui->log->appendPlainText(m_server->errorString());
        ui->log->appendPlainText(tr("Server error = %1").arg(m_server->serverError()));
        qDebug() <<m_server->errorString() << "???";
        return;
    }
    connect(m_server, &QTcpServer::newConnection, this, &Server::onNewConnection );
    connect(ui->disconnectPushButton, &QPushButton::clicked, this, &Server::onDisconnectPushed);
    setWindowTitle("TcpServer");
    ui->disconnectPushButton->setEnabled(false);
}

Server::~Server()
{
    delete ui;
}

void Server::onNewConnection()
{
    while(m_server->hasPendingConnections()){
        QTcpSocket * socket = m_server->nextPendingConnection();
        m_sockets.push_back(socket);
        ui->log->appendPlainText(QString("Konekcija sa %1, port %2").arg(socket->peerAddress().toString(),
                                                                 QString::number( socket->peerPort() )));

        const char ETB = 23; // End-of-transsmision-block
        const char AuthReq = 22; // Auth required
        QString message = "username";

        QByteArray poruka = QString("%1").arg(message).toUtf8();
        poruka.append(AuthReq);
        poruka.append(ETB);
        socket->write(poruka);

        connect(socket, &QTcpSocket::readyRead, this, [this, socket](){ onReadyRead(socket);} );
        connect(socket, &QTcpSocket::disconnected, this, [this, socket](){ onDisconnected(socket);} );

        noConnections +=1;
        ui->disconnectPushButton->setEnabled(true);

    }
}


void Server::onReadyRead(QTcpSocket * socket)
{
    const char ETB = 23; // End-of-transsmision-block
    const char AuthReq = 22; // Auth required

    if( !socket )
        return;
    QByteArray & buffer = m_data[socket];
    buffer.append(socket->readAll());

    while(true){
        auto ETB_pos = buffer.indexOf(ETB);
        auto AuthReq_pos = buffer.indexOf(AuthReq);

        if(ETB_pos < 0) //  poruka nije gotova
            break;

        QString message;
        if(AuthReq_pos >= 0)
        {
            message = QString::fromUtf8( buffer.left(AuthReq_pos) );
            socket->setObjectName(message);
            buffer.remove(0, ETB_pos + 1);
            continue;
        }
        else {
            message = QString::fromUtf8( buffer.left(ETB_pos) );
        }

        buffer.remove(0,ETB_pos+1);
        ui->log->appendPlainText(tr("%1 : %2").arg(socket->objectName(),message));
        QByteArray poruka;
        for( QTcpSocket * klijent : m_sockets){
            if( klijent != socket && klijent->state() == QAbstractSocket::ConnectedState){
                poruka = (socket->objectName() + " : " + message).toUtf8();
                poruka.append(ETB);
                klijent->write(poruka);
            }
        }
    }
}

void Server::onDisconnected(QTcpSocket * socket)
{
    if( !socket )
        return;
    auto it = std::find(m_sockets.begin(),m_sockets.end(), socket);

    if(it != m_sockets.end() )
        m_sockets.erase(it);
    auto it_d = m_data.find(socket);
    if(it_d != m_data.end() )
        m_data.erase(it_d);
    ui->log->appendPlainText("Socket disconnected");
    socket->deleteLater();

    noConnections -= 1;
    if(noConnections == 0 )
        ui->disconnectPushButton->setEnabled(false);
}

void Server::onDisconnectPushed()
{
    foreach (auto item , m_sockets)
        item->close();
}


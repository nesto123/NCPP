#include "client.h"
#include "ui_client.h"

#include <QHostAddress>
#include <QIntValidator>

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client), m_socket(new QTcpSocket(this))
{

    ui->setupUi(this);

    connect(ui->connect, &QPushButton::clicked, this, &Client::onConnectPushed);
    connect(ui->disconnect, &QPushButton::clicked, this, &Client::onDisconnectPushed);

    connect(m_socket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &Client::onErrorOccured);
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);

    connect(ui->text, &QLineEdit::returnPressed, this, &Client::onReturnPressed);

    setWindowTitle("TcpClient");
    // makunt
//    ui->address->insert(tr("127.0.0.1"));
//    ui->port->setValue(9090);
    //

    ui->disconnect->setEnabled(false);
}

Client::~Client()
{
    delete ui;
}

void Client::onConnectPushed()
{
    if( m_socket->state() != QAbstractSocket::ConnectedState )
    {
        //m_socket->connectToHost(QHostAddress::LocalHost, 9090);
        m_socket->connectToHost(QHostAddress(ui->address->text()), ui->port->text().toInt());

        ui->log->append("Connecting...");
        m_name = ui->name->text().trimmed();

        // buttons
        ui->connect->setEnabled(false);
        ui->disconnect->setEnabled(true);
        ui->name->setEnabled(false);
        ui->address->setEnabled(false);
        ui->port->setEnabled(false);
    }
}

void Client::onDisconnected()
{
    ui->log->append("Disconnected.");

    //  Buttons
    ui->connect->setEnabled(true);
    ui->disconnect->setEnabled(false);
    ui->name->setEnabled(true);
    ui->address->setEnabled(true);
    ui->port->setEnabled(true);
}

void Client::onErrorOccured()
{
    ui->log->append(tr("Error: %1").arg(m_socket->errorString()));

    //  Buttons
    ui->connect->setEnabled(true);
    ui->disconnect->setEnabled(false);
    ui->name->setEnabled(true);
    ui->address->setEnabled(true);
    ui->port->setEnabled(true);
}

void Client::onReadyRead()
{
    const char ETB = 23; // End-of-transsmision-block
    const char AuthReq = 22; // Auth required

    if( !m_socket )
        return;
    m_data.append(m_socket->readAll());

    while(true){
        auto ETB_pos = m_data.indexOf(ETB);
        auto AuthReq_pos = m_data.indexOf(AuthReq);

        if(ETB_pos < 0 ) //  poruka nije gotova
            break;

        if(AuthReq_pos >= 0)
        {
            QString message2 = ui->name->text().trimmed();

            QByteArray poruka = QString("%1").arg(message2).toUtf8();
            poruka.append(AuthReq);
            poruka.append(ETB);
            m_socket->write(poruka);
            m_data.remove(0, ETB_pos + 1);
            continue;
        }

        QString message = QString::fromUtf8( m_data.left(ETB_pos) );

        m_data.remove(0,ETB_pos+1);
        ui->log->setTextColor(Qt::green);
        ui->log->append(tr("%1").arg(message));
        ui->log->setTextColor(Qt::black);

    }
}

void Client::onReturnPressed()//plava
{
    const char ETB = 23; // End-of-transsmision-block
    QString message = ui->text->text().trimmed();

    QByteArray poruka = QString("%1").arg(message).toUtf8();
    poruka.append(ETB);
    m_socket->write(poruka);
    ui->log->setTextColor(Qt::blue);
    ui->log->append(QString("Me : %1").arg(message));
    ui->log->setTextColor(Qt::black);

    ui->text->clear();
}

void Client::onDisconnectPushed()
{
    m_socket->close();
}

void Client::auth()
{
    ui->log->append("auth.");
}

void Client::onConnected()
{
    ui->log->append("Connected.");
}


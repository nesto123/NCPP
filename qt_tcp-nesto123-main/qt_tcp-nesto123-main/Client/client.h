#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QByteArray>
#include <QString>


QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();
public slots:
    void onConnectPushed();
    void onConnected();
    void onDisconnected();
    void onErrorOccured();
    void onReadyRead();
    void onReturnPressed();
    void onDisconnectPushed();
    void auth(/*QNetworkProxy *, QAuthenticator **/ );

private:
    Ui::Client *ui;
    QTcpSocket * m_socket;
    QByteArray m_data;
    QString m_name;
};
#endif // CLIENT_H

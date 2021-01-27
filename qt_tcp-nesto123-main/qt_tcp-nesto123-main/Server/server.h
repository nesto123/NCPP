#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QByteArray>

class QTcpServer;
class QTcpSocket;

#include <vector>
#include <unordered_map>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QWidget
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();

public slots:
    void onNewConnection();
    void onReadyRead(QTcpSocket *);
    void onDisconnected(QTcpSocket*);
    void onDisconnectPushed();

private:
    Ui::Server *ui;
    QTcpServer * m_server;
    std::vector<QTcpSocket * > m_sockets;
    std::unordered_map<QTcpSocket *, QByteArray> m_data;
    int noConnections;
};
#endif // SERVER_H

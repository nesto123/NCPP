#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QNetworkReply;
class QNetworkAccessManager;
class QAuthenticator;
class QProgressBar;
class QTemporaryFile;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void onFinished();
    void onReadyRead();
    void onDowloadClicked();
    void onAuthenticationRequired(QNetworkReply *, QAuthenticator * );
    void onDowloadProgress(qint64 bytesSent, qint64 bythesTotal);

private slots:
    void on_clearPushButton_clicked();

    void on_exitPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkReply * reply;
    QNetworkAccessManager * nam;
    QProgressBar * progress;
    QTemporaryFile * file;

    void makeRequest(const QUrl &url);
    void clearOutput();
};
#endif // MAINWINDOW_H

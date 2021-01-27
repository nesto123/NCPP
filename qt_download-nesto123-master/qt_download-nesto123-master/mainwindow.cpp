#include "authenticationdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAuthenticator>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QProgressBar>
#include <QTemporaryFile>
#include <QFileDialog>



#include <utility>
//https://doc.qt.io/qt-5/qstring.html

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), nam(new QNetworkAccessManager(this)),
      progress(new QProgressBar(this))
{
    ui->setupUi(this);
    ui->urllineEdit->setClearButtonEnabled(true);


    connect(ui->downloadPushButton, &QPushButton::clicked, this, &MainWindow::onDowloadClicked);
    connect(nam, &QNetworkAccessManager::authenticationRequired, this, &MainWindow::onAuthenticationRequired);

    progress->setMinimum(0);
    progress->setMaximum(100);
    progress->setValue(0);
    ui->statusbar->addWidget(progress);
    progress->setVisible(false);

    //ui->urllineEdit->setText("https://www.google.com");
    //ui->urllineEdit->setText("http://www.africau.edu/images/default/sample.pdf");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFinished()
{
    //qDebug() << "finish";
    progress->setVisible(false);

    if (reply->error() == QNetworkReply::NoError)
    {
        auto list = reply->rawHeaderPairs();
        for (auto item : list)
            ui->headerPlainTextEdit->textCursor().insertText(item.first + " : " + item.second + "\n");
    }
    else
    {
        ui->statusbar->showMessage(tr("HTTP status = %1").arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()));
        ui->headerPlainTextEdit->clear();
        ui->headerPlainTextEdit->textCursor().insertText(reply->errorString());
    }

    QVariant target = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    reply->deleteLater();

    ///////////////////////////////////7

    file->seek(0);
    if (reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("text", Qt::CaseInsensitive))
        ui->plainTextEdit->textCursor().insertText(file->readAll());
    else
    {
        // file
        QString saveasfile = QFileDialog::getSaveFileName(this, tr("Save as"), "");
        if (saveasfile == "")
        {
            qDebug() << "Canceled.";
        }
        else
        {
            QFileInfo check_file(saveasfile);
            if (check_file.exists() && check_file.isFile()) {            // ako file postoji vec
                QDir dir;
                dir.remove(saveasfile);
            }
            QFile::copy(file->fileName(), saveasfile);
            ui->plainTextEdit->appendPlainText("Saved to " + saveasfile + ".");
        }
    }
    file->remove();

    /// /////////////////////////////////////////

    if (target.isValid())
    {
        QUrl new_url = target.toUrl();
        auto button = QMessageBox::question(this, "Redirection",
                                            tr("Do you want to redirect to %1").arg(new_url.toString()));
        if (button == QMessageBox::No)
            return;
        makeRequest(new_url);
    }
}

void MainWindow::onReadyRead()
{
    //qDebug() << "read";

    //qDebug() <<"\n\n"<<reply->header( QNetworkRequest::ContentTypeHeader);
    file->write(reply->readAll());

    //qDebug() << file->fileName();
}

void MainWindow::onDowloadClicked()
{
    clearOutput();
    auto url_text = ui->urllineEdit->text().trimmed();
    //qDebug() << url_text;
    if (url_text.isEmpty())
        return;

    QUrl url(url_text);
    makeRequest(url);
}

void MainWindow::onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    AuthenticationDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        authenticator->setUser("dialog.getValues().first");
        authenticator->setPassword("dialog.getValues().second");
    }
}

void MainWindow::onDowloadProgress(qint64 bytesSent, qint64 bythesTotal)
{
    qreal progress_ = (bythesTotal < 1) ? 1.0 : static_cast<qreal>(bytesSent) / bythesTotal;
    progress->setValue(progress_ * progress->maximum());
}

void MainWindow::makeRequest(QUrl const &url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "myApplication");

    ///
    file = new QTemporaryFile("bok");

    file->open();
    ///
    reply = nam->get(request);

    connect(reply, &QNetworkReply::finished, this, &MainWindow::onFinished);
    connect(reply, &QNetworkReply::readyRead, this, &MainWindow::onReadyRead);
    connect(reply, &QNetworkReply::downloadProgress, this, &MainWindow::onDowloadProgress);

    progress->setValue(0);
    progress->setVisible(true);
}

void MainWindow::on_clearPushButton_clicked()
{
    clearOutput();
    ui->urllineEdit->clear();
}

void MainWindow::clearOutput()
{
    ui->headerPlainTextEdit->clear();
    ui->plainTextEdit->clear();
}

void MainWindow::on_exitPushButton_clicked()
{
    QApplication::quit();
}

#include "mainwindow.h"
// Vaš kod
#include <QLabel>
#include <QAction>
#include <QApplication>
#include <QPixmap>

#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

#include <QTableView>   // MVC pattern
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>

#include <QMessageBox>

void MainWindow::createIcons(){
    newIcon = QPixmap(":/Ikone/new.png");
}

void MainWindow::setupCoreWidgets()
{
    QWidget * widget = new QWidget();
    QVBoxLayout * vbLayout = new QVBoxLayout();
    QHBoxLayout * hbLayout = new QHBoxLayout();
    QFormLayout * formLayout = new QFormLayout();

    nameLineEdit = new QLineEdit();
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    formLayout->addRow("Name", nameLineEdit);
    formLayout->addRow("Password", passwordLineEdit);

    /*QTableView * tableView = new QTableView();
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QStandardItemModel * tableModel = new QStandardItemModel(1, 3, this);
    tableModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Ime i prezime")));
    tableModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Email")));
    tableModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Napomena")));

    QStandardItem * first = new QStandardItem(QString("John Doe"));
    QStandardItem * second = new QStandardItem(QString("johnDoe@math.hr"));
    QStandardItem * third = new QStandardItem(QString("Računarstvo"));

    tableModel->setItem(0, 0, first);
    tableModel->setItem(0, 1, second);
    tableModel->setItem(0, 2, third);

    tableView->setModel(tableModel);*/

    cancelButton = new QPushButton("Cancel");
    submitButton = new QPushButton("Submit");
    hbLayout->addStretch();
    hbLayout->addWidget(cancelButton);
    hbLayout->addWidget(submitButton);

    vbLayout->addLayout(formLayout);
    //vbLayout->addWidget(tableView);
    vbLayout->addLayout(hbLayout);

    widget->setLayout(vbLayout);
    setCentralWidget(widget);
}

void MainWindow::setupSignalsAndSlots()
{
    connect(cancelButton, &QPushButton::clicked, this, &QApplication::quit);
    connect(submitButton, &QPushButton::clicked, this, &MainWindow::submitButtonClicked);
}

void MainWindow::submitButtonClicked()
{
    QString name = nameLineEdit->text();
    QString password = passwordLineEdit->text();

    if(name ==  "test" && password == "test") {
        successDialog();
    }
    else{
        failureDialog();
        clearFields();
    }
}

void MainWindow::clearFields()
{
    nameLineEdit->clear();
    passwordLineEdit->clear();
}

void MainWindow::failureDialog(){
    /*QMessageBox::about(this,
                      "Failure",   // naslov
                      "Try again." // text
                      );*/
    QMessageBox failure;
    failure.setWindowTitle("Failure");
    failure.setText("Try again.");
    failure.setStandardButtons(QMessageBox::Ok);
    failure.setIconPixmap(newIcon);
    failure.exec();
}

void MainWindow::successDialog(){
    /*QMessageBox::about(this,
                      "Success",   // naslov
                      "You have successfully logged in." // text
                      );*/
    QMessageBox success;
    success.setWindowTitle("Success");
    success.setText("You have successfully logged in.");
    success.setStandardButtons(QMessageBox::Ok);
    success.setIconPixmap(newIcon);

    int res = success.exec();

    if( res == QMessageBox::Ok )
    {
        this->close();
        close();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("qt_login");
    //setWindowIcon(QIcon(":/Ikone/new.png"));
    //resize(400,200);

    createIcons();
    setupCoreWidgets();
    setupSignalsAndSlots();
}

MainWindow::~MainWindow()
{

}

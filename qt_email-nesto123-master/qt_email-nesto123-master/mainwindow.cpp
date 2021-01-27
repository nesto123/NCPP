#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QStandardItemModel>

///
#include <QDebug>
#include <QStatusTipEvent>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    fileName = "";
    ui->setupUi(this);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableModel = new QStandardItemModel(1, 3, this);
    tableModel->setHorizontalHeaderItem(
        0, new QStandardItem(QString("Ime i prezime")));
    tableModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Email")));
    tableModel->setHorizontalHeaderItem(2,
        new QStandardItem(QString("Napomena")));

    QStandardItem* first = new QStandardItem(QString("John Doe"));
    QStandardItem* second = new QStandardItem(QString("johnDoe@math.hr"));
    QStandardItem* third = new QStandardItem(QString("Računarstvo"));
    tableModel->setItem(0, 0, first);
    tableModel->setItem(0, 1, second);
    tableModel->setItem(0, 2, third);

    ui->tableView->setModel(tableModel);
    setupSignalsAndSlots();

    QRegularExpression rx("^[^,]+$"),
        rx2("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",
            QRegularExpression::CaseInsensitiveOption);
    ui->iipLineEdit->setValidator(
        new QRegularExpressionValidator(rx, this));
    ui->napomenaLineEdit->setValidator(
        new QRegularExpressionValidator(rx, this));
    ui->emailLineEdit->setValidator(
        new QRegularExpressionValidator(rx2, this));
}

void MainWindow::setupSignalsAndSlots()
{
    connect(ui->actionQuit, &QAction::triggered, &QApplication::quit);
    connect(ui->savePushButton, &QPushButton::clicked, this,
        &MainWindow::saveButtonClicked);

    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::aboutDialog);
    connect(ui->clearAllPushButton, &QPushButton::clicked, this,
        &MainWindow::clearAllRecords);
    connect(ui->actionDeleteAll, &QAction::triggered, this,
        &MainWindow::clearAllRecords);
    connect(ui->actionDeleteRecord, &QAction::triggered, this,
        &MainWindow::deleteSavedRecord);

    // validation for email
    connect(ui->emailLineEdit, &QLineEdit::textChanged, this,
        &MainWindow::adjustTextColor);
    // file
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);

    connect(this, SIGNAL(message(QString)), ui->statusbar, SLOT(showMessage(QString)));
//ui->statusbar->eventFilter()

}

void MainWindow::saveAs()
{
    QString saveasfile = QFileDialog::getSaveFileName(this, tr("Save as"), "",
        tr("Text files (*.txt)"));
    if (saveasfile == "") {
        qDebug() << "Canceled.";
    } else {
        if (!saveasfile.endsWith(".txt", Qt::CaseInsensitive))
            saveasfile += ".txt";
        QFile fileSave(saveasfile);
        if (!fileSave.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::information(this, tr("Unable to open file"),
                fileSave.errorString());
            return;
        }

        QTextStream stream2(&fileSave);
        for (int i = 0; i < tableModel->rowCount(); ++i) {
            stream2 << tableModel->data(tableModel->index(i, 0)).toString() << ","
                    << tableModel->data(tableModel->index(i, 1)).toString() << ","
                    << tableModel->data(tableModel->index(i, 2)).toString() << "\n";
        }
        //statusBar()->showMessage(saveasfile.split(QRegExp("[/]"))[saveasfile.split(QRegExp("[/]")).size() - 1]);
        emit message(saveasfile.split(QRegExp("[/]"))[saveasfile.split(QRegExp("[/]")).size() - 1]);
        fileSave.close();
    }
}
bool MainWindow::event(QEvent* e)
{
    if( dynamic_cast<QStatusTipEvent*>(e) != nullptr )
        return true;
    return QMainWindow::event(e);
}
void MainWindow::openFile()
{
    if (fileName != "" && compareWithFile() == true) // provjeri jel vec otvoren -- preusmjeri
    {
        auto button = QMessageBox::question(this, "Save changes",
            "Do you want to save changes?");
        if (button == QMessageBox::Yes)
            save();
    }

    QFile file1;

    fileName = QFileDialog::getOpenFileName(this, tr("OpenFile"), ".",
        tr("Text files (*.txt)"));
    if (fileName == "") {
        qDebug() << "Canceled.";
    } else {
        file1.setFileName(fileName);
        file1.open(QIODevice::ReadOnly);
        // qDebug()<<fileName;
        //statusBar()->showMessage(fileName.split(QRegExp("[/]"))[fileName.split(QRegExp("[/]")).size() - 1]);
        emit message(fileName.split(QRegExp("[/]"))[fileName.split(QRegExp("[/]")).size() - 1]);
        QTextStream stream(&file1);
        int i = 0;
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            // qDebug() << line;
            auto parts = line.split(QRegExp("[,]"));

            QStandardItem* first = new QStandardItem(parts[0]);
            QStandardItem* second = new QStandardItem(parts[1]);
            QStandardItem* third = new QStandardItem(parts[2]);
            tableModel->setItem(i, 0, first);
            tableModel->setItem(i, 1, second);
            tableModel->setItem(i++, 2, third);
        }
    }

    file1.close();
}

void MainWindow::save()
{
    if (fileName == "") {
        saveAs();
        return;
    }
    if (compareWithFile() == false)
        return;

    QFile file;
    file.setFileName(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "File 2: Error.";
    else
        qDebug() << "File 2: Opened.";
    QTextStream stream2(&file);
    // stream2 << "Upis podata " << 2 << "?" << 3.14 << "\n";
    for (int i = 0; i < tableModel->rowCount(); ++i) {
        stream2 << tableModel->data(tableModel->index(i, 0)).toString() << ","
                << tableModel->data(tableModel->index(i, 1)).toString() << ","
                << tableModel->data(tableModel->index(i, 2)).toString() << "\n";
    }
    file.close();
}

bool MainWindow::compareWithFile()
{ // ako se promjenila true
    if (fileName == "")
        return false;
    bool flag = false;

    QFile file;

    file.setFileName(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    int i = 0;
    while (!stream.atEnd()) {
        if (i >= tableModel->rowCount()) {
            flag = true;
            break;
        }
        QString line = stream.readLine();
        auto parts = line.split(QRegExp("[,]"));
        if (tableModel->data(tableModel->index(i, 0)).toString() != parts[0] || tableModel->data(tableModel->index(i, 1)).toString() != parts[1] || tableModel->data(tableModel->index(i, 2)).toString() != parts[2]) {
            flag = true;
            break;
        }
        ++i;
    }
    file.close();
    if (flag == false && i < tableModel->rowCount())
        flag = true;

    return flag;
}

void MainWindow::adjustTextColor()
{
    if (!ui->emailLineEdit->hasAcceptableInput())
        ui->emailLineEdit->setStyleSheet("QLineEdit { color: red;}");
    else
        ui->emailLineEdit->setStyleSheet("QLineEdit { color: black;}");
}

void MainWindow::clearFields()
{
    ui->iipLineEdit->clear();
    ui->emailLineEdit->clear();
    ui->napomenaLineEdit->clear();
}

void MainWindow::saveButtonClicked()
{
    if (!ui->emailLineEdit->hasAcceptableInput()) {
        QMessageBox::warning(this, tr("Email verification"),
            tr("Email format is incorrect."), QMessageBox::Ok);
        return;
    }
    QString name = ui->iipLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString napomena = ui->napomenaLineEdit->text();

    QStandardItem* it1 { new QStandardItem(name) };
    QStandardItem* it2 { new QStandardItem(email) };
    QStandardItem* it3 { new QStandardItem(napomena) };

    tableModel->appendRow({ it1, it2, it3 });

    clearFields();
}

void MainWindow::aboutDialog()
{
    QMessageBox::about(this, "About Application", "My app d.o.o.");
}

void MainWindow::clearAllRecords()
{
    auto button = QMessageBox::question(this, "Clear all",
        "Želite li baš sve obrisati????");
    if (button == QMessageBox::Yes)
        tableModel->removeRows(0, tableModel->rowCount());
}
// smije li se??
void MainWindow::deleteSavedRecord()
{
    bool ok = true;
    int row = QInputDialog::getInt(this, tr("Select row to delete"),
        tr("Row ID to be deleted"), 1, 1,
        tableModel->rowCount(), 1, &ok);
    if (ok)
        tableModel->removeRow(row - 1);
}

MainWindow::~MainWindow() { delete ui; }

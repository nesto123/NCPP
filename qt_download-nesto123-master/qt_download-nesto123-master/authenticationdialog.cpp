#include "authenticationdialog.h"
#include "ui_authenticationdialog.h"

AuthenticationDialog::AuthenticationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthenticationDialog)
{
    ui->setupUi(this);
}

AuthenticationDialog::~AuthenticationDialog()
{
    delete ui;
}

std::pair<QString, QString> AuthenticationDialog::getValues()
{
    return std::make_pair(ui->unameLineEdit->text(), ui->passLineEdit_2->text());
}

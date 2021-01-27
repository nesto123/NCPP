#ifndef AUTHENTICATIONDIALOG_H
#define AUTHENTICATIONDIALOG_H

#include <QDialog>
#include <utility>
namespace Ui {
class AuthenticationDialog;
}

class AuthenticationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthenticationDialog(QWidget *parent = nullptr);
    ~AuthenticationDialog();
    std::pair<QString,QString> getValues();
private:
    Ui::AuthenticationDialog *ui;
};

#endif // AUTHENTICATIONDIALOG_H

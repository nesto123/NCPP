#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Vaš kod

#include <QMainWindow>
class QPushButton;
class QLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QMenu * fileMenu;
    QMenu * helpMenu;

    void createIcons();
    void createLineEdit();
    void createButtons();
    void setupCoreWidgets();
    void setupSignalsAndSlots();
    void failureDialog();
    void successDialog();
    void clearFields();

    QPixmap newIcon;
    // druge ikone

    QLineEdit * nameLineEdit;
    QLineEdit * passwordLineEdit;

    QAction * newAction;
    QAction * openAction;
    QAction * quitAction;
    QAction * aboutAction;

    QPushButton * cancelButton;
    QPushButton * submitButton;

private slots:
    void submitButtonClicked();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};



#endif // MAINWINDOW_H

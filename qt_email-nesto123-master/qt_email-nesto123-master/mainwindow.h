#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QStandardItemModel;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    QStandardItemModel* tableModel;
    QString fileName;
    void setupSignalsAndSlots();
    void clearFields();
    void adjustTextColor();
    bool event(QEvent* e) override;
    bool compareWithFile();
private slots:
    void saveButtonClicked();
    void aboutDialog();
    void clearAllRecords();
    void deleteSavedRecord();
    void openFile();
    void save();
    void saveAs();

signals:
    void message(QString msg);
};
#endif // MAINWINDOW_H

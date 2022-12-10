#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileIconProvider>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRefreshButtonClicked();
    void onCbDiskIndexChanged(int index);

private:
    Ui::MainWindow    *ui;
    QFileSystemModel  *fsModel;
    QFileIconProvider iconProvider;
};
#endif // MAINWINDOW_H

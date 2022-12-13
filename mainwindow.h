#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fstatistics.h"

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QItemSelection>
#include <QChart>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class ChartType {
        Bar,
        Pie
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRefreshButtonClicked();
    void onCbDiskIndexChanged(int index);
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void redrawChartView();
    void redrawTableView();

    void onButtonBarClicked();
    void onButtonPieClicked();

private:
    Ui::MainWindow      *ui;
    QFileIconProvider   iconProvider;
    QFileSystemModel    *fsModel;
    QItemSelectionModel *selectionModel;
    FStatistics         statistics;
    QChart              currentChart;
    ChartType           chartType;
};
#endif // MAINWINDOW_H

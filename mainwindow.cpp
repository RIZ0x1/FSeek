#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mountpoints.h"

#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chartType = ChartType::Bar;
    fsModel = new QFileSystemModel(this);
    fsModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    fsModel->setIconProvider(&iconProvider);
    ui->treeView->setModel(fsModel);
    for (int i = 1; i < fsModel->columnCount(); ++i)
        ui->treeView->hideColumn(i);

    selectionModel = new QItemSelectionModel();
    ui->treeView->setSelectionModel(selectionModel);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshButtonClicked);
    connect(ui->cbDisk, &QComboBox::currentIndexChanged, this, &MainWindow::onCbDiskIndexChanged);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onSelectionChanged);
    connect(ui->buttonChart, &QPushButton::clicked, this, &MainWindow::onButtonBarClicked);
    connect(ui->buttonPie, &QPushButton::clicked, this, &MainWindow::onButtonPieClicked);
    connect(&statistics, &FStatistics::resultReady, this, &MainWindow::redrawChartView);
    connect(&statistics, &FStatistics::resultReady, this, &MainWindow::redrawTableView);
    emit ui->refreshButton->clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ****************************************************************************

void MainWindow::onRefreshButtonClicked()
{
    MountPoints::self().refreshMountedVolumes();
    ui->cbDisk->clear();

    foreach ( QStorageInfo volume, MountPoints::self().getNormalVolumes() ) {
        QString visibleText = volume.displayName();
        ui->cbDisk->addItem( visibleText, volume.rootPath() );
    }

    if (ui->cbDisk->count() == 0)
        ui->cbDisk->setPlaceholderText("No available volumes");
}

void MainWindow::onCbDiskIndexChanged(int)
{
    fsModel->setRootPath(ui->cbDisk->currentData().toString());
    ui->treeView->setRootIndex(fsModel->index(ui->cbDisk->currentData().toString()));
}

void MainWindow::onSelectionChanged(const QItemSelection& selected, const QItemSelection&)
{
    QModelIndexList selectedIndexList{ selected.indexes() };

    if (selectedIndexList.size() == 0)
        return ;

    QFileInfo fileInfo{ fsModel->filePath(selected.indexes().first()) };

    if (fileInfo.isDir() && fileInfo.isReadable())
    {
        QString dir = fileInfo.filePath();
        statistics.refresh(dir);
    }
    else
    {
        QMessageBox::information(this, "FSeek", fileInfo.fileName() + " is not readable");
    }
}

void MainWindow::redrawChartView()
{
    ui->chartsWidget->chart()->removeAllSeries();

    if ( chartType == ChartType::Bar )
    {
        QBarSeries* series = statistics.getBarSeries();
        ui->chartsWidget->chart()->addSeries(series);
    }
    else if ( chartType == ChartType::Pie )
    {
        QPieSeries* series = statistics.getPieSeries();
        ui->chartsWidget->chart()->addSeries(series);
    }
    else
        return ;
}

void MainWindow::redrawTableView()
{
    ui->tableWidget->clearContents();

    QLocale locale;

    int i = 0;
    for (auto& format : statistics.getStatistics())
    {
        ui->tableWidget->setRowCount(statistics.getStatistics().size());
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(format.first));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem( QString::number(format.second.count) ));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem( locale.formattedDataSize(format.second.size, QLocale::DataSizeTraditionalFormat) ));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem( locale.formattedDataSize(format.second.size / format.second.count) ));
        (i++);
    }
}

void MainWindow::onButtonBarClicked()
{
    chartType = ChartType::Bar;
    redrawChartView();
}

void MainWindow::onButtonPieClicked()
{
    chartType = ChartType::Pie;
    redrawChartView();
}


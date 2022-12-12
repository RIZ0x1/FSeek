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

    fsModel = new QFileSystemModel(this);
    fsModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    fsModel->setIconProvider(&iconProvider);
    ui->treeView->setModel(fsModel);
    for (int i = 1; i < fsModel->columnCount(); ++i)
        ui->treeView->hideColumn(i);

    selectionModel = new QItemSelectionModel();
    ui->treeView->setSelectionModel(selectionModel);

    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshButtonClicked);
    connect(ui->cbDisk, &QComboBox::currentIndexChanged, this, &MainWindow::onCbDiskIndexChanged);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onSelectionChanged);
    connect(ui->buttonChart, &QPushButton::clicked, this, &MainWindow::redrawChartView);
    connect(ui->buttonPizza, &QPushButton::clicked, this, &MainWindow::redrawChartView);
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
        qDebug() << "ROOT_PATH: " << volume.rootPath();
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
    QModelIndex     selectedIndex{ selectedIndexList.first() };
    QFileInfo       fileInfo{ fsModel->filePath(selectedIndex) };

    if (fileInfo.isDir() && fileInfo.isReadable())
    {
        QString dir = fileInfo.filePath();
        statistics.refresh(dir);
        redrawChartView();
    }
    else
    {
        QMessageBox::information(this, "FSeek", fileInfo.fileName() + " is not readable");
    }
}

void MainWindow::redrawChartView()
{
    foreach ( auto child, ui->chartButtonsLayout->children() )
    {
        if (child != sender())
            qobject_cast<QPushButton*>(child)->setChecked(false);
    }

    ui->chartsWidget->chart()->removeAllSeries();

    if ( ui->buttonChart->isChecked() )
    {
        QBarSeries* series = statistics.getBarSeries();
        ui->chartsWidget->chart()->addSeries(series);
    }
    else if ( ui->buttonPizza->isChecked() )
    {
        QPieSeries* series = statistics.getPieSeries();
        ui->chartsWidget->chart()->addSeries(series);
    }
    ui->chartsWidget->setChart(&currentChart);
}


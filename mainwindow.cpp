#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mountpoints.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshButtonClicked);
    connect(ui->cbDisk, &QComboBox::currentIndexChanged, this, &MainWindow::onCbDiskIndexChanged);

    fsModel = new QFileSystemModel(this);

    fsModel->setIconProvider(&iconProvider);
    fsModel->setRootPath("/media/riz1/DARKSPACE");
    ui->treeView->setModel(fsModel);
    ui->treeView->setRootIndex(fsModel->index("/media/riz1/DARSPACE"));

    // * Remove extra columns from treeView (size, type e.t.c)
    for (int i = 1; i < fsModel->columnCount(); ++i)
        ui->treeView->hideColumn(i);

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
        QString visibleText = volume.displayName() + " [" + volume.device() + ']';
        ui->cbDisk->addItem( visibleText, volume.rootPath() );
        qDebug() << "ROOT_PATH: " << volume.rootPath();
    }

    if (ui->cbDisk->count() == 0)
        ui->cbDisk->setPlaceholderText("No available volumes");
}

void MainWindow::onCbDiskIndexChanged(int index)
{
//    fsModel.setRootPath("/media/riz1/DARKSPACE");
//    fsModel.setRootPath( ui->cbDisk->currentData().toString() );
//    emit fsModel.rootPathChanged( ui->cbDisk->currentData().toString() );
//    ui->treeView->setRootIndex(fsModel.index(ui->cbDisk->currentData().toString()));
//    ui->treeView->setCurrentIndex(fsModel.index(ui->cbDisk->currentData().toString()));
}

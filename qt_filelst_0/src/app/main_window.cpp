/**
 * file list tool in QT, 12/1/15
 */
#include "main_window.hpp"
#include "moc_main_window.cpp"

/**
 * Constructor
 */
MainWindow::MainWindow(QMainWindow* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(doQuit()));
    connect(ui->listFileButton, SIGNAL(clicked()), this, SLOT(doListFile()));
    connect(ui->clearListButton, SIGNAL(clicked()), this, SLOT(doClearList()));
    connect(ui->clearAllButton, SIGNAL(clicked()), this, SLOT(doClearAll()));
}

void MainWindow::doQuit()
{
    close();
}

void MainWindow::doListFile()
{
    QString path_txt = ui->pathEdit->text();
    printString("Path: " + path_txt);
}

void MainWindow::printString(QString txt)
{
    QString full_txt = txt + "\n" + ui->fileListEdit->toPlainText();
    ui->fileListEdit->setText(full_txt);
}

void MainWindow::doClearList()
{
    ui->fileListEdit->clear();
}

void MainWindow::doClearAll()
{
    doClearList();
    ui->pathEdit->clear();
}


/**
 * file list tool in QT, 12/1/15
 */
#include "main_window.hpp"
#include "moc_main_window.cpp"

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "DirMgr.hpp"

using namespace app;

/**
 * Constructor
 */
MainWindow::MainWindow(QMainWindow* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(doQuit()));
    connect(ui->emptyDBButton, SIGNAL(clicked()), this, SLOT(doEmptyDB()));
    connect(ui->displayDBButton, SIGNAL(clicked()), this, SLOT(doDisplayDB()));
    connect(ui->addUpdateDBButton, SIGNAL(clicked()), this, SLOT(doAddUpdateDB()));
    connect(ui->clearDirPathButton, SIGNAL(clicked()), this, SLOT(doClearDirPath()));
    connect(ui->clearDBPathButton, SIGNAL(clicked()), this, SLOT(doClearDBPath()));
    connect(ui->clearOutputButton, SIGNAL(clicked()), this, SLOT(doClearOutput()));
    connect(ui->clearAllButton, SIGNAL(clicked()), this, SLOT(doClearAll()));
}

void MainWindow::doQuit()
{
    close();
}

void MainWindow::printString(QString txt)
{
    QString full_txt = txt + "\n" + ui->outputEdit->toPlainText();
    ui->outputEdit->setText(full_txt);
}

void MainWindow::doClearAll()
{
    doClearDirPath();
    doClearDBPath();
    doClearOutput();
}

void MainWindow::doAddUpdateDB()
{
    QString dirpath = ui->dirPathEdit->text().trimmed();
    if (dirpath.isEmpty()) {
        return;
    }

    QString dbpath = ui->dbPathEdit->text().trimmed();
    if (dbpath.isEmpty()) {
        return;
    }

    try {
        DirMgr dirmgr(dirpath, dbpath);
        printString(dirmgr.toString());
    }
    catch (const runtime_error& ex) {
        // catch all handled error:
        printString("Error:" + QString(ex.what()));
    }
}

void MainWindow::doClearDirPath()
{
    ui->dirPathEdit->clear();
}

void MainWindow::doClearDBPath()
{
    ui->dbPathEdit->clear();
}

void MainWindow::doClearOutput()
{
    ui->outputEdit->clear();
}

void MainWindow::doEmptyDB()
{
    QString dbpath = ui->dbPathEdit->text().trimmed();
    if (dbpath.isEmpty()) {
        return;
    }

    try {
        DirMgr dirmgr(dbpath);
        dirmgr.emptyDB();
        printString(dirmgr.toString());
    }
    catch (const runtime_error& ex) {
        // catch all handled error:
        printString("Error:" + QString(ex.what()));
    }
}

void MainWindow::doDisplayDB()
{
    QString dbpath = ui->dbPathEdit->text().trimmed();
    if (dbpath.isEmpty()) {
        return;
    }

    try {
        DirMgr dirmgr(dbpath);
        printString(dirmgr.displayDB());
        printString(dirmgr.toString());
    }
    catch (const runtime_error& ex) {
        // catch all handled error:
        printString("Error:" + QString(ex.what()));
    }
}


/**
 * dftool: diff 2 path, 12/29/15
 */
#include "main_window.hpp"
#include "moc_main_window.cpp"

#include <string>
#include <boost/regex.hpp>

#include "DiffMgr.hpp"

using namespace app;
using namespace std;

/**
 * Constructor
 */
MainWindow::MainWindow(QMainWindow* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUIData();
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(doQuit()));
    connect(ui->doDiffButton, SIGNAL(clicked()), this, SLOT(doDiff()));
    connect(ui->addSB1Button, SIGNAL(clicked()), this, SLOT(doAddSB1()));
    connect(ui->addSB2Button, SIGNAL(clicked()), this, SLOT(doAddSB2()));
    connect(ui->addSubDirButton, SIGNAL(clicked()), this, SLOT(doAddSubDir()));
    connect(ui->clearSB1Button, SIGNAL(clicked()), this, SLOT(doClearSB1()));
    connect(ui->clearSB2Button, SIGNAL(clicked()), this, SLOT(doClearSB2()));
    connect(ui->clearSubDirButton, SIGNAL(clicked()), this, SLOT(doClearSubDir()));
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

void MainWindow::doDiff()
{
    QString sb1_txt = ui->comboBoxSB1->currentText();
    if (sb1_txt.isEmpty()) {
        return;
    }

    QString sb2_txt = ui->comboBoxSB2->currentText();
    if (sb2_txt.isEmpty()) {
        return;
    }

    QStringList subdir_list;
    QString subdir_txt = ui->lineEditSubDir->text();
    if (!subdir_txt.isEmpty()) {
        subdir_list << subdir_txt;
    }
    QList<QListWidgetItem*> subdir_items = ui->listWidgetSubDir->selectedItems();
    foreach(QListWidgetItem* item, subdir_items) {
        subdir_list << item->text();
    }
    if (subdir_list.isEmpty()) {
        return;
    }

    try {
        DiffMgr diffmgr(this, sb1_txt, sb2_txt, subdir_list);
        printString(diffmgr.toString());
    } catch (const runtime_error& ex) {
        // catch all handled error:
        printString("Error:" + QString(ex.what()));
    }
}

void MainWindow::doClearSB1()
{
    ui->comboBoxSB1->clearEditText();
}

void MainWindow::doClearSB2()
{
    ui->comboBoxSB2->clearEditText();
}

void MainWindow::doClearOutput()
{
    ui->outputEdit->clear();
}

void MainWindow::doClearSubDir()
{
    ui->lineEditSubDir->clear();
    ui->listWidgetSubDir->clearSelection();
}

void MainWindow::doClearAll()
{
    doClearSB1();
    doClearSB2();
    doClearSubDir();
    doClearOutput();
}

void MainWindow::doAddSB(QComboBox* cb)
{
    QString sb_txt = cb->currentText();
    if (sb_txt.isEmpty()) {
        return;
    }

    try {
        string sb_path = DiffMgr::checkPath(sb_txt.toStdString());
        cb->addItem(QString::fromStdString(sb_path));
    }
    catch (const runtime_error& ex) {
        // catch all handled error:
        printString("Error:" + QString(ex.what()));
    }

}

void MainWindow::doAddSB1()
{
    doAddSB(ui->comboBoxSB1);
}

void MainWindow::doAddSB2()
{
    doAddSB(ui->comboBoxSB2);
}

void MainWindow::doAddSubDir()
{
    QString subdir_txt = ui->lineEditSubDir->text();
    if (subdir_txt.isEmpty()) {
        return;
    }

    ui->listWidgetSubDir->addItem(subdir_txt);
}

void MainWindow::setupUIData()
{
    setupComboBoxData();
    setupSubDirListData();
}

string MainWindow::replaceBackslash(const string& txt)
{
    boost::regex re("/");
    return boost::regex_replace(txt, re, "\\");
}

void MainWindow::setupComboBoxData()
{
    static const char* sb_name_table[] =
    { "l:/d2/sb",
    "l:/d2/sb2",
    "s:/lxu/sb",
    "s:/lxu/Acgir2",
    "s:/lxu/b3",
    "s:/lxu/sb4",
    "s:/lxu/sb5",
    "j:/Bcgxe/latest_pass",
    "j:/Bhdlcoder/latest_pass",
    "//mathworks/devel/archive/R2015b/perfect",
    "e:/d2/sb",
    "e:/d2/sb2"
    };

    QStringList sb_name_list;
    int table_sz = sizeof(sb_name_table) / sizeof(sb_name_table[0]);
    for (int i = 0; i < table_sz; i++) {
        sb_name_list << replaceBackslash(string(sb_name_table[i])).c_str();
    }
    ui->comboBoxSB1->addItems(sb_name_list);
    ui->comboBoxSB2->addItems(sb_name_list);
}

void MainWindow::setupSubDirListData()
{
    static const char* subdir_name_table[] =
    { "matlab/src/cgir_plc",
    "matlab/src/cg_ir",
    "matlab/src/cgir_xform",
    "matlab/src/rtwcg",
    "matlab/src/simulink/sl_engin",
    "matlab/toolbox/stateflow/src/stateflow/cdr",
    "matlab/toolbox/plccoder/plccoder/private",
    "matlab/toolbox/plccoder/plccoder/+PLCCoder",
    "matlab/toolbox/plccoder",
    "matlab/test/toolbox/plccoder",
    "matlab/test/toolbox/plccoder/coder",
    "matlab/test/tools/plctools"
    };

    QStringList subdir_name_list;
    int table_sz = sizeof(subdir_name_table) / sizeof(subdir_name_table[0]);
    for (int i = 0; i < table_sz; i++) {
        subdir_name_list << replaceBackslash(string(subdir_name_table[i])).c_str();
    }
    ui->listWidgetSubDir->addItems(subdir_name_list);
    ui->listWidgetSubDir->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
}

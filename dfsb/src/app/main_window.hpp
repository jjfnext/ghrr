/**
 * sb diff: diff 2 sb, 12/29/15
 */
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QtGui>
#include "ui_main_window.h"

#include <string>


/**
 * Main window class for the application
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow* parent = 0);

private slots:
    void doQuit();
    void doDiff();
    void doAddSB1();
    void doAddSB2();
    void doAddSubDir();
    void doClearSB1();
    void doClearSB2();
    void doClearSubDir();
    void doClearOutput();
    void doClearAll();

private:
    void printString(QString txt);
    void setupUIData();
    void setupComboBoxData();
    void setupSubDirListData();
    std::string replaceBackslash(const std::string& txt);
    void doAddSB(QComboBox* cb);

private:
    Ui::MainWindow* ui;
};

#endif

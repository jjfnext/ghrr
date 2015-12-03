/**
 * file list tool in QT, 12/1/15
 */
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QtGui>
#include "ui_main_window.h"

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
    void doListFile();
    void doClearList();
    void doClearAll();

private:
    void printString(QString txt);

private:
    Ui::MainWindow* ui;
};

#endif

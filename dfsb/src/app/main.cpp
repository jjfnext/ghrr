#include <QApplication>
#include "main_window.hpp"

#include <QtGlobal>
#include <QDebug>

#include <windows.h>

void msgHandler(QtMsgType msgtp, const QMessageLogContext & msgctx, const QString & msg)
{
    QString output = msg + "\n";
    OutputDebugString(output.toStdString().c_str());
}

int main(int argc, char* argv[])
{
    qInstallMessageHandler(msgHandler);

    QApplication app(argc, argv);
    MainWindow mainWindow;

    mainWindow.show();
    return app.exec();    
}


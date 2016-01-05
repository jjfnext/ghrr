
#ifndef _filedb_dirmgr_hpp
#define _filedb_dirmgr_hpp

#include <string>
#include <vector>
#include <QtCore>
#include <boost/filesystem.hpp>

#include <memory>

using namespace std;

namespace app {
    class DirMgr {
    public:
        DirMgr(QString& dirpath, QString& dbpath);
        DirMgr(QString& dbpath);
        ~DirMgr(); // need destructor as pimpl is used in main_window.cpp  which calls destructor
        const QString toString();
        void emptyDB();
        QString displayDB();

    public:
        static QString timeNow();

    private:
        class impl;
        unique_ptr<impl> pimpl;
    };
}

#endif
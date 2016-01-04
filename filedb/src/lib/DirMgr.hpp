
#ifndef _filedb_dirmgr_hpp
#define _filedb_dirmgr_hpp

#include <string>
#include <vector>
#include <QtCore>
#include <boost/filesystem.hpp>

using namespace std;
namespace sqlite3pp {
    class database;
}

namespace app {
    class DirMgr {
    public:
        DirMgr(QString& dirpath, QString& dbpath);
        DirMgr(QString& dbpath);
        const QString toString();
        void emptyDB();
        QString displayDB();

    private:
        void checkDirPath(const QString& dirpath);
        void checkDBPath(const QString& dbpath);
        void genDBTable();
        void genDBFile();
        void genSingleDBFile(const boost::filesystem::path& path);
        void genDirectoryDBFiles(const boost::filesystem::path& path);
        void insertDBFile(string fchecksum, string fname, string fpath, int fsize, string fmodtime, string fname_norm, string fext);
        bool checkDBFileDuplicate(sqlite3pp::database& db, string fchecksum);

    private:
        string fDirPath;
        string fDBPath;
    };
}

#endif
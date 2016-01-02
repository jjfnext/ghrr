
#ifndef _filedb_dirmgr_hpp
#define _filedb_dirmgr_hpp

#include <string>
#include <vector>
#include <QtCore>
#include <boost/filesystem.hpp>

using namespace std;

namespace app {
    class DirMgr {
    public:
        DirMgr(QString& dirpath, QString& dbpath);
        const QString toString();

    private:
        void checkDirPath(const QString& dirpath);
        void checkDBPath(const QString& dbpath);
        void genDBTable();
        void genDBFile();
        void genSingleDBFile(const boost::filesystem::path& path);
        void genDirectoryDBFiles(const boost::filesystem::path& path);
        void insertDBFile(string fchecksum, string fname, string fpath, int fsize, string fmodtime);

    private:
        string fDirPath;
        string fDBPath;
    };
}

#endif
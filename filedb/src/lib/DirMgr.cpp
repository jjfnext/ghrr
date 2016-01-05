
#include <stdexcept>

#include <ctime>
#include <iostream>
#include <string>
#include <algorithm>

#include <boost/format.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/tuple/tuple.hpp>

#include <sqlite3pp.h>

#include "ChecksumMgr.hpp"
#include "DirMgr.hpp"

using namespace std;
namespace fs = boost::filesystem;

namespace app {
    class DirMgr::impl {
    public:
        enum Action { AddUpdateDB, EmptyDB, DisplayDB};

    public:
        impl(QString& dirpath, QString& dbpath);
        impl(QString& dbpath);
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
        bool checkDBFileExist(string fchecksum);
        void openDB();
        void closeDB();
        bool isSkipDir(const fs::path& dir_path);
        void updateDBFile(string fchecksum, string fname, string fpath, int fsize, string fmodtime, string fname_norm, string fext);
        void insertOrUpdateDBFile(string fchecksum, string fname, string fpath, int fsize, string fmodtime, string fname_norm, string fext);
        string getStatusTxt();

    private:
        sqlite3pp::database fDB;
        bool fDBOpen;
        string fDirPath;
        string fDBPath;
        Action fStatus;
    };

    DirMgr::impl::impl(QString& dirpath, QString& dbpath)
        : fDBOpen(false)
    {
        // check dirpath exist
        // check dbpath directory exist
        // generate db table
        // recursive dir iter to add to table

        checkDirPath(dirpath);
        checkDBPath(dbpath);
        openDB();
        genDBTable();
        genDBFile();
        closeDB();

        fStatus = AddUpdateDB;
    }

    DirMgr::impl::impl(QString& dbpath)
        : fDBOpen(false)
    {
        checkDBPath(dbpath);
    }

    void DirMgr::impl::checkDirPath(const QString& dirpath)
    {
        // actually, allow both dir and file
        
        string path = dirpath.toStdString();
        fs::path p(path);
        if (!fs::exists(p)) {
            throw runtime_error("'"+path + "' does not exist");
        }

        fDirPath = dirpath.toStdString();
    }

    void DirMgr::impl::checkDBPath(const QString& dbpath)
    {
        string path = dbpath.toStdString();
        fs::path p(path);
        fs::path dir_path = p.parent_path();
        if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
            throw runtime_error("'" + path + "' does not exist");
        }

        fDBPath = dbpath.toStdString();
    }

    string DirMgr::impl::getStatusTxt()
    {
        switch (fStatus) {
        case AddUpdateDB:
            return "AddUpdateDB";
        case EmptyDB:
            return "EmptyDB";
        case DisplayDB:
            return "DisplayDB";
        default:
            assert(false);
            return "";
        }
    }

    const QString DirMgr::impl::toString()
    {
        boost::format f("DB action: %1$s\n\tdir path: %2$s\n\tdb path: %3$s\n");
        f % getStatusTxt() % fDirPath % fDBPath;

        QString result = QString::fromStdString(f.str());

        return result;
    }

    void DirMgr::impl::genDBTable()
    {
        string cmd = "create table if not exists FileTable("
            "FileCheckSum text primary key, "
            "FileName text, "
            "FilePath text, "
            "FizeSize integer, "
            "FileModTime text, "
            "Idx integer, "
            "FileNameNorm text, "
            "FileExt text);";
        int ret = fDB.execute(cmd.c_str());

        /*
        qDebug() << QString("create table cmd: ") << QString::fromStdString(cmd);
        qDebug() << QString("result: ") << ret;
        */

        if (ret) {
            throw runtime_error("Error: failed to create FileTable");
        }
    }

    bool DirMgr::impl::checkDBFileExist(string fchecksum)
    {
        boost::format f("select count(*) from FileTable where FileCheckSum = '%1$s'; ");
        f % fchecksum ; 
        string cmd = f.str();
        //qDebug() << "check dup cmd: " << QString::fromStdString(cmd);

        sqlite3pp::query qry(fDB, cmd.c_str());
        sqlite3pp::query::iterator i = qry.begin();
        int count;
        boost::tie(count) = (*i).get_columns<int>(0);
        //qDebug() << "dup count: " << count;

        if (count) {
            return true;
        }

        return false;
    }

    void DirMgr::impl::updateDBFile(string fchecksum, string fname, string fpath, int fsize, string fmodtime, string fname_norm, string fext)
    {
        boost::format f("update FileTable set "
            "FileName='%1$s', "
            "FilePath='%2$s', "
            "FizeSize=%3$d, "
            "FileModTime='%4$s', "
            "FileNameNorm='%5$s', "
            "FileExt='%6$s' "
            "where FileCheckSum='%7$s'; ");

        f % fname % fpath % fsize % fmodtime % fname_norm % fext % fchecksum; 
        string cmd = f.str();
        //qDebug() << "update cmd: " << QString::fromStdString(cmd);

        int ret = fDB.execute(cmd.c_str());

        if (ret) {
            throw runtime_error("Error: failed to update FileTable");
        }
    }

    void DirMgr::impl::insertDBFile(string fchecksum, string fname, string fpath, int fsize, string fmodtime, string fname_norm, string fext)
    {
        sqlite3pp::query qry(fDB, "select count(*) from FileTable;");
        sqlite3pp::query::iterator i = qry.begin();
        int count;
        boost::tie(count) = (*i).get_columns<int>(0);
        //qDebug() << "db count: " << count;

        boost::format f("insert into FileTable values('%1$s', '%2$s', '%3$s', %4$d, '%5$s', %6$d, '%7$s', '%8$s'); ");
        f % fchecksum % fname % fpath % fsize % fmodtime % (count + 1) % fname_norm % fext; // idx start at 1
        string cmd = f.str();
        //qDebug() << "insert cmd: " << QString::fromStdString(cmd);

        int ret = fDB.execute(cmd.c_str());

        if (ret) {
            throw runtime_error("Error: failed to insert FileTable");
        }
    }

    void DirMgr::impl::insertOrUpdateDBFile(string fchecksum, string fname, string fpath, int fsize, string fmodtime, string fname_norm, string fext)
    {
        if (checkDBFileExist(fchecksum)) { 
            updateDBFile(fchecksum, fname, fpath, fsize, fmodtime, fname_norm, fext);
        } else {
            insertDBFile(fchecksum, fname, fpath, fsize, fmodtime, fname_norm, fext);
        }
    }

    void DirMgr::impl::genSingleDBFile(const fs::path& path)
    {
        //qDebug() << "db file: " << QString::fromStdString(path.string());

        fs::path p(path);
        boost::system::error_code ec;
        p = fs::canonical(p, ec).make_preferred(); // make path canonical
        if (ec) {
            throw runtime_error("Error: "+path.string()+" not valid");
        }
        assert(fs::exists(p));

        string fullpath = p.string();
        string fname = p.filename().string();
        string fname_norm = fname;
        transform(fname_norm.begin(), fname_norm.end(), fname_norm.begin(), tolower);
        string fext = p.has_extension() ? p.extension().string().substr(1) : "noext";
        int fsize = static_cast<int>(fs::file_size(p));
        time_t ts = fs::last_write_time(p);
        tm fmod_time = *localtime(&ts);
        char fmod_txt[200] = { 0 };
        // man page: http://man7.org/linux/man-pages/man3/strftime.3.html
        if (!(strftime(fmod_txt, sizeof(fmod_txt) - 1, "%Y/%m/%d %H:%M:%S", &fmod_time) > 0)) {
            throw runtime_error("Error: invalid file mod time for "+fullpath); // invalid time
        }

        string fchecksum = gen_string_checksum(fullpath);

        insertOrUpdateDBFile(fchecksum, fname, fullpath, fsize, string(fmod_txt), fname_norm, fext);
    }

    bool DirMgr::impl::isSkipDir(const fs::path& dir_path)
    {
        if (dir_path.filename() == ".git") {
            return true;
        }

        return false;
    }

    void DirMgr::impl::genDirectoryDBFiles(const fs::path& dir_path)
    {
        fs::recursive_directory_iterator it(dir_path), end;
        while (it != end) {
//            fs::path p = it->path();
            if (fs::is_directory(*it) && (fs::is_symlink(*it) || isSkipDir(*it))) {
                it.no_push();
            } else if (fs::is_regular_file(*it)) {
                genSingleDBFile(*it);
            }

            ++it; // don't forget this, otherwise, becomes dead loop
        }
    }

    void DirMgr::impl::genDBFile()
    {
        fs::path p(fDirPath);
        if (fs::is_regular_file(p)) {
            genSingleDBFile(p);
            return;
        } else if (fs::is_directory(p)) {
            genDirectoryDBFiles(p);
            return;
        }

        throw runtime_error("Error: " + fDirPath + " is not file or directory");
    }

    void DirMgr::impl::emptyDB()
    {
        openDB();
        string cmd = "drop table if exists FileTable;";
        int ret = fDB.execute(cmd.c_str());
        if (ret) {
            throw runtime_error("Error: failed to create FileTable");
        }
        closeDB();

        fStatus = EmptyDB;
    }

    QString DirMgr::impl::displayDB()
    {
        openDB();

        sqlite3pp::query qry(fDB, "select * from FileTable;");

        QStringList result_list;
        for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
            char const* fchecksum;
            char const* fname;
            char const* fpath;
            int fsize;
            char const* fmodtime;
            char const* fname_norm;
            char const* fext;
            int idx;

            boost::tie(fchecksum, fname, fpath, fsize, fmodtime, idx, fname_norm, fext) = 
                (*i).get_columns<char const*, char const*, char const*, int, char const*, int, char const*, char const*>(0, 1, 2, 3, 4, 5, 6, 7);

            boost::format f("File:\n\tIdx: %1$d\n\tName: %2$s\n\tSize: %3$d\n\tMod: %4$s\n\tPath: %5$s\n\tCheckSum: %6$s\n\tNameNorm: %7$s\n\tExt: %8$s\n");
            f % idx % fname % fsize % fmodtime % fpath % fchecksum % fname_norm % fext; 
            result_list << QString::fromStdString(f.str());
        }
        
        int ret = qry.finish(); 
        if (ret) {
            throw runtime_error("Error: query cannot finish");
        }

        closeDB();
        
        fStatus = DisplayDB;

        return result_list.join("");
    }

    void DirMgr::impl::openDB()
    {
        assert(fDBOpen == false);

        int ret = fDB.connect(fDBPath.c_str());
        if (ret) {
            throw runtime_error("Error: cannot open db file " + fDBPath);
        }

        fDBOpen = true;
    }

    void DirMgr::impl::closeDB()
    {
        assert(fDBOpen == true);

        int ret = fDB.disconnect();
        if (ret) {
            throw runtime_error("Error: cannot close db file " + fDBPath);
        }

        fDBOpen = false;
    }

    DirMgr::DirMgr(QString& dirpath, QString& dbpath) :
        pimpl(new DirMgr::impl(dirpath, dbpath))
    {
    }

    DirMgr::DirMgr(QString& dbpath) :
        pimpl(new DirMgr::impl(dbpath))
    {
    }

    const QString DirMgr::toString()
    {
        return pimpl->toString();
    }

    void DirMgr::emptyDB()
    {
        pimpl->emptyDB();
    }

    QString DirMgr::displayDB()
    {
        return pimpl->displayDB();
    }

    DirMgr::~DirMgr()
    {
    }

    QString DirMgr::timeNow()
    {
        time_t ts = time(nullptr);
        tm time_now = *localtime(&ts);
        char time_now_txt[200] = { 0 };
        // man page: http://man7.org/linux/man-pages/man3/strftime.3.html
        if (!(strftime(time_now_txt, sizeof(time_now_txt) - 1, "%Y/%m/%d %H:%M:%S", &time_now) > 0)) {
            throw runtime_error("Error: cannot get current time"); // invalid time
        }
        return QString(time_now_txt);
    }
}
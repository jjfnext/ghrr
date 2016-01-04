
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
        bool checkDBFileDuplicate(sqlite3pp::database& db, string fchecksum);

    private:
        string fDirPath;
        string fDBPath;
    };

    DirMgr::impl::impl(QString& dirpath, QString& dbpath)
    {
        // check dirpath exist
        // check dbpath directory exist
        // generate db table
        // recursive dir iter to add to table

        checkDirPath(dirpath);
        checkDBPath(dbpath);
        genDBTable();
        genDBFile();
    }

    DirMgr::impl::impl(QString& dbpath)
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

    const QString DirMgr::impl::toString()
    {
        boost::format f("Gen file db: \n\tdir path: %1$s\n\tdb path: %2$s\n");
        f % fDirPath % fDBPath;

        QString result = QString::fromStdString(f.str());

        return result;
    }

    void DirMgr::impl::genDBTable()
    {
        sqlite3pp::database db(fDBPath.c_str());

        string cmd = "create table if not exists FileTable("
            "FileCheckSum text primary key, "
            "FileName text, "
            "FilePath text, "
            "FizeSize integer, "
            "FileModTime text, "
            "Idx integer, "
            "FileNameNorm text, "
            "FileExt text);";
        int ret = db.execute(cmd.c_str());

        /*
        qDebug() << QString("create table cmd: ") << QString::fromStdString(cmd);
        qDebug() << QString("result: ") << ret;
        */

        db.disconnect();
        if (ret) {
            throw runtime_error("Error: failed to create FileTable");
        }
    }

    bool DirMgr::impl::checkDBFileDuplicate(sqlite3pp::database& db, string fchecksum)
    {
        boost::format f("select count(*) from FileTable where FileCheckSum = '%1$s'; ");
        f % fchecksum ; 
        string cmd = f.str();
        qDebug() << "check dup cmd: " << QString::fromStdString(cmd);

        sqlite3pp::query qry(db, cmd.c_str());
        sqlite3pp::query::iterator i = qry.begin();
        int count;
        boost::tie(count) = (*i).get_columns<int>(0);
        qDebug() << "dup count: " << count;

        if (count) {
            return true;
        }

        return false;
    }

    void DirMgr::impl::insertDBFile(string fchecksum, string fname, string fpath, int fsize, string fmodtime, string fname_norm, string fext)
    {
        sqlite3pp::database db(fDBPath.c_str());
        if (checkDBFileDuplicate(db, fchecksum)) { // todo: if duplicate, do update instead of insert
            return;
        }

        sqlite3pp::query qry(db, "select count(*) from FileTable;");
        sqlite3pp::query::iterator i = qry.begin();
        int count;
        boost::tie(count) = (*i).get_columns<int>(0);
        qDebug() << "db count: " << count;

        boost::format f("insert into FileTable values('%1$s', '%2$s', '%3$s', %4$d, '%5$s', %6$d, '%7$s', '%8$s'); ");
        f % fchecksum % fname % fpath % fsize % fmodtime % (count + 1) % fname_norm % fext; // idx start at 1
        string cmd = f.str();
        qDebug() << "insert cmd: " << QString::fromStdString(cmd);

        int ret = db.execute(cmd.c_str());

        db.disconnect();
        if (ret) {
            throw runtime_error("Error: failed to insert FileTable");
        }
    }

    void DirMgr::impl::genSingleDBFile(const fs::path& path)
    {
        qDebug() << "db file: " << QString::fromStdString(path.string());

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

        insertDBFile(fchecksum, fname, fullpath, fsize, string(fmod_txt), fname_norm, fext);
    }

    void DirMgr::impl::genDirectoryDBFiles(const fs::path& dir_path)
    {
        fs::recursive_directory_iterator it(dir_path), end;
        while (it != end) {
            fs::path p = it->path();
            if (fs::is_directory(*it) && fs::is_symlink(*it)) {
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
        sqlite3pp::database db(fDBPath.c_str());

        string cmd = "drop table if exists FileTable;";
        int ret = db.execute(cmd.c_str());
        db.disconnect();
        if (ret) {
            throw runtime_error("Error: failed to create FileTable");
        }
    }

    QString DirMgr::impl::displayDB()
    {
        sqlite3pp::database db(fDBPath.c_str());
        sqlite3pp::query qry(db, "select * from FileTable;");

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

        db.disconnect();

        return result_list.join("");
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
}
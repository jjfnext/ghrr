
#include <stdexcept>

#include <ctime>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/tuple/tuple.hpp>

#include <sqlite3pp.h>

#include "ChecksumMgr.hpp"
#include "DirMgr.hpp"

using namespace std;
namespace fs = boost::filesystem;

namespace app {
    /*
    FileItem::FileItem(string fpath, string fname, int fsize, string fdtstirng, string fchecksum)
        : fPath(fpath)
        , fName(fname)
        , fSize(fsize)
        , fModDateTime(fdtstirng)
        , fChecksum(fchecksum)
    {        
    }

    FileItem::~FileItem()
    {

    }

    std::string FileItem::toString()
    {
        boost::format f("File: %1$s, %2$d, %3$s, %4$s\nPath: %5$s");
        f % fName % fSize % fModDateTime % fChecksum % fPath;

        return f.str();
    }
    */
    static void testdb0()
    {
        sqlite3pp::database db("C:\\lxu\\sdev\\bsd_prj\\new\\prj\\filedb\\car.db");
        db.execute("INSERT INTO Cars (Id, Name, Price) VALUES (100, 'MyCar', 1229)");
        db.disconnect();
    }

    static void testdb()
    {
        sqlite3pp::database db("C:\\lxu\\sdev\\bsd_prj\\new\\prj\\filedb\\car2.db");
        int ret;
        string cmd;
        cmd = "CREATE TABLE cars(Id INTEGER PRIMARY KEY, Name TEXT, Price INTEGER);";
        ret = db.execute(cmd.c_str());

        cmd = "INSERT INTO cars VALUES(1,'Audi',52643);";
        ret = db.execute(cmd.c_str());

        cmd = "INSERT INTO cars VALUES(2,'Mercedes',57642);";
        ret = db.execute(cmd.c_str());

        cmd = "INSERT INTO cars VALUES(5,'Bentley',350000);";
        ret = db.execute(cmd.c_str());

        cmd = "INSERT INTO cars VALUES(6,'Hummer',41400);";
        ret = db.execute(cmd.c_str());

        cmd = "INSERT INTO cars VALUES(1000,'MyCar',1229);";
        ret = db.execute(cmd.c_str());

        sqlite3pp::query qry(db, "SELECT * FROM cars;");
        for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
            int id;
            char const* name;
            int price;
            boost::tie(id, name, price) = (*i).get_columns<int, char const*, int>(0, 1, 2);
            bool debug = true;
            std::cout << id << "\t" << name << "\t" << price << endl;
        }

        db.disconnect();
    }

    DirMgr::DirMgr(QString& dirpath, QString& dbpath)
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

    void DirMgr::checkDirPath(const QString& dirpath)
    {
        // actually, allow both dir and file
        
        string path = dirpath.toStdString();
        fs::path p(path);
        if (!fs::exists(p)) {
            throw runtime_error(path + " does not exist");
        }

        fDirPath = dirpath.toStdString();
    }

    void DirMgr::checkDBPath(const QString& dbpath)
    {
        string path = dbpath.toStdString();
        fs::path p(path);
        fs::path dir_path = p.parent_path();
        if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
            throw runtime_error(path + " does not exist");
        }

        fDBPath = dbpath.toStdString();
    }

    const QString DirMgr::toString()
    {
        boost::format f("Gen file db: \n\tdir path: %1$s\n\tdb path: %2$s\n");
        f % fDirPath % fDBPath;

        QString result = QString::fromStdString(f.str());

        return result;
    }

    void DirMgr::genDBTable()
    {
        sqlite3pp::database db(fDBPath.c_str());

        string cmd = "create table if not exists FileTable("
            "FileCheckSum text primary key, "
            "FileName text, "
            "FilePath text, "
            "FizeSize integer, "
            "FileModTime text, "
            "Idx integer);";
        int ret = db.execute(cmd.c_str());

        /*
        qDebug() << QString("create table cmd: ") << QString::fromStdString(cmd);
        qDebug() << QString("result: ") << ret;
        */

        db.disconnect();
        if (ret) {
            throw runtime_error("Error: failed to create FileTable");
        }

        /*
        cmd = "INSERT INTO cars VALUES(1,'Audi',52643);";
        ret = db.execute(cmd.c_str());

        cmd = "INSERT INTO cars VALUES(2,'Mercedes',57642);";
        ret = db.execute(cmd.c_str());

        cmd = "INSERT INTO cars VALUES(5,'Bentley',350000);";
        ret = db.execute(cmd.c_str());

        cmd = "INSERT INTO cars VALUES(6,'Hummer',41400);";
        ret = db.execute(cmd.c_str());

        cmd = "INSERT INTO cars VALUES(1000,'MyCar',1229);";
        ret = db.execute(cmd.c_str());

        sqlite3pp::query qry(db, "SELECT * FROM cars;");
        for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
            int id;
            char const* name;
            int price;
            boost::tie(id, name, price) = (*i).get_columns<int, char const*, int>(0, 1, 2);
            bool debug = true;
            std::cout << id << "\t" << name << "\t" << price << endl;
        }
        */
    }

    void DirMgr::insertDBFile(string fchecksum, string fname, string fpath, int fsize, string fmodtime)
    {
        sqlite3pp::database db(fDBPath.c_str());
        sqlite3pp::query qry(db, "select count(*) from FileTable;");
        sqlite3pp::query::iterator i = qry.begin();
        int count;
        boost::tie(count) = (*i).get_columns<int>(0);
        qDebug() << "db count: " << count;

        boost::format f("insert into FileTable values('%1$s', '%2$s', '%3$s', %4$d, '%5$s', %6$d); ");
        f % fchecksum % fname % fpath % fsize % fmodtime % (count + 1); // idx start at 1
        string cmd = f.str();
        qDebug() << "insert cmd: " << QString::fromStdString(cmd);

        int ret = db.execute(cmd.c_str());

        db.disconnect();
        if (ret) {
            throw runtime_error("Error: failed to insert FileTable");
        }
    }

    void DirMgr::genSingleDBFile(const fs::path& path)
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
        int fsize = static_cast<int>(fs::file_size(p));
        time_t ts = fs::last_write_time(p);
        tm fmod_time = *localtime(&ts);
        char fmod_txt[200] = { 0 };
        // man page: http://man7.org/linux/man-pages/man3/strftime.3.html
        if (!(strftime(fmod_txt, sizeof(fmod_txt) - 1, "%Y/%m/%d %H:%M:%S", &fmod_time) > 0)) {
            throw runtime_error("Error: invalid file mod time for "+fullpath); // invalid time
        }

        string fchecksum = gen_string_checksum(fullpath);

        insertDBFile(fchecksum, fname, fullpath, fsize, string(fmod_txt));
    }

    void DirMgr::genDirectoryDBFiles(const fs::path& dir_path)
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

    void DirMgr::genDBFile()
    {
        fs::path p(fDirPath);
        if (fs::is_regular_file(p)) {
            genSingleDBFile(p);
        } else if (fs::is_directory(p)) {
            genDirectoryDBFiles(p);
        }

        throw runtime_error("Error: " + fDirPath + " is not file or directory");
    }

    /*
    std::string DirMgr::getFileList()
    {
        string lst = "Dir: " + fDirPath + "\n";
        for (auto& f : fFileList) {
            lst += f.toString() + "\n";
        }

        return lst;
    }

    void DirMgr::genFile(const string& fpath)
    {
        fs::path p(fpath);
        boost::system::error_code ec;
        p = fs::canonical(p, ec).make_preferred(); // make path canonical
        if (ec) {
            return;
        }
        assert(fs::exists(p));

        string fullpath = p.string();
        string fname = p.filename().string();
        int fsize = static_cast<int>(fs::file_size(p));
        time_t ts = fs::last_write_time(p);
        tm fmod_time = *localtime(&ts);
        char fmod_txt[200] = { 0 };
        // man page: http://man7.org/linux/man-pages/man3/strftime.3.html
        if (!(strftime(fmod_txt, sizeof(fmod_txt) - 1, "%Y/%m/%d %H:%M:%S", &fmod_time) > 0)) {
            return; // invalid time
        }

        string fchecksum = gen_string_checksum(fullpath);

        fFileList.push_back(FileItem(fullpath, fname, fsize, string(fmod_txt), fchecksum));
    }

    void DirMgr::genFileList(const string& dpath)
    {
        if (!fs::is_directory(dpath)) {
            boost::format f("Error: %1$s is not a directory");
            f % dpath;
            throw invalid_argument(f.str());
        }

        fDirPath = fs::path(dpath).string();

        for (fs::directory_entry& e : fs::directory_iterator(dpath)) {
            fs::path e_path = e.path();
            if (fs::is_regular_file(e_path)) {
                genFile(e_path.string());
            }
        }
    }
    */
}

#include <stdexcept>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/filesystem/operations.hpp>

#include <QDebug>

#include "DiffMgr.hpp"

using namespace std;
namespace fs = boost::filesystem;

namespace app {
    DiffMgr::DiffMgr(QObject* parent, const QString& sb1, const QString& sb2, const QStringList& subdir_list)
        : fParent(parent)
        , fSubDirList(subdir_list)
    {
        fSB1 = QString::fromStdString(checkPath(sb1.toStdString()));
        fSB2 = QString::fromStdString(checkPath(sb2.toStdString()));
        pruneSBPath(fSB1);
        pruneSBPath(fSB2);
        runDiff();
    }

    string DiffMgr::checkPath(const string& path)
    {
        qDebug() << "Path: " << QString::fromStdString(path) << "\n";

        fs::path p(path);
        if (!fs::exists(p)) {
            throw runtime_error(path + " does not exist");
        }

        /*
        boost::system::error_code errcode;
        p = fs::canonical(p, errcode);
        if (errcode.value()) {
            throw runtime_error(path + " is not valid path");
        }
        */

        string debug_txt = p.string();

        return p.string();
    }

    void DiffMgr::pruneSBPath(const QString& sb)
    {
        string sb_txt = sb.toStdString();
        QStringList prune_list; 

        foreach(QString subdir, fSubDirList) {
            string subdir_txt = subdir.toStdString();
            try {
                checkPath(sb_txt + "\\" + subdir_txt);
                prune_list << QString::fromStdString(subdir_txt);
            } catch (const runtime_error& ex) {
                qDebug() << "Error:" + QString(ex.what());
            }
        }

        fSubDirList = prune_list;
    }

    const QString DiffMgr::toString()
    {
        boost::format f("Do diff: \n\tSB1: %1$s\n\tSB2: %2$s\n");
        f % fSB1.toStdString() % fSB2.toStdString();

        QString result = QString::fromStdString(f.str());

        foreach(QString subdir, fSubDirList) {
            boost::format f2("\t%1$s\n");
            f2 % subdir.toStdString();
            result += QString::fromStdString(f2.str());
        }

        return result;
    }

    void DiffMgr::runDiff()
    {
        QString exe = "C:\\Program Files\\Araxis\\Araxis Merge\\Merge.exe";

        foreach(QString subdir, fSubDirList) {
            QProcess* proc = new QProcess(fParent);
            QStringList arguments;
            arguments << fSB1 + "\\" + subdir << fSB2 + "\\" + subdir;
            proc->start(exe, arguments);
        }
    }
}
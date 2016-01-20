
#ifndef _filelst_diffmgr_hpp
#define _filelst_diffmgr_hpp

#include <string>
#include <vector>
#include <QtCore>

using namespace std;

namespace app {
    class DiffMgr {
    public:
        DiffMgr(QObject* parent, const QString& sb1, const QString& sb2, const QStringList& subdir_list);
        const QString toString();
        static string checkPath(const string& path); // check path exists and return standard path

    private:
        void runDiff();
        void pruneSBPath(const QString& sb); // use sb to prune subdirlist and remove non existing subdir

    private:
        QObject* fParent;
        QString fSB1;
        QString fSB2;
        QStringList fSubDirList;
    };
}

#endif

#ifndef _filelst_dirmgr_hpp
#define _filelst_dirmgr_hpp

#include <string>
#include <vector>

using namespace std;

namespace app {
    class FileItem {
    public:
        FileItem(string fname, int fsize, string fdtstring);
        ~FileItem();
        inline string name() { return fName; }
        inline int sz() { return fSize; }
        inline string modDateTime() { return fModDateTime; }
        string toString();

    private:
        string fName;
        int fSize;
        string fModDateTime;
    };

    class DirMgr {
    public:
        DirMgr(string dpath);
        string getFileList();

    private:
        void genFileList(const string& dpath);
        void genFile(const string& fpath);

    private:
        string fDirPath;
        vector<FileItem> fFileList;
    };
}

#endif
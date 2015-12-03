
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/filesystem/operations.hpp>
#include <ctime>

#include "DirMgr.hpp"

using namespace std;
namespace fs = boost::filesystem;

namespace app {
    FileItem::FileItem(string fname, int fsize, string fdtstirng) 
        : fName(fname)
        , fSize(fsize)
        , fModDateTime(fdtstirng)
    {        
    }

    FileItem::~FileItem()
    {

    }

    std::string FileItem::toString()
    {
        boost::format f("File: %1$s, %2$d, %3$s");
        f % fName % fSize % fModDateTime;

        return f.str();
    }

    DirMgr::DirMgr(string dpath)
        : fDirPath("")
    {
        if (dpath == "") {
            dpath = fs::current_path().string();
        }

        genFileList(dpath);
    }

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
        assert(fs::exists(p));

        string fname = p.filename().string();
        int fsize = static_cast<int>(fs::file_size(p));
        time_t ts = fs::last_write_time(p);
        tm fmod_time = *localtime(&ts);
        char fmod_txt[200] = { 0 };
        if (strftime(fmod_txt, sizeof(fmod_txt) - 1, "%Y/%m/%d %H:%M:%S", &fmod_time) > 0) {
            fFileList.push_back(FileItem(fname, fsize, string(fmod_txt)));
        }
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
}
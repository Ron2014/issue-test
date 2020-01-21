#include <iostream>
#include <string>
#include <vector>
using namespace std;

#ifdef _WIN32
#include <io.h>
#else
#include <dirent.h>
#endif

bool
DeepSearchDirectory(const std::string &path, std::vector<std::string> &filelist, std::vector<std::string> &dirlist) {
#ifdef _WIN32
    _finddata_t file;
    std::string filepath = path + "/*";
    intptr_t handle;

    if ( (int)(handle = _findfirst(filepath.c_str(), &file)) == -1 ) {
        return false;
    }

    do {
        if (file.attrib & _A_SUBDIR) {
            std::string s = file.name;
            if (s == "." || s == "..")
                continue;

            std::string dirname = path + "/" + file.name;
            dirlist.push_back(dirname);
            
            if (!DeepSearchDirectory(dirname, filelist, dirlist))
                return false;

        } else {
            std::string filename = path + "/" + file.name;
            filelist.push_back(filename);
        }

    } while( _findnext(handle, &file) == 0 );

    _findclose(handle);
#else
    DIR *p_dir;
    struct dirent *p_dirent;

    if((p_dir = opendir(path.c_str())) == NULL) {
        return false;
    }

    std::string fullpath;
    while((p_dirent = readdir(p_dir))) {
        if (DT_REG == p_dirent->d_type) {
            // normal type file
            fullpath = path + "/" + p_dirent->d_name;
            filelist.push_back(fullpath);

        }else if (DT_DIR == p_dirent->d_type) {
            // directory
            std::string s = p_dirent->d_name;
            if (s == "." || s == "..")
                continue;
            
            fullpath = path + "/" + p_dirent->d_name;
            dirlist.push_back(fullpath);

            if (!DeepSearchDirectory(fullpath, filelist, dirlist))
                return false;
        }
    }

    closedir(p_dir);
#endif
    return true;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "usage: main dirname" << endl;
        return 0;
    }

    while(--argc) {
        char *dirname = argv[argc];
        std::vector<std::string> filelist, dirlist;
        if (!DeepSearchDirectory(dirname, filelist, dirlist)) {
            cout << "check pir path:" << dirname << " failed" <<endl;
            continue;
        }

        cout << "====== files:" << endl;
        for(std::vector<std::string>::iterator it = filelist.begin(); it != filelist.end(); ++it){
            cout << *it << endl;
        }

        cout << "====== directories:" << endl;
        for(std::vector<std::string>::iterator it = dirlist.begin(); it != dirlist.end(); ++it){
            cout << *it << endl;
        }
    }

    return 0;
}

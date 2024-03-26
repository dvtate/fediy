#include <filesystem>
#include <iostream>

namespace Validate
{
    bool is_dir(const char* path) {
        struct stat s;
        if (stat(path, &s) == 0)
            return ( s.st_mode & S_IFDIR );
        perror("stat()");
        return false;
    }


    bool is_file(const char* path) {
        struct stat s;
        if (stat(path, &s) == 0)
            return ( s.st_mode & S_IFREG );
        perror("stat()");
        return false;
    }
    
} // namespace Validate

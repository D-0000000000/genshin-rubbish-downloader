#ifndef _FILE_LOADER_HPP_
#define _FILE_LOADER_HPP_

#include <string>
#include <vector>

extern struct file_path
{
    int thread_num;
    std::string decompressed_path;
    std::vector<std::string> all_file_list;
    std::vector<std::string> dl_file_list;
} fp;

bool load_file();

#endif

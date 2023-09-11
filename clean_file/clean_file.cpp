#include "file_loader/file_loader.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <set>
#include <cstdio>
#include <cstdlib>
#include <cstring>

int clean_file(int argc, char *argv[])
{
	std::set<std::string> check_clean_file;
	for (auto i : fp.all_file_list)
	{
		check_clean_file.insert(i);
	}
	std::string cwd = std::filesystem::current_path();
	uint32_t path_prefix_length = cwd.length();
	for (auto item : std::filesystem::recursive_directory_iterator(std::filesystem::current_path()))
	{
		if (item.is_directory())
		{
			continue;
		}
		if (item.path().parent_path() == cwd)
		{
			continue;
		}
		std::string cf = item.path().native();
		cf.erase(0, path_prefix_length + 1);
		if (check_clean_file.find(cf) != check_clean_file.end())
		{
			std::cout << "Found exist file " << cf << "\n";
		}
		else
		{
			std::cout << "Cannot find file " << cf << " in file list. Trying to remove\n";
			std::filesystem::remove(item);
		}
	}
	return 0;
}

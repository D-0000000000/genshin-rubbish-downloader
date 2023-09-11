#include <bits/stdc++.h>
#include "download_file/dl_file.hpp"
#include "clean_file/clean_file.hpp"
#include "verify_file/verify_file.hpp"
#include "file_loader/file_loader.hpp"

int main(int argc, char *argv[])
{
	if (argc > 1)
	{
		if (!load_file())
		{
			std::cout << "Configurations load failed\n";
			return 0;
		}
		if (strcmp(argv[1], "clean") == 0)
		{
			clean_file(argc, argv);
		}
		else if (strcmp(argv[1], "download") == 0)
		{
			download_file(argc, argv);
		}
		else if (strcmp(argv[1], "verify") == 0)
		{
			verify_file_ns::verify_file(argc, argv);
		}
		else
		{
			std::cout << "downloader clean\n";
			std::cout << "downloader download\n";
			std::cout << "downloader verify\n";
		}
	}
	else
	{
		std::cout << "downloader clean\n";
		std::cout << "downloader download\n";
		std::cout << "downloader verify\n";
	}
	return 0;
}

#include <bits/stdc++.h>
#include "download_file/dl_file.hpp"
#include "clean_file/clean_file.hpp"

int main(int argc, char *argv[])
{
	if (argc > 2)
	{
		if (strcmp(argv[1], "clean") == 0)
		{
			clean_file(argc, argv);
		}
		else if (strcmp(argv[1], "download") == 0)
		{
			download_file(argc, argv);
		}
		else
		{
			std::cout << "downloader clean <list>\n";
			std::cout << "downloader download <list> <thread> <url>\n";
		}
	}
	else
	{
		std::cout << "downloader clean <list>\n";
		std::cout << "downloader download <list> <thread> <url>\n";
	}
	return 0;
}

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>

std::queue<std::pair<std::string, std::string>> file_list;
std::vector<std::thread> thread_pool;
std::thread logger_thread;
std::thread main_download_thread;
std::mutex mtx;
std::atomic_int32_t file_completed;
int32_t file_total;

void curl_download_file(std::string url, std::string file_name)
{
	std::string cmdline = "curl --create-dirs -s " + url + file_name + " -o " + file_name;
	system(cmdline.c_str());
	file_completed++;
	return;
}

void download()
{
	while (1)
	{
		mtx.lock();
		if (file_list.empty())
		{
			mtx.unlock();
			break;
		}
		auto file_name = file_list.front();
		file_list.pop();
		mtx.unlock();
		curl_download_file(file_name.first, file_name.second);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	return;
}

void download_logger()
{
	auto tt = std::chrono::steady_clock::now();
	std::cout << "\n";
	while (1)
	{
		std::cout << "[" << file_completed << "/" << file_total << "]\n";
		tt += std::chrono::seconds(1);
		if (file_completed == file_total)
		{
			std::cout << "\nDownload finish\n";
			break;
		}
		std::this_thread::sleep_until(tt);
	}
	return;
}

void download_init(int32_t num_thread)
{
	file_completed = 0;
	logger_thread = std::thread(download_logger);
	thread_pool.resize(num_thread);
	for (int i = 0; i < num_thread; i++)
	{
		thread_pool[i] = std::thread(download);
	}
	return;
}

void wait_for_complete(int32_t num_thread)
{
	for (int i = 0; i < num_thread; i++)
	{
		thread_pool[i].join();
	}
	logger_thread.join();
	return;
}

int download_file(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cout << "downloader <url> <download thread number>\n";
		std::cout << "downloader download <list> <thread> <url>\n";
		return 1;
	}
	std::fstream cfgin;
	std::string download_url = argv[4];
	int32_t num_thread = atoi(argv[3]);
	num_thread = std::max(1, num_thread);
	num_thread = std::min(4096, num_thread);
	cfgin.open(argv[2]);
	if (cfgin.fail())
	{
		std::cout << strerror(errno) << "\n";
		return 1;
	}
	std::cout << "Using file list: " << argv[2] << "\n";
	std::cout << "Download connection: " << num_thread << "\n";
	while (!cfgin.eof())
	{
		std::string file_name;
		std::getline(cfgin, file_name);
		if (file_name == "")
		{
			continue;
		}
		file_list.push(std::make_pair(download_url, file_name));
	}
	cfgin.close();
	file_total = file_list.size();
	download_init(num_thread);
	wait_for_complete(num_thread);
	return 0;
}

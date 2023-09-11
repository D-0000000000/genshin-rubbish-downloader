#include "file_loader/file_loader.hpp"
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

// std::queue<std::pair<std::string, std::string>> file_list;
std::queue<std::string> file_list;
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
		curl_download_file(fp.decompressed_path, file_name);
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
	std::cout << "Download connection: " << fp.thread_num << "\n";
	for (auto i : fp.dl_file_list)
	{
		file_list.push(i);
	}
	file_total = file_list.size();
	download_init(fp.thread_num);
	wait_for_complete(fp.thread_num);
	return 0;
}

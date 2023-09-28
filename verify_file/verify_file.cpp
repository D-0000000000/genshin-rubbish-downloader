#include "verify_file.hpp"
#include "checker/checker.hpp"
#include "file_loader/file_loader.hpp"
#include "download_file/dl_file.hpp"
#include "json/nlohmann/json.hpp"
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <iostream>

namespace verify_file_ns
{
	std::queue<std::string> file_list;
	std::vector<std::thread> thread_pool;
	std::thread logger_thread;
	std::thread main_download_thread;
	std::mutex mtx, fp_mtx;
	std::atomic_int32_t file_completed, file_update;
	int32_t file_total;

	void verify_mainthread()
	{
		while (1)
		{
			mtx.lock();
			if (file_list.empty())
			{
				mtx.unlock();
				break;
			}
			auto jfile_attr = file_list.front();
			file_list.pop();
			mtx.unlock();
			nlohmann::json file_attr = nlohmann::json::parse(jfile_attr);
			std::string file_name = file_attr["remoteName"];
			std::string md5 = file_attr["md5"];
			size_t file_size = file_attr["fileSize"];
			file_checker fc(file_name, file_size);
			fc.md5_digest();
			if (!fc.check_digest(md5, 16))
			{
				fp_mtx.lock();
				fp.dl_file_list.push_back(file_name);
				fp_mtx.unlock();
				file_update++;
			}
			file_completed++;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		return;
	}

	void verify_logger()
	{
		auto tt = std::chrono::steady_clock::now();
		std::cout << "\n";
		while (1)
		{
			std::cout << "[" << file_completed << "/" << file_total << "]\n";
			tt += std::chrono::seconds(1);
			if (file_completed == file_total)
			{
				std::cout << "\nVerify finish\n";
				std::cout << "Waiting for download " << file_update << "\n";
				break;
			}
			std::this_thread::sleep_until(tt);
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

	int verify_file(int argc, char *argv[])
	{
		load_file();
		for (auto i : fp.all_file_list)
		{
			file_list.push(i);
		}
		file_total = file_list.size();
		fp.dl_file_list.clear();
		logger_thread = std::thread(verify_logger);
		thread_pool.resize(fp.thread_num);
		for (int i = 0; i < fp.thread_num; i++)
		{
			thread_pool[i] = std::thread(verify_mainthread);
		}
		wait_for_complete(fp.thread_num);
		download_file(0, NULL);
		return 0;
	}
}
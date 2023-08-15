#include <bits/stdc++.h>

std::queue<std::pair<std::string, std::string>> file_list;
std::vector<std::thread> thread_pool;
std::thread main_download_thread;
std::mutex mtx;

void download_file(std::string url, std::string file_name)
{
	std::cout << file_name << "\n";
	std::string cmdline = "curl --create-dirs -s " + url + file_name + " -o " + file_name;
	system(cmdline.c_str());
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
		download_file(file_name.first, file_name.second);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	return;
}

void download_init(uint32_t num_thread)
{
	thread_pool.resize(num_thread);
	for (int i = 0; i < num_thread; i++)
	{
		thread_pool[i] = std::thread(download);
	}
	return;
}

void wait_for_complete(uint32_t num_thread)
{
	for (int i = 0; i < num_thread; i++)
	{
		thread_pool[i].join();
	}
	return;
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cout << "downloader <url> <download thread number>" << std::endl;
		return 0;
	}
	std::fstream cfgin;
	std::string download_url = argv[1];
	uint32_t num_thread = atoi(argv[2]);
	cfgin.open("download_list");
	while (!cfgin.eof())
	{
		std::string file_name;
		std::getline(cfgin, file_name);
		file_list.push(std::make_pair(download_url, file_name));
	}
	cfgin.close();
	download_init(num_thread);
	wait_for_complete(num_thread);
	return 0;
}

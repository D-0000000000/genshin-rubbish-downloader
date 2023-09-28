#include "file_loader.hpp"
#include "json/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <vector>

std::vector<std::string> pkg_name;
nlohmann::json resource_json;
nlohmann::json config_json;
file_path fp;
std::string channel;

bool load_json(char *file_name, nlohmann::json &json)
{
	std::fstream fjson;
	try
	{
		fjson.open(file_name, std::ios_base::in);
		if (fjson.fail())
		{
			std::cout << strerror(errno) << std::endl;
			fjson.close();
			throw 1002;
		}
	}
	catch (const int &ex)
	{
		// load fail
		return false;
	}

	try
	{
		std::string json_str = std::string(std::istreambuf_iterator<char>(fjson), std::istreambuf_iterator<char>());
		// std::cout << json_str << std::endl;
		fjson.close();
		if (json_str.length() < 2)
		{
			throw 1001;
		}
		json = nlohmann::json::parse(json_str);
	}
	catch (const int &ex)
	{
		return false;
	}

	return true;
}

bool load_resource_json()
{
	try
	{
		fp.thread_num = config_json["thread"];
		fp.thread_num = std::max(1, fp.thread_num);
		fp.thread_num = std::min(256, fp.thread_num);
		std::string resource_url = config_json["url_json"];
		channel = "game";
		channel = config_json["channel"];
		pkg_name = config_json["pkg"];
		for (auto i : pkg_name)
		{
			std::cout << i << std::endl;
		}
		std::cout << resource_url << std::endl;
		std::string cmdline = "curl --create-dirs -s -o resource.json \"" + resource_url + "\"";
		system(cmdline.c_str());
		if (!load_json((char *)"resource.json", resource_json))
		{
			throw 1003;
		}
		fp.decompressed_path = resource_json["data"][channel]["latest"]["decompressed_path"];
		if (fp.decompressed_path.back() != '/')
		{
			fp.decompressed_path.push_back('/');
		}
	}
	catch (const int &ex)
	{
		std::cout << "json\n";
		return false;
	}
	return true;
}

bool load_all_file_list()
{
	fp.all_file_list.clear();
	fp.dl_file_list.clear();
	for (auto i : pkg_name)
	{
		std::string cmdline = "curl --create-dirs -s -O \"" + fp.decompressed_path + i + "\"";
		system(cmdline.c_str());
		std::fstream fin(i, std::ios_base::in);
		if (!fin.is_open())
		{
			fin.close();
			return false;
		}
		while (!fin.eof())
		{
			std::string ss;
			std::getline(fin, ss);
			if (ss.length() < 2)
			{
				continue;
			}
			fp.all_file_list.push_back(ss);
			auto file_attr = nlohmann::json::parse(ss);
			std::string file_name = file_attr["remoteName"];
			fp.dl_file_list.push_back(file_name);
		}
		fin.close();
	}
	return true;
}

bool load_file()
{
	fp.thread_num = 1;
	bool ret = true;
	load_json((char *)"config.json", config_json);
	ret &= load_resource_json();
	ret &= load_all_file_list();
	std::cout << "Channel: " << channel << std::endl;
	std::cout << fp.decompressed_path << std::endl;
	std::cout << fp.thread_num << std::endl;
	return ret;
}

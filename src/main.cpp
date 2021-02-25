/* Copyright (c) 2019, isaponsoft (Isao Shibuya) All rights reserved. *
 * Use of this source code is governed by a BSD-style  license that   *
 * can be found in the LICENSE file.                                  */
#include <amtrs/amtrs.hpp>
#include <amtrs/filesystem.hpp>
#include <amtrs/string.hpp>
#include <filesystem>
#include <iostream>

using namespace amtrs;
namespace	fs = amtrs::filesystem;


void update(std::string const& _path);
bool has_license(std::string const& _data);
std::string override_license(std::string_view& _block, std::string const& _data);
void usage();


std::string					license;
std::vector<std::string>	filters;
std::vector<std::string>	without;
bool						testMode		= false;
bool						listupUnmatch	= false;
bool						printModified	= false;
bool						printUnmodified	= false;
bool						printLicense	= false;

int main(int _argc, char** _args)
{
	if (_argc < 3)
	{
		usage();
		return	1;
	}

	std::string	licensefile	= _args[1];
	std::string	directory	= _args[2];

	if (_argc > 3)
	{
		for (int i = 3; i < _argc; )
		{
			std::string	cmd(_args[i++]);
			if (cmd == "--filter")
			{
				if (i == _argc)
				{
					std::cout << "--filter has not patturns." << std::endl;
					return	1;
				}
				for (auto p : split_chars(_args[i++], ";"))
				{
					filters.push_back(std::string(p));
				}
			}
			else if (cmd == "--test")
			{
				testMode		= true;
			}
			else if (cmd == "--listup-unmatch")
			{
				listupUnmatch	= true;
			}
			else if (cmd == "--print-modified")
			{
				printModified	= true;
			}
			else if (cmd == "--print-unmodified")
			{
				printUnmodified	= true;
			}
			else if (cmd == "--print-license")
			{
				printLicense	= true;
			}
			else if (cmd == "--without-directory")
			{
				if (i == _argc)
				{
					std::cout << "--withou-directoryt has not patturns." << std::endl;
					return	1;
				}
				for (auto p : split_chars(_args[i++], ";"))
				{
					without.push_back(std::string(p));
				}
			}
			else if (cmd == "--help")
			{
				usage();
				return	0;
			}
			else
			{
				std::cout << "Unknown command \"" << cmd << "\"." << std::endl;
				return	1;
			}
		}
	}

	// default filters.
	if (filters.empty())
	{
		filters.push_back("*.cpp");
		filters.push_back("*.c");
		filters.push_back("*.hpp");
		filters.push_back("*.h");
		filters.push_back("*.java");
		filters.push_back("*.mm");
		filters.push_back("*.m");
	}

	if (without.empty())
	{
		without.push_back("*.hg");
		without.push_back("*.git");
		without.push_back("*.svn");
	}

	license	= fs::file_get_contents<std::string>(licensefile);
	if (license.empty())
	{
		std::cout << "License file \"" << licensefile << "\" is empty." << std::endl;
		return	1;
	}

	update(directory);

	return	0;
}



void update(std::string const& _path)
{
	for (auto& f : fs::directory_iterator(_path))
	{
		auto	name	= std::string(trim(f.path().string().substr(_path.size()), "/\\"));
		if (std::filesystem::is_directory(f))
		{
			bool	match	= false;
			for (auto& patturn : without)
			{
				if (wcmatch(name, patturn))
				{
					match	= true;
					break;
				}
			}
			if (match)
			{
				continue;
			}
			update(f.path().string());
			continue;
		}

		bool	match	= false;
		for (auto& patturn : filters)
		{
			if (wcmatch(name, patturn))
			{
				match	= true;
				break;
			}
		}
		if (!match)
		{
			if (listupUnmatch)
			{
				std::cout << f.path().string() << std::endl;
			}
			continue;
		}
		if (listupUnmatch)
		{
			continue;
		}

		auto	src		= fs::file_get_contents<std::string>(f.path().string());
		if (src.empty())
		{
			continue;
		}

		std::string_view	block;
		auto				dest	= override_license(block, src);
		if (dest != src)
		{
			if (printModified)
			{
				std::cout << f.path().string() << std::endl;
			}
			if (!testMode)
			{
				fs::file_put_contents<std::string>(f.path().string(), dest);
			}
		}
		else
		{
			if (printUnmodified)
			{
				std::cout << f.path().string() << std::endl;
			}
		}
		if (printLicense && !block.empty())
		{
			std::cout << f.path().string() << " : (" << (block.data() - src.data()) << "," << block.size() << ")" << std::endl;
			std::cout << block << std::endl;
		}
	}
}




std::string_view first_comment_block(std::string_view _data)
{
	auto		lines	= split_chars(_data, "\n\r");
	auto		it		= lines.begin();
	int			match	= 0;
	char const*	begin	= nullptr;
	for ( ; it != lines.end(); ++it)
	{
		auto	line	= trim(*it, "\r\n\t ");
		if (starts_with(line, "//"))
		{
			match	= 1;
			begin	= _data.data() + (line.data() - _data.data());
			break;
		}
		if (auto pos = line.find("/*"); pos != std::string_view::npos)
		{
			match	= 2;
			begin	= _data.data() + (line.data() - _data.data()) + pos;
			break;
		}
	}
	// Nothing comment.
	if (match == 0)
	{
		return	_data.substr(0, 0);
	}

	std::string_view	block(begin, 0);
	for ( ; it != lines.end(); ++it)
	{
		auto	line	= trim(*it, "\r\n\t ");
		if (match == 2)
		{
			if (line.find("*/") != std::string_view::npos)
			{
				match = 0;
			}
			block	= std::string_view(block.data(), line.data() + line.size() - block.data());
			continue;
		}
		if (starts_with(line, "//"))
		{
			block	= std::string_view(block.data(), line.data() + line.size() - block.data());
			continue;
		}
		break;
	}
	if (match == 2)
	{
		return	_data.substr(0, 0);
	}


	// \n & \r
	if (it != lines.end())
	{
		for ( ; it != lines.end(); ++it)
		{
			auto	line	= trim(*it, "\r\n\t ");
			if (!line.empty())
			{
				block	= std::string_view(block.data(), line.data() - block.data());
				break;
			}
		}
		if (it == lines.end())
		{
			block	= std::string_view(block.data(), (*it).data() - block.data() + 1);
		}
	}
	return	block;
}




std::string override_license(std::string_view& _block, std::string const& _data)
{
	auto	block	= first_comment_block(_data);
	_block	= block;
	if (block.empty() || lowercase<std::string>(block).find("copyright") == std::string_view::npos)
	{
		// insert comment block
		if (_data.size() >= 3 && (uint8_t)_data[0] == 0xEF && (uint8_t)_data[1] == 0xBB && (uint8_t)_data[2] == 0xBF)
		{
			return	_data.substr(0,3) + license + _data.substr(3);
		}
		else
		{
			return	license + _data;
		}
	}
	else
	{
		// rewrite comment block
		std::string	before	(_data.substr(0, block.data() - _data.data()));
		std::string	after	(block.data() + block.size());
		return	before + license + after;
	}
}

void usage()
{
	std::cout << "addlicense LICENSE.TXT scan-directory [Options...]" << std::endl;
	std::cout << R"(
  Options
    --filter "*.cpp;*.c;*.hpp;*.h;*.java"
        file name filter.

    --test
        not modified files. test only.

    --listup-unmatch
        listup filter unmatch files.

    --print-modified
        print modified files.

    --print-unmodified
        print not modified files.

    --print-license
        ptrint license text in files.

    --whithout-directory "*.hg;*.git;*.svn"
        whitout directory
)";
}

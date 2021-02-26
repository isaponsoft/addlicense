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

enum class	bommode
{
	none,
	remove,
	add,
};


bool updatefile(std::string const& _path);
bool updatedir(std::string const& _path);
bool has_license(std::string const& _data);
bool has_bom(std::string_view _txt);
std::string override_license(std::string_view& _block, std::string const& _data);
void usage();


std::string					license;
std::string					retcode;
std::vector<std::string>	filters;
std::vector<std::string>	without;
bool						testMode		= false;
bool						listupUnmatch	= false;
bool						printLicense	= false;
bommode						bomMode			= bommode::none;

int main(int _argc, char** _args)
{
	int	i	= 1;
	if (_argc == 1)
	{
		usage();
		return	0;
	}


	std::vector<std::string>	files;
	while (i < _argc)
	{
		std::string	cmd(_args[i++]);
		if (cmd == "--license")
		{
			if (i == _argc)
			{
				std::cout << "--license has not filename." << std::endl;
				return	1;
			}
			std::string	licensefile	= _args[i++];
			license	= fs::file_get_contents<std::string>(licensefile);
			if (license.empty())
			{
				std::cout << licensefile << " is empty or can't read." << std::endl;
				return	1;
			}
		}
		else if (cmd == "--filter")
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
		else if (cmd == "--return")
		{
			if (i == _argc)
			{
				std::cout << "--return has not return code." << std::endl;
				return	1;
			}
			for (char const* c = _args[i++]; *c; ++c)
			{
				     if (*c == 'n') { retcode += "\n"; }
				else if (*c == 'r') { retcode += "\r"; }
				else
				{
					std::cout << "--return unkown return code" << std::endl;
					return	1;
				}
			}
		}
		else if (cmd == "--test")
		{
			testMode		= true;
		}
		else if (cmd == "--bom")
		{
			if (i == _argc)
			{
				std::cout << "--bom has not mode." << std::endl;
				return	1;
			}
			std::string	mode	= _args[i++];
			if (mode == "add")
			{
				bomMode	= bommode::add;
			}
			else if (mode == "remove")
			{
				bomMode	= bommode::remove;
			}
			else
			{
				std::cout << "--bom unknown mode \"" << mode << "\"." << std::endl;
				return	1;
			}
		}
		else if (cmd == "--listup-unmatch")
		{
			listupUnmatch	= true;
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
		else if (starts_with(cmd, "--"))
		{
			std::cout << "Unknown command \"" << cmd << "\". --help is show help." << std::endl;
			return	1;
		}
		else
		{
			files.push_back(std::move(cmd));
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

	for (auto& filename : files)
	{
		if (std::filesystem::is_directory(filename))
		{
			if (!updatedir(filename))
			{
				return	1;
			}
		}
		else if (!updatefile(filename))
		{
			return	1;
		}
	}
	return	0;
}

bool updatefile(std::string const& _path)
{
	std::string	src;
	if (!fs::file_get_contents<std::string>(src, _path))
	{
		std::cout << "Can't open " << _path << std::endl;
		return	false;
	}

	std::cout <<_path << " ";

	std::string_view	block;
	auto				dest	= override_license(block, src);
	if (bomMode == bommode::add)
	{
		if (!has_bom(dest))
		{
			char	bom[3] = { (char)0xEF, (char)0xBB, (char)0xBF };
			dest	= std::string(bom, 3) + dest;
		}
	}
	else if (bomMode == bommode::remove)
	{
		if (has_bom(dest))
		{
			dest	= dest.substr(3);
		}
	}

	if (!retcode.empty())
	{
		std::vector<std::string_view>	lines;
		auto*	cur	= dest.data();
		auto*	end	= dest.data() + dest.size();
		while (cur != end)
		{
			auto	linestart	= cur;
			while ((cur != end) && *cur != '\n' && *cur != '\r')
			{
				++cur;
			}
			lines.push_back(std::string_view(linestart, cur - linestart));
			if (cur != end)
			{
				auto	c	= *cur++;
				if (cur != end && (*cur == '\n' || *cur == '\r') && *cur != c)
				{
					++cur;
				}
			}
		}
		lines.push_back(std::string_view(cur, 0));
		dest	= join<std::string>(lines.begin(), lines.end(), retcode);
	}

	if (dest != src)
	{
		std::cout <<_path << " update";
		if (!testMode)
		{
			fs::file_put_contents<std::string>(_path, dest);
		}
	}
	else
	{
		std::cout <<_path << " skip";
	}
	if (printLicense)
	{
		std::cout << " : (" << (block.data() - src.data()) << "," << block.size() << ")" << std::endl;
		std::cout << block;
	}
	std::cout << std::endl;
	return	true;
}



bool updatedir(std::string const& _path)
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
			if (!updatedir(f.path().string()))
			{
				return	false;
			}
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

		if (!updatefile(f.path().string()))
		{
			return	false;
		}
	}
	return	true;
}


std::string_view first_comment_block(std::string_view _data)
{
	auto*	end		= _data.data() + _data.size();

	// find comment block.
	auto*	beg		= _data.data();
	while (beg != end)
	{
		std::string_view	txt(beg, end - beg);
		if (starts_with(txt, "/*") || starts_with(txt, "/*"))
		{
			break;
		}
		++beg;
	}
	if (beg == end)
	{
		return	_data.substr(0, 0);
	}

	// search un-comment block.
	auto*	tail	= beg + 2;
	int		mode	= starts_with(std::string_view(beg, end - beg), "/*") ? 1 : 2;
	while (tail != end)
	{
		std::string_view	txt(tail, end - tail);
		if (mode == 1)
		{
			if (starts_with(txt, "*/"))
			{
				mode	=  0;
				tail	+= 2;
				continue;
			}
		}
		else if (mode == 2)
		{
			if (*tail == '\n' || *tail == '\r')
			{
				mode	=  0;
				tail	+= 1;
				continue;
			}
		}
		else if (*tail != '\n' && *tail != '\r' && *tail != ' ' && *tail != '\t')
		{
			break;
		}
		++tail;
	}
	
	return	std::string_view(beg, tail - beg);
}




std::string override_license(std::string_view& _block, std::string const& _data)
{
	auto	block	= first_comment_block(_data);
	_block	= block;
	if (block.empty() || lowercase<std::string>(block).find("copyright") == std::string_view::npos)
	{
		// insert comment block
		if (has_bom(_data))
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


bool has_bom(std::string_view _txt)
{
	return	_txt.size() >= 3 && (uint8_t)_txt[0] == 0xEF && (uint8_t)_txt[1] == 0xBB && (uint8_t)_txt[2] == 0xBF;
}


void usage()
{
	std::cout << "addlicense LICENSE.TXT [Options...] scan-file..." << std::endl;
	std::cout << R"(
  Options
    --license LICENSE-FILE
       rewrite license to LICENSE-FILE file

    --filter "*.cpp;*.c;*.hpp;*.h;*.java"
        file name filter.

    --test
        not modified files. test only.

    --listup-unmatch
        listup filter unmatch files.

    --print-license
        ptrint license text in files.

    --whithout-directory "*.hg;*.git;*.svn"
        whitout directory from directory recursive mode.

    --bom [add|remove]
        add bom or remove bom

    --return [n|r|nr|rn]
        Align the line feed code.
        n = LF (Unix), rn = CRLF(Win), r = CR(mac)
)";
}

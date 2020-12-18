#include "stdafx.h"
#include "logger.h"

logger_t::logger_t(std::string _file_path_) : file_path(_file_path_)
{
	if (file_path.length() > 0) init();
}

logger_t::~logger_t()
{
	if (file.is_open())
	{
		file.close();
	}
}

void logger_t::init()
{
	if (file.is_open()) this->~logger_t();
	return file.open(file_path.c_str());
}

void logger_t::log(std::string str, ...)
{
	char buffer[512];
	va_list ap;
	va_start(ap, str);
	sprintf_s(buffer, 512, str.c_str(), ap);
	va_end(ap);
	file << buffer << std::endl;
}

void logger_t::log_hex(std::string var_name, std::size_t var_val)
{
	file << var_name << " = 0x" << std::hex << var_val << std::dec << std::endl;
}
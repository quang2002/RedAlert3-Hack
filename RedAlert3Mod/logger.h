#ifndef __LOGGER_H__
#define __LOGGER_H__

#pragma once
#include <string>
#include <fstream>
#include <cstdarg>

class logger_t
{
	std::string file_path;
	std::ofstream file;
public:
	logger_t(std::string _file_path_ = "");

	~logger_t();

	void init();

	void log(std::string str, ...);

	void log_hex(std::string var_name, std::size_t var_val);

	template<typename _T>
	void log_var(std::string var_name, _T var_val);

	template<typename _T>
	void log_array(std::string arr_name, _T* arr, std::size_t arr_size);
};

#endif // !__LOGGER_H__

template<typename _T>
inline void logger_t::log_var(std::string var_name, _T var_val)
{
	file << var_name << " = " << var_val << std::endl;
}

template<typename _T>
inline void logger_t::log_array(std::string arr_name, _T* arr, std::size_t arr_size)
{
	file << arr_name << ":\n";
	for (std::size_t i = 0; i < arr_size; ++i)
	{
		file << "|\t" << arr_name << "[" << i << "] = " << arr[i] << std::endl;
	}
}

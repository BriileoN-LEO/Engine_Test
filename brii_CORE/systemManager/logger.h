#ifndef LOGGER_H
#define LOGGER_H 

#include <iostream>
#include <string>
#include <string_view>
#include <source_location>


class logger
{
  private: 
  std::string nameLogger{};
  uint64_t error_counter{};

  public:

  explicit logger(std::string_view name) : nameLogger(name){};
	 
  void success(std::string_view success_message);
  void error(std::string_view error_message, const std::source_location location = std::source_location::current());
  void warning(std::string_view warning_message, const std::source_location location = std::source_location::current());
  void info(std::string_view info_message);
};

namespace log_System
{
extern logger fileLogger;
extern logger dataLogger;
extern logger materialCooker_logger;
extern logger meshCooker_logger;
extern logger modelCooker_logger;
extern logger assimp_logger;
}

#endif

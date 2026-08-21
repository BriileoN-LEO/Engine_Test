#ifndef LOGGER_H
#define LOGGER_H 

#include <iostream>
#include <string>
#include <string_view>
#include <source_location>

enum class BE_log_T : uint32_t
{
 PLATFORM = 0
};

struct BE_logContext
{
 BE_log_T log_T{};
 std::string name_log_T{};
};

class BE_log
{
 private:
 ///HERE SAVE A VECTOR OF BE_logContext 
 ///Think more about how to implement this logger in a better way
 ///Change the logs from files:
 //-Platform/Private/Platform_SDL/SDL3_window.cpp 

 public:

};


class logger
{
  private: 
  std::string nameLogger{}; //change for a uint64_t to get the name of the logger in external data 
  uint64_t error_counter{};

  public:

  explicit logger(std::string_view name) : nameLogger(name){};

  void change_nameLogger(std::string nameLogger); ///REMEMBER CHANGE THIS IN THE FUTURE
   
  void success(std::string_view success_message);
  void error(std::string_view error_message, const std::source_location location = std::source_location::current());
  void warning(std::string_view warning_message, const std::source_location location = std::source_location::current());
  void info(std::string_view info_message);
};

namespace log_System
{
extern logger fileLogger;
extern logger dataLogger;
extern logger texturesCooker_logger;
extern logger materialCooker_logger;
extern logger meshCooker_logger;
extern logger modelCooker_logger;
extern logger assimp_logger;
}

namespace log_SystemEngine
{
extern logger meshEngine_log;
extern logger modelEngine_log;
}



#endif

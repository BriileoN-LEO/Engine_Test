
#include "logger.h"

  void logger::success(std::string_view success_message)
  {
    std::cout << "[" << nameLogger << "]" << "[SUCCESS]" << success_message << "\n";
  }

  void logger::error(std::string_view error_message, const std::source_location location)
  {
    std::cerr << "[" << nameLogger << "]" << "[ERROR] " << error_message << " | " << location.file_name() << " : " << location.line() << " | " << location.function_name()<<"\n";
    error_counter++;
  }

  void logger::warning(std::string_view error_message, const std::source_location location)
  {
    std::cerr << "[" << nameLogger << "]" << "[WARNING] " << error_message << " | " << location.file_name() << " : " << location.line() << " | " << location.function_name()<<"\n";
  }

  void logger::info(std::string_view info_message)
  {
   std::cout << "[" << nameLogger << "]" << "[INFO]" << info_message << "\n";
  }



namespace log_System
{
 logger fileLogger{"FILE_LOGGER"};
 logger dataLogger{"DATA_LOGGER"};
 logger materialCooker_logger{"MATERIAL_COOKER_LOGGER"};
 logger meshCooker_logger{"MESH_COOKER_LOGGER"};
 logger modelCooker_logger{"MODEL_COOKER_LOGGER"};
 logger assimp_logger{"ASSIMP_LOGGER"};

}




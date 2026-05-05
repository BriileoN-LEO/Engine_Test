//
// Created by brii on 03/04/26.
//

#ifndef LEARNING_FILES_DATAMANAGER_H
#define LEARNING_FILES_DATAMANAGER_H

#include "learningOpenGL.h"

namespace filesData
{
    std::string read_FileData_str_(const char* file_path, int line, const char* file);
#define read_FileData_str(file_path) read_FileData_str_(file_path, __LINE__, __FILE__)   //////////CREO QUE LA FUNCION NO RETORNA EL VALOR
}

#endif //LEARNING_FILES_DATAMANAGER_H
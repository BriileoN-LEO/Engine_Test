#include "manager_briiCooker.h"
#include "files_CoreManager/files_Core.h"

int main()
{
 //=======INIT THE DATA UTILITIES======= --- THE CLASES OF CONTAINERS OF ALL DATA TO BINAR}Y
 data_leoBinary::init_dataUtilities_Cooker();

 /////////CREATION FILES/////
 bool dirE_tex{};
 filesystem_manager::create_DirectoryFile(manager_GD::pathTextures, dirE_tex);


 return 0;
}

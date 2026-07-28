#include "manager_briiCooker.h"
#include "files_CoreManager/files_Core.h"
#include "dataCooker.h"

int main()
{
 dataCooker_start::create_directoriesCooker();
 dataCooker_start::insert_ModelsCooker();
 dataCooker_start::create_Binaries();

 return 0;
}

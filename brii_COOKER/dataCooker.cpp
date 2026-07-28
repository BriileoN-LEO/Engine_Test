
#include "dataCooker.h"
#include "manager_briiCooker.h"
#include "Render/configFilesTXT.h"


namespace dataCooker_start
{
  uint32_t create_directoriesCooker()
  {
 /////////CREATION FILES/////
   uint32_t dirE_tex{};
   filesystem_manager::create_DirectoryFile(manager_GD::pathModels, dirE_tex);
   filesystem_manager::create_DirectoryFile(manager_GD::pathMeshes, dirE_tex);
   filesystem_manager::create_DirectoryFile(manager_GD::pathMaterials, dirE_tex);
   filesystem_manager::create_DirectoryFile(manager_GD::pathTextures, dirE_tex);

   return dirE_tex;
  }

  void insert_ModelsCooker()
  {
   data_leoBinary::init_dataUtilities_Cooker();
   
   unsigned int aiProcessFlags{ aiProcess_Triangulate
		| aiProcess_FlipUVs
		| aiProcess_ImproveCacheLocality
	//	| aiProcess_CalcTangentSpace
		| aiProcess_GenSmoothNormals
		//| aiProcess_GenNormals
		| aiProcess_SortByPType 
		| aiProcess_JoinIdenticalVertices
	//	| aiProcess_PreTransformVertices
   };


   data_leoBinary::loadModel(sponza_2_model, aiProcessFlags, 1);
   data_leoBinary::loadModel(backpack_Model, aiProcessFlags, 1);
   data_leoBinary::loadModel(floor2_Model, aiProcessFlags, 1);
   data_leoBinary::loadModel(flashLight_Model, aiProcessFlags, 1);
   data_leoBinary::loadModel(campo_01, aiProcessFlags, 1);
   data_leoBinary::loadModel(vegetacion_01, aiProcessFlags, 1);
  }

  void create_Binaries()
  {
    file_OP::writeFlags flagsBin {file_OP::writeFlags::OVERWRITE};

    data_leoBinary::create_ModelBinaries(manager_GD::pathModels, flagsBin);
    data_leoBinary::create_MeshBinaries(manager_GD::pathMeshes, flagsBin);  
    data_leoBinary::create_MaterialBinaries(manager_GD::pathMaterials, flagsBin);
  }

}



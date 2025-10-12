#ifndef ScreenHit
#define ScreenHit
#include "ModelAssimp.h"
#include "Render/RenderData.h"
#include "learningOpenGL.h"

namespace stencilTest_SelectObj
{
	enum class selectObjParts
	{
		triangles = 0,
		mesh = 1,
		model = 2
	};

	extern Assimp::Model* currentSelect;
	extern bool changeStateSelection;

	std::map<std::string, glm::vec3> calculateWorldCoord_WindowPos();


}

void calculateAllScreenHit();




#endif ScreenHit
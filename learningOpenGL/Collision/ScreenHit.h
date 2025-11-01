#ifndef ScreenHit
#define ScreenHit
#include "ModelAssimp.h"
#include "CollisionAABB.h" 
#include "learningOpenGL.h"


namespace opScreenHit
{
	glm::vec3 calc_NormalPlane(AABB::triAABB vertPlane);
	float calc_T(AABB::triAABB vertPlane, glm::vec3 nearPT_screen, glm::vec3 direction_R);
	bool samePlane_Technique(AABB::triAABB tris, glm::vec3 PointCollisicion);

}

namespace ScreenCalc_Hit
{
	enum class selectObjParts
	{
		triangles = 0,
		mesh = 1,
		model = 2
	};

	//extern Assimp::Model* currentSelect; std::vector<Assimp::structModelName>
	extern std::vector<Assimp::structModelName> nameMesh_Hit;

	std::map<std::string, glm::vec3> calculateWorldCoord_WindowPos();

	void calculateIntersect_Objects(); 
	bool calc_IntersectTriangleMesh(std::vector< Assimp::structModelName>& Mesh, glm::vec3 nearPt, glm::vec3 direction_R);
	void calc_IntersectAABB();
	void calc_IntersectALL();
}




#endif ScreenHit
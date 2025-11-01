#ifndef collisionAABB
#define collisionAABB
#include "learningOpenGL.h"
#include "ModelAssimp.h"
#include "Render/RenderData.h"


namespace AABB
{
	struct boxAABB
	{
		glm::vec3 v1{};
		glm::vec3 v2{};
		glm::vec3 v3{};
		glm::vec3 v4{};
		glm::vec3 v5{};
		glm::vec3 v6{};
		glm::vec3 v7{};
		glm::vec3 v8{};
	};

	struct triAABB
	{
		glm::vec3 v1{};
		glm::vec3 v2{};
		glm::vec3 v3{};
	};

	struct numberVertSelected
	{
		int v1{};
		int v2{};
		int v3{};

	};

	class BoundingBox
	{
	public:

		Assimp::structModelName nameAABB{}; //Linkearlo con el mesh indicado
		unsigned int VAO{};
		unsigned int VBO{};

		std::map<std::string, triAABB> boxTrisAABB{};
		std::map<std::string, triAABB> boxTrisAABB_Base{};
		boxAABB box_CoordBase{};
		boxAABB box_CoordActual{};
		glm::mat4 modelAABB{ glm::mat4(1.0f) };

		std::vector<glm::vec3> vertAABB{};
		shading::shader shaderAABB{};
		
		BoundingBox();
		BoundingBox(Assimp::structModelName nameAABB, std::vector<glm::vec3> vertAABB, boxAABB box_Coord, std::map<std::string, triAABB> boxTrisAABB);
		void loadBoundingBox(Assimp::structModelName nameAABB, std::vector<glm::vec3> vertAABB, boxAABB box_Coord, std::map<std::string, triAABB> boxTrisAABB);
		void setUpBox();
		void draw();
		void destroy();

		void updatePosicion(glm::mat4& modelMesh);
		void updateBoxTrisPos(glm::mat4& modelMesh);
	};

	extern std::vector<BoundingBox> meshBoundingBox;


	std::pair<std::vector<glm::vec3>, boxAABB> calcBox(glm::vec3 minPos, glm::vec3 maxPos);
	std::map<std::string, triAABB> calcTrisAABB(boxAABB vertex);
	void create_BoundingBox_Mesh();
	void test_BoundingBos();
	void setShader_AABB();
	void updateCoordAABB_All();

}

namespace data_HitAABB
{
	//extern Assimp::structModelName nameModel_Stencil;
	extern std::pair<Assimp::structModelName, AABB::numberVertSelected> selectedObj;
	extern individualComp::singleTriangle triangleStencil;
	extern bool renderSelection;
}



#endif 
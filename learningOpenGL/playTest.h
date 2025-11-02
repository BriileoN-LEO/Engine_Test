#ifndef playTest
#define playTest

#include "SHADER_H.h"
#include "ModelAssimp.h"

namespace testPlay
{
	struct tranformationT
	{
	public:

		GLfloat ang{};
		GLfloat velocityAng{};
		GLfloat topeAng{ 4.0f };

		glm::vec3 pivotRotPos{};
		glm::vec3 posicion{};
		glm::vec3 scale{};

		bool scaleStop{ false };

		tranformationT();
		tranformationT(glm::vec3 posicion, glm::vec3 scale, glm::vec3 pivotRotPos, GLfloat ang, GLfloat velocityAng);
		void setSettingsTransform(glm::vec3 posicion, glm::vec3 scale, glm::vec3 pivotRotPos, GLfloat velocityAng);
		void transformUniqueModel(ObjCreation::ModelCreation* modelTest, camera::camera1 cam, light::light1 lightModel);
		void transformMeshLight(ObjCreation::ModelCreation* modelTest, light::light1* lightModel);
		void transformModel_test1(Assimp_D::coordModel* coordM);
		void moveModel_Test(Assimp_D::coordModel* coordM);

		void ColorSeq(ObjCreation::ModelCreation* modelTest, light::light1* lightModel);
	};

	tranformationT getTranformationT_randomPivot(glm::vec3 posicion, glm::vec3 scale, GLfloat ang, GLfloat velocityAng);
	tranformationT getTranformationT_radial(glm::vec3 posicion, glm::vec3 scale, glm::vec3 pivotRotPos, GLfloat ang, GLfloat velocityAng);
	
	const void setTransformation_Objects();
	void renderTranformations_Objects();
}

namespace camera_Transforms
{
	void attachObject_Cam(transformation_basics::basics_Model3D& N, camera::camera1& cam);
	void setAllTranforms_Cam();
}

namespace light_Transforms
{
	void spotLight_AttachLintern(light::SpotLight& spotToAttach, transformation_basics::basics_Model3D& posicionModel);	
	void setAllTranforms_light();
}

#endif playTest
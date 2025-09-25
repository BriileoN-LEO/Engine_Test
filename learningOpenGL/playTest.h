#ifndef playTest
#define playTest

#include "SHADER_H.h"

namespace testPlay
{
	struct tranformationT
	{
	public:

		GLfloat ang{};
		GLfloat velocityAng{};

		glm::vec3 pivotRotPos{};
		glm::vec3 posicion{};
		glm::vec3 scale{};

		tranformationT();
		void setSettingsTransform(glm::vec3 posicion, glm::vec3 scale, glm::vec3 pivotRotPos, GLfloat velocityAng);
		void transformUniqueModel(ObjCreation::ModelCreation* modelTest, camera::camera1 cam, light::light1 lightModel);
		void transformMeshLight(ObjCreation::ModelCreation* modelTest, light::light1* lightModel);

	};


}

#endif playTest
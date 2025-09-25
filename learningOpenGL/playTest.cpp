#include "playTest.h"

namespace testPlay
{

	tranformationT::tranformationT() {};
	void tranformationT::setSettingsTransform(glm::vec3 posicion, glm::vec3 scale, glm::vec3 pivotRotPos, GLfloat velocityAng)
	{
		this->velocityAng = velocityAng;
		this->pivotRotPos = glm::normalize(pivotRotPos);
		this->posicion = posicion;
		this->scale = scale;
	}

	void tranformationT::transformUniqueModel(ObjCreation::ModelCreation* modelTest, camera::camera1 cam, light::light1 lightModel)
	{
		ang += velocityAng;
		modelTest->setPosModelTransforms(posicion, scale, pivotRotPos, ang);
		modelTest->renderModel(cam, lightModel);
	}
}
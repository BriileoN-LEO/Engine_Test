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
	void tranformationT::transformMeshLight(ObjCreation::ModelCreation* modelTest, light::light1* lightModel)
	{
		ang += velocityAng;

		modelTest->modelCoord.translateModel(posicion);
		modelTest->modelCoord.scaleModel(scale);
		modelTest->modelCoord.setPivotRotModel(pivotRotPos);
		modelTest->modelCoord.setAngRotModel(ang);
		modelTest->modelCoord.setInverseTransformsAll();
		lightModel->Posicion = modelTest->modelCoord.translateM;
	}
}
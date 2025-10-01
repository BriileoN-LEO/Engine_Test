#include "playTest.h"

namespace testPlay
{

	tranformationT::tranformationT() {};
	tranformationT::tranformationT(glm::vec3 posicion, glm::vec3 scale, glm::vec3 pivotRotPos, GLfloat ang, GLfloat velocityAng)
	{
		setSettingsTransform(posicion, scale, pivotRotPos, velocityAng);
		this->ang = ang;
	}
	
	void tranformationT::setSettingsTransform(glm::vec3 posicion, glm::vec3 scale, glm::vec3 pivotRotPos, GLfloat velocityAng)
	{
		this->velocityAng = velocityAng;
		this->pivotRotPos = glm::normalize(pivotRotPos);
		this->posicion = posicion;
		this->scale = scale;
	}

	void tranformationT::transformUniqueModel(ObjCreation::ModelCreation* modelTest, camera::camera1 cam, light::light1 lightModel)
	{
		ang += velocityAng; // para rotar el Mesh
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
	void tranformationT::transformModel_test1(Assimp::coordModel* coordM)
	{
	ang += velocityAng;

		float max{ 0.5f };
		float min{ 2.0f };

		if (scaleStop == false)
		{
			scale.y += 0.01f;
			if (scale.y >= max)
			{
				scaleStop = true;
			}
		}
		else if (scaleStop == true)
		{
			scale.y -= 0.01f;

			if (scale.y <= min)
			{
				scaleStop = false;
			}

		}
		coordM->posicion = this->posicion;
		//ordM->scale = this->scale;
		
		if (coordM->pivotRot != this->pivotRotPos)
		{
			coordM->pivotRot = this->pivotRotPos;
		}
	
			
			coordM->angRot = ang;
			
			SDL_Log(std::to_string(ang).c_str());
		


	}
	void tranformationT::moveModel_Test(Assimp::coordModel* coordM)
	{
		const bool* stateKeys = SDL_GetKeyboardState(nullptr);

		if (stateKeys[SDL_SCANCODE_UP] == true)
		{
			coordM->posicion.z += 0.1f;

		}

		if (stateKeys[SDL_SCANCODE_DOWN] == true)
		{
			coordM->posicion.z -= 0.1f;

		}

		if (stateKeys[SDL_SCANCODE_LEFT] == true)
		{
			coordM->posicion.x -= 0.1f;

		}

		if (stateKeys[SDL_SCANCODE_RIGHT] == true)
		{
			coordM->posicion.x += 0.1f;

		}

	}

	void tranformationT::ColorSeq(ObjCreation::ModelCreation* modelTest, light::light1* lightModel)
	{

	}
}
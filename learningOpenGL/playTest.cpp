#include "playTest.h"
#include "Render/RenderData.h"

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

		//SDL_Log(std::to_string(ang).c_str());



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

	tranformationT getTranformationT_randomPivot(glm::vec3 posicion, glm::vec3 scale, GLfloat ang, GLfloat velocityAng)
	{
		glm::vec3 randomPivot{ randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) };
		return tranformationT(posicion, scale, randomPivot, ang, velocityAng);
	}

	tranformationT getTranformationT_radial(glm::vec3 posicion, glm::vec3 scale, glm::vec3 pivotRotPos, GLfloat ang, GLfloat velocityAng)
	{
		glm::vec3 posicionRand{ randomN::randomNum(-posicion.x, posicion.x),posicion.y, randomN::randomNum(-posicion.z, posicion.z) };
		pivotRotPos = glm::normalize(pivotRotPos);
		return tranformationT(posicion, scale, pivotRotPos, ang, velocityAng);
	}

	 testPlay::tranformationT testTransLight{};
	 testPlay::tranformationT Trans_pointLight02{};
	 std::vector< testPlay::tranformationT> PointLightsTrans{};
	 std::vector< testPlay::tranformationT> lightTransSet{};
	 testPlay::tranformationT testTranforms{};
	 testPlay::tranformationT testBackPack{};
	 testPlay::tranformationT testFloor{};
	
	const void setTransformation_Objects()
	{
		testTransLight = getTranformationT_randomPivot(glm::vec3(6.0f, 6.0f, 6.0f), glm::vec3(1.0f), 0.0f, 0.05f);
		Trans_pointLight02 = getTranformationT_randomPivot(glm::vec3(6.0f, 6.0f, 6.0f), glm::vec3(1.0f), 0.0f, 0.05f);
		
		std::vector< testPlay::tranformationT> pre_PL
		{
			getTranformationT_randomPivot(glm::vec3(6.0f, 6.0f, 6.0f), glm::vec3(1.0f), 0.0f, 0.05f),

			getTranformationT_randomPivot(glm::vec3(6.0f, 6.0f, 6.0f), glm::vec3(1.0f), 0.0f, 0.05f),

			getTranformationT_randomPivot(glm::vec3(6.0f, 6.0f, 6.0f), glm::vec3(1.0f), 0.0f, 0.05f),

			getTranformationT_randomPivot(glm::vec3(6.0f, 6.0f, 6.0f), glm::vec3(1.0f), 0.0f, 0.05f),

			getTranformationT_randomPivot(glm::vec3(6.0f, 6.0f, 6.0f), glm::vec3(1.0f), 0.0f, 0.05f),
		};

		PointLightsTrans = pre_PL;
		
		std::vector< testPlay::tranformationT> pre_PL_2 
		{ 
			getTranformationT_radial(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.5f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.021f),
			getTranformationT_radial(glm::vec3(9.0f, 4.0f, 9.0f), glm::vec3(0.5f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.023f),
			getTranformationT_radial(glm::vec3(15.0f, 5.0f, 15.0f), glm::vec3(0.5f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.025f),
			getTranformationT_radial(glm::vec3(21.0f, 6.0f, 21.0f), glm::vec3(0.5f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.028f),
			getTranformationT_radial(glm::vec3(27.0f, 7.0f, 27.0f), glm::vec3(0.5f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.031f),
			getTranformationT_radial(glm::vec3(33.0f, 8.0f, 33.0f), glm::vec3(0.5f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.034f),
			getTranformationT_radial(glm::vec3(39.0f, 9.0f, 39.0f), glm::vec3(0.5f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.037f),
			getTranformationT_radial(glm::vec3(45.0f, 10.0f, 45.0f), glm::vec3(0.5f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.04f),
			getTranformationT_radial(glm::vec3(51.0f, 11.0f, 51.0f), glm::vec3(0.5f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.043f)

		};

		lightTransSet = pre_PL_2;

		testTranforms = getTranformationT_randomPivot(glm::vec3(2.0f, 3.0f, 2.0f), glm::vec3(1.0f), 0.0f, 0.1f);
		testBackPack = tranformationT(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.001f);
		testFloor = tranformationT(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.001f);

	}
    void renderTranformations_Objects()
	{
		//Para transfomar la luz con el Mesh light
//testTransLight.transformMeshLight(&RenderData_Set::MeshLights_MCD[0], &RenderData_Set::pointLights_D[0]);
//Trans_pointLight02.transformMeshLight(&RenderData_Set::MeshLights_MCD[1], &RenderData_Set::pointLights_D[1]);

		/*
		for (int i = 0; i < static_cast<int>(PointLightsTrans.size()); i++)
		{
			PointLightsTrans[i].transformMeshLight(&RenderData_Set::MeshLights_MCD[i], &RenderData_Set::pointLights_D[i]);

		}
		*/
		
		for (int i = 0; i < static_cast<int>(lightTransSet.size()); i++)
		{
			lightTransSet[i].transformMeshLight(&RenderData_Set::MeshLights_MCD[i], &RenderData_Set::pointLights_D[i]);

		}
		

		//Para transforma un unico cubo de minecraft 
		testTranforms.transformUniqueModel(&RenderData_Set::ModelCreation_D["MinecraftCube"], cameras::aerialCamera, RenderData_Set::pointLights_D[0]);

		//Transformacion del backpack para que gire
		testBackPack.transformModel_test1(&RenderData_Set::AssimpModel_D["backPack"].ModelGlobal_Coord);
		RenderData_Set::AssimpModel_D["backPack"].refresh_ModelCoord();
		
		//transformacion del floor para que se mueva
		testFloor.moveModel_Test(&RenderData_Set::AssimpModel_D["Floor"].ModelGlobal_Coord);
		RenderData_Set::AssimpModel_D["Floor"].refresh_ModelCoord();
	}
}
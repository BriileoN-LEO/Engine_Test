#include "GLM_test.h"

namespace randomN
{
	inline std::mt19937 generateRand()
	{
		std::random_device rd{};
		
		std::seed_seq ss
		{
			static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
				rd(), rd(), rd(), rd(), rd(), rd(), rd()
		};
		
		return std::mt19937{ ss };
	}

	inline std::mt19937 mt { generateRand() };

	float randomNum(float NewMin, float NewMax)
	{	 
		
		std::uniform_real_distribution<float> fDist(NewMin, NewMax);
	///d::uniform_int_distribution iDist(static_cast<int>(NewMin), static_cast<int>(NewMax));	
		return fDist(mt);
	}

	glm::vec3 randomPos(glm::vec3 fitNEW_min, glm::vec3 fitNEW_max)
	{
		glm::vec3 NewPos{};
		
		NewPos.x = randomNum(fitNEW_min.x, fitNEW_max.x);
		NewPos.y = randomNum(fitNEW_min.y, fitNEW_max.y);
		NewPos.z = randomNum(fitNEW_min.z, fitNEW_max.z);

	//td::cout << NewPos.x << '\t' << NewPos.y << '\t' << NewPos.z << '\n';
		
		return NewPos;
	}

}


namespace reflectionMatrixOP
{
	glm::mat4 calcReflectMatrix(glm::vec3 cameraPos, glm::vec3 pointPlane, glm::vec3 planeNormal, glm::vec3 cameraUp, glm::vec3 cameraViewTarget)
	{
		glm::vec3 reflectedToCam{ cameraPos - pointPlane };
		reflectedToCam = glm::reflect(reflectedToCam, planeNormal);
		glm::vec3 reflectedPos{ pointPlane + reflectedToCam };

		glm::vec3 reflectedToTarget{ cameraViewTarget - pointPlane };
		reflectedToTarget = glm::reflect(reflectedToTarget, planeNormal);
		glm::vec3 reflectedTarget{ pointPlane + reflectedToTarget };

		glm::vec3 reflectedUp{ glm::reflect(cameraUp, planeNormal) };

		glm::mat4 reflectViewMatrix
		{
			glm::lookAt(reflectedPos, reflectedTarget, reflectedUp)
		};
		///CONTINUAR AQUI
		return reflectViewMatrix;
	}

	glm::mat4 calcObliqueProjection(glm::mat4 originalProjectionMat, glm::mat4 reflectMatrix, glm::vec3 planePoint, glm::vec3 planeNormal)
	{
		float w{ -glm::dot(planeNormal, planePoint) };
		glm::vec4 planeEq{ glm::vec4(planeNormal, w) };

		glm::mat4 viewInverseTranspose{ glm::transpose(glm::inverse(reflectMatrix)) };
		glm::vec4 viewSpacePlane{ viewInverseTranspose * planeEq };

		glm::vec4 q
		{
			(glm::sign(viewSpacePlane.x) + originalProjectionMat[2][0] / originalProjectionMat[0][0]),
			(glm::sign(viewSpacePlane.y) + originalProjectionMat[2][1] / originalProjectionMat[1][1]),
			-1.0f,
			(1.0f + originalProjectionMat[2][2]) / originalProjectionMat[2][3]
		};

		glm::vec4 c{ viewSpacePlane * (2.0f / glm::dot(viewSpacePlane, q)) };

		glm::mat4 obliqueProjection{ originalProjectionMat };

		obliqueProjection[0][2] = c.x;
		obliqueProjection[1][2] = c.y;
		obliqueProjection[2][2] = c.z + 1.0f;
		obliqueProjection[3][2] = c.w;

		return obliqueProjection;
	}

}

namespace transformation_basics
{

	glm::vec3 centroidObj(std::vector<glm::vec3> Obj)
	{
		GLuint numVertices = static_cast<GLuint>(Obj.size());
		std::vector<glm::vec3> middlePtVer{};

		///calculamos el punto medio de cada punto
		for (int i = 0; i < numVertices; i++)
		{
			glm::vec3 vertexL{};
			glm::vec3 vertexN{};
			if (i == numVertices - 1)
			{
				vertexL = Obj[i];
				vertexN = Obj[0];
			}
			else
			{
				vertexL = Obj[i];
				vertexN = Obj[i + 1];
			}

			glm::vec3 test1 = vertexL - vertexN;
			test1 = vertexL - (test1 * 0.5f);

			middlePtVer.emplace_back(test1);
		}

		glm::vec3 sumVer{};
		for (auto& mVer : middlePtVer)
		{
			sumVer += mVer;

		}

		sumVer /= numVertices;

		return sumVer;
	}
	float areaTriangle(std::array<glm::vec3, 3> triangle)
	{
		glm::vec3 vecAB = triangle[1] - triangle[0];
		glm::vec3 vecAC = triangle[2] - triangle[0];

		//Calculamos el producto vectorial de los dos vectores 
		float i = (vecAB.y * vecAC.z) - (vecAB.z * vecAC.y);
		float j = (vecAB.x * vecAC.z) - (vecAB.z * vecAC.x);
		float z = (vecAB.x * vecAC.y) + (vecAB.y * vecAC.x);

		//calculamos la magnitud del producto vectorial
		float magnitudVec{ static_cast<float>(sqrt(pow(i, 2) + pow(j, 2) + pow(z, 2))) };

		//calculamos el area
		float areaTriangle{ magnitudVec / 2.0f };

		return areaTriangle;

	}
	glm::vec3 centerGeo(std::vector<std::pair<glm::vec3, float>> triCenter)
	{
		glm::vec3 sumCenterArea{};
		GLfloat sumArea{};

		for (auto& tri : triCenter)
		{
			sumCenterArea += tri.first * tri.second;
			sumArea += tri.second;
		}

		sumCenterArea /= sumArea;

		return sumCenterArea;
	}
	glm::vec3 calcCenterGeo(std::vector<glm::vec3> vertex)
	{
		std::vector<std::pair<glm::vec3, float>>tris{};

		for (int i = 0; i < vertex.size(); i++)
		{
			std::vector<glm::vec3> tri
			{
				vertex[i],
				vertex[i + 1],
				vertex[i + 2]
			};
			
			glm::vec3 centroidTri{ centroidObj(tri) };

			std::array<glm::vec3, 3> arrayTri{ tri[0], tri[1], tri[2] };
			float areaTri{ areaTriangle(arrayTri) };

			tris.emplace_back(std::pair<glm::vec3, float>(centroidTri, areaTri));

			i += 2;
		}

		return centerGeo(tris);
	}

	basics_posGEO::basics_posGEO() {};
	basics_posGEO::~basics_posGEO() {};
	void basics_posGEO::detectTranslate(SDL_Event* event)
	{
		generalTranform = true;
		translateTest = true;
		keyTranslate = event->key.key;
	}
	void basics_posGEO::detectRot(SDL_Event* event)
	{
		if (event->key.key == SDLK_R)
		{
			generalTranform = true;
			rotTest = true;
			keyRot = SDLK_R;

			if (ang < 0)
			{
				ang *= -1;
			}
		}

		else if (event->key.key == SDLK_T)
		{
			generalTranform = true;
			rotTest = true;
			keyRot = SDLK_T;

			if (ang > 0)
			{
				ang *= -1;
			}
		}

	}
	void basics_posGEO::detectScale(SDL_Event* event)
	{
		if (event->key.key == SDLK_S)
		{
			generalTranform = true;
			scaleTest = true;
			keyScale = SDLK_S;
		}

		else if (event->key.key == SDLK_A)
		{
			generalTranform = true;
			scaleTest = true;
			keyScale = SDLK_A;
		}
	}
	void basics_posGEO::movePivot()
	{
		if (translateTest == true)
		{
			glm::mat4 matTranslate{ glm::mat4(1.0f) }; //matrix ident()
			glm::vec4 vec(0.0f, 0.0f, 0.0f, 0.0f);

			if (keyTranslate == SDLK_UP)
			{
				vec.y += 0.01f;
				keyTranslate = SDLK_UP;
			}

			if (keyTranslate == SDLK_DOWN)
			{
				vec.y -= 0.01f;
				keyTranslate = SDLK_DOWN;
			}

			if (keyTranslate == SDLK_LEFT)
			{
				vec.x -= 0.01f;
				keyTranslate = SDLK_LEFT;
			}

			if (keyTranslate == SDLK_RIGHT)
			{
				vec.x += 0.01f;
				keyTranslate = SDLK_RIGHT;
			}

			matTranslate = glm::translate(matTranslate, glm::vec3(vec.x, vec.y, vec.z));
			pivotPosGEO = matTranslate * pivotPosGEO;

		}

	}
	void basics_posGEO::transformBasic(std::vector<float*> posicionVec)
	{
		glm::vec4 vec(*posicionVec[0], *posicionVec[1], *posicionVec[2], 1.0f);
		glm::mat4 matTransformTranslate = glm::mat4(1.0f);
		glm::mat4 matTransformSR = glm::mat4(1.0f);

		if (translateTest == true)
		{
			glm::vec3 translate{ 0.0f, 0.0f, 0.0f };
			if (keyTranslate == SDLK_UP)
			{
				translate.y += 0.01f;
				keyTranslate = SDLK_UP;
			}

			if (keyTranslate == SDLK_DOWN)
			{
				translate.y -= 0.01f;
				keyTranslate = SDLK_DOWN;
			}

			if (keyTranslate == SDLK_LEFT)
			{
				translate.x -= 0.01f;
				keyTranslate = SDLK_LEFT;
			}

			if (keyTranslate == SDLK_RIGHT)
			{
				translate.x += 0.01f;
				keyTranslate = SDLK_RIGHT;
			}

			matTransformTranslate = glm::translate(matTransformTranslate, glm::vec3(translate.x, translate.y, translate.z));
			vec = matTransformTranslate * vec;

		}

		if (rotTest == true)
		{
			matTransformSR = glm::rotate(matTransformSR, glm::radians(ang), glm::vec3(0.0f, 0.0f, 1.0f));

		}

		if (scaleTest == true)
		{
			glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

			if (keyScale == SDLK_S)
			{
				scale.x += 0.01f;
				scale.y += 0.01f;
			}

			else if (keyScale == SDLK_A)
			{
				scale.x -= 0.01f;
				scale.y -= 0.01f;
			}

			matTransformSR = glm::scale(matTransformSR, scale);

		}

		//transformacion del vec para cambiar de angulo
		glm::vec4 posV1 = pivotPosGEO - vec;
		posV1 = -posV1;

		posV1 = matTransformSR * posV1;

		posV1 = pivotPosGEO + posV1;

		*posicionVec[0] = posV1.x;
		*posicionVec[1] = posV1.y;
		*posicionVec[2] = posV1.z;
	}
	glm::mat4 basics_posGEO::transformBasicShader_rot()
	{
		if (keyRot == SDLK_R)
		{

			angShader += 3.0f;

		}

		else if (keyRot == SDLK_T)
		{

			angShader -= 3.0f;

		}

		glm::mat4 transformShader = glm::mat4(1.0f);

		transformShader = glm::rotate(transformShader, glm::radians(angShader), glm::vec3(0.0f, 0.0f, 1.0f));

		return transformShader;
	}
	glm::mat4 basics_posGEO::transformBasicShader_translate()
	{
		if (keyTranslate == SDLK_UP)
		{
			translateS.y += 0.05f;

		}

		else if (keyTranslate == SDLK_DOWN)
		{
			translateS.y -= 0.05f;

		}

		else if (keyTranslate == SDLK_LEFT)
		{
			translateS.x -= 0.05f;

		}

		else if (keyTranslate == SDLK_RIGHT)
		{
			translateS.x += 0.05f;

		}

		glm::mat4 matTrans{ glm::mat4(1.0f) };

		matTrans = glm::translate(matTrans, translateS);

		return matTrans;

	}
	glm::mat4 basics_posGEO::transfomrBasicShader_scale()
	{
		if (keyScale == SDLK_S)
		{
			scaleS.x += 0.01f;
			scaleS.y += 0.01f;
		}


		if (keyScale == SDLK_A)
		{
			scaleS.x -= 0.01f;
			scaleS.y -= 0.01f;
		}

		glm::mat4 transform_S{ glm::mat4(1.0f) };
		transform_S = glm::scale(transform_S, scaleS);

		return transform_S;
	}
	void basics_posGEO::resetTests()
	{
		translateTest = false;
		rotTest = false;
		scaleTest = false;

	}


	/// ////////////////////////////////


	basics_Model3D::basics_Model3D() {};
	glm::mat4 basics_Model3D::rotShaderModelSeq()
	{
		ang += 1.0f;

		glm::mat4 modelMat{ glm::mat4(1.0f) };
		modelMat = glm::rotate(modelMat, glm::radians(ang), pivotRotPos);

		return modelMat;
	}
	glm::mat4 basics_Model3D::viewShaderModel()
	{
		glm::mat view{ glm::mat4(1.0f) };
		view = glm::translate(view, viewOrient);

		return view;
	}
	glm::mat4 basics_Model3D::projectionShaderModel()
	{
		glm::mat4 projection{ glm::mat4(1.0f) };
		projection = glm::perspective(glm::radians(90.0f), static_cast<float>(screenSettings::screen_w) / static_cast<float>(screenSettings::screen_h), 0.1f, 100.0f);

		return projection;
	}

	void basics_Model3D::sumAng(GLfloat angSum)
	{
		ang += angSum;
	}
	glm::mat4 basics_Model3D::rotPivotShaderModel(glm::vec3 pivot, glm::mat4 model)
	{
		ang += 1.0f;
		model = glm::rotate(model, glm::radians(ang), pivot);
		return model;
	}
	glm::mat4 basics_Model3D::viewPosShaderModel(glm::vec3 diferentPos, glm::mat4 model)
	{
		model = glm::translate(model, diferentPos);

		return model;
	}

	/////rotacion alrededor de un pivote
	glm::mat4 basics_Model3D::rotatePerPivot(glm::vec3 center, glm::vec3 pivot, glm::vec3& posicionCube, glm::quat& lastRot, glm::vec3& lastPos_01)
	{
		glm::vec3 posChange{ glm::vec3(0.0f, 0.0f, -0.3f) - glm::normalize(posicionCube) };
		pivot = glm::normalize(pivot);

		glm::mat4 matRotPivot{ glm::mat4(1.0f) };
		matRotPivot = glm::translate(matRotPivot, posChange);

		glm::qua rot{ glm::angleAxis(glm::radians(ang), pivot) };
		//lastRot = rot;
		//glm::qua rotInterpolate = glm::slerp(lastRot, rot, (float)alphaInterpolation);

		//std::cout << static_cast<float>(alphaInterpolation) << '\n';

		matRotPivot = glm::rotate(matRotPivot, glm::radians(ang), pivot); //INTERPOLAR ESTA ROTACION /////////////
	//	matRotPivot = glm::mat4_cast(rotInterpolate) * matRotPivot;
		


		glm::vec3 posicionConvertion{ glm::vec3(matRotPivot * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) )};

		//glm::vec3 posBack = - posicionConvertion;
		glm::vec3 posBack = posicionCube + posicionConvertion;

		glm::vec3 translateBack{ posicionConvertion - posBack };

		//glm::vec3 translateBack_Interpolate{ glm::mix(lastPos_01, translateBack, (float)alphaInterpolation) };
	//	lastPos_01 = translateBack;

		matRotPivot = glm::translate(matRotPivot, translateBack);
	

		return matRotPivot;
	}
	void basics_Model3D::rotatePerPivot_Temporal(glm::vec3 center, glm::vec3 pivot, glm::vec3& posicionCube, glm::quat& lastRot, glm::vec3& lastPos_01, glm::quat& newRot, glm::vec3& newPos_01)
	{
		lastRot = newRot;
		lastPos_01 = newPos_01;

		glm::vec3 posChange{ glm::vec3(0.0f, 0.0f, -0.3f) - posicionCube};
	
		newPos_01 = posChange;

		pivot = glm::normalize(pivot);
		glm::qua rot{ glm::angleAxis(glm::radians(ang), pivot) };

		newRot = rot;

	}



	void basics_Model3D::refreshCenter_Pos()
	{

		glm::mat4 matPos{ glm::mat4(1.0f) };
		matPos = glm::translate(matPos, posModel_Base);

		matPos = model * matPos;

		posModel = glm::vec3(matPos[3].x, matPos[3].y, matPos[3].z);

	}

	void basics_Model3D::translateModel(glm::vec3 transModel)
	{
		translateM = transModel;
	
	}
	void basics_Model3D::scaleModel(glm::vec3 scaleModel)
	{
		scaleS = scaleModel;
	}
	void basics_Model3D::setPivotRotModel(glm::vec3 pivotRotModel)
	{
		pivotRotPos = glm::normalize(pivotRotModel);
	}
	void basics_Model3D::setAngRotModel(GLfloat ang)
	{
		this->ang = ang;
	}
	void basics_Model3D::setNormalModelMatrix()
	{
		normalModelMatrix = glm::transpose(glm::inverse(model));
	}
	void basics_Model3D::setTransformsAll()
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, translateM);
		model = glm::scale(model, scaleS);
		model = glm::rotate(model, ang, pivotRotPos);

		modelCurrent = model;
	}
	void basics_Model3D::setInverseTransformsAll()
	{
		model = glm::mat4(1.0f);
		model = glm::rotate(model, ang, pivotRotPos);
		model = glm::scale(model, scaleS);
		model = glm::translate(model, translateM);

		modelCurrent = model;

		translateM = model * glm::vec4(1.0f);
	}


}

namespace camera
{
	camera1::camera1() {}; 
	camera1::camera1(glm::vec3 posCam, GLfloat fovCam, GLfloat nearCut, GLfloat maxCut)
	{
		setSettingsCamera(posCam, fovCam, nearCut, maxCut);

	};

	void camera1::setSettingsCamera(glm::vec3 posCam, GLfloat fovCam, GLfloat nearCut, GLfloat maxCut)
	{
		this->posCam = posCam;
		this->fovCam = fovCam;
		this->nearCut = nearCut;
		this->maxCut = maxCut;

		cam = glm::lookAt(posCam, posCam + directionView, glm::vec3(0.0f, 1.0f, 0.0f));
	    //to rot cam
	//	cam = glm::rotate(cam, 180.0f, directionView);
		camProjection = glm::perspective(glm::radians(this->fovCam), static_cast<float>(screenSettings::screen_w) / static_cast<float>(screenSettings::screen_h), this->nearCut, this->maxCut);
	};

	void camera1::rotateCam()
	{
		/*
		ang += 0.01f;
		glm::mat4 camRot{ glm::mat4(1.0f) };
		camRot = glm::translate(camRot, glm::vec3(0.0f, 0.0f, -0.3f));
		camRot = glm::rotate(camRot, ang, pivotCam);
	*/
		cameraRight = glm::cross(directionView, glm::vec3(0.0f, 1.0f, 0.0f));
	//lm::vec3 cameraRight{ glm::cross(directionView, glm::vec3(0.0f, 1.0f, 0.0f)) }; ///colocar este para hacer la rotacion del objeto con la camara
		cameraRight = glm::normalize(cameraRight);
		
		cameraUp = glm::cross(directionView, cameraRight);
//glm::vec3 cameraUp{ glm::cross(directionView, cameraRight) };
		cameraUp = glm::normalize(cameraUp);

		camRotate = glm::lookAt(posCam, posCam + directionView, -cameraUp);
		//camRotate = glm::rotate(camRotate, 180.0f, directionView);
	}
	void camera1::detectRotCamMouse(glm::vec2 posMouse)
	{
		posMouse.x *= sensitivity;
		posMouse.y *= sensitivity;
		
		yaw += posMouse.x;
		pitch += posMouse.y;

		if (pitch < -89.9)
		{
			pitch = -89.9;

		}
		else if (pitch > 89.9)
		{
			pitch = 89.9;
		}

		directionView.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		directionView.y = sin(glm::radians(pitch));
		directionView.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		directionView = glm::normalize(directionView);

//		std::string dirCam{ std::to_string(directionView.x) + '\t' + std::to_string(directionView.y) + '\t' + std::to_string(directionView.z) + '\n' };
		//SDL_Log(dirCam.c_str());
		//SDL_Log (std::to_string(pitch).c_str());
	}
	void camera1::moveCamera()
	{
		const bool* stateKeyBoard = SDL_GetKeyboardState(nullptr);
		
		glm::vec3 posC{};

		if (stateKeyBoard[SDL_SCANCODE_W] == true)
		{
			posC += speedCam * directionView;
			posCam += speedCam * directionView;
		//	posC.x += speedCam;
		//	posCam.x += speedCam;
			camTranslate = glm::translate(cam, posC);
			moveCameraTest = true;
		}
		if (stateKeyBoard[SDL_SCANCODE_S] == true)
		{
			posC -= speedCam * directionView;
			posCam -= speedCam * directionView;
			//posC.x -= speedCam;
			//posCam.x -= speedCam;

			camTranslate = glm::translate(cam, posC);
			moveCameraTest = true;
		}

		if (stateKeyBoard[SDL_SCANCODE_A] == true)
		{
			posC = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), directionView));
			posC += speedCam * posC;
			posCam += speedCam * posC;
			//posC.z += speedCam;
			//posCam.z += speedCam;
			camTranslate = glm::translate(cam, posC);
			moveCameraTest = true;
		}
		if (stateKeyBoard[SDL_SCANCODE_D] == true)
		{
			posC = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), directionView));
			posC -= speedCam * posC;
			posCam -= speedCam * posC;

			//posC.z -= speedCam;
			//posCam.z -= speedCam;
			camTranslate = glm::translate(cam, posC);
			moveCameraTest = true;
		}
		
		if (stateKeyBoard[SDL_SCANCODE_SPACE] == true)
		{
			posCam.y += speedCam;
			camTranslate = glm::translate(cam, posC);
			moveCameraTest = true;
		}

		if (stateKeyBoard[SDL_SCANCODE_LSHIFT] == true)
		{
			posCam.y -= speedCam;
			camTranslate = glm::translate(cam, posC);
			moveCameraTest = true;
		}

		//SDL_Log(std::to_string(posCam.y).c_str());
		//cam = glm::translate(cam, posCam);
	}
	void camera1::cameraProjection(SDL_Event* event)
	{
		if (event->wheel.y < 0)
		{
			fovCam += 1.0f;
		}

		if (event->wheel.y > 0)
		{
			fovCam -= 1.0f;

		}

		//SDL_Log(std::to_string(fovCam).c_str());
		camProjection = glm::perspective(glm::radians(fovCam), static_cast<float>(screenSettings::screen_w) / static_cast<float>(screenSettings::screen_h), nearCut, maxCut);

	};

	void camera1::resetTest()
	{
		if (moveCameraTest == true)
		{
			moveCameraTest = false;
		}

		if (cameraFovTest == true)
		{
			cameraFovTest = false;
		}

	}

	void camera1::updateCameraOut()
	{
		directionView.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		directionView.y = sin(glm::radians(pitch));
		directionView.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		directionView = glm::normalize(directionView);
		rotateCam();
		cam = camRotate;
		camProjection = glm::perspective(glm::radians(fovCam), static_cast<float>(screenSettings::screen_w) / static_cast<float>(screenSettings::screen_h), nearCut, maxCut);


	}
	void camera1::controlEventsCamera()
	{
		rotateCam();
		moveCamera();
	
		cam = camRotate;
	}
	
	void camera1::updateSettingsCam(glm::mat4 camView, glm::mat4 camProjection)
	{
		cam = camView;
		this->camProjection = camProjection;
	}

}

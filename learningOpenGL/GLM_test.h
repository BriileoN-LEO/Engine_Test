#ifndef GLM_test
#define GLM_test

#include "learningOpenGL.h"
#include "config_window.h"
#include <chrono>
#include <random>


namespace randomN
{
	inline std::mt19937 generateRand();
	inline std::mt19937 getRand{ generateRand() };

	float randomNum(float NewMin, float NewMax);
	glm::vec3 randomPos(glm::vec3 fitNEW_min, glm::vec3 fitNEW_max);


}

namespace reflectionMatrixOP
{
	glm::mat4 calcReflectMatrix(glm::vec3 cameraPos, glm::vec3 pointPlane, glm::vec3 planeNormal, glm::vec3 cameraUp, glm::vec3 cameraViewTarget);
	glm::mat4 calcObliqueProjection(glm::mat4 originalProjectionMat, glm::mat4 reflectMatrix, glm::vec3 planePoint, glm::vec3 planeNormal);
}

namespace transformation_basics
{
	glm::vec3 centroidObj(std::vector<glm::vec3> Obj);
	float areaTriangle(std::array<glm::vec3, 3> triangle);
	glm::vec3 centerGeo(std::vector<std::pair<glm::vec3, float>> triCenter);
	glm::vec3 calcCenterGeo(std::vector<glm::vec3> vertex);

	//hacer la seccion de random Numbers
	
	 
	class basics_posGEO
	{
	public:

		glm::vec3 translateS{ 0.0f, 0.0f, 0.0f };
		glm::vec3 scaleS{ 0.0f, 0.0f, 0.0f };
		//glm::vec3 rotShader{}
		//glm::vec3 scale{};
		glm::vec4 pivotPosGEO{0.0f, 0.0f, 1.0f, 1.0f}; ///El pivote de la geometria

		GLfloat ang{ 3.0f };
		GLfloat angShader{ 3.0f };
		
		bool translateTest{ false };
		bool rotTest{ false };
		bool scaleTest{ false };
		bool generalTranform{ false };

		SDL_Keycode keyTranslate{};
		SDL_Keycode keyRot{};
		SDL_Keycode keyScale{};

		basics_posGEO();
		~basics_posGEO();
		void detectTranslate(SDL_Event* event);
		void detectRot(SDL_Event* event);
		
		void detectScale(SDL_Event* event);
		void movePivot();
		void transformBasic(std::vector<float*> posicionVec);
		
		glm::mat4 transformBasicShader_rot();
		glm::mat4 transformBasicShader_translate();
		glm::mat4 transfomrBasicShader_scale();

		void resetTests();

	};
	
	class basics_Model3D
	{
	public:

		glm::mat4 lastModel{ glm::mat4(1.0f) };

		glm::mat4 model{ glm::mat4(1.0f) };
		glm::mat4 modelCurrent{ glm::mat4(1.0f) };
		glm::mat3 normalModelMatrix{};

		glm::vec3 translateM{};
		glm::vec3 scaleS{ 1.0f };
		glm::quat rotateR{};

		glm::vec3 lastTranslateM{};
		glm::vec3 lastScaleS{ 1.0f };
		glm::quat lastRotateR{};

		glm::vec3 pivotRotPos{ 1.0f, 1.0f, 0.0f };
		glm::vec3 viewOrient{ 0.0f, 0.0f, -3.0f };
		
		glm::vec3 posModel_Base{};
		glm::vec3 posModel{};

		GLfloat ang{ 0.0f };


		basics_Model3D();
		glm::mat4 rotShaderModelSeq();
		glm::mat4 viewShaderModel();
		glm::mat4 projectionShaderModel();
		
		basics_Model3D operator=(const basics_Model3D& coordModel)
		{
			return coordModel;
		}

		void sumAng(GLfloat angSum);
		glm::mat4 rotPivotShaderModel(glm::vec3 pivot, glm::mat4 model);
		glm::mat4 viewPosShaderModel(glm::vec3 diferentPos, glm::mat4 model);

		glm::mat4 rotatePerPivot(glm::vec3 center, glm::vec3 pivot, glm::vec3& posicionCube, glm::quat& lastRot, glm::vec3& lastPos_01); /////////////////revisarrrr
		void rotatePerPivot_Temporal(glm::vec3 center, glm::vec3 pivot, glm::vec3& posicionCube, glm::quat& lastRot, glm::vec3& lastPos_01, glm::quat&newRot, glm::vec3& newPos_01);

		////////STANDARD TRANSFORMS///////
		void refreshCenter_Pos();

		void translateModel(glm::vec3 transModel);
		void scaleModel(glm::vec3 scaleModel);
		void setPivotRotModel(glm::vec3 pivotRotModel);
		void setAngRotModel(GLfloat ang);
		void setNormalModelMatrix();
		void setTransformsAll();
		void setInverseTransformsAll();
	
	};

}


namespace camera
{
	enum class camState
	{
		cameraAE = 0,
		cameraCount = 1
	};


	class camera1
	{
	public:
		//glm::vec3 pivotCam{ 0.0f, 0.0f, 1.0f };
		glm::vec3 directionView{ 0.0f, 0.0f, -1.0f };
		glm::vec3 posCam{ 0.0f, 0.0f, 3.0f };

		glm::mat4 cam{ glm::mat4(1.0f) };///transformaciones de la camera
		glm::mat4 camProjection{ glm::mat4(1.0f) };

		glm::mat4 camRotate{ glm::mat4(1.0f) };
		glm::mat4 camTranslate{ glm::mat4(1.0f) };
		GLfloat ang{};

		GLfloat yaw{};
		GLfloat pitch{};
		glm::vec3 cameraUp{};
		glm::vec3 cameraRight{};

		GLfloat sensitivity{ 0.01f };
		GLfloat speedCam{ 0.1f };
		bool moveCameraTest{};

		GLfloat fovCam{ 45.0f };
		GLfloat nearCut{};
		GLfloat maxCut{};
		bool cameraFovTest{};

		camera1();
		camera1(glm::vec3 posCam, GLfloat fovCam, GLfloat nearCut, GLfloat maxCut);
		
		
		camera1 operator=(const camera1& came)
		{
		
			directionView = came.directionView;
			posCam = came.posCam;

			cam = came.cam;///transformaciones de la camera
			camProjection = came.camProjection;

			camRotate = came.camRotate;
			camTranslate = came.camTranslate;
			ang = came.ang;

			yaw = came.yaw;
			pitch = came.pitch;
			cameraUp = came.cameraUp;
			cameraRight = came.cameraRight;

			sensitivity = came.sensitivity;
			speedCam = came.speedCam;
			moveCameraTest = came.moveCameraTest;

			fovCam = came.fovCam;
			nearCut = came.nearCut;
			maxCut = came.maxCut;
			cameraFovTest = came.cameraFovTest;

			return *this;
		}
		
		void setSettingsCamera(glm::vec3 posCam, GLfloat fovCam, GLfloat nearCut, GLfloat maxCut);

		void rotateCam();
		void detectRotCamMouse(glm::vec2 posMouse);
		void moveCamera();//// A,S,D,W PARA MOVER LA CAMARA
		void cameraProjection(SDL_Event* event); ///ALT + RUEDA DEL MOUSE PARA HACER ZOOM

		void resetTest();

		void updateCameraOut();
		void controlEventsCamera();
		void updateSettingsCam(glm::mat4 camView, glm::mat4 camProjection);

	};

}





#endif GLM_test
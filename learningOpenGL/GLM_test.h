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

namespace transformation_basics
{
	glm::vec3 centroidObj(std::vector<glm::vec3> Obj);
	float areaTriangle(std::array<glm::vec3, 3> triangle);
	glm::vec3 centerGeo(std::vector<std::pair<glm::vec3, float>> triCenter);

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

		glm::vec3 translateM{ 0.0f, 0.0f, 0.0f };
		glm::vec3 scaleS{ 0.0f, 0.0f, 0.0f };

		glm::vec3 pivotRot{ 1.0f, 1.0f, 0.0f };
		glm::vec3 viewOrient{ 0.0f, 0.0f, -3.0f };

		GLfloat ang{ 0.0f };

		basics_Model3D();
		glm::mat4 rotShaderModelSeq();
		glm::mat4 viewShaderModel();
		glm::mat4 projectionShaderModel();

		void sumAng(GLfloat angSum);
		glm::mat4 rotPivotShaderModel(glm::vec3 pivot, glm::mat4 model);
		glm::mat4 viewPosShaderModel(glm::vec3 diferentPos, glm::mat4 model);
		
		glm::mat4 rotatePerPivot(glm::vec3 center, glm::vec3 pivot, glm::vec3& posicionCube); /////////////////revisarrrr

	};

}

namespace camera
{
	class camera1
	{
	public: 
		//glm::vec3 pivotCam{ 0.0f, 0.0f, 1.0f };
		glm::vec3 directionView{ 0.0f, 0.0f, -1.0f };
		glm::vec3 posCam{ 0.0f, 0.0f, 3.0f};

		glm::mat4 cam{ glm::mat4(1.0f) };///transformaciones de la camera
		glm::mat4 camRotate{ glm::mat4(1.0f) };
		glm::mat4 camTranslate{ glm::mat4(1.0f) };
		GLfloat ang{};


		GLfloat yaw{};
		GLfloat pitch{};

		const GLfloat sensitivity{ 0.01f };
		const GLfloat speedCam{ 0.1f };
		
		camera1();
		void rotateCam();
		void detectRotCamMouse(glm::vec2 posMouse);
		void moveCamera();//////////////////////////Revisar

		void cameraTransform();
	};

}


#endif GLM_test
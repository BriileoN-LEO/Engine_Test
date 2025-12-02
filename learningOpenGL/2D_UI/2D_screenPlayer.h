#ifndef screenPlayer
#define screenPlayer

#include "ModelAssimp.h"
#include "learningOpenGL.h"

namespace screenUI
{
	namespace screenWorldPos
	{
		glm::vec3 getScreenPos(float width, float heigth, float distanceScreen);
	}

	class pointerScreen
	{
	public:
		unsigned int VAO{};
		unsigned int VBO{};

		glm::vec3 posPointer{};
		glm::vec3 colorPointer{};
		float sizePoint{};
		std::string nameShader{};
		shading::shader shaderPointer{};
		glm::mat4 model{};


		pointerScreen();
		pointerScreen(const char* vertexPath, const char* fragmentPath, float size, glm::vec3 posPointer, glm::vec3 colorPointer);
		pointerScreen(std::string nameShader, float size, glm::vec3 posPointer, glm::vec3 colorPointer);
		void setPointer();

		void drawPoint();
		void updatePoint();
		void updatePos(glm::vec3 pos);
	};
	


}







#endif screenPlayer
#ifndef frameBuffers
#define frameBuffers

#include "learningOpenGL.h"
#include "ModelAssimp.h"

namespace frameBuff
{
	enum class typeFrameBuffer
	{
		bufferScreen = 0,
	    bufferAssimp = 1

	};

	struct data_frameBuffer
	{
		unsigned int FBO;
		unsigned int TCB;
		unsigned int RBO;

		[[maybe_unused]] unsigned int VAO;
		[[maybe_unused]] unsigned int VBO;
		[[maybe_unused]] unsigned int EBO;

		[[maybe_unused]] Assimp_D::structModelName nameAssimp{};
	};

	struct vertex_Buffer
	{
		glm::vec2 posicion{};
		glm::vec2 TexCoord{};

	};

	class frameBuffer
	{
	public:

		std::string nameFrameBuffer{};
		typeFrameBuffer type{};
		data_frameBuffer dataBuffer{};
		int sizeIndices{};


		frameBuffer();
		frameBuffer(std::string name, typeFrameBuffer type, Assimp_D::structModelName nameAssimp = Assimp_D::structModelName());
		
		
		void loadFrameBuffer();
		void loadScreenView();
		void bindFrameBuffer();
		void useFrameBufferScreen();
		void useFrameBufferModel();


	};

}

namespace frameBuff_Obj
{
	void set_PlanarReflection_Dir(std::string& nameMesh, camera::camera1& cameraToUpdate);

}

#endif frameBuffers 
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
		unsigned int FBO; //FRAMEBUFFER
		unsigned int TCB; //TEXTURE
		unsigned int RBO; //RENDER BUFFER OBJECT

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
		void useFrameBuffer_textureShadow(unsigned int& TCB, uint8_t layer);

		void useFrameBufferModel();

        void clear();

	};

}

namespace frameBuff_Obj
{
	void set_PlanarReflection_Dir(uint32_t mesh_ID, camera::camera1& cameraToUpdate);

}

#endif //frameBuffers
#ifndef SHADER_H
#define SHADER_H

#include "data_save.h" 
#include "GLM_test.h"
#include "LIGHTS_test.h"

#include <string>
#include <fstream> 
#include <sstream>
#include <iostream>
#include <array> 
#include <ranges>

namespace shading
{
	class shader
	{
	public:

		unsigned int ID{};
		Uint64 rotAng{};

		shader();
		shader(unsigned int ID);
		shader(const char* vertexPath, const char* fragmentPath);

		void shaderCreation(const char* vertexPath, const char* fragmentPath);

		void use();
		
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec3(const std::string& name, glm::vec3 value) const;

		void sumRotAng();
		void rotateTex(const std::string& name) const;
		void scaleTex(const std::string& name, vec::vec2 size) const;
	
		void GLM_scaleTex(const std::string& name, glm::vec2 size) const;

		void transformMat(const std::string& name, glm::mat4 valueT) const;
		void transformMat3(const std::string& name, glm::mat3 valueT) const;
		
		void destroy();
	};
}
namespace Vertex
{
	struct VertexData
	{
		unsigned int VAO{};
		unsigned int VBO{};
		unsigned int EBO{};

		VertexData();
	};

	class vertexBuild
	{
	public:

		unsigned int VAO{};
		unsigned int VBO{};
		unsigned int EBO{};

		std::vector<VertexData> dataVertex_Multiple{};
		std::map<std::string, VertexData> dataVertex_Save{};

		bool isInitVAO{ false };
		bool isInitVBO{ false };

		vertexBuild();
		vertexBuild(float(&vertices)[9]);
		vertexBuild(float(&verticesColor)[18]);

		void buildVer(float(&vertices)[9]);
		void buildVerColor(float(&verticesColor)[18]);
		void BuildVerEBO_Color(float(&verticesEBO_TEX)[24], unsigned int(&indices)[6]);
		void BuildVerEBOTex(float(&verticesEBO_TEX)[32], unsigned int(&indices)[6]);

		void BuildVerMultiple(float(&verticesMulti)[24]);
		void BuildVAO_fase1(float(&verticesMulti)[9]);
	
		void useMultipleVAO(int posVAO);
		void useBindVAO();
		void destroy();

		

	};


}
namespace texture
{
	enum class textureUnits
	{
		TEXTURE0 = 0,
		TEXTURE1 = 1,
		TEXTURE2 = 2,
		TEXTURE3 = 3,
		TEXTURE4 = 4,
		TEXTURE5 = 5,
		TEXTURE6 = 6,
		TEXTURE7 = 7,
		TEXTURE8 = 8,
		TEXTURE9 = 9,
		TEXTURE10 = 10,
		TEXTURE11 = 11,
		TEXTURE12 = 12,
		TEXTURE13 = 13,
		TEXTURE14 = 14,
		TEXTURE15 = 15,
	};

	inline std::array<textureUnits, 15> textureUnits_Data
	{
		  texture::textureUnits::TEXTURE0,
		  texture::textureUnits::TEXTURE1,
		  texture::textureUnits::TEXTURE2,
		  texture::textureUnits::TEXTURE3,
		  texture::textureUnits::TEXTURE4,
		  texture::textureUnits::TEXTURE5,
		  texture::textureUnits::TEXTURE6,
		  texture::textureUnits::TEXTURE7,
		  texture::textureUnits::TEXTURE8,
		  texture::textureUnits::TEXTURE9,
		  texture::textureUnits::TEXTURE10,
		  texture::textureUnits::TEXTURE11,
		  texture::textureUnits::TEXTURE12,
		  texture::textureUnits::TEXTURE13,
		  texture::textureUnits::TEXTURE14,
	};

	struct textureData
	{
		texture::textureUnits texUnit{};
		int width{};
		int height{};
		int nrChannels{};
		unsigned int textureID{};
		std::string type{};
		std::string path{};


		textureData() {};
		textureData(unsigned int textureID, std::string type, texture::textureUnits texUnit) : textureID(textureID), type(type), texUnit(texUnit) {};
		textureData(unsigned int textureID, int width, int height, int nrChannels, texture::textureUnits texUnit) : 
			textureID(textureID), width(width), height(height), nrChannels(nrChannels), texUnit(texUnit) {};
		textureData(unsigned int textureID, std::string type, int width, int height, int nrChannels, texture::textureUnits texUnit) :
			textureID(textureID), type(type), width(width), height(height), nrChannels(nrChannels), texUnit(texUnit) {
		};
	};

	class textureBuild
	{
	public:

		int width{};
		int height{};
		int nrChannels{};
		std::vector<unsigned int> texture ///insertamos las texturas por unidad que son 15
		{
		  0,0,0,0,0,
		  0,0,0,0,0,
		  0,0,0,0,0

		};

		std::vector<textureData> texU_Data{};

		Uint8 textureUnits{};

		textureBuild();
		textureBuild(const char* data, texture::textureUnits posicion_TEX, bool flipImage);
		void loadData(const char* data, int posicion_TEX, Uint8& saveUnitTEX, bool flipImage);
		void loadTexPerFace(const char* data, texture::textureUnits posicion_TEX, bool flipImage);
		void loadTexUnit(const char* data, const std::string name, const texture::textureUnits posicion_TEX, bool flipImage);

		void setTexturesUnits(); ////Este es especial para el Model Assimp

		void useTexurePerUnit(unsigned int textureID, texture::textureUnits texUnit);
		void useTextures_PerMaterial(shading::shader& shaderID);
		void useTextures(); 

	};

}

namespace ObjCreation
{
	
	class Vertex_Color
	{
	public: 
		
		shading::shader shaderColor{};
		Vertex::vertexBuild vertexData{};
		timeS::getTimeSec timeSec{};

		std::map<std::string, bool> VertexReady
		{
			{"Shader", false},
			{"Vertex", false}

		};
		
		float vertices[18]
		{
		  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		};

		std::map<std::string, char> changeColorPos{};

		 Vertex_Color();
		 Vertex_Color(std::vector<vec::vec3> posVerticesColor);
		 void insertVertices(std::vector<vec::vec3> posVerticesColor);
		 void insertShader(const char* vertexPath, const char* fragmentPath);
		 void insertVerToVAO();
		 void render_VertexColor();
		 void setTimer(float sec);
		 void setChangeColor();
		 void changeColor_withTime();
		 
	};

	class VertexColor_Tex
	{
	public:

		shading::shader shaderColor{};
		Vertex::vertexBuild vertexData{};
		texture::textureBuild tex{};
		Uint8 numTexUnit{};
		timeS::getTimeSec timeSec{};
		timeS::getTimeSec timeRot{};

		//para las transformaciones 
		transformation_basics::basics_posGEO vertexTransform{};


		bool scaleTest{};
		SDL_Keycode scalerDir{};
		vec::vec2 scaleTexU{ 1.0f, 1.0f };

		std::map<std::string, char> changeColorPos{};
		
		float vertices[32]
		{
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f,-2.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,-2.0f,-2.0f,
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,-2.0f, 2.0f,
		};

		float verticesEBO[24]
		{
		0.3f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.3f, -0.3f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.3f, 0.3f, 0.0f, 1.0f, 1.0f, 0.0f
		};
		/*
		float vertices[32]
		{
		0.3f, 0.3f, 0.0f ,  1.0f, 0.0f, 0.0f ,  1.0f, 1.0f,
		 0.3f, -0.3f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.3f, 0.3f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		};
		*/
	
		unsigned int indices[6]
		{
			0, 1, 3,
			1, 2, 3

		};

		//Vertex_Color::Vertex_Color;
		VertexColor_Tex();
		VertexColor_Tex(std::map<std::pair<vec::vec3, vec::vec3>, vec::vec2> posVerticesColor);
		void insertVertices (std::map<std::pair<vec::vec3, vec::vec3>, vec::vec2> posVerticesColor);
		void startBuildVertexTex(const char* vertexPath, const char* fragmentPath);
		void setTextures();
		void loadTEX(const char* filePath, int posicion_TEX, bool flipTex);
		void render_VertexTex();

		///para renderizarlo con color
		void set_seqColor();
		void changeColor_withTime(std::string vertex);

		///para escalar mi textura
		void pressScaleTex(SDL_Event* event);
		void controlScaleTex();
		void scale_Tex(const std::string& parameter);
		void outScaleTex();
		
		//para detectar Tranformaciones en mi textura
		void detectTranfomrsVertex();
		void detectTransformShader();

	};	

	class ModelCreation
	{
	public:
		shading::shader shaderColor{};
		Vertex::vertexBuild vertexData{};
		texture::textureBuild tex{};
		
		std::vector<texture::textureBuild> minecraftCube{};

		transformation_basics::basics_Model3D modelCoord{};

		GLuint numberTris{ 0 };//Numero de tris que hay
		glm::vec3 centerBBOX{};

		std::vector<std::array<float, 24>> modelVertices{};
		std::vector<std::array<float, 9>> modelVertices_fase1{};

		std::vector<std::array<glm::vec3, 3>> posTriangles{};

		std::vector<glm::vec3> posCubes{};
		std::vector<glm::vec3> pivotCubes{};
		//td::map<float, bool> moveZ{};
		std::map<int, std::pair<float, bool>> moveZ{};

		glm::vec3 modelColor{};

		ModelCreation();
		ModelCreation(std::vector<std::array<float, 24>> vertices);
		ModelCreation(std::vector<std::array<float, 9>> vertices);

		void insertVertices(std::vector<std::array<float, 24>> vertices);
		void insertVertices_Fase1(std::vector<std::array<float, 9>>vertices);

		void createVAO();
		void createVAO_Fase1();

		void BuildVertexShader(const char* vertexPath, const char* fragmentPath);

		void LoadVertexTex(const char* imagePath, int posicionTEX, bool flipImage);
		void LoadMultipleTex(const char* data, texture::textureUnits posicion_TEX, bool flipImage);
		void SetTextures(const std::string textureName, const int textureUnit);

		void renderModelMultiple(camera::camera1 cam, glm::mat4 model, light::light1 light);	
		void renderModel(camera::camera1 cam, light::light1 light);
		void renderMeshLight(camera::camera1 cam, light::light1 light);

		void renderMultipleModels(int numScene, camera::camera1 cam, light::light1 light);

		void setPosModel(const int numModels);

		void calculateCenterBOX();
		void setRotModelCoord();

		void setModelCoord(glm::mat4 model);
		void setModelView(glm::mat4 camView);
		void setModelProjection(glm::mat4 camProjection);

		void setColorModel(glm::vec3 colorModel);

		void setPosModelTransforms(glm::vec3 posModel, glm::vec3 scaleModel, glm::vec3 pivotRot, GLfloat ang);

		void setCameraTransforms(camera::camera1 cam1);

		/////testfloat
		float setModelZ(float* z, float& posZcube, bool* stateZ);
	};

}

namespace vertexCreationData
{
	struct cube
	{
		static std::array<float, 24> Tri1_face1;
		static std::array<float, 24> Tri2_face1;
		static std::array<float, 24> Tri1_face2;
		static std::array<float, 24> Tri2_face2;
		static std::array<float, 24> Tri1_face3;
		static std::array<float, 24> Tri2_face3;
		static std::array<float, 24> Tri1_face4;
		static std::array<float, 24> Tri2_face4;
		static std::array<float, 24> Tri1_face5;
		static std::array<float, 24> Tri2_face5;
		static std::array<float, 24> Tri1_face6;
		static std::array<float, 24> Tri2_face6;
	};

	struct cube_fase1
	{
		static std::array<float, 9> Tri1_face1;
		static std::array<float, 9> Tri2_face1;
		static std::array<float, 9> Tri1_face2;
		static std::array<float, 9> Tri2_face2;
		static std::array<float, 9> Tri1_face3;
		static std::array<float, 9> Tri2_face3;
		static std::array<float, 9> Tri1_face4;
		static std::array<float, 9> Tri2_face4;
		static std::array<float, 9> Tri1_face5;
		static std::array<float, 9> Tri2_face5;
		static std::array<float, 9> Tri1_face6;
		static std::array<float, 9> Tri2_face6;

	};


}


#endif
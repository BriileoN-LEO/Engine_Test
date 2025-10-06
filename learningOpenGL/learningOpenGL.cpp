

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad.h>
#include "learningOpenGL.h"
#include "config_window.h"
#include "data_save.h"
#include "control.h"
#include "SHADER_H.h"
#include "playTest.h"
#include "LIGHTS_test.h"
#include "ModelAssimp.h"
#include "stb_image.h"
#include "render/configFilesTXT.h"
#include "Render/RenderData.h"
#include "Render/Render.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <cassert>
#include <filesystem>

SDL_Renderer* gRender{ nullptr };
SDL_GLContext contextOpenGl;
SDL_Window* gWindow{ nullptr };
SDL_Cursor* gCursor{ nullptr };
bool correct_init{ true };

void init()
{

	if (SDL_Init(SDL_INIT_VIDEO) == false)
	{
		SDL_Log("no se pudo abrir la ventana");
		SDL_Log(SDL_GetError());
		assert(gWindow != nullptr);
		correct_init = false;
	}

	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


		gWindow = SDL_CreateWindow("LearningOpenGL", screenSettings::screen_w, screenSettings::screen_h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE); //Creacion de la ventana

		if (gWindow == nullptr)
		{
			SDL_Log("No se pudo crear la ventana");
			SDL_Log(SDL_GetError());
			correct_init = false;
			assert(gWindow != nullptr);
		}

		else
		{
			contextOpenGl = SDL_GL_CreateContext(gWindow);  //Creacion del contexto

			if (contextOpenGl == NULL)
			{
				SDL_Log("Error al crear el contexto de OpenGL");
				SDL_Log(SDL_GetError());
				SDL_DestroyWindow(gWindow);
				SDL_Quit();
				correct_init = false;
				assert(contextOpenGl != nullptr);
			}

			else
			{
				
				if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) //Inializador de glad
				{
					SDL_Log("Fallo al inicializar glad");
					SDL_Log(SDL_GetError());
					bool correct_init = false;

					

				}
			}


		}

	}
}

enum class scenes
{
	distintFragmentShader_2 = 0,
	distintPos_3 = 1,
	seqTriangles_4 = 2,
	colorTriangle_5 = 3,
	VertexColorChange_6 = 4,
	sizeScenes = 5

};

enum class camState
{
	cameraAE = 0,
	cameraCount = 1
};


int main(int argc, char* argv[])
{
	init();

	glViewport(0, 0, screenSettings::screen_w, screenSettings::screen_h);

	scenes scenesChange[static_cast<int>(scenes::sizeScenes)]
	{
		scenes::distintFragmentShader_2,
		scenes::distintPos_3,
		scenes::seqTriangles_4,
		scenes::colorTriangle_5,
		scenes::VertexColorChange_6
	};

	int controlScene{};

	/*
	std::vector<vec::vec3> vertices
	{
		{-0.3f, -0.3f, 0.0f},
		{0.3f, -0.3f, 0.0f},
		{0.0f, 0.3f, 0.0f}
	};

	std::vector<vec::vec3> verticesRectangle
	{
		{0.3f, 0.3f, 0.0f},
		{-0.3f, 0.3f, 0.0f},
		{-0.3f, -0.3f, 0.0f},
		{0.3f, -0.3f, 0.0f},

	};


	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos; \n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
		"}\0";

	const char* vertexShaderSourceOut = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos; \n"
		"out vec4 colorVertex;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
		"colorVertex = vec4(0.8f, 0.4f, 0.6f, 1.0f);\n"
		"}\0";

	//////////////////////////////////////////////////////////////////
	//UNIFORM SHADER SOURCE
	const char* fragmentShaderColorUniform = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 colorVertex;\n"
		"void main()\n"
		"{\n"
		"FragColor = colorVertex;\n"
		"}\n";

	//////////////////////////////////////////////////////////////////

	const char* fragmentShaderColorPass2 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec4 colorVertex;\n"
		"void main()\n"
		"{\n"
		"FragColor = colorVertex;\n"
		"}\n";

	const char* fragmentShaderSource1 = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(0.5f, 0.3f, 0.4f, 1.0f);\n"
		"}\n";

	const char* fragmentShaderSourceYellow = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
		"}\n";

	const char* fragmentShaderSourceBG = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(0.05f, 0.59f, 0.72f, 1.0f);\n"
		"}\n";

	///1---Primer TRIANGULO
	vertexCreation::triangle triangleCreation(vertices[0], vertices[1], vertices[2]);
	triangleCreation.insertVertices_Rectangle(verticesRectangle);
	triangleCreation.vertexShaderCreation(vertexShaderSource);
	triangleCreation.fragmentShaderCreation(fragmentShaderColorPass2);
	triangleCreation.linkShadersToProgram();
	//triangleCreation.creation_VBO_VAO();
	triangleCreation.creation_EBO();


	///2---PARA LA CREACION DE TRES TRIANGULOS CON DISTINTO VBO Y VAO, TAMBIEN CON DISTINCO FRAGMENTSHADER
	vertexCreation::triangle tri_1;
	vertexCreation::triangle tri_2;
	vertexCreation::triangle tri_3;

	vec::vec3 posTriangle1{ 0.3f, 0.2f, 0.0f };
	vec::vec3 posTriangle2{ 0.5f, 0.4f, 0.0f };
	vec::vec3 posTriangle3{ 0.1f, 0.4f, 0.0f };

	vertexCreation::creatingTwoTriangles(tri_1, posTriangle1, vertexShaderSource, fragmentShaderSource1);
	vertexCreation::creatingTwoTriangles(tri_2, posTriangle2, vertexShaderSource, fragmentShaderSourceYellow);
	vertexCreation::creatingTwoTriangles(tri_3, posTriangle3, vertexShaderSource, fragmentShaderSourceBG);

	///3---PARA LA CREACION DE VARIOS TRIANGULOS DE DIFERENTE PUNTO PERO MISMO TAMAÑO y mismo VAO, VBO
	vertexCreation::triangle multiplesTriangles;
	multiplesTriangles.vertexShaderCreation(vertexShaderSource);
	multiplesTriangles.fragmentShaderCreation(fragmentShaderSourceYellow);
	multiplesTriangles.linkShadersToProgram();

	std::vector<vec::vec3>multiplePoints
	{
		{0.2f, 0.3f, 0.0f},
		{0.4f, -0.7f, 0.0f},
		{-0.3f, -0.4f, 0.0f}
	};

	std::vector<vec::vec3> sizeTriangle
	{
		{-0.3, -0.3f, 0.0f},
		{0.3f, -0.3f, 0.0f},
		{0.0f, 0.3f, 0.0f}
	};


	//4---Creacion de secuencia de triangulos

	vertexCreation::triangle seqTriangles;
	seqTriangles.vertexShaderCreation(vertexShaderSource);
	seqTriangles.fragmentShaderCreation(fragmentShaderSourceYellow);
	seqTriangles.linkShadersToProgram();

	seqTriangles.insertSeq(5, 5);

	//5---Creacion de triangulo pero con distinto color de punto

	const char* vertexShaderColorSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"

		"out vec3 ourColor;\n"

		"void main()\n"
		"{\n"

		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"ourColor = aColor;\n"

		"}\0";

	const char* fragmentShaderColorSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"

		"void main()\n"
		"{\n"
		"FragColor = vec4(ourColor.xyz, 1.0);\n"
		"}\n";

	std::vector<vec::vec3> posAndColor
	{
		{0.0f, 0.3f, 0.0f}, {0.0f, 0.0f, 1.0f},
		{0.3f, -0.3f, 0.0f}, {1.0f, 0.0f, 0.0f},
		{-0.3f, -0.3f, 0.0f}, {0.0f, 1.0f, 0.0f}

	};

	vertexCreation::vertex ColorVertex(posAndColor);
	ColorVertex.vertex_fragment_ShaderCreation(vertexShaderColorSource, fragmentShaderColorSource);
	ColorVertex.linkShadersToProgram();
	ColorVertex.creation_VBO_VAO_color();

	//6---VertexColor - Creacion de un vertex con un nuevo metodo y con cambio de color por tecla

	ObjCreation::Vertex_Color ColorVertexPath(posAndColor);
	ColorVertexPath.insertShader(vShader_V1.c_str(), fShader_V1.c_str());
	//ColorVertexPath.insertShader(vertexShaderColorSource, fragmentShaderColorSource);
	ColorVertexPath.insertVerToVAO();
	ColorVertexPath.setChangeColor();
	ColorVertexPath.setTimer(0.01f);

	//7---1era textura - Creacion del VAO con EBO haciendo uso de una textura
	std::map<std::pair<vec::vec3, vec::vec3>, vec::vec2> vertices_tex
	{    //VertexPos           //VertexColor       //VertexTexture coords
		{{{0.3f, 0.3f, 0.0f}, {1.0f, 0.0f, 0.0f}}, {1.0f, 1.0f}},
		{{{0.3f, -0.3f, 0.0f}, {0.0f, 1.0f, 0.0f}}, {1.0f, 0.0f}},
		{{{-0.3f, -0.3f, 0.0f}, {0.0f, 0.0f, 1.0f}}, {0.0f, 0.0f}},
		{{{-0.3f, 0.3f, 0.0f}, {1.0f, 1.0f, 0.0f}}, {0.0f, 1.0f}},

	};

	//ObjCreation::VertexColor_Tex TexVertex(vertices_tex);
	ObjCreation::VertexColor_Tex TexVertex;
	TexVertex.startBuildVertexTex(vShader_Tex_V1.c_str(), fShader_Tex_V1.c_str());
	TexVertex.loadTEX(image_Wood.c_str(), 0, false);
	TexVertex.loadTEX(image_happyFace.c_str(), 1, true);
	//TexVertex.loadTEX(image_happyFace.c_str(), 1);
	TexVertex.setTextures();

	//para hacer la secuencia de color con la textura
	TexVertex.set_seqColor();
	TexVertex.timeSec.setTimeSec(0.1f);

	TexVertex.timeRot.setTimeSec(0.5f);

	TexVertex.shaderColor.scaleTex("scaleTexContainer", vec::vec2(1.0f, 1.0f));
	TexVertex.shaderColor.scaleTex("scaleTexFaces", vec::vec2(0.5f, 0.5f));
	*/

	/*
		//Para un modelo 3D
		std::vector<std::array<float, 24>> vertex{};
		vertex.emplace_back(vertexCreationData::cube::Tri1_face1);
		vertex.emplace_back(vertexCreationData::cube::Tri2_face1);
		vertex.emplace_back(vertexCreationData::cube::Tri1_face2);
		vertex.emplace_back(vertexCreationData::cube::Tri2_face2);
		vertex.emplace_back(vertexCreationData::cube::Tri1_face3);
		vertex.emplace_back(vertexCreationData::cube::Tri2_face3);
		vertex.emplace_back(vertexCreationData::cube::Tri1_face4);
		vertex.emplace_back(vertexCreationData::cube::Tri2_face4);
		vertex.emplace_back(vertexCreationData::cube::Tri1_face5);
		vertex.emplace_back(vertexCreationData::cube::Tri2_face5);
		vertex.emplace_back(vertexCreationData::cube::Tri1_face6);
		vertex.emplace_back(vertexCreationData::cube::Tri2_face6);

		ObjCreation::ModelCreation FirstModel(vertex);
		FirstModel.BuildVertexShader(vShader_Model_V1.c_str(), fShader_Model_V1.c_str());

		FirstModel.LoadVertexTex(image_Wood.c_str(), 0, false);
		FirstModel.LoadVertexTex(image_happyFace.c_str(), 1, false);

		////////////////////SETEAR LAS TEXTURAS MULTIPLES PARA CREAR EL CUBO DE MINECRAFT////////////////
		FirstModel.LoadMultipleTex(image_MineGrass_lateral.c_str(), texture::textureUnits::TEXTURE0, false);
		FirstModel.LoadMultipleTex(image_MineGrass_top.c_str(), texture::textureUnits::TEXTURE0, false);
		FirstModel.LoadMultipleTex(image_MineGrass_down.c_str(), texture::textureUnits::TEXTURE0, false);

		FirstModel.SetTextures("texture1", 0);
		FirstModel.SetTextures("texture2", 1);

		FirstModel.setColorModel(glm::vec3(0.5f, 0.5f, 0.4f));

		FirstModel.shaderColor.GLM_scaleTex("texTransform1", glm::vec2(1.0f, 1.0f));
		FirstModel.shaderColor.GLM_scaleTex("texTransform2", glm::vec2(0.5f, 0.5f));

		FirstModel.createVAO();
		//FirstModel.setModelProjection();
		FirstModel.setPosModel(50);

		//PARA CREAR UN BASIC LIGHT
		std::vector<std::array<float, 9>> vertexLight{};
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri1_face1);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri2_face1);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri1_face2);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri2_face2);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri1_face3);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri2_face3);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri1_face4);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri2_face4);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri1_face5);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri2_face5);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri1_face6);
		vertexLight.emplace_back(vertexCreationData::cube_fase1::Tri2_face6);

		ObjCreation::ModelCreation BasicLight;
		BasicLight.insertVertices_Fase1(vertexLight);
		BasicLight.BuildVertexShader(vShader_Light_V1.c_str(), fShader_Light_V1.c_str());
		//para colocar el color del objeto y de la luz


		//BasicLight.setPosModeslTransforms(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.7f), glm::vec3(0.0f, 1.0f, 0.0f), 2.0f);
		BasicLight.createVAO_Fase1();
		glm::vec3 randomPivotMesh{ randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) };
		glm::vec3 randomPosLight{ glm::normalize(randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f))) };
		BasicLight.setPosModelTransforms(randomPosLight * 1.5f, glm::vec3(0.7f), randomPivotMesh, 1.0f);

		///Creacion de camaras
		camState camP{ camState::cameraAE };
		camera::camera1 aerialCamera(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, 0.1f, 100.0f);

		////Point Light principal
		glm::vec3 purpleLight{ 0.7f, 0.5f, 1.8f };
		glm::vec3 witheLight{ 1.0f, 1.0f, 1.0f };
		glm::vec3 green_blue{ 1.02f, 1.18f, 1.44f };
		light::light1 lightTest_01(glm::vec3(0.0f, 0.0f, 0.0f), green_blue);
		lightTest_01.setAttenuation(1.0f, 0.0035f, 0.0032f);

		testPlay::tranformationT testTransLight;
		glm::vec3 randomPivotL{ randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) };
		testTransLight.setSettingsTransform(glm::vec3(6.0f, 6.0f, 6.0f), glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.05f);


		testPlay::tranformationT testTranforms;
		glm::vec3 randomPivotTestTrans{ randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) };
		testTranforms.setSettingsTransform(glm::vec3(2.0f, 3.0f, 2.0f), glm::vec3(1.0f), randomPivotTestTrans, 0.1f);

		//para colocar el color del objeto y de la luz
		BasicLight.shaderColor.use();
		BasicLight.shaderColor.setVec3("objectColor", lightTest_01.Color);
		BasicLight.shaderColor.setVec3("lightColor", lightTest_01.Posicion);

		/////////////////////////CREACION DE ASSIMP MODEL///////////////////////////////////////

		Assimp::shaderSettings ss_Model_v1
		{
			glm::vec3(0.8f, 0.8f, 0.8f),
			glm::vec3(1.0f),
			glm::vec3(1.0f),
			glm::vec3(0.8f),
			32.0f
		};

		testPlay::tranformationT testBackPack(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.001f);
		Assimp::coordModel coordBackPack{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 210.0f };

		std::filesystem::path pathBackpack{ backpack_Model };
		Assimp::Model modelBackpack(pathBackpack.string(), vShader_ModelT1.c_str(), fShader_ModelT1.c_str(), coordBackPack, ss_Model_v1);

		/////////////////////CREACION DE FLOOR MODEL ///////////

		Assimp::shaderSettings ss_Model_v2
		{
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f),
			glm::vec3(0.8f),
			glm::vec3(0.5f),
			64.0f

		};

		testPlay::tranformationT testFloor(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.001f);
		Assimp::coordModel coord_FloorModel{ glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(15.0f, 1.0f, 15.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.0f };
		std::filesystem::path path_FloorModel{ floor2_Model };
		Assimp::Model model_Floor(path_FloorModel.string(), vShader_Standard_v1.c_str(), fShader_Standard_v1.c_str(), coord_FloorModel, ss_Model_v2);
		*/
		////////////////CREACION DE MI DIRECTIONAL LIGHT/////////////////////////////////////

		//light::DirectionalLight directionalLight_1(glm::vec3(0.0f, -1.0f, 2.0f), glm::vec3(0.3f, 0.3f, 0.3f));

		////////////////CREACION DE A LOT OF LIGHTS/////////////////////////////
		/*
		glm::vec3 purpleLight{ 0.7f, 0.5f, 1.8f };
		glm::vec3 witheLight{ 1.0f, 1.0f, 1.0f };
		glm::vec3 green_blue{ 1.02f, 1.18f, 1.44f };

		light::light1 lightTest_01(glm::vec3(0.0f, 0.0f, 0.0f), green_blue);
		lightTest_01.setAttenuation(1.0f, 0.0035f, 0.0032f);

		testPlay::tranformationT testTransLight;
		glm::vec3 randomPivotL{ randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) };
		testTransLight.setSettingsTransform(glm::vec3(6.0f, 6.0f, 6.0f), glm::vec3(1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.05f);
		*/

	camState camP{ camState::cameraAE };
	RenderData_Set::set_AllObjects();
	cameras::setCameras();
	testPlay::setTransformation_Objects();

	SDL_SetWindowRelativeMouseMode(gWindow, true);
	SDL_WarpMouseInWindow(gWindow, static_cast<float>(screenSettings::screen_w) * 0.5f, static_cast<float>(screenSettings::screen_w) * 0.5f);
	screenSettings::vSync::inFPS(screenSettings::fps);

	if (correct_init == true)
	{
		SDL_Event event;
		bool loopEvent{ false };

		while (!loopEvent)
		{

			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_EVENT_QUIT)
				{
					loopEvent = true;
				}

				if (event.type == SDL_EVENT_WINDOW_RESIZED)
				{
					screenSettings::framebuffer_size_callback(gWindow);

					if (screenSettings::detectChangeSizeWindow == true)
					{
						glViewport(0, 0, screenSettings::screen_w, screenSettings::screen_h);
						screenSettings::detectChangeSizeWindow = false;

					}
				}

				if (event.type == SDL_EVENT_KEY_DOWN)
				{
					//para cambiar la escena
					if (event.key.key == SDLK_O)
					{
						controlScene += 1;

						if (controlScene == static_cast<int>(scenes::sizeScenes))
						{
							controlScene = 0;

						}

					}

					if (!controlMove::detectSDLK_code::detectKey(event))
					{
						if (controlMove::detectMove == false)
						{
							controlMove::detectMove = true;
						}

						//	playingTest::moveRecSeq_Key(seqTriangles, event);

					}
					if (!controlMove::detectSDLK_code::detectKeyColor(event))
					{
						if (shading_2::changeColorTest == false)
						{
							shading_2::changeColorTest = true;

						}

					}
					if (controlMove::detectSDLK_code::detectKeyScale(event))
					{
						//	TexVertex.pressScaleTex(&event);

					}
					if (controlMove::detectSDLK_code::detectKeyTranslate(&event))
					{
						//					TexVertex.vertexTransform.detectTranslate(&event);

						if (camP == camState::cameraAE)
						{
							cameras::aerialCamera.moveCameraTest = true;
						}
					}

					//			TexVertex.vertexTransform.detectRot(&event);
					//			TexVertex.vertexTransform.detectScale(&event);

				}

				if (event.type == SDL_EVENT_KEY_UP)
				{

					controlMove::detectSDLK_code::resetKeys();

					if (controlMove::detectMove == true)
					{
						controlMove::detectMove = false;
					}

					if (shading_2::changeColorTest == true)
					{
						shading_2::changeColorTest = false;

					}


					//	if (TexVertex.scaleTest == true)
						//{
					//		TexVertex.outScaleTex();
					//	}

					//	TexVertex.vertexTransform.resetTests();

					if (camP == camState::cameraAE)
					{
						cameras::aerialCamera.resetTest();
					}

				}

				if (event.type == SDL_EVENT_MOUSE_MOTION)
				{
					//FirstModel.cam1.detectRotCamMouse(controlMouse::getDistanceMotionMouse());

					if (camP == camState::cameraAE)
					{
						cameras::aerialCamera.detectRotCamMouse(controlMouse::getDistanceMotionMouse());
						cameras::aerialCamera.moveCameraTest = true;
					}
				}

				if (event.type == SDL_EVENT_MOUSE_WHEEL)
				{
					SDL_Keymod modStateKey = SDL_GetModState();
					if (modStateKey & SDL_KMOD_ALT)
					{
						if (camP == camState::cameraAE)
						{
							cameras::aerialCamera.cameraProjection(&event);
						}
					}
				}

				if (event.key.key == SDLK_ESCAPE)
				{
					loopEvent = true;

				}

				////para controlar la salida del mouse de la pantalla
				if (event.key.key == SDLK_LGUI || event.key.key == SDLK_RGUI)
				{
					screenSettings::outWindow = true;
					SDL_SetWindowRelativeMouseMode(gWindow, false);
				}

				if (screenSettings::outWindow == true && event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
				{
					screenSettings::outWindow = false;
					SDL_SetWindowRelativeMouseMode(gWindow, true);
					SDL_WarpMouseInWindow(gWindow, static_cast<float>(screenSettings::screen_w) * 0.5f, static_cast<float>(screenSettings::screen_w) * 0.5f);
				}


			}

			if (screenSettings::vSync::frameT == true)
			{

				glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);

				if (screenSettings::outWindow == false)
				{
					SDL_WarpMouseInWindow(gWindow, static_cast<float>(screenSettings::screen_w) * 0.5f, static_cast<float>(screenSettings::screen_w) * 0.5f);

					if (camP == camState::cameraAE)
					{
						cameras::aerialCamera.controlEventsCamera();
					}

				}


				/*
				std::vector<light::light1> pointLights
				{
					lightTest_01
				};

				std::vector<light::DirectionalLight> directionalLights
				{
					directionalLight_1
				};

				//FirstModelTest
				testTransLight.transformMeshLight(&BasicLight, &lightTest_01);
				testTranforms.transformUniqueModel(&FirstModel, aerialCamera, lightTest_01);
				FirstModel.renderMultipleModels(1, aerialCamera, lightTest_01);/////Colocamos el light para saber la posicion del light
				BasicLight.renderMeshLight(aerialCamera, lightTest_01);

				///Draw Backpack
				//modelBackpack.Draw(aerialCamera, lightTest_01);
				modelBackpack.Draw_WL(aerialCamera, pointLights, directionalLights);
				testBackPack.transformModel_test1(&coordBackPack);
				modelBackpack.setModelCoord(coordBackPack);

				///Draw Floor model
				//model_Floor.Draw(aerialCamera, lightTest_01);
				model_Floor.Draw_WL(aerialCamera, pointLights, directionalLights); ///para renderizar con directional lights
				testFloor.moveModel_Test(&coord_FloorModel);
				model_Floor.setModelCoord(coord_FloorModel);
				*/

				render::renderAll();
				testPlay::renderTranformations_Objects();

				SDL_UpdateWindowSurface(gWindow);
				SDL_GL_SwapWindow(gWindow);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				screenSettings::vSync::frameT = false;
				screenSettings::vSync::stopTimeNS = SDL_GetTicksNS();
			}

			screenSettings::vSync::countTimeRender();

		}

	}

	//multiplesTriangles.destroy();
	destroy::destroyModels();
	SDL_GL_DestroyContext(contextOpenGl);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();

	return 0;
}

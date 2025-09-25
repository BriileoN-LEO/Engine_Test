// learningOpenGL.cpp : Defines the entry point for the application.
//
#define STB_IMAGE_IMPLEMENTATION
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/glad.h>
#include "learningOpenGL.h"
#include "config_window.h"
#include "data_save.h"
#include "control.h"
#include "SHADER_H.h"
#include "playTest.h"
#include "configFilesTXT.h"
#include "LIGHTS_test.h"
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <cassert>


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
	FirstModel.setPosModel(80);
	/*
	//1---Creacion del vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	register_Errors::testCompileShader(vertexShader, "VERTEX");

	//2---Creacion del Fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	register_Errors::testCompileShader(fragmentShader, "FRAGMENT");

	//3---Linkear los shaders con el programShader
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int testProgram;
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &testProgram);
	if (!testProgram)
	{
		char infoLog[512];
		glGetShaderInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
		SDL_Log("ERROR::SHADER::PROGRAM::COMPILATION_FAILED");

	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//4---creacion de los vertices
	float vec[]
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f

	};

	//5---Creacion del VBO(vertex buffer objects) y VAO(vertex array objects)
	unsigned int VBO;
	unsigned int VAO;

	glGenVertexArrays(1, &VAO); //se crea primero el vertexArray
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec), vec, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//6---Parar liberar el Buffer y el VertexArray
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	*/

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

	
	//BasicLight.setPosModelTransforms(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.7f), glm::vec3(0.0f, 1.0f, 0.0f), 2.0f);
	BasicLight.createVAO_Fase1();
	BasicLight.setPosModelTransforms(glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.7f), glm::vec3(0.0f, 1.0f, 0.0f), 2.0f);

	///Creacion de camaras
	camState camP{ camState::cameraAE };
	camera::camera1 aerialCamera(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f, 0.1f, 100.0f);

	////Light principal
	glm::vec3 purpleLight{ 0.7f, 0.5f, 1.8f };
	light::light1 lightTest_01(glm::vec3(0.0f, 0.0f, 0.0f), purpleLight);
	testPlay::tranformationT testTransLight;
	glm::vec3 randomPivotL{ randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) };
	testTransLight.setSettingsTransform(glm::vec3(6.0f, 6.0f, 6.0f), glm::vec3(1.0f), randomPivotL, 0.05f);

	
	testPlay::tranformationT testTranforms;
	glm::vec3 randomPivotTestTrans{ randomN::randomPos(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) };
	testTranforms.setSettingsTransform(glm::vec3(2.0f, 3.0f, 2.0f), glm::vec3(1.0f), randomPivotTestTrans, 0.1f);



	//para colocar el color del objeto y de la luz
	BasicLight.shaderColor.use();
	BasicLight.shaderColor.setVec3("objectColor", lightTest_01.Color);
	BasicLight.shaderColor.setVec3("lightColor", lightTest_01.Posicion);

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

						playingTest::moveRecSeq_Key(seqTriangles, event);

					}
					if (!controlMove::detectSDLK_code::detectKeyColor(event))
					{
						if (shading::changeColorTest == false)
						{
							shading::changeColorTest = true;

						}
	
					}
					if (controlMove::detectSDLK_code::detectKeyScale(event))
					{
					TexVertex.pressScaleTex(&event);

					}
					if (controlMove::detectSDLK_code::detectKeyTranslate(&event))
					{
						TexVertex.vertexTransform.detectTranslate(&event);

						if (camP == camState::cameraAE)
						{
							aerialCamera.moveCameraTest = true;
						}
					}
					
					TexVertex.vertexTransform.detectRot(&event);
					TexVertex.vertexTransform.detectScale(&event);

				}

				if (event.type == SDL_EVENT_KEY_UP)
				{

					controlMove::detectSDLK_code::resetKeys();

					if (controlMove::detectMove == true)
					{
						controlMove::detectMove = false;
					}

					if (shading::changeColorTest == true)
					{
						shading::changeColorTest = false;

					}

					if (TexVertex.scaleTest == true)
					{
						TexVertex.outScaleTex();
					}
					
					TexVertex.vertexTransform.resetTests();

					if (camP == camState::cameraAE)
					{
						aerialCamera.resetTest();
					}

				}

				if (event.type == SDL_EVENT_MOUSE_MOTION)
				{
					//FirstModel.cam1.detectRotCamMouse(controlMouse::getDistanceMotionMouse());

					if (camP == camState::cameraAE)
					{
						aerialCamera.detectRotCamMouse(controlMouse::getDistanceMotionMouse());
						aerialCamera.moveCameraTest = true;
					}
				}

				if (event.type == SDL_EVENT_MOUSE_WHEEL)
				{
					SDL_Keymod modStateKey = SDL_GetModState();
					if (modStateKey & SDL_KMOD_ALT)
					{
						if (camP == camState::cameraAE)
						{
							aerialCamera.cameraProjection(&event);
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

				/*
				//7---Para dibujar el triangulo
				glUseProgram(shaderProgram);
				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 3);
				*/

				//triangleCreation.render_Triangle();
				//triangleCreation.render_Elements();

				/////////////////////////SCENES//////////////////////////////////
				/*
				if (scenesChange[controlScene] == scenes::distintFragmentShader_2)
				{
					tri_1.render_Triangle();
					tri_2.render_Triangle();
					tri_3.render_Triangle();
				}

				if (scenesChange[controlScene] == scenes::distintPos_3)
				{
					if (shading::changeColorTest == true)
					{
						shading::changeColorUniform_Keys(&multiplesTriangles.shaderProgram, "colorVertex", multiplesTriangles.colorTriangle);

					}

					multiplesTriangles.renderAlotTriangles(multiplePoints, sizeTriangle);
				}

				if (scenesChange[controlScene] == scenes::seqTriangles_4)
				{
					playingTest::rectangles_Sequence(seqTriangles);
					playingTest::moveSeqRectangle(seqTriangles);
				}

				if (scenesChange[controlScene] == scenes::colorTriangle_5)
				{
					ColorVertex.render_VertexArrays();

				}

				if (scenesChange[controlScene] == scenes::VertexColorChange_6)
				{
					ColorVertexPath.render_VertexColor();

				}
				
		        */
//		TexVertex.detectTranfomrsVertex(); ///////////////Transformacion con vertex
			//exVertex.detectTransformShader(); //////////////Transformacion con shaders
			//exVertex.render_VertexTex();
				
				//FirstModel.setRotModelCoord();
				//FirstModel.renderModel();

				//FirstModel.setModelView(); // para actualizar el estado de la camara 
				//FirstModel.cam1.rotateCam(); // para rotar en secuencia la camara

				if (screenSettings::outWindow == false)
				{
					SDL_WarpMouseInWindow(gWindow, static_cast<float>(screenSettings::screen_w) * 0.5f, static_cast<float>(screenSettings::screen_w) * 0.5f);

					if (camP == camState::cameraAE)
					{
						aerialCamera.controlEventsCamera();
					//	FirstModel.setCameraTransforms(aerialCamera);
			         //   BasicLight.setCameraTransforms(aerialCamera);
					}
		
				}
			
				//FirstModelTest
				testTransLight.transformMeshLight(&BasicLight, &lightTest_01);
				testTranforms.transformUniqueModel(&FirstModel, aerialCamera, lightTest_01); /////////////Corregir este ERRORRRR
 				FirstModel.renderMultipleModels(1, aerialCamera, lightTest_01);/////Colocamos el light para saber la posicion del light			
				BasicLight.renderMeshLight(aerialCamera, lightTest_01);


				SDL_UpdateWindowSurface(gWindow);
				SDL_GL_SwapWindow(gWindow);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				screenSettings::vSync::frameT = false;
				screenSettings::vSync::stopTimeNS = SDL_GetTicksNS();
			}

			screenSettings::vSync::countTimeRender();

		}
		
}

multiplesTriangles.destroy();
SDL_GL_DestroyContext(contextOpenGl);
SDL_DestroyWindow(gWindow);
SDL_Quit();

	return 0;
	

	/*
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
		return 1;
	}

	// 3. Añade la bandera SDL_WINDOW_OPENGL al crear la ventana
	SDL_Window* window = SDL_CreateWindow("Hola OpenGL", 800, 600, SDL_WINDOW_OPENGL);
	if (!window) {
		std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// 4. Crea el contexto de OpenGL, que une OpenGL con tu ventana
	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!context) {
		std::cerr << "Error al crear el contexto de OpenGL: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// 5. ¡Paso CRÍTICO! Inicializa GLAD después de crear el contexto
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "Error al inicializar GLAD" << std::endl;
	//DL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// --- Bucle principal ---
	bool running = true;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		// 6. Usa funciones de OpenGL para limpiar la pantalla
		glClearColor(0.1f, 0.2f, 0.4f, 1.0f); // Azul oscuro
		glClear(GL_COLOR_BUFFER_BIT);

		// 7. Intercambia los buffers para mostrar lo que dibujaste
		SDL_GL_SwapWindow(window);
	}

	// --- Limpieza ---
//DL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
	*/
}
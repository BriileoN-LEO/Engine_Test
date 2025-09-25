#include "SHADER_H.h"
#include "stb_image.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace shading
{
	shader::shader() {};
	shader::shader(const char* vertexPath, const char* fragmentPath)
	{
		shaderCreation(vertexPath, fragmentPath);
	}
	void shader::shaderCreation(const char* vertexPath, const char* fragmentPath)
	{
		std::ifstream vertexStreamFile;
		std::ifstream fragmentStreamFile;

		vertexStreamFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentStreamFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		std::stringstream vShaderStream, fShaderStream;

		try
		{
			vertexStreamFile.open(vertexPath);
			fragmentStreamFile.open(fragmentPath);

			vShaderStream << vertexStreamFile.rdbuf();
			fShaderStream << fragmentStreamFile.rdbuf();

			vertexStreamFile.close();
			fragmentStreamFile.close();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
			//std::cout << e.what();
		}

		std::string VC = vShaderStream.str();
		std::string FC = fShaderStream.str();

		const char* vertexCode = VC.c_str();
		const char* fragmentCode = FC.c_str();

		//d::cout << vertexCode;
		//td::cout << fragmentCode;

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexCode, nullptr);
		glCompileShader(vertexShader);
		register_Errors::testCompileShader(vertexShader, "VERTEX", 0);

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
		glCompileShader(fragmentShader);
		register_Errors::testCompileShader(fragmentShader, "FRAGMENT", 0);

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		register_Errors::testCompileShader(ID, "PROGRAM", 1);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}
	void shader::use()
	{
		glUseProgram(ID);

	}
	void shader::setBool(const std::string& name, bool value) const
	{
		int location{ glGetUniformLocation(ID, name.c_str()) };
		glUniform1i(location, static_cast<int>(value));
	}
	void shader::setInt(const std::string& name, int value) const
	{
		int location{ glGetUniformLocation(ID, name.c_str()) };
		glUniform1i(location, static_cast<int>(value));
	}
	void shader::setFloat(const std::string& name, float value) const
	{
		int location{ glGetUniformLocation(ID, name.c_str()) };
		glUniform1i(location, static_cast<int>(value));

	}
	void shader::setVec3(const std::string& name, glm::vec3 value) const
	{
		int location{ glGetUniformLocation(ID, name.c_str()) };
		glUniform3fv(location, GL_TRUE, glm::value_ptr(value));
	
	}

	void shader::sumRotAng()
	{
		rotAng += 1;
	}
	void shader::rotateTex(const std::string& name) const
	{
		int location{ glGetUniformLocation(ID, name.c_str()) };
		glUniform1f(location, rotAng);
	}

	void shader::transformMat(const std::string& name, glm::mat4 valueT) const
	{
		unsigned int transMat = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix4fv(transMat, 1, GL_FALSE, glm::value_ptr(valueT));
	
	}
	void shader::transformMat3(const std::string& name, glm::mat3 valueT) const
	{
		unsigned int transMat = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix3fv(transMat, 1, GL_FALSE, glm::value_ptr(valueT));
	}

	void shader::scaleTex(const std::string& name, vec::vec2 size) const
	{
		int location{ glGetUniformLocation(ID, name.c_str()) };
		glUniform2f(location, size.s, size.t);
		
	}
	
	void shader::GLM_scaleTex(const std::string& name, glm::vec2 size) const
	{
		int location{ glGetUniformLocation(ID, name.c_str()) };
		glUniform2f(location, size.x, size.y);
       
	}

	void shader::destroy()
	{
		glDeleteProgram(ID);
	}

}

namespace Vertex
{

	VertexData::VertexData() {};

	vertexBuild::vertexBuild() {};
	vertexBuild::vertexBuild(float(&vertices)[9])
	{
		buildVer(vertices);

	}
	vertexBuild::vertexBuild(float(&verticesColor)[18])
	{
		buildVerColor(verticesColor);
	}
	void vertexBuild::buildVer(float(&vertices)[9])
	{
		if (isInitVAO == true && isInitVBO == true)
		{
			destroy();
		}


		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
	void vertexBuild::buildVerColor(float(&verticesColor)[18])
	{

		if (isInitVAO == true && isInitVBO == true)
		{
			destroy();
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColor), verticesColor, GL_STATIC_DRAW);

		//POS---seteamos las posiciones de los vertices conforme al la posicion del buffer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//RGB---seteamos el attributo RGB conforme a la posicion de los valores en el Buffer
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void vertexBuild::BuildVerEBO_Color(float(&verticesEBO_TEX)[24], unsigned int(&indices)[6])
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesEBO_TEX), verticesEBO_TEX, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
	void vertexBuild::BuildVerEBOTex(float(&verticesEBO_TEX)[32], unsigned int(&indices)[6])
	{
		if (isInitVAO == true && isInitVBO == true)
		{
			destroy();
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesEBO_TEX), verticesEBO_TEX, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void vertexBuild::BuildVerMultiple(float(&verticesMulti)[24])
	{
		VertexData dataV;

		glGenVertexArrays(1, &dataV.VAO);
		glGenBuffers(1, &dataV.VBO);

		glBindVertexArray(dataV.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, dataV.VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesMulti), verticesMulti, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		dataVertex_Multiple.emplace_back(dataV);
	}
	void vertexBuild::BuildVAO_fase1(float(&verticesMulti)[9])
	{
		VertexData dataV_fase1;

		glGenVertexArrays(1, &dataV_fase1.VAO);
		glGenBuffers(1, &dataV_fase1.VBO);

		glBindVertexArray(dataV_fase1.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, dataV_fase1.VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesMulti), verticesMulti, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		dataVertex_Multiple.emplace_back(dataV_fase1);
	}

	void vertexBuild::useMultipleVAO(int posVAO)
	{
		glBindVertexArray(dataVertex_Multiple[posVAO].VAO);

	}
	void vertexBuild::useBindVAO()
	{
		glBindVertexArray(VAO);
	}
	void vertexBuild::destroy()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
	///////////////////////////////////////////////////////////////////////////AQUI 08/09/2025
}

namespace texture
{
	textureBuild::textureBuild() {};
	textureBuild::textureBuild(const char* data, texture::textureUnits posicion_TEX, bool flipImage)
	{
		loadTexPerFace(data, posicion_TEX, flipImage);
	}
	void textureBuild::loadData(const char* data, int posicion_TEX, Uint8& saveUnitTEX, bool flipImage)
	{
		if (posicion_TEX == 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &texture[0]);
			glBindTexture(GL_TEXTURE_2D, texture[0]);

			//		glActiveTexture(GL_TEXTURE0);
				//	glBindTexture(GL_TEXTURE_2D, texture[0]);
			if (saveUnitTEX < 1)
			{
				saveUnitTEX = 1;
			}
		}
		if (posicion_TEX == 1)
		{
			glActiveTexture(GL_TEXTURE1);
			glGenTextures(1, &texture[1]);
			glBindTexture(GL_TEXTURE_2D, texture[1]);

			if (saveUnitTEX < 2)
			{
				saveUnitTEX = 2;
			}
		}

	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//lTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//lTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		
		stbi_set_flip_vertically_on_load(flipImage);
		unsigned char* DataT = stbi_load(data, &width, &height, &nrChannels, 0);
		

		std::stringstream tasteData;
		

		//data = stbi_load(data, &width, &height, &nrChannels, 0);
		if (DataT)
		{
			//para saber que tipo imagen es 
			std::string tipeInfo{};
			tasteData << data;
			tasteData >> tipeInfo;

			auto tipeR = tipeInfo.rbegin();
			
			std::string new_Info{};
			new_Info.push_back(tipeR[2]);
			new_Info.push_back(tipeR[1]);;
			new_Info.push_back(tipeR[0]);
			SDL_Log(new_Info.c_str());


			if (new_Info == "jpg")
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, DataT);
				glGenerateMipmap(GL_TEXTURE_2D);

			}

			if (new_Info == "png")
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, DataT);
				glGenerateMipmap(GL_TEXTURE_2D);
			}

		}
		
		else
		{
			SDL_Log("ERRO::FAIL LOAD TEXTURE");
		}

		stbi_image_free(DataT);
		
		
	}
	void textureBuild::loadTexPerFace(const char* data, texture::textureUnits posicion_TEX, bool flipImage)
	{
		texture::textureUnits texU{};
		unsigned int tex{ };
		int width{};
		int height{};
		int nrChannels{};

		auto setTextureUnit = [&]()
			{
				if (posicion_TEX == textureUnits::TEXTURE0)
				{
					glActiveTexture(GL_TEXTURE0);
					glGenTextures(1, &tex);
					glBindTexture(GL_TEXTURE_2D, tex);
					texU = textureUnits::TEXTURE0;

				}

				if (posicion_TEX == textureUnits::TEXTURE1)
				{
					glActiveTexture(GL_TEXTURE1);
					glGenTextures(1, &tex);
					glBindTexture(GL_TEXTURE_2D, tex);
					texU = textureUnits::TEXTURE1;
				}

				if (posicion_TEX == textureUnits::TEXTURE2)
				{
					glActiveTexture(GL_TEXTURE2);
					glGenTextures(1, &tex);
					glBindTexture(GL_TEXTURE_2D, tex);
					texU = textureUnits::TEXTURE2;
				}

				if (posicion_TEX == textureUnits::TEXTURE3)
				{
					glActiveTexture(GL_TEXTURE3);
					glGenTextures(1, &tex);
					glBindTexture(GL_TEXTURE_2D, tex);
					texU = textureUnits::TEXTURE3;
				}
			};

		setTextureUnit();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		//lTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//lTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_set_flip_vertically_on_load(flipImage);
		unsigned char* DataT{ stbi_load(data, &width, &height, &nrChannels, 0) };

		std::stringstream testData;

		if (DataT)
		{
			std::string dataReverse{};

			testData << data;
			testData >> dataReverse;

			auto dataR = dataReverse.rbegin();

			std::string typeInfo{};

			
			typeInfo.push_back(dataR[2]);
			typeInfo.push_back(dataR[1]);
			typeInfo.push_back(dataR[0]);
			

		//	SDL_Log(typeInfo.c_str());
			if (typeInfo == "jpg")
			{

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,  GL_RGB, GL_UNSIGNED_BYTE, DataT);
				glGenerateMipmap(GL_TEXTURE_2D);
			}

			else if (typeInfo == "png")
			{

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, DataT);
				glGenerateMipmap(GL_TEXTURE_2D);
			}


			texU_Data.emplace_back(tex, width, height, nrChannels, texU);
		}

		else
		{
			SDL_Log("ERRO::FAIL LOAD TEXTURE");
		}


		stbi_image_free(DataT);
	}

	void textureBuild::useTexurePerUnit(unsigned int textureID, texture::textureUnits texUnit)
	{

		if (texUnit == textureUnits::TEXTURE0)
		{
			glActiveTexture(GL_TEXTURE0);
			//SDL_Log("USE::TEXTURE0");
		}

		if (texUnit == textureUnits::TEXTURE1)
		{
			glActiveTexture(GL_TEXTURE1);
	
		}

		if (texUnit == textureUnits::TEXTURE2)
		{
			glActiveTexture(GL_TEXTURE2);
	
		}

		if (texUnit == textureUnits::TEXTURE3)
		{
			glActiveTexture(GL_TEXTURE3);

		}

		if (texUnit == textureUnits::TEXTURE4)
		{
			glActiveTexture(GL_TEXTURE4);

		}

		if (texUnit == textureUnits::TEXTURE5)
		{
			glActiveTexture(GL_TEXTURE5);

		}

		if (texUnit == textureUnits::TEXTURE6)
		{
			glActiveTexture(GL_TEXTURE6);

		}

		if (texUnit == textureUnits::TEXTURE7)
		{
			glActiveTexture(GL_TEXTURE7);

		}

		if (texUnit == textureUnits::TEXTURE8)
		{
			glActiveTexture(GL_TEXTURE8);
		}

		if (texUnit == textureUnits::TEXTURE9)
		{
			glActiveTexture(GL_TEXTURE9);
	
		}

		if (texUnit == textureUnits::TEXTURE10)
		{
			glActiveTexture(GL_TEXTURE10);
	
		}

		if (texUnit == textureUnits::TEXTURE11)
		{
			glActiveTexture(GL_TEXTURE11);
	
		}

		if (texUnit == textureUnits::TEXTURE12)
		{
			glActiveTexture(GL_TEXTURE12);

		}

		if (texUnit == textureUnits::TEXTURE13)
		{
			glActiveTexture(GL_TEXTURE13);

		}

		if (texUnit == textureUnits::TEXTURE14)
		{
			glActiveTexture(GL_TEXTURE14);

		}

		glBindTexture(GL_TEXTURE_2D, textureID);

	}
	void textureBuild::useTextures()
	{
		texture::textureUnits textureUnitsCount[15]
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

		texture::textureUnits lastTextureUnit{};
		for (int i = 0; i < static_cast<int>(texU_Data.size()); i++)
		{
			lastTextureUnit = textureUnitsCount[i];
			for (int u = 0; u < static_cast<int>(texU_Data.size()); u++)
			{
				if (texU_Data[u].texUnit == lastTextureUnit)
				{
					useTexurePerUnit(texU_Data[u].textureID, texU_Data[u].texUnit);
					//SDL_Log("USING_TEXTURE");
				}

			}
		}
	}
}

namespace ObjCreation
{
	Vertex_Color::Vertex_Color() {};
	Vertex_Color::Vertex_Color(std::vector<vec::vec3> posVerticesColor)
	{
		insertVertices(posVerticesColor);
	}
	void Vertex_Color::insertVertices(std::vector<vec::vec3> posVerticesColor)
	{
		unsigned int pos_up = 0;

		for (auto& valVC : posVerticesColor)
		{
			vertices[pos_up] = valVC.x;
			vertices[pos_up + 1] = valVC.y;
			vertices[pos_up + 2] = valVC.z;

			pos_up += 3;
		}

	}
	void Vertex_Color::insertShader(const char* vertexPath, const char* fragmentPath)
	{
		shaderColor.shaderCreation(vertexPath, fragmentPath);
	}
	void Vertex_Color::insertVerToVAO()
	{
		vertexData.buildVerColor(vertices);

	}
	void Vertex_Color::render_VertexColor()
	{
		changeColor_withTime();

		shaderColor.use();
		vertexData.useBindVAO();
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	void Vertex_Color::setTimer(float sec)
	{
		timeSec.setTimeSec(sec);
	}
	void Vertex_Color::setChangeColor()
	{
		int countPosRGB{ -3 };

		for (int i = 0; i < 3; i++)
		{
			countPosRGB += 6;

			if (vertices[countPosRGB] == 1.0f)
			{
				changeColorPos.emplace("vertex" + std::to_string(i + 1), 'x');

			}

			if (vertices[countPosRGB + 1] == 1.0f)
			{
				changeColorPos.emplace("vertex" + std::to_string(i + 1), 'y');

			}

			if (vertices[countPosRGB + 2] == 1.0f)
			{
				changeColorPos.emplace("vertex" + std::to_string(i + 1), 'z');

			}
		}
	}
	void Vertex_Color::changeColor_withTime()
	{
		auto change_Color = [&](std::string ver)
			{
				int pos{};
				auto posVer{ ver.rbegin() };

				std::stringstream num;
				num << *posVer;
				num >> pos;

				pos = -3 + (pos * 6);

				//d::cout << std::to_string(pos).c_str();

				if (changeColorPos[ver] == 'x')
				{
					vertices[pos + 1] += 0.1f;
					vertices[pos] -= 0.1f;

					if (vertices[pos + 1] >= 1.0f)
					{
						changeColorPos[ver] = 'y';

					}
				}

				if (changeColorPos[ver] == 'y')
				{
					vertices[pos + 2] += 0.1f;
					vertices[pos + 1] -= 0.1f;


					if (vertices[pos + 2] >= 1.0f)
					{
						changeColorPos[ver] = 'z';

					}
				}

				if (changeColorPos[ver] == 'z')
				{
					vertices[pos] += 0.1f;
					vertices[pos + 2] -= 0.1f;

					if (vertices[pos] >= 1.0f)
					{
						changeColorPos[ver] = 'x';

					}

				}

			};

		bool stopTime{ timeSec.stopTime() };

		if (stopTime == true)
		{
			change_Color("vertex1");
			change_Color("vertex2");
			change_Color("vertex3");
			insertVerToVAO();
		}

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VertexColor_Tex::VertexColor_Tex() {};
	VertexColor_Tex::VertexColor_Tex(std::map<std::pair<vec::vec3, vec::vec3>, vec::vec2> posVerticesColor)
	{
		insertVertices(posVerticesColor);
	}
	void VertexColor_Tex::insertVertices(std::map<std::pair<vec::vec3, vec::vec3>, vec::vec2> posVerticesColor)
	{
		int vec_pos{};
		auto verColorTex{ posVerticesColor.begin() };

		for (int i = 0; i < static_cast<int>(posVerticesColor.size()); i++)
		{
			vertices[vec_pos] = verColorTex->first.first.x;
			vertices[vec_pos + 1] = verColorTex->first.first.y;
			vertices[vec_pos + 2] = verColorTex->first.first.z;

			SDL_Log(std::to_string(verColorTex->first.first.x).c_str());

			vertices[vec_pos + 3] = verColorTex->first.second.x;
			vertices[vec_pos + 4] = verColorTex->first.second.y;
			vertices[vec_pos + 5] = verColorTex->first.second.z;

			//	SDL_Log(std::to_string(verColorTex->first.first.x).c_str());

			vertices[vec_pos + 6] = verColorTex->second.s;
			vertices[vec_pos + 7] = verColorTex->second.t;

			//			SDL_Log(std::to_string(verColorTex->second.s).c_str());

			vec_pos += 8;
			verColorTex++;
		}

	}
	void VertexColor_Tex::startBuildVertexTex(const char* vertexPath, const char* fragmentPath)
	{

		shaderColor.shaderCreation(vertexPath, fragmentPath);
		//ertexData.BuildVerEBO_Color(verticesEBO, indices);
		vertexData.BuildVerEBOTex(vertices, indices);



	}
	void VertexColor_Tex::setTextures()
	{
		shaderColor.use();
		shaderColor.setInt("texture1", 0);
		shaderColor.setInt("texture2", 1);

	}
	void VertexColor_Tex::loadTEX(const char* filePath, int posicion_TEX, bool flipTex)
	{
		tex.loadData(filePath, posicion_TEX, numTexUnit, flipTex);
	}
	void VertexColor_Tex::render_VertexTex()
	{
		auto loadUnitTex = [&]()
			{
				if (numTexUnit == 1)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex.texture[0]);
					//SDL_Log(std::to_string(numTexUnit).c_str());
				}

				if (numTexUnit == 2)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex.texture[0]);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, tex.texture[1]);
					//DL_Log(std::to_string(numTexUnit).c_str());
				}

			};

		bool timeR = { timeRot.stopTime() };

		if (timeR == true)
		{

			//	shaderColor.sumRotAng();
			//	shaderColor.rotateTex("rotTexture");

		}

		bool time{ timeSec.stopTime() };


		//para escalar la textura
		controlScaleTex();
		scale_Tex("scaleTexContainer");
		scale_Tex("scaleTexFaces");

		///

		if (time == true)
		{
			changeColor_withTime("vertex1");
			changeColor_withTime("vertex2");
			changeColor_withTime("vertex3");
			changeColor_withTime("vertex4");
			vertexData.BuildVerEBOTex(vertices, indices);
		}

		shaderColor.use();
		loadUnitTex();
		vertexData.useBindVAO();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	}

	void VertexColor_Tex::set_seqColor()
	{
		int countPosRGB{ -5 };

		for (int i = 0; i < 4; i++)
		{
			countPosRGB += 8;

			if (vertices[countPosRGB] == 1.0f)
			{
				changeColorPos.emplace("vertex" + std::to_string(i + 1), 'x');

			}

			if (vertices[countPosRGB + 1] == 1.0f)
			{
				changeColorPos.emplace("vertex" + std::to_string(i + 1), 'y');

			}

			if (vertices[countPosRGB + 2] == 1.0f)
			{
				changeColorPos.emplace("vertex" + std::to_string(i + 1), 'z');

			}
		}


	}
	void VertexColor_Tex::changeColor_withTime(std::string vertex)
	{
		std::stringstream ss{};
		ss << vertex;

		std::string num{};
		ss >> num;
		std::reverse(num.begin(), num.end());

		int n{ std::atoi(&num[0]) };

		n = -5 + (8 * n);

		//SDL_Log(std::to_string(n).c_str());

		if (changeColorPos[vertex] == 'x')
		{
			vertices[n] -= 0.1f;
			vertices[n + 1] += 0.1f;

			if (vertices[n + 1] > 1)
			{
				changeColorPos[vertex] = 'y';

			}

		}

		if (changeColorPos[vertex] == 'y')
		{
			vertices[n + 1] -= 0.1f;
			vertices[n + 2] += 0.1f;

			if (vertices[n + 2] > 1)
			{
				changeColorPos[vertex] = 'z';

			}

		}

		if (changeColorPos[vertex] == 'z')
		{
			vertices[n + 2] -= 0.1f;
			vertices[n] += 0.1f;

			if (vertices[n] > 1)
			{
				changeColorPos[vertex] = 'x';

			}

		}

	}

	void VertexColor_Tex::pressScaleTex(SDL_Event* event)
	{
		scaleTest = true;
		scalerDir = event->key.key;

	}
	void VertexColor_Tex::controlScaleTex()
	{
		if (scaleTest == true)
		{
			if (scalerDir == SDLK_P)
			{
				scaleTexU += 0.1f;

			}

			if (scalerDir == SDLK_M)
			{
				scaleTexU -= 0.1f;

			}
		}

	}
	void VertexColor_Tex::scale_Tex(const std::string& parameter)
	{
		if (scaleTest == true)
		{
			SDL_Log(std::to_string(scaleTexU.s).c_str());
			shaderColor.scaleTex(parameter, scaleTexU);
		}

	}
	void VertexColor_Tex::outScaleTex()
	{
		scaleTest = false;
	}

	void VertexColor_Tex::detectTranfomrsVertex()
	{
		if (vertexTransform.generalTranform == true)
		{
			int pos{};

			vertexTransform.movePivot();

			for (int i = 0; i < 4; i++)
			{
				std::vector<float*> posVertex{ &vertices[pos], &vertices[pos + 1], &vertices[pos + 2] };
				vertexTransform.transformBasic(posVertex);

				pos += 8;
			}
			SDL_Log("MOVE");
			vertexData.BuildVerEBOTex(vertices, indices);
		}

	}
	void VertexColor_Tex::detectTransformShader()
	{

		if (vertexTransform.rotTest == true)
		{
			glm::mat4 rot{ vertexTransform.transformBasicShader_rot() };
			shaderColor.transformMat("transformSR", rot);

		}

		if (vertexTransform.translateTest == true)
		{
			glm::mat4 trans{ vertexTransform.transformBasicShader_translate() };
			shaderColor.transformMat("tranformTranslate", trans);

		}

		if (vertexTransform.scaleTest == true)
		{
			glm::mat4 scale{ vertexTransform.transfomrBasicShader_scale() };
			shaderColor.transformMat("tranformScale", scale);
		}

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ModelCreation::ModelCreation() {}
	ModelCreation::ModelCreation(std::vector<std::array<float, 24>> vertices)
	{
		insertVertices(vertices);
	}
	ModelCreation::ModelCreation(std::vector<std::array<float, 9>> vertices)
	{
		insertVertices_Fase1(vertices);
	}

	void ModelCreation::insertVertices(std::vector<std::array<float, 24>> vertices)
	{
		for (auto& ver : vertices)
		{
			std::array<glm::vec3, 3> vertex{ glm::vec3(0), glm::vec3(0), glm::vec3(0) };

			int iP{ 0 };
			for (int i = 0; i < 3; i++)
			{
				vertex[i].x = ver[iP];
				vertex[i].y = ver[iP + 1];
				vertex[i].z = ver[iP + 2];

				iP += 8;
			}

			posTriangles.emplace_back(vertex);
			numberTris += 1;
		}

		modelVertices = vertices;

	}
	void ModelCreation::insertVertices_Fase1(std::vector<std::array<float, 9>>vertices)
	{
		for (auto& ver : vertices)
		{
			std::array<glm::vec3, 3> vertex
			{
				glm::vec3(ver[0], ver[1], ver[2]),
				glm::vec3(ver[3], ver[4], ver[5]),
				glm::vec3(ver[6], ver[7], ver[8])

			};

			posTriangles.emplace_back(vertex);
			numberTris += 1;
		}

		modelVertices_fase1 = vertices;
	}

	void ModelCreation::createVAO()
	{
		for (auto& vertice : modelVertices)
		{
			float vertex[24];
			std::copy(vertice.begin(), vertice.end(), vertex);

			//SDL_Log(std::to_string(vertex[2]).c_str());

			vertexData.BuildVerMultiple(vertex);
		}

	}
	void ModelCreation::createVAO_Fase1()
	{
		for (auto& ver : modelVertices_fase1)
		{
			float vertex[9];
			std::copy(ver.begin(), ver.end(), vertex);
			vertexData.BuildVAO_fase1(vertex);
		}

	}

	void ModelCreation::BuildVertexShader(const char* vertexPath, const char* fragmentPath)
	{
		shaderColor.shaderCreation(vertexPath, fragmentPath);
	}
	void ModelCreation::LoadVertexTex(const char* imagePath, int posicionTEX, bool flipImage)
	{
		tex.loadData(imagePath, posicionTEX, tex.textureUnits, flipImage);

	}
	void ModelCreation::LoadMultipleTex(const char* data, texture::textureUnits posicion_TEX, bool flipImage)
	{
		minecraftCube.emplace_back(data, posicion_TEX, flipImage);
	}
	void ModelCreation::SetTextures(const std::string textureName, const int textureUnit)
	{
		shaderColor.use();
		shaderColor.setInt(textureName, textureUnit);
	}

	
	void ModelCreation::renderModelMultiple(camera::camera1 cam, glm::mat4 model, light::light1 light)
	{
		auto loadTextures = [&]()
			{
				if (tex.textureUnits == 1)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex.texture[0]);

				}

				else if (tex.textureUnits == 2)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex.texture[0]);

					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, tex.texture[1]);
				}


			};

		auto loadSingleTexture = [&]()
			{
				for (int i = 0; i < static_cast<int>(numberTris); i++)
				{

					shaderColor.use();
					loadTextures();
					vertexData.useMultipleVAO(i);
					glDrawArrays(GL_TRIANGLES, 0, 3);

				}
			};

		/////para colocar las texturas de minecraft al cubo
		//loadSingleTexture();
		
		for (int i = 0; i < static_cast<int>(numberTris); i++)
		{
			GLuint posicion{ numberTris - 1 };

			shaderColor.use();
			shaderColor.transformMat("model", model);
			shaderColor.transformMat3("modelMatrix", modelCoord.normalModelMatrix);
			setCameraTransforms(cam);
			shaderColor.setVec3("viewPos", cam.posCam);
			shaderColor.setVec3("objectColor", glm::vec3(0.8f, 0.5f, 0.4f));
			shaderColor.setVec3("lightColor", light.Color);
			shaderColor.setVec3("lightPos", light.Posicion);
			if (i < posicion - 3)
			{
		minecraftCube[0].useTextures();

			}
		
			else if (i <= posicion - 2 && i >= posicion - 3)
			{
		minecraftCube[1].useTextures();

			}
			
			else if (i > posicion - 2)
			{
		minecraftCube[2].useTextures();

			}
			vertexData.useMultipleVAO(i);
			glDrawArrays(GL_TRIANGLES, 0, 3);

		}
		

		//lDrawArrays(GL_TRIANGLES, 0, 36);
	}
	void ModelCreation::renderModel(camera::camera1 cam, light::light1 light) 
	{
		for (int i = 0; i < static_cast<int>(numberTris); i++)
		{
			GLuint posicion{ numberTris - 1 };

			shaderColor.use();
			setModelCoord(modelCoord.model);
			setCameraTransforms(cam);

			//settings para la luz
			shaderColor.transformMat3("modelMatrix", modelCoord.normalModelMatrix);
			shaderColor.setVec3("objectColor", modelColor); ///
			shaderColor.setVec3("lightColor", light.Color);
			shaderColor.setVec3("lightPos", light.Posicion);

			if (i < posicion - 3)
			{
				minecraftCube[0].useTextures();

			}

			else if (i <= posicion - 2 && i >= posicion - 3)
			{
				minecraftCube[1].useTextures();

			}

			else if (i > posicion - 2)
			{
				minecraftCube[2].useTextures();

			}
			vertexData.useMultipleVAO(i);
			glDrawArrays(GL_TRIANGLES, 0, 3);

		}
	}
	void ModelCreation::renderMeshLight(light::light1 light)
	{
		for (int i = 0; i < static_cast<int>(numberTris); i++)
		{

			shaderColor.use();
			setModelCoord(modelCoord.model);
			setCameraTransforms(cam);
			vertexData.useMultipleVAO(i);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

	}

	void ModelCreation::renderMultipleModels(int numScene, camera::camera1 cam, light::light1 light)
	{
		auto moveZ_models = [&](int pM) -> glm::mat4
			{
				glm::mat4 model{ glm::mat4(1.0f) };
				setModelZ(&moveZ[pM].first, posCubes[pM - 1].z, &moveZ[pM].second);
		
				float zM = setModelZ(&moveZ[pM].first, posCubes[pM - 1].z, &moveZ[pM].second);

				glm::vec3 posC{ posCubes[pM - 1].x, posCubes[pM - 1].y , zM };

				model = modelCoord.viewPosShaderModel(posC, model);
				model = modelCoord.rotPivotShaderModel(pivotCubes[pM - 1], model);

				return model;
			};

		auto rotate_ModelsPivot = [&](int pM) -> glm::mat4
			{

				glm::mat4 rotMP{ modelCoord.rotatePerPivot(glm::vec3(0.0f, 0.0f, 0.0f), pivotCubes[pM], posCubes[pM]) };
				modelCoord.setNormalModelMatrix();

				return rotMP;
			};

		
		int rotateSpeed{ 4 };
		int rotSpeedState{};
		for (int p = 0; p < static_cast<int>(posCubes.size() + 1); p++)
		{

			if (p == 0)
			{
			//odel = modelCoord.viewPosShaderModel(modelCoord.viewOrient, model);
		//setRotModelCoord();
			//haderColor.transformMat("model", model);

			}
			
			else if(p > 0)
			{
				if (numScene == 0)
				{
					shaderColor.transformMat("model", moveZ_models(p));
				}
				else if (numScene == 1)
				{
					if (rotSpeedState < rotateSpeed)
					{
						rotSpeedState += 1;
					}

					else if (rotSpeedState == rotateSpeed)
					{
						modelCoord.sumAng(0.07f);
						rotSpeedState = 0;
					}

	//				shaderColor.transformMat("model", rotate_ModelsPivot(p-1));
				}
				renderModelMultiple(cam, rotate_ModelsPivot(p - 1), light);
			}

			//renderModelMultiple(cam, rotate_ModelsPivot(p - 1));

		}

	}

	void ModelCreation::setPosModel(const int numModels)
	{ 
		glm::vec3 min{ -10.0f, -10.0f, -10.0f };
		glm::vec3 max{ 10.0f, 10.0f, 10.0f };

		glm::vec3 min_pivot{ -1.0f, -1.0f, -1.0f };
		glm::vec3 max_pivot{ 1.0f, 1.0f, 1.0f };

		for (int i = 0; i < numModels; i++)
		{
			glm::vec3 RandPos{ randomN::randomPos(min, max) };

			//posicion random
			std::cout << RandPos.x << '\t' << RandPos.y << '\t' << RandPos.z << '\n';
			posCubes.emplace_back(RandPos);

			//para colocar el pivote random
			glm::vec3 RandPivot{ randomN::randomPos(min_pivot, max_pivot) };
			RandPivot = glm::normalize(RandPivot);
			pivotCubes.emplace_back(RandPivot);

			//moveZ random
			float randZ{ randomN::randomNum(0.0f, 4.0f) };
		//oveZ.emplace(randZ, false);
			moveZ.emplace(i, std::make_pair(randZ, false));
		}


	}

	void ModelCreation::calculateCenterBOX()
	{
		std::vector<std::pair<glm::vec3, float>> trisCA{};

		for (int i = 0; i < numberTris; i++)
		{
			std::vector<glm::vec3> vertexC{ posTriangles[i][0], posTriangles[i][1], posTriangles[i][2] };
			glm::vec3 centroidVertex{ transformation_basics::centroidObj(vertexC) }; ////////////////////calcular el centro de los triangulos
			float areaVertex{ transformation_basics::areaTriangle(posTriangles[i]) }; //////////////////para calcular el area del triangulo

			trisCA.emplace_back(std::make_pair(centroidVertex, areaVertex));
		}

		centerBBOX = transformation_basics::centerGeo(trisCA);
	}
	void ModelCreation::setRotModelCoord()
	{
		glm::mat4 model{ modelCoord.rotShaderModelSeq() };
		shaderColor.transformMat("model", model);

	}

	void ModelCreation::setModelCoord(glm::mat4 model)
	{
		shaderColor.transformMat("model", model);
	}
	void ModelCreation::setModelView(glm::mat4 camView)
	{
		shaderColor.transformMat("view", camView);
	}
	void ModelCreation::setModelProjection(glm::mat4 camProjection)
	{
		shaderColor.transformMat("projection", camProjection);
	}
	
	void ModelCreation::setColorModel(glm::vec3 colorModel)
	{
		modelColor = colorModel;
		shaderColor.setVec3("ObjectColor", modelColor);
	}

	void ModelCreation::setPosModelTransforms(glm::vec3 posModel, glm::vec3 scaleModel, glm::vec3 pivotRot, GLfloat ang)
	{
		modelCoord.translateModel(posModel);
		modelCoord.scaleModel(scaleModel);
		modelCoord.setPivotRotModel(pivotRot);
		modelCoord.setAngRotModel(ang);
		modelCoord.setTransformsAll();
		modelCoord.setNormalModelMatrix(); ///para setear el normal matrix del mat4 model 
		//setModelCoord(modelCoord.model);
	}

	void ModelCreation::setCameraTransforms(camera::camera1 cam1)
	{
		setModelView(cam1.cam);
		setModelProjection(cam1.camProjection);

	}

	//test
	float ModelCreation::setModelZ(float* z, float& posZcube, bool* stateZ)
	{
		auto testZ = [&]()
			{
				if (*z > 4.0f)
				{
				*stateZ = true;
				}

				else if (*z < 0.0f)
				{
				*stateZ = false;
				}

			};

		float speed{ 0.1f };
		testZ();

		if (*stateZ == false)
		{
			*z += speed;
		}

		else if (*stateZ == true)
		{
			*z -= speed;
		}

		 return  posZcube - *z;
	}


}

namespace vertexCreationData
{
	
	//AÑADIDO DE NORMALES EN EL SEGUNDO 
	std::array<float, 24> cube::Tri1_face1
	{
	  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
	};
	std::array<float, 24> cube::Tri2_face1
	{
	  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
	};
	std::array<float, 24> cube::Tri1_face2
	{
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	};
	std::array<float, 24> cube::Tri2_face2
	{
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	};
	std::array<float, 24> cube::Tri1_face3
	{
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

	};
	std::array<float, 24> cube::Tri2_face3
	{
	  -0.5f, -0.5f,-0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
     -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
	};
	std::array<float, 24> cube::Tri1_face4
	{ 
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,-0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 

	};
	std::array<float, 24> cube::Tri2_face4
	{
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
		-0.5f, -0.5f,-0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	std::array<float, 24> cube::Tri1_face5
	{
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-0.5f,-0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,-0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f
	};
	std::array<float, 24> cube::Tri2_face5
	{
		-0.5f, -0.5f,-0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f,-0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,-0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f
	};
	std::array<float, 24> cube::Tri1_face6
	{
		0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f

	};
	std::array<float, 24> cube::Tri2_face6
	{
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
	};

	std::array<float, 9> cube_fase1::Tri1_face1
	{
	  -0.5f, 0.5f, 0.5f,
	  0.5f, 0.5f, 0.5f,
	  -0.5f, -0.5f, 0.5f, 
	};
	std::array<float, 9> cube_fase1::Tri2_face1
	{
	  0.5f, -0.5f, 0.5f, 
	  0.5f, 0.5f, 0.5f,
	  -0.5f, -0.5f, 0.5f, 
	};
	std::array<float, 9> cube_fase1::Tri1_face2
	{
		0.5f, 0.5f, -0.5f, 
		0.5f, 0.5f, 0.5f, 
		0.5f, -0.5f, -0.5f, 
	};
	std::array<float, 9> cube_fase1::Tri2_face2
	{
		0.5f, -0.5f, 0.5f, 
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
	};
	std::array<float, 9> cube_fase1::Tri1_face3
	{
		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		-0.5f, 0.5f, -0.5f, 

	};
	std::array<float, 9> cube_fase1::Tri2_face3
	{
	  -0.5f, -0.5f,-0.5f, 
	  0.5f, -0.5f, -0.5f,
	 -0.5f, 0.5f, -0.5f,
	};
	std::array<float, 9> cube_fase1::Tri1_face4
	{
		-0.5f, 0.5f, -0.5f, 
		-0.5f, -0.5f,-0.5f, 
		-0.5f, 0.5f, 0.5f, 

	};
	std::array<float, 9> cube_fase1::Tri2_face4
	{
		-0.5f, -0.5f, 0.5f, 
		-0.5f, -0.5f,-0.5f, 
		-0.5f, 0.5f, 0.5f, 
	};
	std::array<float, 9> cube_fase1::Tri1_face5
	{
		0.5f, -0.5f, 0.5f, 
		-0.5f,-0.5f, 0.5f, 
		0.5f, -0.5f,-0.5f, 
	};
	std::array<float, 9> cube_fase1::Tri2_face5
	{
		-0.5f, -0.5f,-0.5f,
		-0.5f,-0.5f, 0.5f, 
		0.5f, -0.5f,-0.5f, 
	};
	std::array<float, 9> cube_fase1::Tri1_face6
	{
		0.5f, 0.5f,-0.5f, 
		0.5f, 0.5f, 0.5f, 
		-0.5f, 0.5f,-0.5f,

	};
	std::array<float, 9> cube_fase1::Tri2_face6
	{
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f,-0.5f,
	};
	
	

}
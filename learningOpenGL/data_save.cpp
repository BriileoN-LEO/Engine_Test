#include "data_save.h" 


namespace vectors
{
	vertices::vertices() {};
	vertices vertices::operator+(const vertices& loc)
	{
		float x_p = this->x + loc.x;
		float y_p = this->y + loc.y;
		float z_p = this->z + loc.z;

		return vertices(x_p, y_p, z_p);
	}

	void vertices::insertNew_Pos(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;

	}
}

namespace register_Errors
{
	void testCompileShader(unsigned int shader, std::string type, Uint8 typeTest)
	{
		int test_Shader;

		if (typeTest == 0)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &test_Shader);
		}

		else if (typeTest == 1)
		{
			glGetShaderiv(shader, GL_LINK_STATUS, &test_Shader);
		}

		if (!test_Shader)
		{
			char infoLog[512];
			glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);

			std::string messageError{ "ERROR::SHADER::" + type + "::COMPILATION_FAILED" };

			SDL_Log(messageError.c_str());
			SDL_Log(infoLog);
			

		}

	}

}


namespace vertexCreation
{

	triangle::triangle() {};
	triangle::~triangle() {};

	triangle::triangle(vec::vec3 vertex1, vec::vec3 vertex2, vec::vec3 vertex3)
	{
		insertVertices(vertex1, vertex2, vertex3);
	};

	void triangle::insertVertices(vec::vec3 vertex1, vec::vec3 vertex2, vec::vec3 vertex3)
	{
		ver[0] = vertex1.x;
		ver[1] = vertex1.y;
		ver[2] = vertex1.z;
		ver[3] = vertex2.x;
		ver[4] = vertex2.y;
		ver[5] = vertex2.z;
		ver[6] = vertex3.x;
		ver[7] = vertex3.y;
		ver[8] = vertex3.z;

	};
	void triangle::insertVertices_Rectangle(std::vector<vec::vec3>& vertexs)
	{
		int seq{};
		for (auto verti : vertexs)
		{
			rectangle[seq] = verti.x;
			rectangle[seq + 1] = verti.y;
			rectangle[seq + 2] = verti.z;

			SDL_Log(std::to_string(rectangle[seq]).c_str());
			SDL_Log(std::to_string(rectangle[seq+1]).c_str());
			SDL_Log(std::to_string(rectangle[seq+2]).c_str());

			seq += 3;
		}

	}
	void triangle::vertexShaderCreation(const char* vertexShaderSource)
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);
		register_Errors::testCompileShader(vertexShader, "VERTEX", 0);
	}
	void triangle::fragmentShaderCreation(const char* fragmentShaderSource)
	{
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);
		register_Errors::testCompileShader(fragmentShader, "VERTEX", 0);

	}
	void triangle::linkShadersToProgram()
	{
		shaderProgram = glCreateProgram();
		
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		
		register_Errors::testCompileShader(shaderProgram, "PROGRAM", 1);
		
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}
	void triangle::creation_VBO_VAO()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ver), ver, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
	void triangle::creation_EBO()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void triangle::render_Triangle()
	{

		if (controlMove::detectMove == true)
		{
			controlMove::moveTriangle(ver);
			creation_VBO_VAO();

		}
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	void triangle::render_Elements()
	{
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}
	void triangle::renderAlotTriangles(std::vector<vec::vec3>& posicions, std::vector<vec::vec3> sizeTriangle)
	{

		float new_x, new_y, new_z;

		
		for (auto& pos : posicions)
		{
			if (controlMove::detectMove == true)
			{
				controlMove::moveVec3(pos);
			}

			std::vector<vec::vec3> newPos;

			for (int i = 0; i < sizeTriangle.size(); i++)
			{
				new_x = pos.x + sizeTriangle[i].x;
				new_y = pos.y + sizeTriangle[i].y;
				new_z = pos.z + sizeTriangle[i].z;
				newPos.emplace_back(new_x, new_y, new_z);
			}

			insertVertices(newPos[0], newPos[1], newPos[2]);
			creation_VBO_VAO();
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);

		}


	}
	void triangle::insertSeq(int Columns, int Pilas)
	{
		float colum{ 2.0f / ((static_cast<float>(Columns) * 2.0f) + 2.0f) };
		float pilum{ 2.0f / ((static_cast<float>(Pilas) * 2.0f) + 2.0f) };

		SDL_Log(std::to_string(colum).c_str());

		float startColum{ -1 + colum };
		float startPilum{ -1 + pilum };


		for(int c = 0; c < Columns; c++)
		{

			for (int p = 0; p < Pilas; p++)
			{
				posRectangles.emplace_back(startColum, startPilum, 0.0f);
				rotationRectangles.emplace_back(45.0f);
				startPilum += pilum * 2;

			}
			SDL_Log(std::to_string(c).c_str());
			 
			startPilum = -1 + pilum;
			startColum += colum * 2;
		}

		SDL_Log(std::to_string(posRectangles.size()).c_str());

		rectangle[0] = 0.0f;
		rectangle[1] = pilum;
		rectangle[2] = 0.0f;
		rectangle[3] = colum;
		rectangle[4] = 0.0f;
		rectangle[5] = 0.0f;
		rectangle[6] = 0.0f;
		rectangle[7] = -pilum;
		rectangle[8] = 0.0f;
		rectangle[9] = -colum;
		rectangle[10] = 0.0f;
		rectangle[11] = 0.0f;

		distance_Up = colum;
		distance_Horizontal = pilum;
	}
	void triangle::destroy()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(shaderProgram);

	}

	void creatingTwoTriangles(triangle& triangle_1, vec::vec3 posCenter, const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		std::vector<vec::vec3> vertices_pos
		{
			{-0.2f, -0.2f, 0.0f},
			{0.2f, -0.2f, 0.0f},
			{0.0f, 0.0f, 0.0f}
		};

		std::vector<vec::vec3> Tri_1_vertices;

		for (auto ver : vertices_pos)
		{
			float new_x = ver.x * -1;
			float new_y = ver.y * -1;
			float new_z = ver.z * -1;
			
			new_x = posCenter.x - new_x;
			new_y = posCenter.y - new_y;
			new_z = posCenter.z - new_z;

			Tri_1_vertices.emplace_back(new_x, new_y, new_z);
		}
		triangle_1.insertVertices(Tri_1_vertices[0], Tri_1_vertices[1], Tri_1_vertices[2]);
		triangle_1.vertexShaderCreation(vertexShaderSource);
		triangle_1.fragmentShaderCreation(fragmentShaderSource);
		triangle_1.linkShadersToProgram();
		triangle_1.creation_VBO_VAO();
	

	}
	
	vertex::vertex() {}
	vertex::vertex(std::vector<vec::vec3> vertexColors)
	{
		insertVerticesColor(vertexColors);
	}
	void vertex::insertVerticesColor(std::vector<vec::vec3> vertexColors)
	{
		int sequence = 0;
		int seq_Colors = 0;
		for (int i = 0; i < 3; i++, sequence += 2, seq_Colors += 6)
		{
			verticesAndColor[seq_Colors] = vertexColors[sequence].x;
			verticesAndColor[seq_Colors + 1] = vertexColors[sequence].y;
			verticesAndColor[seq_Colors + 2] = vertexColors[sequence].z;
			verticesAndColor[seq_Colors + 3] = vertexColors[sequence + 1].x;
			verticesAndColor[seq_Colors + 4] = vertexColors[sequence + 1].y;
			verticesAndColor[seq_Colors + 5] = vertexColors[sequence + 1].z;
		}

	}
	void vertex::vertex_fragment_ShaderCreation(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		//creacion del vertex shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);
		register_Errors::testCompileShader(vertexShader, "VERTEX", 0);

		//creacion del fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);
		register_Errors::testCompileShader(fragmentShader, "FRAGMENT", 0);
	}
	void vertex::linkShadersToProgram()
	{
		shaderProgram = glCreateProgram();
		
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		register_Errors::testCompileShader(shaderProgram, "PROGRAM", 1);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	void vertex::creation_VBO_VAO_color()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAndColor), verticesAndColor, GL_STATIC_DRAW);
		
		//Attributo de posicion del vertex
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void vertex::render_VertexArrays()
	{
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

	}
}

namespace shading
{
	bool changeColorTest = false;
	void changeColorUniform_Keys(unsigned int* shaderProgram, std::string colorLocation, vec::rgb3& color)
	{
		const bool* keyStates = SDL_GetKeyboardState(nullptr);

		int vertexColorLocation = glGetUniformLocation(*shaderProgram, colorLocation.c_str());
		glUseProgram(*shaderProgram);

		if (keyStates[SDL_SCANCODE_R])
		{
			color.r += 0.01f;
		}

		if (keyStates[SDL_SCANCODE_G])
		{
			color.g += 0.01f;
		}

		if (keyStates[SDL_SCANCODE_B])
		{
			color.b += 0.01f;
		}

		glUniform4f(vertexColorLocation, color.r, color.g, color.b, 1.0f);

	}

}

namespace playingTest
{
	void creation_EBO_Sequence(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, float(&rectCoordinates)[12], unsigned int(&indices)[6])
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rectCoordinates), rectCoordinates, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void rectangles_Sequence(vertexCreation::triangle& tri)
	{

		float posicion_RecSeq[12]
		{
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f
		};

		for (int i = 0; i < static_cast<int>(tri.posRectangles.size()); i++)
		{
			int num_Pos{};
			while (num_Pos < 11)
			{
				posicion_RecSeq[num_Pos] = tri.posRectangles[i].x + tri.rectangle[num_Pos];
				posicion_RecSeq[num_Pos + 1] = tri.posRectangles[i].y + tri.rectangle[num_Pos + 1];
				posicion_RecSeq[num_Pos + 2] = tri.posRectangles[i].z + tri.rectangle[num_Pos + 2];

				num_Pos += 3;
			}

//			SDL_Log(std::to_string(posicion_RecSeq[0]).c_str());
			num_Pos = 0;
			creation_EBO_Sequence(tri.VAO, tri.VBO, tri.EBO, posicion_RecSeq, tri.indices_recSeq);
			tri.render_Elements();
		}

		
	}

	void moveSeqRectangle(vertexCreation::triangle& tri)
	{

		for (auto& posRec : tri.posRectangles)
		{
			if (tri.dirSeq == SDLK_UP)
			{
				posRec.y += tri.velocity;

				if (posRec.y >= 1)
				{
					posRec.y = -1;
				}
			}

			if (tri.dirSeq == SDLK_DOWN)
			{
				posRec.y -= tri.velocity;

				if (posRec.y <= -1)
				{
					posRec.y = 1;
				}
			}

			if (tri.dirSeq == SDLK_RIGHT)
			{
				posRec.x += tri.velocity;

				if (posRec.x >= 1)
				{
					posRec.x = -1;
				}
			}

			if (tri.dirSeq == SDLK_LEFT)
			{
				posRec.x -= tri.velocity;

				if (posRec.x <= -1)
				{
					posRec.x = 1;
				}

			}

		}

	}

	void moveRecSeq_Key(vertexCreation::triangle& tri, SDL_Event event)
	{
		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			if (controlMove::detectSDLK_code::keys.find(event.key.key) != controlMove::detectSDLK_code::keys.end())
			{
				tri.dirSeq = event.key.key;
			}
		}

	}

}
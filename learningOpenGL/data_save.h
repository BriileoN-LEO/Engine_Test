#ifndef data_save
#define data_save

#include "config_window.h"
#include "control.h"

namespace vectors
{
	class vertices
	{
	public:

		float x{};
		float y{};
		float z{};

		vertices();
		vertices(float x, float y, float z) : x(x), y(y), z(z) {};

		vertices operator+(const vertices& loc);

		void insertNew_Pos(float x, float y, float z);

	};
	
}
namespace register_Errors
{
	void testCompileShader(GLuint& shader, std::string type, Uint8 typeTest);
	void debug_renderError_(const char* file, int line);
#define debug_renderError() debug_renderError_(__FILE__, __LINE__)
}
namespace vertexCreation
{
	class triangle
	{
	public:

		unsigned int vertexShader{};
		unsigned int fragmentShader{};
		unsigned int shaderProgram{};

		unsigned int VBO{};
		unsigned int VAO{};
		unsigned int EBO{};

		vec::rgb3 colorTriangle{};

		std::vector<vec::vec3> posRectangles{};
		std::vector<float> rotationRectangles{};

		//Para saber la distancia entre cada punto de la sequencia
		float distance_Up{};
		float distance_Horizontal{};

		SDL_Keycode dirSeq{ SDLK_UP };
		float velocity{ 0.1f };

		float ver[9]
		{
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,

		};

		float rectangle[12]//Rectangulo con 3 vertices
		{
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f

		};

		float verticesAndColor[24]
		{ //posicions         //colors
		  0.0f, 0.0f, 0.0f,	  0.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 0.0f,	  0.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 0.0f,	  0.0f, 0.0f, 0.0f,
		};

		unsigned int indices[6]
		{
			0, 1, 3,
			1, 2, 3
		};

		unsigned int indices_recSeq[6]
		{
			3, 0, 1,
			1, 2, 3
		};

		//std::vector<float[12]> posicions

		triangle();
		~triangle();
		triangle(vec::vec3 vertex1, vec::vec3 vertex2, vec::vec3 vertex3);

		void insertVertices(vec::vec3 vertex1, vec::vec3 vertex2, vec::vec3 vertex3);
		void insertVertices_Rectangle(std::vector<vec::vec3>& vertexs);
		void vertexShaderCreation(const char* vertexShaderSource);
		void fragmentShaderCreation(const char* fragmentShaderSource);
		void linkShadersToProgram();
		void creation_VBO_VAO();
		void creation_EBO();
		void render_Triangle();
		void render_Elements();
		void renderAlotTriangles(std::vector<vec::vec3>& posicions, std::vector<vec::vec3> sizeTriangle);

		void insertSeq(int Columns, int Pilas);

		void destroy();
	};

	class vertex
	{
	public:

		unsigned int vertexShader{};
		unsigned int fragmentShader{};
		unsigned int shaderProgram{};

		unsigned int VAO{};
		unsigned int VBO{};
		unsigned int EBO{};

		float verticesAndColor[18]
		{ //posicions         //colors
		  0.0f, 0.0f, 0.0f,	  0.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 0.0f,	  0.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 0.0f,	  0.0f, 0.0f, 0.0f,
		};


		vertex();
		vertex(std::vector<vec::vec3> vertexColors);
		void insertVerticesColor(std::vector<vec::vec3> vertexColors);
		void vertex_fragment_ShaderCreation(const char* vertexShaderSource, const char* fragmentShaderSource);
		void linkShadersToProgram();
		void creation_VBO_VAO_color();
		void render_VertexArrays();
		


	};

	void creatingTwoTriangles(triangle& triangle_1, vec::vec3 posCenter, const char* vertexShaderSource, const char* fragmentShaderSource);

}
namespace shading_2
{
	extern bool changeColorTest;

	void changeColorUniform_Keys(unsigned int* shaderProgram, std::string colorLocation, vec::rgb3& color);

}
namespace playingTest
{
	void creation_EBO_Sequence(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, float(&rectCoordinates)[12],unsigned int(&indices)[6]);
	void rectangles_Sequence(vertexCreation::triangle& tri);
	void moveSeqRectangle(vertexCreation::triangle& tri);
	void moveRecSeq_Key(vertexCreation::triangle& tri, SDL_Event event);
}

#endif 
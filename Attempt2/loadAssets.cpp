#include "loadAssets.h"
#include "vertexData.h"

//add read from file and convert to string for shaders

GLuint vao, vertexBufferObject, theProgram, vertex, normal, uv;
GLint positionLocation, colorLocation, modelMatrixLocation, viewMatrixLocation;

std::string LoadShaderFromFile(const std::string& filename)
{
	std::ifstream file;
	std::string source;
	std::string line;

	file.open(filename);

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			source.append(line + "\n");
		}
	}
	else
	{
		std::cout << "Couldn't open shader: " << filename << std::endl;
	}

	return source;
}

GLuint createShader(GLenum eShaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint createProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}

loadAssets::loadAssets()
{
	initializeProgram(); //create GLSL Shaders, link into a GLSL program

	initializeVertexBuffer(); //load data into a vertex buffer

	glGenVertexArrays(1, &vao); //create a Vertex Array Object
	glBindVertexArray(vao); //make the VAO active

	std::cout << "Vertex Array Object created OK! GLUint is: " << vao << std::endl;
}

void initializeProgram()
{
	std::string vertexSource = LoadShaderFromFile("shaders/shader.vert");
	std::string fragmentSource = LoadShaderFromFile("shaders/shader.frag");

	std::vector<GLuint> shaderList;

	shaderList.push_back(createShader(GL_VERTEX_SHADER, vertexSource));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, fragmentSource));

	theProgram = createProgram(shaderList);
	if (theProgram == 0)
	{
		std::cout << "GLSL program creation error." << std::endl;
		SDL_Quit();
		exit(1);
	}

	positionLocation = glGetAttribLocation(theProgram, "position");
	colorLocation = glGetAttribLocation(theProgram, "color");
	modelMatrixLocation = glGetUniformLocation(theProgram, "modelMatrix");
	viewMatrixLocation = glGetUniformLocation(theProgram, "viewMatrix");

	//clean up shaders (we don't need them anymore as they are no in theProgram
	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

void initializeVertexBuffer()
{
	glGenBuffers(1, &vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

loadAssets::~loadAssets()
{
}

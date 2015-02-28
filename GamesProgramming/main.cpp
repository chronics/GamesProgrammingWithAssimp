#include <iostream>
#include <vector>
#include <algorithm>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <fstream>
#include <string>

#define GLM_FORCE_RADIANS //force glm to use radians //must do **before** including GLM headers
//NOTE: GLSL uses radians, so will do the same, for consistency

#include <glm/glm.hpp> //include the main glm header
#include <glm/gtc/matrix_transform.hpp> //include functions to ease the calculation of the view and projection matrices
#include <glm/gtc/type_ptr.hpp> //include functionality for converting a matrix object into a float array for usage in OpenGL


//include headers
#include "createWindow.h"
#include "vertexData.h"
#include "setGLAttributes.h"
//#include "LoadAssets.h"

using namespace std;

const std::string strVertexShader("in vec4 position;\n" "in vec4 color;\n" "uniform mat4 modelMatrix;\n" "uniform mat4 viewMatrix;\n" "uniform mat4 projectionMatrix;\n"
	"smooth out vec4 theColor;\n" "void main()\n" "{\n" "gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;\n" "theColor = color;\n"	"}\n");

const std::string strFragmentShader("#version 140\n" "smooth in vec4 theColor;\n" "out vec4 outputColor;\n" "void main()\n"
	"{\n" "outputColor = theColor;\n" "}\n");


glm::mat4 modelMatrix; // the modelMatrix for our object - which is the identity matrix by default
glm::mat4 viewMatrix; // the viewMatrix for our "camera" - which is the identity matrix by default



void initialise()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	cout << "SDL initialised OK!\n";
}

#pragma region buffers from load assets cpp and h files

GLuint vao, vertexBufferObject, theProgram, vertex, normal, uv;
GLint positionLocation, colorLocation, modelMatrixLocation, viewMatrixLocation;

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
void initializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(createShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, strFragmentShader));

	theProgram = createProgram(shaderList);
	if (theProgram == 0)
	{
		cout << "GLSL program creation error." << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		cout << "GLSL program creation OK! GLUint is: " << theProgram << std::endl;
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
	cout << "positionBufferObject created OK! GLUint is: " << vertexBufferObject << std::endl;
}

#pragma endregion


void render()
{
	glUseProgram(theProgram); //installs the program object specified by program as part of current rendering state

	//load data to GLSL that **may** have changed
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix)); //uploaed the modelMatrix to the appropriate uniform location
	// upload only one matrix, and don't transpose it

	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix)); //uploaed the modelMatrix to the appropriate uniform location
	// upload only one matrix, and don't transpose it



	size_t colorData = sizeof(vertexData) / 2;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); //bind positionBufferObject

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);

	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, 0, 0); //define **how** values are reader from positionBufferObject in Attrib 0
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorData); //define **how** values are reader from positionBufferObject in Attrib 1

	glDrawArrays(GL_TRIANGLES, 0, 36); //Draw something, using Triangles, and 3 vertices - i.e. one lonely triangle

	glDisableVertexAttribArray(0); //cleanup
	glUseProgram(0); //clean up

}



int main(int argc, char *argv[])
{
	initialise();
	createWindow createWindow("Games Programming", 1366, 768); // this will set the program info (name, x, y)
	setGLAttributes setGLAttributes(3, 3);
	
	//LoadAssets LoadAssets(strVertexShader, strFragmentShader);
	
	initializeProgram(); //create GLSL Shaders, link into a GLSL program

	initializeVertexBuffer(); //load data into a vertex buffer

	glGenVertexArrays(1, &vao); //create a Vertex Array Object
	glBindVertexArray(vao); //make the VAO active

	std::cout << "Vertex Array Object created OK! GLUint is: " << vao << std::endl;


	
	glClearColor(1.0f,0.0f,0.0f,1.0f);

	SDL_Delay(10000);

	SDL_Quit();

	return 0;
}


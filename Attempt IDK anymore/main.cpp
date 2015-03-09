#include <iostream>
#include <vector>
#include <algorithm>

#include <fstream>
#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#define GLM_FORCE_RADIANS //force glm to use radians //must do **before** including GLM headers
//NOTE: GLSL uses radians, so will do the same, for consistency

#include <glm/glm.hpp> //include the main glm header
#include <glm/gtc/matrix_transform.hpp> //include functions to ease the calculation of the view and projection matrices
#include <glm/gtc/type_ptr.hpp> //include functionality for converting a matrix object into a float array for usage in OpenGL

using namespace std;

/////////////////////
// global variables

std::string exeName;
SDL_Window *win; //pointer to the SDL_Window
SDL_GLContext context; //the SDL_GLContext

//string holding the **source** of our vertex shader, to save loading from a file
const std::string strVertexShader(
#ifdef OPENGL_VERSION_3_1
	"#version 140\n"
#endif
#ifdef OPENGL_VERSION_3_3
	"#version 140\n"
#endif
	"in vec4 position;\n"
	"in vec4 color;\n"
	"uniform mat4 modelMatrix;\n"
	"uniform mat4 viewMatrix;\n"
	"uniform mat4 projectionMatrix;\n"
	"smooth out vec4 theColor;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;\n"
	"   theColor = color;\n" //just pass on the color. It's a **smooth**, so will be interpolated
	"}\n"
	);

//string holding the **source** of our fragment shader, to save loading from a file
const std::string strFragmentShader(
#ifdef OPENGL_VERSION_3_1
	"#version 140\n"
#endif
#ifdef OPENGL_VERSION_3_3
	"#version 140\n"
#endif
	"smooth in vec4 theColor;\n"
	"out vec4 outputColor;\n"
	"void main()\n"
	"{\n"
	"   outputColor = theColor;\n"
	"}\n"
	);


//our variables
bool done = false;

const float vertexData[] = {

	//positions
	-0.25f, -0.25f, -0.25f, 1.0f,
	-0.25f, -0.25f, 0.25f, 1.0f,
	-0.25f, 0.25f, 0.25f, 1.0f,

	-0.25f, -0.25f, -0.25f, 1.0f,
	-0.25f, 0.25f, 0.25f, 1.0f,
	-0.25f, 0.25f, -0.25f, 1.0f,

	0.25f, 0.25f, -0.25f, 1.0f,
	-0.25f, -0.25f, -0.25f, 1.0f,
	-0.25f, 0.25f, -0.25f, 1.0f,

	0.25f, 0.25f, -0.25f, 1.0f,
	0.25f, -0.25f, -0.25f, 1.0f,
	-0.25f, -0.25f, -0.25f, 1.0f,

	0.25f, -0.25f, 0.25f, 1.0f,
	-0.25f, -0.25f, -0.25f, 1.0f,
	0.25f, -0.25f, -0.25f, 1.0f,

	0.25f, -0.25f, 0.25f, 1.0f,
	-0.25f, -0.25f, 0.25f, 1.0f,
	-0.25f, -0.25f, -0.25f, 1.0f,

	-0.25f, 0.25f, 0.25f, 1.0f,
	-0.25f, -0.25f, 0.25f, 1.0f,
	0.25f, -0.25f, 0.25f, 1.0f,

	0.25f, 0.25f, 0.25f, 1.0f,
	-0.25f, 0.25f, 0.25f, 1.0f,
	0.25f, -0.25f, 0.25f, 1.0f,

	0.25f, 0.25f, 0.25f, 1.0f,
	0.25f, -0.25f, -0.25f, 1.0f,
	0.25f, 0.25f, -0.25f, 1.0f,

	0.25f, -0.25f, -0.25f, 1.0f,
	0.25f, 0.25f, 0.25f, 1.0f,
	0.25f, -0.25f, 0.25f, 1.0f,

	0.25f, 0.25f, 0.25f, 1.0f,
	0.25f, 0.25f, -0.25f, 1.0f,
	-0.25f, 0.25f, -0.25f, 1.0f,

	0.25f, 0.25f, 0.25f, 1.0f,
	-0.25f, 0.25f, -0.25f, 1.0f,
	-0.25f, 0.25f, 0.25f, 1.0f,




	//colors

	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,

	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,

	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,
	0.8f, 0.8f, 0.8f, 1.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,

	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,

	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,

	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f,

};

const float vertexData2D[] = {
	//tree
	0.0f, 0.0f, 0.0f, 1.0f,
	-4.0f, -4.0f, 0.0f, 1.0f,
	4.0f, -4.0f, -1.0f, 1.0f,

	//floor
	-4.0f, -4.5f, -3.0f, 1.0f,
	-4.0f, -4.5f, 0.0f, 1.0f,
	4.0f, -4.5f, -3.0f, 1.0f,

	-4.0f, -4.5f, 0.0f, 1.0f,
	4.0f, -4.5f, 0.0f, 1.0f,
	4.0f, -4.5f, -3.0f, 1.0f,

	//star
	-0.5f, 0.4f, 0.0f, 1.0f,
	0.0f, -0.6f, 0.0f, 1.0f,
	0.5f, 0.4f, 0.0f, 1.0f,

	-0.5f, -0.4f, 0.0f, 1.0f,
	0.5f, -0.4f, 0.0f, 1.0f,
	0.0f, 0.6f, 0.0f, 1.0f,




	// colour

	0.0f, 0.5f, 0.0f, 1.0f,
	0.0f, 0.5f, 0.0f, 1.0f,
	0.0f, 0.5f, 0.0f, 1.0f,

	1.0f, 1.0f, 0.5f, 1.0f,
	1.0f, 1.0f, 0.5f, 1.0f,
	1.0f, 1.0f, 0.5f, 1.0f,

	1.0f, 1.0f, 0.5f, 1.0f,
	1.0f, 1.0f, 0.5f, 1.0f,
	1.0f, 1.0f, 0.5f, 1.0f,

	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,

	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,

};

//the rotate we'll pass to the GLSL
int oldTime = 0;
int currentTime = 0;
float deltaTime = 1.0f;

//cube1 params
glm::mat4 modelMatrix;// the modelMatrix for our object - which is the identity matrix by default
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::mat4 rotationMatrix; // the rotationMatrix for our object - which is the identity matrix by default
glm::mat4 translationMatrix; // the translationMatrix for our object - which is the identity matrix by default

float rotateSpeed = 3.0f; //rate of change of the rotate - in radians per second
glm::vec3 translateSpeed = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 translateAcceleration = glm::vec3(0.5f, 0.5f, 0.5f);

//offsets
glm::vec3 offset = glm::vec3(0.5f, -1.0f, 0.0f);
glm::vec3 offset2 = glm::vec3(-0.7f, -1.0f, 0.6f);
glm::vec3 offset3 = glm::vec3(1.0f, -2.0f, 0.0f);
glm::vec3 offset4 = glm::vec3(-1.2f, -2.5f, 1.0f);
glm::vec3 offset5 = glm::vec3(-2.2f, -4.5f, 2.0f);


//cube2 params
glm::mat4 modelMatrix2; // the modelMatrix for our object - which is the identity matrix by default
glm::mat4 rotationMatrix2; // the rotationMatrix for our object - which is the identity matrix by default
glm::mat4 translationMatrix2; // the translationMatrix for our object - which is the identity matrix by default

float rotateSpeed2 = 3.0f; //rate of change of the rotate - in radians per second
glm::vec3 translateSpeed2 = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 translateAcceleration2 = glm::vec3(0.5f, 0.5f, 0.5f);

//cube3 params
glm::mat4 modelMatrix4; // the modelMatrix for our object - which is the identity matrix by default
glm::mat4 rotationMatrix4; // the rotationMatrix for our object - which is the identity matrix by default
glm::mat4 translationMatrix4; // the translationMatrix for our object - which is the identity matrix by default

float rotateSpeed4 = 0.0f; //rate of change of the rotate - in radians per second
glm::vec3 translateSpeed4 = glm::vec3(0.0f, 0.0f, 0.0f);

//sCube params
glm::mat4 modelMatrix5; // the modelMatrix for our object - which is the identity matrix by default
glm::mat4 rotationMatrix5; // the rotationMatrix for our object - which is the identity matrix by default
glm::mat4 translationMatrix5; // the translationMatrix for our object - which is the identity matrix by default

float rotateSpeed5 = -3.0f; //rate of change of the rotate - in radians per second
glm::vec3 translateSpeed5 = glm::vec3(0.0f, 0.0f, 0.0f);


//floor params
glm::mat4 modelMatrix3;


// camera variables

glm::vec3 eyePoint = glm::vec3(0, 0, 5);
glm::vec3 lookAtPoint = glm::vec3(1, 1, 1);
glm::vec3 upVector = glm::vec3(0, 1, 0);

glm::vec3 eyePointMove = glm::vec3(0, 0, 0);
glm::vec3 eyePointAcceleration = glm::vec3(0.025, 0.025, 0.025);


int mousePosX, mousePosY;
float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;
float speed = 2.0f;
float mouseSpeed = 0.005f;

glm::vec3 rightVar(
	sin(horizontalAngle - 3.14f / 2.0f),
	0,
	cos(horizontalAngle - 3.14f / 2.0f)
	);

//our GL and GLSL variables

GLuint theProgram; //GLuint that we'll fill in to refer to the GLSL program (only have 1 at this point)
GLint positionLocation; //GLint that we'll fill in with the location of the `position` attribute in the GLSL
GLint colorLocation; //GLint that we'll fill in with the location of the `color` attribute in the GLSL
GLint modelMatrixLocation; //GLint that we'll fill in with the location of the `modelMatrix` uniform in the GLSL
GLint viewMatrixLocation;//GLint that we'll fill in with the location of the `modelMatrix` uniform in the GLSL
GLint projectionMatrixLocation;

GLuint vertexBufferObject;
GLuint vertexBufferObject2D;
GLuint vao;

// end Global Variables
/////////////////////////


void initialise()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	cout << "SDL initialised OK!\n";
}

void createWindow()
{
	//get executable name, and use as window title
	int beginIdxWindows = exeName.rfind("\\"); //find last occurrence of a backslash
	int beginIdxLinux = exeName.rfind("/"); //find last occurrence of a forward slash
	int beginIdx = max(beginIdxWindows, beginIdxLinux);
	std::string exeNameEnd = exeName.substr(beginIdx + 1);
	const char *exeNameCStr = exeNameEnd.c_str();

	//create window
	win = SDL_CreateWindow(exeNameCStr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE); //same height and width makes the window square with resizable but needs to be tabbed out
	//error handling
	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	cout << "SDL CreatedWindow OK!\n";
}

void setGLAttributes()
{
	cout << "Built for OpenGL Version 3.3" << endl;
	// set the opengl context version
	int major = 3;
	int minor = 1;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //core profile
	cout << "Set OpenGL context to version " << major << "." << minor << " OK!\n";
}

void createContext()
{
	context = SDL_GL_CreateContext(win);
	if (context == nullptr){
		SDL_DestroyWindow(win);
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	cout << "Created OpenGL context OK!\n";
}

void initGlew()
{
	GLenum rev;
	glewExperimental = GL_TRUE; //GLEW isn't perfect - see https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
	rev = glewInit();
	if (GLEW_OK != rev){
		std::cout << "GLEW Error: " << glewGetErrorString(rev) << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		cout << "GLEW Init OK!\n";
	}
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
	projectionMatrixLocation = glGetUniformLocation(theProgram, "projectionMatrix");

	//check errors on locations

	//clean up shaders (we don't need them anymore as they are no in theProgram
	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

void initializeVertexBuffer()
{


	glGenBuffers(1, &vertexBufferObject2D);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject2D);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData2D), vertexData2D, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "positionBufferObject2D created OK! GLUint is: " << vertexBufferObject << std::endl;

	glGenBuffers(1, &vertexBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "positionBufferObject created OK! GLUint is: " << vertexBufferObject << std::endl;
}


void loadAssets()
{
	initializeProgram(); //create GLSL Shaders, link into a GLSL program

	initializeVertexBuffer(); //load data into a vertex buffer

	glGenVertexArrays(1, &vao); //create a Vertex Array Object
	glBindVertexArray(vao); //make the VAO active
	cout << "Vertex Array Object created OK! GLUint is: " << vao << std::endl;

	//setup face culling details.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW); //front faces are counter-clockwise

	cout << "Loaded Assets OK!\n";
}

void updateSimulation(double simLength) //update simulation with an amount of time to simulate for (in seconds)
{

	const glm::vec3 unitX = glm::vec3(1, 0, 0);
	const glm::vec3 unitY = glm::vec3(0, 1, 0);
	const glm::vec3 unitZ = glm::vec3(0, 0, 1);
	const glm::vec3 unit45 = glm::normalize(glm::vec3(0, 1, 1));

	//cube 1
	//calculate the amount of rotate for this timestep
	float rotate = (float)simLength * rotateSpeed; //simlength is a double for precision, but rotateSpeedVector in a vector of float, alternatively use glm::dvec3

	//modify the rotationMatrix with the rotate, as a rotate, around the z-axis
	rotationMatrix = glm::rotate(rotationMatrix, rotate, unit45);

	glm::vec3 translate = float(simLength) * translateSpeed; //scale the translationSpeed by time to get the translation amount
	translationMatrix = glm::translate(translationMatrix, translate);

	modelMatrix = translationMatrix * rotationMatrix;

	//cube 2
	//calculate the amount of rotate for this timestep
	float rotate2 = (float)simLength * rotateSpeed2; //simlength is a double for precision, but rotateSpeedVector in a vector of float, alternatively use glm::dvec3

	//modify the rotationMatrix with the rotate, as a rotate, around the z-axis
	rotationMatrix2 = glm::rotate(rotationMatrix2, rotate2, unit45);

	glm::vec3 translate2 = float(simLength) * translateSpeed2; //scale the translationSpeed by time to get the translation amount
	translationMatrix2 = glm::translate(translationMatrix2, translate2);

	modelMatrix2 = translationMatrix2 * rotationMatrix2;

	//cube 3
	//calculate the amount of rotate for this timestep
	float rotate4 = (float)simLength * rotateSpeed4; //simlength is a double for precision, but rotateSpeedVector in a vector of float, alternatively use glm::dvec3

	//modify the rotationMatrix with the rotate, as a rotate, around the z-axis
	rotationMatrix4 = glm::rotate(rotationMatrix4, rotate4, unit45);

	glm::vec3 translate4 = float(simLength) * translateSpeed4; //scale the translationSpeed by time to get the translation amount
	translationMatrix4 = glm::translate(translationMatrix4, translate4);

	modelMatrix4 = translationMatrix4 * rotationMatrix4;

	//cube 2
	//calculate the amount of rotate for this timestep
	float rotate5 = (float)simLength * rotateSpeed5; //simlength is a double for precision, but rotateSpeedVector in a vector of float, alternatively use glm::dvec3

	//modify the rotationMatrix with the rotate, as a rotate, around the z-axis
	rotationMatrix5 = glm::rotate(rotationMatrix5, rotate5, unit45);

	glm::vec3 translate5 = float(simLength) * translateSpeed5; //scale the translationSpeed by time to get the translation amount
	translationMatrix5 = glm::translate(translationMatrix5, translate5);

	modelMatrix5 = translationMatrix5 * rotationMatrix5;

	//camera

	eyePoint += eyePointMove;

	glm::vec3 lookAtPoint(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	glm::vec3 rightVar(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	glm::vec3 upVector = glm::cross(rightVar, lookAtPoint);

	viewMatrix = glm::lookAt(eyePoint, (eyePoint + lookAtPoint), upVector);
	float fovyRadians = glm::radians(90.0f);
	float aspectRatio = 1.0f;
	float nearClipPlane = 0.1f;
	float farClipPlane = 100.0f;

	projectionMatrix = glm::perspective(fovyRadians, aspectRatio, nearClipPlane, farClipPlane);
}

void input()
{

	SDL_Event event; //somewhere to store an event

	oldTime = currentTime;
	currentTime = SDL_GetTicks();
	deltaTime = (currentTime - oldTime) / 10;

	//NOTE: there may be multiple events per frame
	while (SDL_PollEvent(&event)) { //loop until SDL_PollEvent returns 0 (meaning no more events)
		switch (event.type)
		{
		case SDL_QUIT:
			done = true; //set done flag if SDL wants to quit (i.e. if the OS has triggered a close event,
			//  - such as window close, or SIGINT
			break;

			//keydown handling - we should to the opposite on key-up for direction controls (generally)
		case SDL_KEYDOWN:

			if (!event.key.repeat)
				switch (event.key.keysym.sym)
			{
				//hit escape to exit
				case SDLK_ESCAPE: done = true;

					//cube2 movement isng W,A,S,D
				case SDLK_a:  translateSpeed2.x -= translateAcceleration2.x, translateSpeed.x += translateAcceleration.x; break;
				case SDLK_d: translateSpeed2.x += translateAcceleration2.x, translateSpeed.x -= translateAcceleration.x; break;

				case SDLK_w:    translateSpeed.y += translateAcceleration.y, translateSpeed2.y += translateAcceleration2.y; break;
				case SDLK_s:  translateSpeed.y -= translateAcceleration.y, translateSpeed2.y -= translateAcceleration2.y; break;

				case SDLK_KP_5: eyePointMove.z -= eyePointAcceleration.z; break;
				case SDLK_KP_0: eyePointMove.z += eyePointAcceleration.z; break;

				case SDLK_KP_6: eyePointMove.x -= eyePointAcceleration.x; break;
				case SDLK_KP_4: eyePointMove.x += eyePointAcceleration.x; break;

				case SDLK_KP_8: eyePointMove.y -= eyePointAcceleration.y; break;
				case SDLK_KP_2: eyePointMove.y += eyePointAcceleration.y; break;

			}
			break;

			//keyup handling
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
				//cube2 movement isng W,A,S,D
			case SDLK_a:  translateSpeed2.x += translateAcceleration2.x, translateSpeed.x -= translateAcceleration.x;; break;
			case SDLK_d: translateSpeed2.x -= translateAcceleration2.x, translateSpeed.x += translateAcceleration.x;; break;

			case SDLK_w:    translateSpeed.y -= translateAcceleration.y, translateSpeed2.y -= translateAcceleration2.y; break;
			case SDLK_s:  translateSpeed.y += translateAcceleration.y, translateSpeed2.y += translateAcceleration2.y; break;

			case SDLK_KP_5: eyePointMove.z += eyePointAcceleration.z; break;
			case SDLK_KP_0: eyePointMove.z -= eyePointAcceleration.z; break;

			case SDLK_KP_6: eyePointMove.x += eyePointAcceleration.x; break;
			case SDLK_KP_4: eyePointMove.x -= eyePointAcceleration.x; break;

			case SDLK_KP_8: eyePointMove.y += eyePointAcceleration.y; break;
			case SDLK_KP_2: eyePointMove.y -= eyePointAcceleration.y; break;
			}
			break;
			//mouse handling*/

		case SDL_MOUSEMOTION:
		{
			mousePosX = event.motion.x;
			mousePosY = event.motion.y;

			horizontalAngle += (mouseSpeed * float(300 - mousePosX));
			verticalAngle += (mouseSpeed * float(300 - mousePosY));

			SDL_WarpMouseInWindow(win, 300, 300);

		};
			break;

		default: //one dose not simply forget a default case
			break;


		}
	}
}

void render()
{
	glUseProgram(theProgram); //installs the program object specified by program as part of current rendering state



	//load data to GLSL that **may** have changed
	//cube1 modelMatrix
	//glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix)); //uploaed the modelMatrix to the appropriate uniform location
	// upload only one matrix, and don't transpose it

	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix)); //uploaed the modelMatrix to the appropriate uniform location
	// upload only one matrix, and don't transpose it

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix)); //uploaed the modelMatrix to the appropriate uniform location
	// upload only one matrix, and don't transpose it

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);

	//floor
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject2D);
	size_t colorData2 = sizeof(vertexData2D) / 2;
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, 0, 0); //define **how** values are reader from positionBufferObject in Attrib 0
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorData2); //define **how** values are reader from positionBufferObject in Attrib 1
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix3)); //uploaed the modelMatrix to the appropriate uniform location
	glDrawArrays(GL_TRIANGLES, 0, 15); //Draw something, using Triangles, and 9 vertices 

	//cube 1
	size_t colorData = sizeof(vertexData) / 2;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); //bind positionBufferObject
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, 0, 0); //define **how** values are reader from positionBufferObject in Attrib 0
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorData); //define **how** values are reader from positionBufferObject in Attrib 1
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36); //Draw something, using Triangles, and 36 vertices 

	//cube2 modelMatrix
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix2)); //uploaed the modelMatrix to the appropriate uniform location
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 3
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(modelMatrix4), offset)));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 4
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(modelMatrix4), offset2)));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 5
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(modelMatrix4), offset3)));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 6
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(modelMatrix4), offset4)));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 7
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(modelMatrix5), offset5)));
	glDrawArrays(GL_TRIANGLES, 0, 36);


	glDisableVertexAttribArray(0); //cleanup
	glUseProgram(0); //clean up

}
void cleanUp()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	cout << "Cleaning up OK!\n";
}

int main(int argc, char* args[])
{
	exeName = args[0];
	//setup
	//- do just once
	initialise();
	createWindow();
	setGLAttributes();
	createContext();
	initGlew();


	//load stuff from files
	//- usually do just once
	loadAssets();


	while (!done && (SDL_GetTicks() < 500000)) //LOOP FROM HERE, for 2000ms (or if done flag is set)
		//WARNING: SDL_GetTicks is only accurate to milliseconds, use SDL_GetPerformanceCounter and SDL_GetPerformanceFrequency for higher accuracy
	{
		//GET INPUT HERE - PLACEHOLDER

		input();


		updateSimulation(0.002); //call update simulation with an amount of time to simulate for (in seconds)
		//WARNING - we are always updating by a constant amount of time. This should be tied to how long has elapsed
		// see, for example, http://headerphile.blogspot.co.uk/2014/07/part-9-no-more-delays.html

		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

		render(); //RENDER HERE - PLACEHOLDER

		SDL_GL_SwapWindow(win);; //present the frame buffer to the display (swapBuffers)

	} //LOOP TO HERE

	//cleanup and exit
	cleanUp();
	SDL_Quit();

	return 0;
}

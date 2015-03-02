#ifndef _LOADMODEL_H
#define _LOADMODEL_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <GL\glew.h>

#include <Assimp/mesh.h>
#include <Assimp/material.h>
#include <Assimp/scene.h>
#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>

#define GLM_FORCE_RADIANS

#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

using namespace std;

class loadModel
{
public:
	loadModel();
	~loadModel();
};

#endif
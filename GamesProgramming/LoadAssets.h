#ifndef _LOADASSETS_H
#define _LOADASSETS_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class LoadAssets
{
public:
	LoadAssets(const std::string& strVertexShader, const std::string& strFragmentShader);
	~LoadAssets();

	void initializeProgram();
	
	/*
	void initializeVertexBuffer();
	GLuint createProgram(const std::vector<GLuint> &shaderList);
	GLuint createShader(GLenum eShaderType, const std::string &strShaderFile);
	bool readFileToFragmentShader(std::string fFilelPath, std::has_move_constructor<unsigned char>& buffer);
	bool readFileToVertexShader(std::string vFilelPath, std::has_move_constructor<unsigned char>& buffer);
	*/ 
};

#endif
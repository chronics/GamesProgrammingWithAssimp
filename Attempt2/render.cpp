#include "render.h"
#include "loadAssets.h"
#include "vertexData.h"


render::render()
{
	glUseProgram(theProgram2); //installs the program object specified by program as part of current rendering state

	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix)); //uploaed the modelMatrix to the appropriate uniform location
	// upload only one matrix, and don't transpose it

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix)); //uploaed the modelMatrix to the appropriate uniform location
	// upload only one matrix, and don't transpose it

	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);

	//cube 1
	size_t colorData = sizeof(vertexData) / 2;
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); //bind positionBufferObject
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, 0, 0); //define **how** values are reader from positionBufferObject in Attrib 0
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorData); //define **how** values are reader from positionBufferObject in Attrib 1
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36); //Draw something, using Triangles, and 36 vertices 
}


render::~render()
{
}

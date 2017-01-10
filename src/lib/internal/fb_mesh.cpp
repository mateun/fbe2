#include "include\fb_mesh.h"
#include <stdio.h>
#include <SDL.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr


Mesh::Mesh(int numVertices, int numIndices, float* positions, float* texCoords, float* normals, GLuint* indices) {
	_numVertices = numVertices;
	_numIndices = numIndices;
	_positions = positions;
	_texCoords = texCoords;
	_indices = indices;
	_normals = normals;

	SDL_Log("Mesh created with %d vertices", numVertices);
	SDL_Log("Using indexed rendering %s", _numIndices != 0 ? "true" : "false");

	glGenBuffers(1, &_positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(float), _positions, GL_STATIC_DRAW);

	glGenBuffers(1, &_textureCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _textureCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * numVertices * sizeof(float), _texCoords, GL_STATIC_DRAW);

	if (_normals != 0) {
		glGenBuffers(1, &_normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(float), _normals, GL_STATIC_DRAW);
	}
	

	// We only setup the index buffer, if we 
	// have any indices to work with.
	if (_numIndices != 0 && _indices != 0) {
		glGenBuffers(1, &_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices * sizeof(GLuint), _indices, GL_STATIC_DRAW);
	}
	
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _positionBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _textureCoordBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	if (_normals != 0) {
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	}

	GLenum err = glGetError();
	if (err != 0) {
		SDL_Log("GL error after creating the mesh: %d", err);
	}


	// We can skip to bind the index buffer into the VertexArrayObject,
	// as this is simply not working :) 
	// It seems the VertexArrays just ignore this index buffer binding.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}

Mesh::~Mesh() {
	printf("Destroying the mesh");

}

void Mesh::Draw(GLSLProgram* shader, Texture* texture) {
	shader->use();
	GLenum err = glGetError();

	if (err != 0) {
		SDL_Log("GL error after using shader: %d", err);
	}

	if (texture != NULL) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureHandle());
		// We don't do this here as we want to avoid having to pass in the texture
		// sampler name from outside. 
		// We rely on the fact that there is a sampler available and activated at
		// stage 0 by default by OpenGL (hopefully this is true, at least it seems
		// to be :)=
		//glUniform1i(glGetUniformLocation(shader->getHandle(), "sampler"), 0);
	}

	err = glGetError();
	if (err != 0) {
		SDL_Log("GL error after setting up texture for rendering: %d", err);
	}

	// We use a vertex array now
	glBindVertexArray(_vao);

	// We still need to bind the index buffer for drawing, because 
	// this is (at least on my Intel HD4400 under Ubuntu 14.04) 
	// not part of the VAO scope, which we bound at construction time of 
	// this mesh object. 
	if (_numIndices != 0 && _indices != 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
		glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, _numVertices);
	}

}


// SolidColorMesh stuff
SolidColorMesh::SolidColorMesh(int numVertices, int numIndices, float* positions, float* normals, GLuint* indices) 
	: Mesh(numVertices, numIndices, positions, NULL, normals, indices)
{
	_numVertices = numVertices;
	_numIndices = numIndices;
	_positions = positions;
	_normals = normals;
	_indices = indices;

	SDL_Log("SolidColorMesh created with %d vertices", numVertices);

	glGenBuffers(1, &_positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(float), _positions, GL_STATIC_DRAW);

	glGenBuffers(1, &_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * numVertices * sizeof(float), _normals, GL_STATIC_DRAW);

	glGenBuffers(1, &_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices * sizeof(GLuint), _indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _positionBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	GLenum err = glGetError();
	if (err != 0) {
		SDL_Log("GL error after creating the mesh: %d", err);
	}


	// We can skip to bind the index buffer into the VertexArrayObject,
	// as this is simply not working :) 
	// It seems the VertexArrays just ignore this index buffer binding.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

SolidColorMesh::~SolidColorMesh() {
	SDL_Log("Destroying the solid color mesh\n");
}

void SolidColorMesh::DrawSolid(GLSLProgram* shader, glm::vec4 polyColor, glm::vec3 directionalLight) {
	shader->use();
	GLenum err = glGetError();

	if (err != 0) {
		SDL_Log("GL error after using shader: %d", err);
	}

	// Set the color and directional light uniforms
	GLint polyColorLocation = glGetUniformLocation(shader->getHandle(), "polyColor");
	GLint directionalLightLocation = glGetUniformLocation(shader->getHandle(), "directionalLight");
	glUniform4fv(polyColorLocation, 1, glm::value_ptr(polyColor));
	glUniform4fv(directionalLightLocation, 1, glm::value_ptr(directionalLight));

	err = glGetError();

	if (err != 0) {
		SDL_Log("GL error after using shader: %d", err);
	}

	// We use a vertex array now
	glBindVertexArray(_vao);

	err = glGetError();

	if (err != 0) {
		SDL_Log("GL error after using shader: %d", err);
	}

	// We still need to bind the index buffer for drawing, because 
	// this is (at least on my Intel HD4400 under Ubuntu 14.04) 
	// not part of the VAO scope, which we bound at construction time of 
	// this mesh object. 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

	glDrawElements(GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, _numVertices);
}
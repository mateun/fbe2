#pragma once

#ifndef MESH_H
#define MESH_H

#include "fb_export_spec.h"
#include "fb_glsl_program.h"
#include "fb_texture.h"
#include <SDL.h>
#include <glew.h>


#ifdef _WIN32
class FB_ENGINE_API Mesh
#else
class Mesh
#endif
{

public:
	Mesh(int numVertices, int numIndices, float* positions, float* texCoords, float* normals, GLuint* indices);
	~Mesh();
	virtual void Draw(GLSLProgram* shader, Texture *texture);

protected:
	int _numVertices;
	int _numIndices;
	float* _positions;
	float* _texCoords;
	float* _normals;
	GLuint* _indices;
	GLuint _positionBuffer;
	GLuint _textureCoordBuffer;
	GLuint _normalBuffer;
	GLuint _indexBuffer;
	GLuint _vao;

};

class FB_ENGINE_API SolidColorMesh : public Mesh {
public:
	SolidColorMesh(int numVertices, int numIndices, float* positions, float* normals, GLuint* indices);
	~SolidColorMesh();
	void DrawSolid(GLSLProgram* shader, glm::vec4 polyColor, glm::vec3 directionalLight);

};

#endif

#ifndef C_MESH_H
#define C_MESH_H

#include <vector>
#include <iostream>

// assimp include files. These three are usually needed.
#include <assimp/Importer.hpp>	//OO version Header!
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
/* GLFW 3.2.1
 * Open Source, multi-platform library for OpenGL
 */

#define VERTEX_BUFFER 0
#define TEXCOORD_BUFFER 1
#define NORMAL_BUFFER 2
#define INDEX_BUFFER 3

class cMeshEntry {
public:
	GLuint vao;
	GLuint vbo[4];
	float* vertices1D;
	float** vertices2D;
	int** faceIndexes2D;
	int* faceIndexes1D;

	unsigned int nbVertices, nbFaces, elementCount;

	cMeshEntry(aiMesh* mesh);
	~cMeshEntry();

	void render();
};

class cMesh {
public:
	std::vector<cMeshEntry*> meshEntries;

	cMesh(const char *filename);
	~cMesh(void);

	void render();
};

#endif

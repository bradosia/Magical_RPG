#include "cMesh.h"
/**
 *	Constructor, loading the specified aiMesh
 **/
cMeshEntry::cMeshEntry(aiMesh* mesh) {
	vao = 0;
	vbo[VERTEX_BUFFER] = 0;
	vbo[TEXCOORD_BUFFER] = 0;
	vbo[NORMAL_BUFFER] = 0;
	vbo[INDEX_BUFFER] = 0;

	glGenVertexArrays(1, &vao);
	glBindVertexArray (vao);

	nbVertices = mesh->mNumVertices;
	nbFaces = mesh->mNumFaces;
	elementCount = mesh->mNumFaces * 3;

	if (mesh->HasPositions()) {
		vertices1D = new float[mesh->mNumVertices * 3];
		vertices2D = new float*[mesh->mNumVertices];
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			vertices1D[i * 3] = (float) mesh->mVertices[i].x;
			vertices1D[i * 3 + 1] = (float) mesh->mVertices[i].y;
			vertices1D[i * 3 + 2] = (float) mesh->mVertices[i].z;
			vertices2D[i] = new float[3];
			vertices2D[i][0] = (float) mesh->mVertices[i].x;
			vertices2D[i][1] = (float) mesh->mVertices[i].y;
			vertices2D[i][2] = (float) mesh->mVertices[i].z;
			std::cout << "vertices:" << mesh->mVertices[i].x << ","
					<< mesh->mVertices[i].y << "," << mesh->mVertices[i].z
					<< std::endl;
		}

		glGenBuffers(1, &vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat),
				vertices1D, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
	}

	if (mesh->HasTextureCoords(0)) {
		float *texCoords = new float[mesh->mNumVertices * 2];
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
			texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat),
				texCoords, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		delete[] texCoords;
	}

	if (mesh->HasNormals()) {
		float *normals = new float[mesh->mNumVertices * 3];
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}

		glGenBuffers(1, &vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat),
				normals, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		delete[] normals;
	}

	if (mesh->HasFaces()) {
		unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
		faceIndexes1D = new int[mesh->mNumFaces * 3];
		faceIndexes2D = new int*[mesh->mNumFaces];
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
			faceIndexes1D[i * 3] = (int) mesh->mFaces[i].mIndices[0];
			faceIndexes1D[i * 3 + 1] = (int) mesh->mFaces[i].mIndices[1];
			faceIndexes1D[i * 3 + 2] = (int) mesh->mFaces[i].mIndices[2];
			faceIndexes2D[i] = new int[3];
			faceIndexes2D[i][0] = (int) mesh->mFaces[i].mIndices[0];
			faceIndexes2D[i][1] = (int) mesh->mFaces[i].mIndices[1];
			faceIndexes2D[i][2] = (int) mesh->mFaces[i].mIndices[2];
			std::cout << "faces:" << mesh->mFaces[i].mIndices[0] << ","
					<< mesh->mFaces[i].mIndices[1] << ","
					<< mesh->mFaces[i].mIndices[2] << std::endl;
			std::cout << "faces:" << faceIndexes2D[i][0] << ","
					<< faceIndexes2D[i][1] << "," << faceIndexes2D[i][2]
					<< std::endl;
		}

		glGenBuffers(1, &vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		delete[] indices;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/**
 *	Deletes the allocated OpenGL buffers
 **/
cMeshEntry::~cMeshEntry() {
	if (vbo[VERTEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[VERTEX_BUFFER]);
	}

	if (vbo[TEXCOORD_BUFFER]) {
		glDeleteBuffers(1, &vbo[TEXCOORD_BUFFER]);
	}

	if (vbo[NORMAL_BUFFER]) {
		glDeleteBuffers(1, &vbo[NORMAL_BUFFER]);
	}

	if (vbo[INDEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[INDEX_BUFFER]);
	}

	glDeleteVertexArrays(1, &vao);
}

/**
 *	Renders this cMeshEntry
 **/
void cMeshEntry::render() {
	glBindVertexArray (vao);
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

/**
 *	cMesh constructor, loads the specified filename if supported by Assimp
 **/
cMesh::cMesh(const char *filename) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,
			aiProcessPreset_TargetRealtime_Quality);
	if (!scene) {
		printf("Unable to laod cMesh: %s\n", importer.GetErrorString());
	}

	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		meshEntries.push_back(new cMeshEntry(scene->mMeshes[i]));
	}
}

/**
 *	Clears all loaded meshEntries
 **/
cMesh::~cMesh(void) {
	for (unsigned int i = 0; i < meshEntries.size(); ++i) {
		delete meshEntries.at(i);
	}
	meshEntries.clear();
}

/**
 *	Renders all loaded meshEntries
 **/
void cMesh::render() {
	for (unsigned int i = 0; i < meshEntries.size(); ++i) {
		meshEntries.at(i)->render();
	}
}

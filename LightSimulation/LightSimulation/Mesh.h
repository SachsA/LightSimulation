#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <vector>

#include "assimp\scene.h"
#include "assimp\mesh.h"
#include "Loader.h"

class Mesh
{
public:
	struct MeshEntry {
		static enum BUFFERS {
			VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
		};
		GLuint vao;
		GLuint vbo[4];

		unsigned int elementCount;
		aiColor3D dcolor;
		aiColor3D acolor;
		aiColor3D scolor;
		float shininessStrength;
		MeshEntry(aiMesh *mesh, const aiScene* scene, Mesh * m);
		~MeshEntry();
		Mesh * parent;
		void render();
	};

public:
	Mesh(const char *filename);
	~Mesh(void);


	std::vector<MeshEntry*> meshEntries;

	ShaderProgram * shader;
	void draw();
};
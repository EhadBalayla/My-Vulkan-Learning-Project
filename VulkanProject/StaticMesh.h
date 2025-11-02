#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class StaticMesh
{
public:
	void LoadMesh(const char* path); //simply loads in the first static mesh of an FBX file
	void UnloadMesh(); //simply unloads the mesh

	void Draw();
private:
	VertexBuffer VBO;
	IndexBuffer EBO;

	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);

	uint32_t indiciesCount;
};


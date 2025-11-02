#include "StaticMesh.h"

#include <iostream>
#include <vector>
#include "Engine.h"

void StaticMesh::LoadMesh(const char* path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    ProcessNode(scene->mRootNode, scene);
}
void StaticMesh::UnloadMesh() {
    VBO.destroyBuffer();
    EBO.destroyBuffer();
}




void StaticMesh::ProcessNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* firstMesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(firstMesh, scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}
void StaticMesh::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> verticies;
    std::vector<uint32_t> indicies;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.pos = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->HasTextureCoords(0)) {
            vertex.uv.x = mesh->mTextureCoords[0][i].x;
            vertex.uv.y = mesh->mTextureCoords[0][i].y;
        }

        verticies.push_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indicies.push_back(face.mIndices[j]);
    }

    VBO.createVertexBuffer(verticies);
    EBO.createIndexBuffer(indicies);

    indiciesCount = indicies.size();
}
void StaticMesh::Draw() {
    EBO.bindBuffer();
    VBO.bindBuffer();

    vkCmdDrawIndexed(Engine::m_Renderer.getBufferAt(Engine::m_Renderer.currentFrame), indiciesCount, 1, 0, 0, 0);
}
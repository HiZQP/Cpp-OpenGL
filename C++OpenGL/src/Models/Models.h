#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"

#include "glm/glm.hpp"

#include "LogSystem.h"

#include <vector>

namespace WTE {
	struct Vertex {
		glm::vec3 Pos;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	struct Texture {
		unsigned int id;
		std::string type;
		std::string path;
	};
}

class Mesh {
public:
	Mesh(const std::vector<WTE::Vertex>& vertices,
		 const std::vector<unsigned int>& indices,
		 const std::vector<WTE::Texture>& textures)
		: m_Vertices(vertices), m_Indices(indices), m_Textures(textures), m_Vao(0), m_Vbo(0), m_Ebo(0) { };
	~Mesh();

	void setup();

	void draw(const Shader& shader);

private:
	std::vector<WTE::Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<WTE::Texture> m_Textures;

	unsigned int m_Vao = 0, m_Vbo = 0, m_Ebo = 0;
};

class Model {
public:
	Model(const std::string& path);

	void draw(const Shader& shader);

	std::vector<Mesh> m_Meshes;
	std::vector<WTE::Texture> m_Textures_Loaded;
	std::string m_Directory;
private:
	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<WTE::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
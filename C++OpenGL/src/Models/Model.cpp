#include "Models.h"

Model::Model(const std::string& path)
	: m_Directory(path) {
	loadModel(m_Directory);
	for (unsigned int i = 0; i < m_Meshes.size(); i++) {
		m_Meshes[i].setup();
	}
}

void Model::draw(const Shader& shader) {
	for (unsigned int i = 0; i < m_Meshes.size(); i++) {
		m_Meshes[i].draw(shader);
	}
}

void Model::loadModel(const std::string& path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | 
												   aiProcess_FlipUVs | 
												   aiProcess_CalcTangentSpace);
	if (!scene) {
		LOG(LogLevel::LOG_LEVEL_ERROR, "模型加载失败" + (std::string)importer.GetErrorString() + path);
		return;
	}

	m_Directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// 这里的node是正常的树形结构，一个node可以对应多个mesh，多个node可以对应同一个mesh，有点像索引。
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<WTE::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<WTE::Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		WTE::Vertex vertex;
		vertex.Pos.x = mesh->mVertices[i].x;
		vertex.Pos.y = mesh->mVertices[i].y;
		vertex.Pos.z = mesh->mVertices[i].z;
		if (mesh->HasNormals()) {
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}
		if (mesh->mTextureCoords[0]) {
			vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = mesh->mTextureCoords[0][i].y;
		}
		else
			vertex.texCoord = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	return Mesh(vertices, indices, textures);
}

std::vector<WTE::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<WTE::Texture> a;
	return a;
}

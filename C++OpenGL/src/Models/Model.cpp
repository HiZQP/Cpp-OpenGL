#include "Models.h"

#include "Texture.h"

Model::Model(const std::string& path)
	: m_Directory(path) {
	loadModel(m_Directory);
	for (unsigned int i = 0; i < m_Meshes.size(); i++) {
		m_Meshes[i].setup();
	}
}

Model::~Model() {
	for (auto& tex : m_Textures_Loaded) {
		glDeleteTextures(1, &tex.id);
	}
}

void Model::draw(Shader& shader) {
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

unsigned int Model::loadTextureFromFile(const std::string& path) {
	unsigned int textureID;
	Texture texture(path);
	if (texture.getRendererID() == 0) {
		LOG(LogLevel::LOG_LEVEL_ERROR, "纹理加载失败: " + path);
		return 0;
	}
	return texture.getRendererID();
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// 这里的node是正常的树形结构，一个node可以对应多个mesh，多个node可以对应同一个mesh，有点像索引。
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.emplace_back(processMesh(mesh, scene));
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

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// 漫反射
		std::vector<WTE::Texture> diffuseMaps = loadMaterialTextures(material,
																	 aiTextureType_DIFFUSE, 
																	 "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 高光纹理
		std::vector<WTE::Texture> specularMaps = loadMaterialTextures(material,
																	 aiTextureType_SPECULAR,
																	 "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, indices, textures);
}

// 模型顶点数据与纹理数据不在同一个文件中，纹理的路径通常记录模型文件中
std::vector<WTE::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<WTE::Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < m_Textures_Loaded.size(); j++) {
			if (std::strcmp(m_Textures_Loaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(m_Textures_Loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			WTE::Texture texture;
			texture.id = loadTextureFromFile(m_Directory + '/' + str.C_Str());
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			m_Textures_Loaded.push_back(texture);
		}
	}
	return textures;
}

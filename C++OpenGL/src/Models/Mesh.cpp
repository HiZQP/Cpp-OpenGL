#include "Models.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"

Mesh::~Mesh() {
}

void Mesh::setup() {
	VertexBuffer vbo(&m_Vertices[0], m_Vertices.size() * sizeof(WTE::Vertex));
	m_Vbo = vbo.getRendererID();
	IndexBuffer ebo(&m_Indices[0], m_Indices.size());
	m_Ebo = ebo.getRendererID();

	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(3);
	layout.push<float>(2);

	VertexArray vao;
	vao.addBuffer(vbo, layout);
	m_Vao = vao.getRendererID();
	vao.unbind();
}

void Mesh::draw(Shader& shader) {
	shader.bind();
	unsigned int diffuseCount = 1;
	unsigned int specularCount = 1;
	for (unsigned int slot = 0; slot < m_Textures.size(); slot++) {
		GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // 激活纹理单元
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Textures[slot].id)); // 绑定纹理
		if (m_Textures[slot].type == "texture_diffuse") {
			shader.setUniform1i("u_Texture_Diffuse" + std::to_string(diffuseCount++), slot);
		}
		else if (m_Textures[slot].type == "texture_specular") {
			shader.setUniform1i("u_Texture_Specular" + std::to_string(specularCount++), slot);
		}
	}
	GLCall(glBindVertexArray(m_Vao));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo));
	GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr));
}
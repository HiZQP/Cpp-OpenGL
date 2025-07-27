#include "Models.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"

Mesh::~Mesh() {
	GLCall(glDeleteBuffers(1, &m_Vao));
	GLCall(glDeleteBuffers(1, &m_Vbo));
	GLCall(glDeleteBuffers(1, &m_Ebo));
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

void Mesh::draw(const Shader& shader) {
	shader.bind();
	GLCall(glBindVertexArray(m_Vao));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo));
	GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr));
}
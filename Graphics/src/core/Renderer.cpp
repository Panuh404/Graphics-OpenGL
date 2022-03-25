#include "Renderer.h"
#include <iostream>

void Renderer::Clear() const {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawElements(const VertexArray& vertexArray, const ElementBuffer& elementBuffer, const Shader& shader) const {
	shader.Bind();
	vertexArray.Bind();
	elementBuffer.Bind();
	glDrawElements(GL_TRIANGLES, elementBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawArrays(const VertexArray& vertexArray, const GLint first, const GLsizei count, const Shader& shader) const {
	shader.Bind();
	vertexArray.Bind();
	glDrawArrays(GL_TRIANGLES, first, count);
}
#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Shader.h"

class Renderer {
public:
	void Clear() const;
	void DrawElements(const VertexArray& vertexArray, const ElementBuffer& elementBuffer, const Shader& shader) const;
	void DrawArrays(const VertexArray& vertexArray, const GLint first, const GLsizei count, const Shader& shader) const;
};

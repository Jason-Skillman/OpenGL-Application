#pragma once

#include <memory>

#include "Test.h"
#include "glm/fwd.hpp"
#include "glm/gtc/matrix_transform.hpp"

class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Shader;
class Texture;

namespace test {
	class TestBatchRenderTextures : public Test {
	private:
		glm::vec3 translationA;
		glm::vec3 translationB;

		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture1, texture2;

		glm::mat4 projection, view;
		
	public:
		TestBatchRenderTextures();
		~TestBatchRenderTextures();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}

#include "engine/engine.h"
#include "engine/input/input.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/program.h"
#include "engine/renderer/vertex_index_array.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/material.h"
#include "engine/renderer/light.h"
#include "engine/renderer/mesh.h"
#include "engine/math/math.h"
#include "engine/renderer/gui.h"

int main(int argc, char** argv)
{
	#pragma region Filesystem Path Set
		filesystem::set_current_path("content");
	#pragma endregion
	#pragma region SDL Initialization and Preparation
		int result = SDL_Init(SDL_INIT_VIDEO);
		if (result != 0)
		{
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		}


		std::shared_ptr<Input> input = std::make_shared<Input>();
		input->Initialize();

		std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();
		renderer->Initialize(1280, 720);


		
		GUI::Initialize(renderer.get());
	#pragma endregion

	#pragma region Vertex Array

		VertexArray vertex_array;
		
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texcoords;
		
		Mesh::Load("meshes/ogre.obj", positions, normals, texcoords);

		if (normals.empty())
		{
			for (size_t i = 0; i < positions.size() - 2; i += 3)
			{
				glm::vec3 normal = math::calculate_normal(positions[i], positions[i + 1], positions[i + 2]);

				normals.push_back(normal);
				normals.push_back(normal);
				normals.push_back(normal);
			}
		}
		//glm::mat3 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//math::transform(positions, rotate);
		//math::transform(normals, rotate);

		if (!positions.empty())
		{
			vertex_array.CreateBuffer(VertexArray::POSITION, static_cast<GLsizei>(positions.size() * sizeof(glm::vec3)), static_cast<GLsizei>(positions.size()), (void*)&positions[0]);
			vertex_array.SetAttribute(VertexArray::POSITION, 3, 0, 0);
		}
		if (!normals.empty())
		{
			vertex_array.CreateBuffer(VertexArray::NORMAL, static_cast<GLsizei>(normals.size() * sizeof(glm::vec3)), static_cast<GLsizei>(normals.size()), (void*)&normals[0]);
			vertex_array.SetAttribute(VertexArray::NORMAL, 3, 0, 0);
		}
		if (!texcoords.empty())
		{
			vertex_array.CreateBuffer(VertexArray::TEXCOORD, static_cast<GLsizei>(texcoords.size() * sizeof(glm::vec2)), static_cast<GLsizei>(texcoords.size()), (void*)&texcoords[0]);
			vertex_array.SetAttribute(VertexArray::TEXCOORD, 2, 0, 0);
		}



		//vertex_array.CreateBuffer(VertexArray::MULTI, sizeof(cube_vertices), sizeof(cube_vertices) / sizeof(GLfloat), (void*)cube_vertices);
		//vertex_array.CreateIndexBuffer(GL_UNSIGNED_SHORT, sizeof(cube_elements) / sizeof(GLushort), (void*)cube_elements);
		//vertex_array.SetAttribute(VertexArray::POSITION, 3, 6 * sizeof(GLfloat), 0);
		//vertex_array.SetAttribute(VertexArray::NORMAL, 3, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));
		//vertex_array.SetAttribute(VertexArray::COLOR, 3, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat));
		//vertex_array.SetAttribute(VertexArray::TEXCOORD, 2, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat));

		Material material;
		Program* shader = new Program();
		
		shader->CreateShaderFromFile("shaders/texture_phong.vert", GL_VERTEX_SHADER);
		shader->CreateShaderFromFile("shaders/texture_phong.frag", GL_FRAGMENT_SHADER);
		shader->Link();
		shader->Use();


		material.ambient = glm::vec3(1.0f);
		material.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		material.specular = glm::vec3(1.0f);
		material.shininess = 64.0f;

		Texture* texture = new Texture();
		texture->CreateTexture("textures/ogre_diffuse.bmp");

		material.SetShader(shader);
		material.Use();

		Light light;
		light.position = glm::vec4(5.0f, 5.0f, 5.0f, 1.0f);
		light.ambient = glm::vec3(0.1f);
		light.diffuse = glm::vec3(1.0f);
		light.specular = glm::vec3(1.0f);
	
	#pragma endregion



	#pragma region Vertex and Fragment Source with Shader Program
		
		glm::vec3 eye = glm::vec3(0.0f, 0.0f, 5.0f);
		glm::mat4 mxView = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 mxProjection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.01f, 1000.0f);
		
		glm::mat4 mxTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 mxRotate = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	#pragma endregion

	#pragma region Render Loop
		bool quit = false;
		while (!quit)
		{
			SDL_Event event;
			SDL_PollEvent(&event);
	
			switch (event.type)
			{
				case SDL_QUIT:
					quit = true;
					break;
		
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) quit = true;
			}

			SDL_PumpEvents();
			g_timer.tick();

			glm::vec3 translate(0.0f, 0.0f, 0.0f);
			float speed = 10.0f;
			if (input->GetKey(SDL_SCANCODE_RIGHT))	translate.x = speed;
			if (input->GetKey(SDL_SCANCODE_LEFT))	translate.x = -speed;
			if (input->GetKey(SDL_SCANCODE_UP))	    translate.y = speed;
			if (input->GetKey(SDL_SCANCODE_DOWN))	translate.y = -speed;
			if (input->GetKey(SDL_SCANCODE_W))	    translate.z = speed;
			if (input->GetKey(SDL_SCANCODE_S))	    translate.z = -speed;

			mxTranslate = glm::translate(mxTranslate, translate * g_timer.dt());
			mxRotate = glm::rotate(mxRotate, glm::radians(45.0f) * g_timer.dt(), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 mxModel = mxTranslate * mxRotate;

			glm::mat4 model_view_matrix = mxView * mxModel;
			shader->SetUniform("model_view_matrix", model_view_matrix);
			
			glm::mat4 mvp_matrix = mxProjection * model_view_matrix;
			shader->SetUniform("mvp_matrix", mvp_matrix);
			
			material.SetShader(shader);
			light.SetShader(shader, mxView);

			GUI::Update(event);
			GUI::Begin(renderer.get());

			ImGui::Text("Hello world");
			light.Edit();
			material.Edit();
			
			GUI::End();
			
			renderer->ClearBuffer();
			GUI::Draw();
			vertex_array.Draw();
			renderer->SwapBuffer();
		}

	#pragma endregion

	#pragma region Exit and cleanup
		GUI::Shutdown();
		delete shader;
		material.Destroy();
		material.Destroy();
		renderer->Shutdown();
		input->Shutdown();

		SDL_Quit();
	#pragma endregion
	#pragma region Return
		return 0;
	#pragma endregion
}
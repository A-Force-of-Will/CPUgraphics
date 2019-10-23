#include "engine/engine.h"
#include "engine/input/input.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/program.h"
#include "engine/renderer/vertex_index_array.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/material.h"
#include "engine/renderer/light.h"

#pragma region GL Shader Vector Positions
const GLfloat positions[] =
{
	-0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
};

const GLfloat colors[] =
{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

//const GLfloat vertices[] =
//{
//	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
//	 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
//	 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
//};

const GLfloat vertices[] = {
-1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
	1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
	1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f  // bottom-left
};

const GLuint indices[] = {
	0, 1, 2,
	2, 3, 0
};

static float cube_vertices[] = {
	// Front
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	// Right
	1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
	1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
	1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	// Back
	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
	// Left
	-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
	// Bottom
	-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
	1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
	1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
	// Top
	-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f
};

static GLushort cube_elements[] =
{
	   0,  1,  2,  0,  2,  3,
	   4,  5,  6,  4,  6,  7,
	   8,  9, 10,  8, 10, 11,
	   12, 13, 14, 12, 14, 15,
	   16, 17, 18, 16, 18, 19,
	   20, 21, 22, 20, 22, 23
};


//const GLfloat cube_vertices[] = {
//	// front
//	-1.0, -1.0,  1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
//	 1.0, -1.0,  1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
//	 1.0,  1.0,  1.0, 0.0, 0.0, 1.0, 1.0, 1.0,
//	-1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 0.0, 1.0,
//	// back
//	-1.0, -1.0, -1.0, 1.0, 0.0, 0.0, 1.0, 0.0,
//	 1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0,
//	 1.0,  1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 1.0,
//	-1.0,  1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0
//};

//GLushort cube_elements[] = {
//	// front
//	0, 1, 2,
//	2, 3, 0,
//	// right
//	1, 5, 6,
//	6, 2, 1,
//	// back
//	7, 6, 5,
//	5, 4, 7,
//	// left
//	4, 0, 3,
//	3, 7, 4,
//	// bottom
//	4, 5, 1,
//	1, 0, 4,
//	// top
//	3, 2, 6,
//	6, 7, 3
//};
#pragma endregion

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
		renderer->Initialize(800, 600);
	#pragma endregion

	#pragma region Vertex Array

		VertexIndexArray vertex_array;
		
		vertex_array.CreateBuffer(VertexArray::MULTI, sizeof(cube_vertices), sizeof(cube_vertices) / sizeof(GLfloat), (void*)cube_vertices);
		vertex_array.CreateIndexBuffer(GL_UNSIGNED_SHORT, sizeof(cube_elements) / sizeof(GLushort), (void*)cube_elements);
		vertex_array.SetAttribute(VertexArray::POSITION, 3, 6 * sizeof(GLfloat), 0);
		vertex_array.SetAttribute(VertexArray::NORMAL, 3, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));
		//vertex_array.SetAttribute(VertexArray::COLOR, 3, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat));
		//vertex_array.SetAttribute(VertexArray::TEXCOORD, 2, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat));

		Material material;
		material.program = new Program();
		material.program->CreateShaderFromFile("shaders/phong.vert", GL_VERTEX_SHADER);
		material.program->CreateShaderFromFile("shaders/phong.frag", GL_FRAGMENT_SHADER);
		material.program->Link();
		material.program->Use();

		//Texture* texture = new Texture();
		//texture->CreateTexture("textures/nc.bmp");
		//material.textures.push_back(texture);

		material.ambient = glm::vec3(1.0f);
		material.diffuse = glm::vec3(0.2f, 0.2f, 1.0f);
		material.specular = glm::vec3(1.0f);
		material.shininess = 32.0f;

		material.Update();
		material.Use();

		Light light;
		light.position = glm::vec4(5.0f, 5.0f, 5.0f, 1.0f);
		light.ambient = glm::vec3(0.1f);
		light.diffuse = glm::vec3(1.0f);
		light.specular = glm::vec3(1.0f);
	
	#pragma endregion

	#pragma region Old Vertex Source
		//const char* vertex_source = "#version 430 \n \
		//layout (location = 0) in vec3 vposition; \n \
		//layout (location = 1) in vec3 vcolor; \n \
		//out vec3 fcolor; \n \
		//void main() \n \
		//{ \n \
		//	fcolor = vcolor; \n \
		//	gl_Position = vec4(vposition, 1.0); \n \
		//}";
	#pragma endregion

	#pragma region Old Fragment Source
		//const char* fragment_source = "#version 430 \n \
		//in vec3 fcolor; \n \
		//out vec4 color; \n \
		//void main() \n \
		//{ \n \
		//	color = vec4(fcolor, 1.0); \n \
		//}";
	#pragma endregion

	#pragma region Vertex and Fragment Source with Shader Program
		
		glm::vec3 eye = glm::vec3(0.0f, 0.0f, 5.0f);
		glm::mat4 mxView = glm::lookAt(eye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 mxProjection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.01f, 1000.0f);
		
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
			material.program->SetUniform("model_view_matrix", model_view_matrix);
			
			glm::mat4 mvp_matrix = mxProjection * model_view_matrix;
			material.program->SetUniform("mvp_matrix", mvp_matrix);

			light.SetShader(material.program, mxView);
			
			renderer->ClearBuffer();
			vertex_array.Draw();
			renderer->SwapBuffer();
		}

	#pragma endregion

	#pragma region Exit and cleanup
		material.Destroy();
		renderer->Shutdown();
		input->Shutdown();

		SDL_Quit();
	#pragma endregion
	#pragma region Return
		return 0;
	#pragma endregion
}
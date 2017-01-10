#include <stdio.h>
#include <fb_engine.h>
#include <string>
#include <fbxsdk.h>

/*
	This method retrieves the executable path 
	from the startup path which includes the 
	name of the .exe. 
	This is stripped from the path and the 
	front part of the path is returned.
*/
std::string GetExePath(const char* startupArg) {
	std::string argString = startupArg;
	std::size_t dotExePos = argString.find("game.exe");
	if (dotExePos != std::string::npos) {
		std::string exePath = argString.substr(0, dotExePos);
		return exePath;
	}
	else {
		return NULL;
	}
}

void DoNoiseTest() {
	FBNoise noise;
	double val = noise.GetValueForInput(1.25, 0.75, 0.5);
}


int main(int argc, char** args) {
	printf("hello king3D - try nr. 34!\n");

	// temp test
	DoNoiseTest();

	Display display;
	display.ShowWindow(800, 600, true);

	std::string exePath = GetExePath(args[0]);

	GLSLProgram simpleShader;
	std::string vertexShaderName = exePath + "simpleVS.glsl";
	std::string pixelShaderName = exePath + "simplePS.glsl";

	bool vsOK = simpleShader.compileShaderFromFile(vertexShaderName.c_str(), GLSLShader::VERTEX);
	bool psOK = simpleShader.compileShaderFromFile(pixelShaderName.c_str(), GLSLShader::FRAGMENT);
	SDL_Log("vsOK / psOK: %d, %d", vsOK, psOK);
	simpleShader.bindAttribLocation(0, "pos");
	simpleShader.bindAttribLocation(1, "texCoord");
	if (!simpleShader.link()) {
		SDL_Log("Shader linking failed!");
		exit(1);
	}
		
	GLfloat positions[] = {
		// Front plane
		-0.25, -0.25, 0, 
		0.25, -0.25, 0,
		0.25, 0.25, 0,
		-0.25, 0.25, 0,
		// Top plane
		-0.25, 0.25, 0,
		0.25, 0.25, 0,
		0.25, 0.25, -0.25,
		-0.25, 0.25, -0.25

	};

	GLfloat normals[] = {
		0, 0, 1, 
		0, 0, 1, 
		0, 0, 1,
		0, 0, 1,
		0, 1, 0, 
		0, 1, 0, 
		0, 1, 0,
		0, 1, 0
	};

	GLfloat uvs[] = {
		0, 1, 
		1, 1,
		1, 0,
		0, 0,
		0, 0,
		1, 0,
		1, 0,
		0, 0
	};

	GLuint indices[] = {
		// Front plane
		0, 1, 2,
		0, 2,3,
		// Top plane
		4, 5, 6,
		4, 6, 7
	};

	ModelImporter modelImporter;
	
	SolidColorMesh *scm = new SolidColorMesh(8, 12, positions, normals, indices);

	Mesh *mesh2 = modelImporter.ImportModelFromFile(exePath + "crate.fbx", FBX, false);
	 
	/*GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glUseProgram(simpleShader.getHandle());
	glPointSize(10);*/

	GLenum err = glGetError();
	if (err != 0)
		SDL_Log("GLError0: %d", err);

	GLint mvpLocation = glGetUniformLocation(simpleShader.getHandle(), "mvpMatrix");
	float horMove = 0.0f;
	
	Camera *cam = new Camera(90.0f, 4, 3);
	cam->Move(glm::vec3(0, 16.0f, -2));

	// Setup a texture for our mesh
	Texture *mainTexture = new Texture((exePath + "cube_diff2.bmp").c_str());

	err = glGetError();
	if (err != 0)
		SDL_Log("GLError1.3: %d", err);

	// Uncomment this for wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glCullFace(GL_BA);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_MULTISAMPLE);
	
	glLineWidth(1.5);
	
	glm::vec3 camMove = glm::vec3(0);
	float camSpeed = 3.0f;
	float objectRotate = 0;
	while (true) {
		display.PollForEvents();
		display.Clear(0, 0, 0.5f);

		err = glGetError();
		if (err != 0)
			SDL_Log("GLError1.4: %d", err);

		// some movement variable
		// We move the camera left and right
		if (display.IsKeyDown(SDL_Scancode::SDL_SCANCODE_RIGHT)) {
			camMove.x = 0.15f;
		}
		else if (display.IsKeyDown(SDL_Scancode::SDL_SCANCODE_LEFT)) {
			camMove.x = -0.15f;
		}
		else {
			camMove.x = 0;
		}
		if (display.IsKeyDown(SDL_SCANCODE_UP)) {
			camMove.z = -0.15f;
		}
		else if (display.IsKeyDown(SDL_SCANCODE_DOWN)) {
			camMove.z = 0.15f;
		}
		else {
			camMove.z = 0;
		}
		
		camMove *= camSpeed;
		
		err = glGetError();
		if (err != 0) 
			SDL_Log("GLError1.5: %d", err);

		// Setup the transformation matrix.
		glUseProgram(simpleShader.getHandle());
		glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		Model = glm::rotate(Model, objectRotate * 0, glm::vec3(1, 1, 1));
		Model = glm::translate(Model, glm::vec3(-4, 0, -10));
		cam->Move(camMove);
		cam->UpdateViewMatrix();
		glm::mat4 MVP = cam->GetProjectionMatrix() * cam->GetViewMatrix() * Model;
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

		// Set the directional light vector.
		glm::vec3 directionalLight = glm::vec3(-0.25f, 0.2f, 0.35f);
		float dot = glm::dot(glm::normalize(glm::vec3(-0, 0, 1)), glm::normalize((directionalLight)));
		GLint directionalLightLocation = glGetUniformLocation(simpleShader.getHandle(), "directionalLight");
		glUniform3fv(directionalLightLocation, 1, glm::value_ptr(directionalLight));

		GLint modelLocation = glGetUniformLocation(simpleShader.getHandle(), "worldMatrix");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Model));

		err = glGetError();
		if (err != 0)
			SDL_Log("GLError1.6: %d", err);
		
		mesh2->Draw(&simpleShader, mainTexture);

		for (int z = 0; z < 10; z++) {
			for (int x = 0; x < 10; x++) {
				Model = glm::translate(glm::mat4(1.0f), glm::vec3(x * 4, 0, -15*z));
				MVP = cam->GetProjectionMatrix() * cam->GetViewMatrix() * Model;
				glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));
				//scm->DrawSolid(&simpleShader, glm::vec4(0.2f, 1.0f, 0.1f, 1.0f), glm::vec3(0.0, -0.5f, -0.1f));
				mesh2->Draw(&simpleShader, mainTexture);
				err = glGetError();
				if (err != 0)
					SDL_Log("GLError2: %d", err);
			}
		}

		// Draw a second model to the right side
		/*for (int x = 0; x < 10; x++) {
			Model = glm::translate(glm::mat4(1.0f), glm::vec3(x*2, 0, 0));
			MVP = cam->GetProjectionMatrix() * cam->GetViewMatrix() * Model;
			glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));
			mesh->Draw(&simpleShader, grassTexture);
			err = glGetError();
			if (err != 0)
				SDL_Log("GLError2: %d", err);
		}*/
		
		
		/*GLfloat attrib[] = { (float)sin(SDL_GetTicks() * 0.0005f),
							 (float)cos(SDL_GetTicks() * 0.0006f),
							 0.0f, 0.0f };
		glVertexAttrib4fv(0, attrib);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		err = glGetError();
		if (err != 0)
			SDL_Log("GLError2: %d", err);
			*/

		display.Present();
	}

	return 0;
}
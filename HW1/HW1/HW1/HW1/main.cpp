#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.h"
#include "Shader.h"

using namespace std;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int modelVAO(Object& model);
void drawModel(const string& target, unsigned int& shaderProgram, const glm::mat4& M, const glm::mat4& V, const glm::mat4& P);

// Objects to display
Object rectangleModel("obj/rectangle.obj");
Object triangleModel("obj/triangle.obj");
Object clockModel("obj/clock.obj");
Object clockHandModel("obj/clock_hand.obj");
Object rabbitModel("obj/rabbit.obj");
Object tortoiseModel("obj/tortoise.obj");

unsigned int rectangleVAO, triangleVAO, clockVAO, clockHandVAO, rabbitVAO, tortoiseVAO;
int windowWidth = 800, windowHeight = 600;
float speed = 1;
float r = 0;

int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFE_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW1", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD\n";
		return -1;
	}

	// VAO, VBO
	rectangleVAO = modelVAO(rectangleModel);
	triangleVAO = modelVAO(triangleModel);
	clockVAO = modelVAO(clockModel);
	clockHandVAO = modelVAO(clockHandModel);
	rabbitVAO = modelVAO(rabbitModel);
	tortoiseVAO = modelVAO(tortoiseModel);

	// Shaders
	Shader shader("vertexShader.vert", "fragmentShader.frag");
	glUseProgram(shader.program);

	// TODO: Enable depth test, face culling
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Display loop
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);

	double lastTime = glfwGetTime();
	int numFrames = 0;

	glm::mat4 body(1.0f);
	glm::mat4 roof(1.0f);
	glm::mat4 clock(1.0f);
	glm::mat4 minute(1.0f);
	glm::mat4 hour(1.0f);
	glm::mat4 rabbit(1.0f);
	glm::mat4 tortoise(1.0f);

	body = glm::translate(body, glm::vec3(0.0f, 5.0f, 0.0f));
	roof = glm::translate(roof, glm::vec3(-0.2f, 16.25f, -0.35f));
	clock = glm::translate(clock, glm::vec3(0.0f, 9.5f, 4.3f));
	clock = glm::rotate(clock, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	minute = glm::translate(minute, glm::vec3(0.0f, 9.5f, 4.9f));
	minute = glm::rotate(minute, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	hour = glm::translate(hour, glm::vec3(0.0f, 9.5f, 4.55f));
	hour = glm::rotate(hour, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	rabbit = glm::translate(rabbit, glm::vec3(15.0f, 1.0f, 0.0f));
	rabbit = glm::scale(rabbit, glm::vec3(0.08f, 0.08f, 0.08f));

	tortoise = glm::translate(tortoise, glm::vec3(18.0f, 1.5f, 0.0f));
	tortoise = glm::scale(tortoise, glm::vec3(0.2f, 0.2f, 0.2f));
	tortoise = glm::rotate(tortoise, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	tortoise = glm::rotate(tortoise, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	tortoise = glm::rotate(tortoise, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	while (!glfwWindowShouldClose(window))
	{
		// Calculate time per frame
		double currentTime = glfwGetTime();
		numFrames++;
		// If last cout was more than 1 sec ago
		if (currentTime - lastTime >= 1.0)
		{
			// Print and reset timer
			cout << 1000.0 / numFrames << " ms/frame\n";
			numFrames = 0;
			lastTime += 1.0;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO: Create model, view, and perspective matrix
		glm::mat4 ground(1.0f);
		glm::mat4 body_r(1.0f);
		glm::mat4 rabbit_r(1.0f);
		glm::mat4 tortoise_r(1.0f);
		glm::mat4 ground_r(1.0f);

		glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 30.0f, 50.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		glm::mat4 perspective = glm::perspective(
			glm::radians(45.0f),
			(float)windowWidth / (float)windowHeight,
			0.1f,
			100.0f
		);

		// TODO: Draw base of clock tower
		ground_r = glm::translate(ground_r, glm::vec3(0.0f, -10.0f, -3.0f));
		ground = ground_r * ground;
		ground = glm::scale(ground, glm::vec3(20.0f, 1.0f, 21.0f));
		drawModel("rectangle", shader.program, ground, view, perspective);

		// TODO: Draw body of clock tower
		body_r = glm::rotate(body_r, glm::radians(0.5f*r), glm::vec3(0.0f, 1.0f, 0.0f));	//
		body = body_r * body;
		drawModel("rectangle", shader.program, glm::scale(body, glm::vec3(4.5f, 10.0f, 3.5f)), view, perspective);

		// TODO: Draw roof of clock tower
		roof = body_r * roof;
		drawModel("triangle", shader.program, glm::scale(roof, glm::vec3(5.0f, 4.0f, 3.3f)), view, perspective);

		// TODO: Draw clock on the clock tower
		clock = body_r * clock;
		drawModel("clock", shader.program, glm::scale(clock, glm::vec3(0.013f, 0.013f, 0.013f)), view, perspective);

		// TODO: Draw minute hand on the clock 
		minute = body_r * minute;
		minute = glm::rotate(minute, glm::radians(1.0f*speed), glm::vec3(0.0f, 0.0f, 1.0f));	//
		drawModel("clock hand", shader.program, glm::scale(minute, glm::vec3(0.8f, 0.7f, 1.0f)), view, perspective);

		// TODO: Draw hour hand on the clock
		hour = body_r * hour;
		hour = glm::rotate(hour, glm::radians(1.0f*speed / 60), glm::vec3(0.0f, 0.0f, 1.0f));	//
		drawModel("clock hand", shader.program, glm::scale(hour, glm::vec3(1.0f, 0.6f, 0.6f)), view, perspective);

		// TODO: Draw rabbit revolves around the clock tower (not (0, 0, 0))
		
		rabbit_r = glm::rotate(rabbit_r, glm::radians(-0.7f*speed), glm::vec3(0.0f, 1.0f, 0.0f));	//
		rabbit = rabbit_r * rabbit;
		drawModel("rabbit", shader.program, ground_r * rabbit, view, perspective);

		// TODO: Draw tortoise revolves around the clock tower (not (0, 0, 0))
		
		tortoise_r = glm::rotate(tortoise_r, glm::radians(-0.35f*speed), glm::vec3(0.0f, 1.0f, 0.0f));	//
		tortoise = tortoise_r * tortoise;;
		drawModel("tortoise", shader.program, ground_r * tortoise, view, perspective);

		// TODO: Control speed and rotation


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// TODO:
//	 1. press 1 to double the rotation speed
//   2. press 2 to halve the rotation speed
//   3. press 3 to rotate the clock tower
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)	speed *= 2;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)	speed /= 2;
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		if (r == 1)	r = 0;
		else r = 1;
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

unsigned int modelVAO(Object& model)
{
	unsigned int VAO, VBO[3];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model.positions.size()), &(model.positions[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model.normals.size()), &(model.normals[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model.texcoords.size()), &(model.texcoords[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

void drawModel(const string& target, unsigned int& shaderProgram, const glm::mat4& M, const glm::mat4& V, const glm::mat4& P)
{
	unsigned int mLoc, vLoc, pLoc;
	mLoc = glGetUniformLocation(shaderProgram, "M");
	vLoc = glGetUniformLocation(shaderProgram, "V");
	pLoc = glGetUniformLocation(shaderProgram, "P");
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(M));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(V));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(P));

	if (target == "rectangle")
	{
		glBindVertexArray(rectangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, rectangleModel.positions.size());
	}
	else if (target == "triangle")
	{
		glBindVertexArray(triangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, triangleModel.positions.size());
	}
	else if (target == "clock")
	{
		glBindVertexArray(clockVAO);
		glDrawArrays(GL_TRIANGLES, 0, clockModel.positions.size());
	}
	else if (target == "clock hand")
	{
		glBindVertexArray(clockHandVAO);
		glDrawArrays(GL_TRIANGLES, 0, clockHandModel.positions.size());
	}
	else if (target == "rabbit")
	{
		glBindVertexArray(rabbitVAO);
		glDrawArrays(GL_TRIANGLES, 0, rabbitModel.positions.size());
	}
	else if (target == "tortoise")
	{
		glBindVertexArray(tortoiseVAO);
		glDrawArrays(GL_TRIANGLES, 0, tortoiseModel.positions.size());
	}
	glBindVertexArray(0);
}
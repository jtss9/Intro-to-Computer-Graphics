#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.h"

using namespace std;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int createShader(const string& filename, const string& type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int modelVAO(Object& model);
unsigned int loadTexture(const char* tFileName);


Object penguinModel("obj/penguin.obj");
Object boardModel("obj/surfboard.obj");

unsigned int penguinVAO, boardVAO;
int windowWidth = 800, windowHeight = 600;

// You can use these parameters
float swingAngle = 0;
float swingPos = 0;
float swingAngleDir = -1;
float swingPosDir = 1;
float squeezeFactor = 0;
bool squeezing = false;
bool useGrayscale = false;
bool useRainbow = false;
float color_time = 0;
float swingSpeed = 1;

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
	/* TODO#0: Change window title to "HW2 - your student id"
   *        Ex. HW2 - 312550000
   */
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW2-110550071", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// You are encouraged to complete TODO#1~3 by finishing the functions createShader, createProgram, loadTexture, and modelVAO, but it's not mandatory.
	// In other words, you can complete TODO#1~3 without using these four functions.

	/* TODO#1: Create vertex shader, fragment shader, shader program
	 * Hint:
	 *       createShader, createProgram, glUseProgram
	 * Note:
	 *       vertex shader filename: "vertexShader.vert"
	 *		 fragment shader filename: "fragmentShader.frag"
	 */
	unsigned int vertexShader, fragmentShader, shaderProgram;
	vertexShader = createShader("vertexShader.vert", "vert");
	fragmentShader = createShader("fragmentShader.frag", "frag");
	shaderProgram = createProgram(vertexShader, fragmentShader);
	glUseProgram(shaderProgram);
	/* TODO#2: Load texture
     * Hint:
     *       loadTexture
	 * Note:
	 *       penguin texture filename: "obj/penguin_diffuse.jpg"
	 *       board texture filename : "obj/surfboard_diffuse.jpg"
     */
	unsigned int penguinTexture, boardTexture;
	penguinTexture = loadTexture("obj/penguin_diffuse.jpg");
	boardTexture = loadTexture("obj/surfboard_diffuse.jpg");
	/* TODO#3: Set up VAO, VBO
	 * Hint:
	 *       modelVAO
	 */
	penguinVAO = modelVAO(penguinModel);
	boardVAO = modelVAO(boardModel);

	// Display loop
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	double dt;
	double lastTime = glfwGetTime();
	double currentTime;

	/* TODO#4: Data connection - Retrieve uniform variable locations
	 *    1. Retrieve locations for model, view, and projection matrices.
	 *    2. Obtain locations for squeezeFactor, grayscale, and other parameters.
	 * Hint:
	 *    glGetUniformLocation
	 */
	GLuint modelMatrixLoc = glGetUniformLocation(shaderProgram, "M");
	GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "V");
	GLuint projectionMatrixLoc = glGetUniformLocation(shaderProgram, "P");

	GLuint squeezeFactorLoc = glGetUniformLocation(shaderProgram, "squeezeFactor");
	GLuint grayscaleLoc = glGetUniformLocation(shaderProgram, "useGrayscale");
	GLuint rainbowLoc = glGetUniformLocation(shaderProgram, "useRainbow");
	GLuint timeLoc = glGetUniformLocation(shaderProgram, "useTime");

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 perspective = glm::perspective(glm::radians(45.0f),(float)windowWidth / (float)windowHeight,1.0f, 100.0f);

		glm::mat4 view = glm::lookAt(glm::vec3(0, 5, 5),glm::vec3(0, 0.5, 0),glm::vec3(0, 1, 0));

		/* TODO#5-1: Render Board
		 *    1. Set up board model matrix.
		 *    2. Send model, view, and projection matrices to the program.
		 *    3. Send squeezeFactor, useGrayscale, or other parameters to the program.(for key 's')
		 *    4. Set the value of the uniform variable "useGrayscale" and render the board.(for key 'g')
		 * Hint:
		 *	  rotate, translate, scale
		 *    glUniformMatrix4fv, glUniform1i
		 *    glUniform1i, glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
		 */
		glm::mat4 board_model = glm::mat4(1.0f);
		board_model = glm::rotate(board_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		board_model = glm::rotate(board_model, glm::radians(swingAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		board_model = glm::translate(board_model, glm::vec3(0.0f, 0.0f, -0.5f));
		board_model = glm::translate(board_model, glm::vec3(0.0f, -swingPos, 0.0f));
		board_model = glm::scale(board_model, glm::vec3(0.03f, 0.03f, 0.03f));

		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, value_ptr(board_model));
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, value_ptr(perspective));

		glUniform1i(boardTexture, 0);
		glUniform1f(squeezeFactorLoc, 0);
		glUniform1f(grayscaleLoc, useGrayscale);
		glUniform1f(rainbowLoc, useRainbow);
		glUniform1f(timeLoc, color_time);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boardTexture);
		glBindVertexArray(boardVAO);
		glDrawArrays(GL_TRIANGLES, 0, boardModel.positions.size());

	    /* TODO#5-2: Render Penguin
	     *    1. Set up penguin model matrix.
	     *    2. Send model, view, and projection matrices to the program.
	     *    3. Send squeezeFactor, useGrayscale, or other parameters to the program.(for key 's')
	     *    4. Set the value of the uniform variable "useGrayscale" and render the penguin.(for key 'g')
	     * Hint:
	     *	 rotate, translate, scale
	     *   glUniformMatrix4fv, glUniform1i
	     *   glUniform1i, glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
	     */
		glm:: mat4 penguin_model = glm::mat4(1.0f);
		penguin_model = glm::rotate(penguin_model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		penguin_model = glm::rotate(penguin_model, glm::radians(swingAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		penguin_model = glm::translate(penguin_model, glm::vec3(0.0f, -swingPos, 0.0f));
		penguin_model = glm::scale(penguin_model, glm::vec3(0.025f, 0.025f, 0.025f));
		
		glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, value_ptr(penguin_model));
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, value_ptr(perspective));

		glUniform1i(penguinTexture, 0);
		glUniform1f(squeezeFactorLoc, squeezeFactor);
		glUniform1f(grayscaleLoc, useGrayscale);
		glUniform1f(rainbowLoc, useRainbow);
		glUniform1f(timeLoc, color_time);

		glUniform1i(glGetUniformLocation(shaderProgram, "useGrayscale"), useGrayscale ? 1 : 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, penguinTexture);
		glBindVertexArray(penguinVAO);
		glDrawArrays(GL_TRIANGLES, 0, penguinModel.positions.size());
		glBindVertexArray(0);

		// Status update
		currentTime = glfwGetTime();
		dt = currentTime - lastTime;
		lastTime = currentTime;
		color_time = currentTime;
	    /* TODO#6: Update "swingAngle", "swingPos", "squeezeFactor"
	     * Speed:
	     *	  swingAngle   : 20 degrees/sec
		 *    swingPos     : 1.0f / sec
		 *    squeezeFactor: 90 degrees/sec
	     */
		swingAngle += dt * 20 * swingAngleDir * swingSpeed;
		if (swingAngle < -20) {
			swingAngle = -20;
			swingAngleDir = -swingAngleDir;
		}
		else if (swingAngle > 20) {
			swingAngle = 20;
			swingAngleDir = -swingAngleDir;
		}
		swingPos += dt * 1 * swingPosDir * swingSpeed;
		if (swingPos > 2) {
			swingPos = 2;
			swingPosDir = -swingPosDir;
		}
		else if (swingPos < 0) {
			swingPos = 0;
			swingPosDir = -swingPosDir;
		}
		if (squeezing) {
			squeezeFactor += dt * 3.14/2;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Add key callback

/* TODO#7: Key callback
 *    1. Press 's' to squeeze the penguin.
 *    2. Press 'g' to change to grayscale.
 *    3. Print "KEY S PRESSED" when key 's' is pressed, and similarly for other keys.
 */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		printf("KEY S PRESSED\n");
		squeezing = !squeezing;
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		printf("KEY G PRESSED\n");
		useGrayscale = !useGrayscale;
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		printf("KEY R PRESSED\n");
		useRainbow = !useRainbow;
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		swingSpeed *= 1.35;
		printf("SPEED UP  speed=%f\n", swingSpeed);
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		swingSpeed /= 1.35;
		printf("SPEED DOWN  speed=%f\n", swingSpeed);
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

/* TODO#1-1: createShader
* Hint:
*       glCreateShader, glShaderSource, glCompileShader
*/
unsigned int createShader(const string& filename, const string& type)
{
	unsigned int shader;
	if (type == "vert")
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (type == "frag")
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else
	{
		cout << "Unknown Shader Type.\n";
		return 0;
	}

	// Read the code of shader file
	ifstream fs(filename);
	stringstream ss;
	string s;
	while (getline(fs, s))
	{
		ss << s << "\n";
	}
	string temp = ss.str();
	const char* source = temp.c_str();

	// Compile shader
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	// Debug info
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::" << type << "::COMPLIATION_FAILED\n" << infoLog << "\n";
		return 0;
	}

	return shader;
}

/* TODO#1-2: createProgram
* Hint:
*       glCreateProgram, glAttachShader, glLinkProgram, glDetachShader
*/
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	//Link our program
	glLinkProgram(program);
	//Note the different functions here: glGetProgram* instead of glGetShader*.
	int success = 0;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
		return -1;
	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

/* TODO#2: Load texture
 * Hint:
 *       glEnable, glGenTextures, glBindTexture, glTexParameteri, glTexImage2D
 */
unsigned int loadTexture(const char* tFileName) {
	unsigned int texture;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
	return texture;
}

/* TODO#3: Set up VAO, VBO
 * Hint:
 *       glGenVertexArrays, glBindVertexArray, glGenBuffers, glBindBuffer, glBufferData,
 *       glVertexAttribPointer, glEnableVertexAttribArray, 
 */
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



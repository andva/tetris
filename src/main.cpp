#include "rendering.h"
#include "grid.h"

#include <assert.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <memory>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static std::unique_ptr<PieceGrid> currentPG;
static uint32_t ptVbo;
static int32_t ptSize;

static void update_renderable() {
	auto pts = currentPG->GetRenderable();
	glBindBuffer(GL_ARRAY_BUFFER, ptVbo);
	glBufferData(GL_ARRAY_BUFFER, pts.size() * sizeof(float), pts.data(), GL_DYNAMIC_DRAW);
	ptSize = pts.size();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		currentPG->Rotate(LEFT);
		update_renderable();
	}
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		currentPG->Rotate(RIGHT);
		update_renderable();
	}
}

int main(int argc, char *argv[]) {
	GLFWwindow* window = init_rendering();
	int32_t shader_program = compile_program();
	glfwSetKeyCallback(window, key_callback);
	PieceArray pa;
	pa[0] = {{0, 0, 0, 0, 0}};
	pa[1] = {{0, 0, 0, 0, 0}};
	pa[2] = {{0, 1, 1, 1, 0}};
	pa[3] = {{0, 0, 1, 0, 0}};
	pa[4] = {{0, 0, 0, 0, 0}};
	currentPG.reset(new PieceGrid(0, 0, pa));
	ptVbo = 0;
	glGenBuffers(1, &ptVbo);
	update_renderable();
	
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_program);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, ptVbo);
		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, ptSize / 3);
		// update other events like input handling 
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

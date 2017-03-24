#include "rendering.h"
#include "grid.h"
#include "piece.h"
#include "piecefactory.h"
#include "piecemanager.h"
#include "game.h"
#include "ai.h"

#include <assert.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#define NO_AI false

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_J && action == GLFW_RELEASE) {
		ExecuteAction(LEFT, ROTATE);
	}
	else if (key == GLFW_KEY_K && action == GLFW_RELEASE) {
		ExecuteAction(RIGHT, ROTATE);
	}
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		ExecuteAction(LEFT, MOVE);
	}
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		ExecuteAction(RIGHT, MOVE);
	}
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		ExecuteAction(DOWN, MOVE);
	}
	else if (key == GLFW_KEY_H && action == GLFW_RELEASE) {
		Hold();
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
		Drop(sGrid);
	}
}

uint32_t create_render_grid() {
	const int32_t X_D = 10;
	const int32_t Y_D = 20;
	uint32_t gridVbo = 0;
	glGenBuffers(1, &gridVbo);
	std::vector<float> pts;
	for (int y = 0; y <= Y_D; y++) {
		for (int x = 0; x <= X_D; x++) {
			glm::vec2 p = glm::vec2(
				static_cast<float>(x) / static_cast<float>(X_D),
				static_cast<float>(y) / static_cast<float>(Y_D));
			pts.push_back(p.x * 2.0f - 1.0f);
			pts.push_back(1.0f - p.y * 2.0f);
			pts.push_back(0.0f);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, gridVbo);
	glBufferData(GL_ARRAY_BUFFER, pts.size() * sizeof(float), pts.data(), GL_STATIC_DRAW);
	return gridVbo;
}

void draw(IboData data, GLint colorLoc, int32_t color) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.id);
	if (colorLoc != -1) {
		glUniform1ui(colorLoc, color);
	}
	glDrawElements(
		GL_TRIANGLES,
		data.size,
		GL_UNSIGNED_INT,
		(void*) 0
		);
}

void Reset() {
	sPieceManager.reset(new PieceManager);
	sGrid.reset(new Grid);
	update_renderable(sPieceManager->GetPiece()->GetRenderable(), pieceIbo);
	for (int i = 0; i < gridIbos.size(); i++) {
		update_renderable(sGrid->GetRenderable(i + 1), gridIbos[i]);
	}
	sHeurestics.aggregateHeight = 0;
	sHeurestics.bumpiness = 0;
	sHeurestics.completeLines = 0;
	sHeurestics.holes = 0;
	sHeurestics.score = 0;
}

int main(int argc, char *argv[]) {
	GLFWwindow* window = init_rendering();
	int32_t shader_program = compile_program();
#if NO_AI
	glfwSetKeyCallback(window, key_callback);
#endif
	Ai ai(-0.510066, 0.760666, -0.35663, -0.184483);
	uint32_t ptVbo = create_render_grid();
	glGenBuffers(1, &pieceIbo.id);
	for (int i = 0; i < gridIbos.size(); i++) {
		glGenBuffers(1, &gridIbos[i].id);
	}

	Reset();

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glUseProgram(shader_program);
	GLint loc = glGetUniformLocation(shader_program, "color");
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, ptVbo);
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		// AI
#if !NO_AI
		ai.CalculateOptimal(sPieceManager->GetPiece()->GetType(), sGrid);
		Drop(sGrid);
#endif
		// Drawing
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (uint32_t i = 0; i < gridIbos.size(); i++) {
			draw(gridIbos[i], loc, i + 1);
		}

		draw(pieceIbo, loc, sPieceManager->GetPiece()->GetType());

		
		glfwPollEvents();

		glfwSetWindowTitle(window, sPieceManager->GetFutureState().c_str());
		glfwSwapBuffers(window);
		
		if (sGrid->HasLost()) {
			Reset();
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(400));
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

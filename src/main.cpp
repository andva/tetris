#include "rendering.h"
#include "grid.h"
#include "piece.h"
#include "piecefactory.h"
#include "piecemanager.h"
#include "game.h"
#include "ai.h"
#include "human.h"

#include <assert.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#define USE_AI false

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
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



void Reset() {
	//sPieceManager.reset(new PieceManager);
	//sGrid.reset(new Grid);
	//update_renderable(sGrid->GetRenderable(sPieceManager->GetPiece()->GetCollisionObject()), pieceIbo);
	//for (int i = 0; i < gridIbos.size(); i++) {
	//	update_renderable(sGrid->GetRenderable(i + 1), gridIbos[i]);
	//}
	//sHeurestics.aggregateHeight = 0;
	//sHeurestics.bumpiness = 0;
	//sHeurestics.completeLines = 0;
	//sHeurestics.holes = 0;
	//sHeurestics.score = 0;
}

int main(int argc, char *argv[]) {
	GLFWwindow* window = init_rendering();
	int32_t shader_program = compile_program();

	uint32_t ptVbo = create_render_grid();

#if USE_AI
	Ai ai(-0.510066f, 0.760666f, -0.35663f, -0.184483f);
#else
	Human h;
	h.GetGame().Reset();
	glfwSetKeyCallback(window, Human::HumanKeyCallback);
#endif

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if USE_AI
		ai.CalculateOptimal(sPieceManager->GetPiece()->GetType(), sGrid);
		Drop(sGrid);
		ai.GetGame().Render(loc);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
#else
		glfwPollEvents();
		h.Update();
		h.GetGame().Render(loc);
#endif

		//glfwSetWindowTitle(window, sPieceManager->GetFutureState().c_str());
		glfwSwapBuffers(window);
		
		/*if (sGrid->HasLost()) {
			Reset();
		}*/
		
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

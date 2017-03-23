#include "rendering.h"
#include "grid.h"
#include "piece.h"

#include <assert.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <functional>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

struct IboData {
	uint32_t id;
	int32_t size;
};

static std::unique_ptr<Piece> currentPiece;
static std::unique_ptr<Grid> sGrid;

static IboData pieceIbo;
static IboData gridIbo;

static void update_renderable(const std::vector<uint32_t>& indices, IboData& iboData) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboData.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	iboData.size = indices.size();
}

static Direction GetOpposite(Direction d) {
	if (d == LEFT) {
		return RIGHT;
	}
	if (d == RIGHT) {
		return LEFT;
	}
	if (d == UP) {
		return DOWN;
	}
	if (d == DOWN) {
		return UP;
	}
}

static void ExecuteAction(Direction d, Action a) {
	std::function<void(Direction)> action;
	if (a == MOVE) {
		action = [] (Direction d) { currentPiece->Move(d); };
	}
	else if (a == ROTATE) {
		action = [] (Direction d) { currentPiece->Rotate(d); };
	}
	action(d);
	if (!sGrid->Validate(currentPiece->GetCollisionObject())) {
		action(GetOpposite(d));
	}
	else {
		update_renderable(currentPiece->GetRenderable(), pieceIbo);
	}
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
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
		if (sGrid->Place(currentPiece->GetCollisionObject(), 1)) {
			currentPiece.reset();
			PieceArray pa;
			pa[0] = {{0, 0, 0, 0, 0}};
			pa[1] = {{0, 1, 1, 1, 0}};
			pa[2] = {{0, 0, 1, 0, 0}};
			pa[3] = {{0, 0, 0, 0, 0}};
			pa[4] = {{0, 0, 0, 0, 0}};

			currentPiece.reset(new Piece(0, 0, pa));
			update_renderable(currentPiece->GetRenderable(), pieceIbo);
			update_renderable(sGrid->GetRenderable(), gridIbo);
		}
	}
}

uint32_t create_render_grid() {
	const int32_t X_D = 10;
	const int32_t Y_D = 22;
	uint32_t gridVbo = 0;
	glGenBuffers(1, &gridVbo);
	std::vector<float> pts;
	for (int y = 0; y <= Y_D; y++) {
		for (int x = 0; x <= X_D; x++) {
			glm::vec2 p = glm::vec2(
				static_cast<float>(x) / static_cast<float>(X_D),
				static_cast<float>(y) / static_cast<float>(Y_D));
			pts.push_back((p.x * 2.0f - 1.0f));
			pts.push_back((1.0f - p.y * 2.0f));
			pts.push_back(0.0f);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, gridVbo);
	glBufferData(GL_ARRAY_BUFFER, pts.size() * sizeof(float), pts.data(), GL_STATIC_DRAW);
	return gridVbo;
}

int main(int argc, char *argv[]) {
	GLFWwindow* window = init_rendering();
	int32_t shader_program = compile_program();
	glfwSetKeyCallback(window, key_callback);
	PieceArray pa;
	pa[0] = {{0, 0, 0, 0, 0}};
	pa[1] = {{0, 1, 1, 1, 0}};
	pa[2] = {{0, 0, 1, 0, 0}};
	pa[3] = {{0, 0, 0, 0, 0}};
	pa[4] = {{0, 0, 0, 0, 0}};

	currentPiece.reset(new Piece(0, 0, pa));
	sGrid.reset(new Grid);
	uint32_t ptVbo = create_render_grid();
	glGenBuffers(1, &pieceIbo.id);
	glGenBuffers(1, &gridIbo.id);
	update_renderable(currentPiece->GetRenderable(), pieceIbo);
	update_renderable(sGrid->GetRenderable(), gridIbo);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_program);
		
		// Draw grid
		
		
		// Draw piece
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, ptVbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pieceIbo.id);
		glBindVertexArray(vao);
		glDrawElements(
			GL_TRIANGLES,
			pieceIbo.size,
			GL_UNSIGNED_INT,
			(void*)0
 		);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIbo.id);
		glBindVertexArray(vao);
		glDrawElements(
			GL_TRIANGLES,
			gridIbo.size,
			GL_UNSIGNED_INT,
			(void*)0
 		);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

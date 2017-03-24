#include "rendering.h"
#include "grid.h"
#include "piece.h"
#include "piecefactory.h"
#include "piecemanager.h"

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

struct Heurestics {
	uint32_t aggregateHeight;
	uint32_t completeLines;
	uint32_t holes;
	uint32_t bumpiness;
	uint32_t score;
};

static Heurestics sHeurestics;

static IboData pieceIbo;
static std::unique_ptr<PieceManager> sPieceManager;
static std::unique_ptr<Grid> sGrid;
std::array<IboData, NUM_PIECES> gridIbos;

static void print_heuristics() {
	std::cout
		<< "AH:" <<sHeurestics.aggregateHeight << " "
		<< "BN:" << sHeurestics.bumpiness << " "
		<< "CL:" << sHeurestics.completeLines << " "
		<< "HL:" << sHeurestics.holes << " "
		<< "SC:" << sHeurestics.score << " "
		<< std::endl;
}

static void update_renderable(const std::vector<uint32_t>& indices, IboData& iboData) {
	if (indices.size() == 0) {
		iboData.size = 0;
		return;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboData.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		&indices[0],
		GL_STATIC_DRAW);
	iboData.size = static_cast<int>(indices.size());
}

void CalculateScore(std::pair<uint32_t, bool> res) {
	int32_t score = 0;
	if (!res.second) {
		if (res.first == 1) {
			score += 100;
		}
		else if (res.first == 2) {
			score += 300;
		}
		else if (res.first == 3) {
			score += 500;
		}
		else if (res.first == 4) {
			score += 800;
		}
	}
	else {
		assert(res.first <= 3);
		if (res.first == 1) {
			score += 800;
		}
		else if (res.first == 2) {
			score += 1200;
		}
		else if (res.first == 3) {
			score += 1600;
		}
	}
	sHeurestics.score += score;
}

static bool ExecuteAction(Direction d, Action a) {
	std::function<void(Direction)> action;
	if (a == MOVE) {
		action = [] (Direction d) { sPieceManager->GetPiece()->Move(d); };
	}
	else if (a == ROTATE) {
		action = [] (Direction d) {sPieceManager->GetPiece()->Rotate(d); };
	}
	action(d);
	if (!sGrid->Validate(sPieceManager->GetPiece()->GetCollisionObject())) {
		action(GetOpposite(d));
		return false;
	}
	else {
		update_renderable(sPieceManager->GetPiece()->GetRenderable(), pieceIbo);
		return true;
	}
}

static void Hold() {
	if (sPieceManager->Hold()) {
		update_renderable(sPieceManager->GetPiece()->GetRenderable(), pieceIbo);
	}
}

static void Drop() {
	while (ExecuteAction(DOWN, MOVE));
	auto collObj = sPieceManager->GetPiece()->GetCollisionObject();
	if (sGrid->IsAnythingUnder(collObj)) {
		auto res = sGrid->Place(
			collObj,
			sPieceManager->GetPiece()->GetType());
		sHeurestics.completeLines += res.first;

		sPieceManager->SetNext();
			
		update_renderable(sPieceManager->GetPiece()->GetRenderable(), pieceIbo);
		for (int i = 0; i < gridIbos.size(); i++) {
			update_renderable(sGrid->GetRenderable(i + 1), gridIbos[i]);
		}
		sGrid->CalculateGridHeuristics(
			sHeurestics.holes,
			sHeurestics.aggregateHeight,
			sHeurestics.bumpiness
			);
		CalculateScore(res);
		print_heuristics();

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
	else if (key == GLFW_KEY_H && action == GLFW_RELEASE) {
		Hold();
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
		Drop();
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
}

int main(int argc, char *argv[]) {
	GLFWwindow* window = init_rendering();
	int32_t shader_program = compile_program();
	glfwSetKeyCallback(window, key_callback);

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
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

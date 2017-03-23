#include <stdio.h>


#include "rendering.h"
#include "grid.h"
#include "piece.h"
#include "piecefactory.h"

#include <assert.h>
#include <vector>
#include <list>
#include <iostream>
#include <memory>
#include <functional>
#include <sstream>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

struct IboData {
	uint32_t id;
	int32_t size;
};

static std::list<PieceType> sPieceQueue;
static PieceType sStoredPiece;
static bool sHoldLock;
static IboData pieceIbo;
static std::unique_ptr<Piece> sPiece;

static std::unique_ptr<Grid> sGrid;
std::array<IboData, NUM_PIECES> gridIbos;

void fillPieceQueue() {
	while (sPieceQueue.size() < 4) {
		sPieceQueue.push_back(getRandomPiece());
	}
}

void SetNewPiece() {
	sPiece.reset(CreatePiece(sPieceQueue.front()));
	sPieceQueue.pop_front();
	fillPieceQueue();
}

static void update_renderable(const std::vector<uint32_t>& indices, IboData& iboData) {
	if (indices.size() == 0) {
		iboData.size = 0;
		return;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboData.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	iboData.size = indices.size();
}

static bool ExecuteAction(Direction d, Action a) {
	std::function<void(Direction)> action;
	if (a == MOVE) {
		action = [] (Direction d) { sPiece->Move(d); };
	}
	else if (a == ROTATE) {
		action = [] (Direction d) { sPiece->Rotate(d); };
	}
	action(d);
	if (!sGrid->Validate(sPiece->GetCollisionObject())) {
		action(GetOpposite(d));
		return false;
	}
	else {
		update_renderable(sPiece->GetRenderable(), pieceIbo);
		return true;
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
		if (sStoredPiece == NUM_PIECES) {
			sStoredPiece = sPiece->GetType();
			SetNewPiece();
			
			update_renderable(sPiece->GetRenderable(), pieceIbo);
			sHoldLock = true;
		}
		else if (!sHoldLock) {
			PieceType newType = sPiece->GetType();
			sPiece.reset(CreatePiece(sStoredPiece));
			sStoredPiece = newType;
			sHoldLock = true;
			update_renderable(sPiece->GetRenderable(), pieceIbo);
		}
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
		while (ExecuteAction(DOWN, MOVE));
		if (sGrid->Place(sPiece->GetCollisionObject(), sPiece->GetType())) {
			SetNewPiece();
			
			update_renderable(sPiece->GetRenderable(), pieceIbo);
			for (int i = 0; i < gridIbos.size(); i++) {
				update_renderable(sGrid->GetRenderable(i + 1), gridIbos[i]);
			}
			sHoldLock = false;
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
	sStoredPiece = NUM_PIECES;
	sHoldLock = false;
	fillPieceQueue();
	SetNewPiece();

	sGrid.reset(new Grid);
	uint32_t ptVbo = create_render_grid();
	glGenBuffers(1, &pieceIbo.id);
	for (int i = 0; i < gridIbos.size(); i++) {
		glGenBuffers(1, &gridIbos[i].id);
	}
	update_renderable(sPiece->GetRenderable(), pieceIbo);
	for (int i = 0; i < gridIbos.size(); i++) {
		update_renderable(sGrid->GetRenderable(i + 1), gridIbos[i]);
	}

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_program);
		
		// Draw piece
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, ptVbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pieceIbo.id);
		glBindVertexArray(vao);
		GLint loc = glGetUniformLocation(shader_program, "color");
		if (loc != -1) {
   			glUniform1ui(loc, sPiece->GetType() - 1);
		}
		glDrawElements(
			GL_TRIANGLES,
			pieceIbo.size,
			GL_UNSIGNED_INT,
			(void*)0
 		);
		for (uint32_t i = 0; i < gridIbos.size(); i++) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIbos[i].id);
			if (loc != -1) {
   				glUniform1ui(loc, i);
			}
			glDrawElements(
				GL_TRIANGLES,
				gridIbos[i].size,
				GL_UNSIGNED_INT,
				(void*)0
 			);
		}
		glfwPollEvents();
		std::stringstream ss;
		for (auto pt : sPieceQueue) {
			ss << ToName(pt) << " ";
		}
		ss << "(" << ToName(sStoredPiece) << ")";
		glfwSetWindowTitle(window, ss.str().c_str());
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

#define GLEW_STATIC
#include <sstream>
#include "GameWindow.h"
using namespace glm;

GameWindow::GameWindow(int width, int height, int cameraHeight, int mouseBoard, const char *name, bool fullScreen) {
	mousePressed = false;
	windowBoard = mouseBoard;
	cameraX = 0; cameraY = 0; cameraZ = cameraHeight;
	if(!glfwInit())	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}
    GLFWmonitor *mode = NULL;
    if (fullScreen) {
        mode = glfwGetPrimaryMonitor();
    }
	window = glfwCreateWindow(width, height, name, mode, NULL);
    //gets window size like this, because if glfw cant open window with our parametrs it takes nearest
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    pointTransformY = (std::tan(22.5 / 180 * 3.14)) * cameraHeight;
    pointTransformX = pointTransformY * ((float)windowWidth / (float)windowHeight);

	if( window == NULL ){		
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, width / 2, height / 2);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 		
	glEnable(GL_CULL_FACE);

	programID = loadProgram("Shaders/VertexShader.vsh", "Shaders/FragmentShader.fsh", false);
	textView = new TextView("Textures/text.dds", vec2((float)windowWidth / 2, (float)windowHeight / 2));

	matrixID = glGetUniformLocation(programID, "mvpMatrix");
	lightID = glGetUniformLocation(programID, "lightPositionWorldSpace");
	textureID  = glGetUniformLocation(programID, "myTextureSampler");
	viewMatrixID = glGetUniformLocation(programID, "viewMatrix");
	modelMatrixID = glGetUniformLocation(programID, "modelMatrix");
	brightID = glGetUniformLocation(programID, "bright");
	alphaID = glGetUniformLocation(programID, "alpha");
	myFilterID = glGetUniformLocation(programID, "myFilter");
	myColorID = glGetUniformLocation(programID, "myColor");
	projectionMatrix = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(
		glm::vec3(0, 0, cameraHeight),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)  
		);
	lightPosition = vec3(0, 0, 12);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameWindow::pointsTransform(double &x, double &y) {
    if (x > windowWidth / 2) {
        x = cameraX + ((x - windowWidth / 2) / (windowWidth / 2)) * pointTransformX;
    } else {
        x = cameraX - ((windowWidth / 2 - x) / (windowWidth / 2)) * pointTransformX;
    }
    if (y > windowHeight / 2) {
        y = cameraY - ((y - windowHeight / 2) / (windowHeight / 2)) * pointTransformY;
    } else {
        y = cameraY + ((windowHeight / 2 - y) / (windowHeight / 2)) * pointTransformY;
    }
}

bool GameWindow::mouseHandler(double &clickX, double &clickY, float speed) {
	double cursorX, cursorY;
	bool res = false;
	glfwGetCursorPos(window, &cursorX, &cursorY);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		mousePressed = false;
	}
	if (!mousePressed && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		mousePressed = true;
		clickX = cursorX;
		clickY = cursorY;
		pointsTransform(clickX, clickY);
		res = true;
	}		
	float h = 0, v = 0;		
	if (cursorX > windowWidth - windowBoard) {
		h = -1;
	}
	if (cursorX < windowBoard) {
		h = 1;			
	}
	if (cursorY > windowHeight - windowBoard) {
		v = 1;
	}
	if (cursorY < windowBoard) {
		v = -1;
	}
	if (h != 0 && v != 0) {
		h /= std::sqrt(2);
		v /= std::sqrt(2);
	}
	if (cursorX > windowWidth) {
		glfwSetCursorPos(window, windowWidth, cursorY);
	}
	if (cursorX < 0) {
		glfwSetCursorPos(window, 0, cursorY);
	}
	if (cursorY > windowHeight) {
		glfwSetCursorPos(window, cursorX, windowHeight);
	}
	if (cursorY < 0) {
		glfwSetCursorPos(window, cursorX, 0);
	}
	cameraMove(h * speed, v * speed);
	return res;
}

void GameWindow::cameraMove(float pointsX, float pointsY) {
	cameraX -= pointsX; cameraY -= pointsY;
	viewMatrix = glm::lookAt(
		glm::vec3(cameraX, cameraY, cameraZ),
		glm::vec3(cameraX, cameraY, 0),
		glm::vec3(0, 1, 0)  
		);
	lightPosition.x = cameraX;
	lightPosition.y = cameraY;
}

void GameWindow::drawStart() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
}

void GameWindow::drawEnd() {
	keyListen();
    buttonListen();
	glfwSwapBuffers(window);
}

void GameWindow::drawText(std::string s, float x, float y, float size, vec3 color, vec4 backGroundColor) {
    textView->printText(s.c_str(), x * windowWidth, y * windowHeight, size, color, backGroundColor);
}


void GameWindow::drawModel(GLuint &vertexBuffer, GLuint &uvBuffer, GLuint &normalBuffer, GLuint &indexBuffer, GLuint &texture,
                           int indexCount, float x, float y, float rotation, float brightness, float alpha,
                           vec3 filter, vec3 color) {
	mat4 modelMatrix = mat4(1.0f) * translate(x, y, 0.0f) * rotate(-(rotation * 180 / 3.14f) , vec3(0, 0, 1));
	mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

	glUseProgram(programID);

	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvpMatrix[0][0]);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform3f(myFilterID, filter.x, filter.y, filter.z);
	glUniform3f(myColorID, color.x, color.y, color.z);
	glUniform3f(lightID, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform1i(textureID, 0);
	glUniform1f(brightID, brightness);
	glUniform1f(alphaID, alpha);


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(2, 3,	GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, (void*)0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void GameWindow::closeWindow() {
    glfwDestroyWindow(window);
	glDeleteProgram(programID);
	glDeleteTextures(1, &textureID);
	glfwTerminate();
	textView->clean();
    delete(textView);
}

GameWindow::KeyCallBack::KeyCallBack() {
   isClicked = false;
   isPressed = false;
   x = 0;
   y = 0;
}

void GameWindow::addKeyListener(int keyID) {
	keyMap.push_back(std::pair<int, KeyCallBack>(keyID, KeyCallBack()));
}

bool GameWindow::isKeyClicked(int keyID) {
	for (size_t i = 0; i < keyMap.size(); i++) {
		if (keyMap[i].first == keyID) {
			if (keyMap[i].second.isClicked) {
				keyMap[i].second.isClicked = false;
				return true;
			}
            return false;
		}
	}
    return false;
}

bool GameWindow::isKeyClicked(int keyID, double &x, double &y) {
	for (size_t i = 0; i < keyMap.size(); i++) {
		if (keyMap[i].first == keyID) {
			if (keyMap[i].second.isClicked) {
				keyMap[i].second.isClicked = false;
				x = keyMap[i].second.x;
				y = keyMap[i].second.y;
				return true;
			}
		}
	}
    return false;
}

void GameWindow::keyListen() {
    glfwPollEvents();
    for (size_t i = 0; i < keyMap.size(); i++) {
        if (glfwGetKey(window, keyMap[i].first) == GLFW_PRESS) {
            if (!keyMap[i].second.isPressed) {
                keyMap[i].second.isPressed = true;
                keyMap[i].second.isClicked = true;
				double x, y;
				glfwGetCursorPos(window, &x, &y);
				pointsTransform(x, y);
				keyMap[i].second.x = x;
				keyMap[i].second.y = y;
            }
		} else {
            keyMap[i].second.isPressed = false;
        }
    }
}

GameWindow::Button::Button(float x, float y, float screenSize, float sizeX, float sizeY,
                           std::string text, vec3 color, vec4 backGroundColor) {
    this->x = x;
    this->y = y;
    this->screenSize = screenSize;
    this->sizeY = sizeY;
    this->sizeX = sizeX;
    this->text = text;
    this->color = color;
    this->backGroundColor = backGroundColor;
    isPressed = false;
    isClicked = false;
    isActive = false;
}

void GameWindow::Button::clickCheck(double x, double y, bool onPressed) {
    if (!isActive) {
        if (x > this->x && x < this->x + (float) text.size() * sizeX / 2 && y > this->y && y < this->y + sizeY) {
            if (onPressed) {
                if (!isPressed) {
                    isPressed = true;
                    negativeColor();
                }
            } else {
                if (isPressed) {
                    isPressed = false;
                    isClicked = true;
                    negativeColor();
                }
            }
        } else {
            if (isPressed) {
                isPressed = false;
                isClicked = true;
                negativeColor();
            }
        }
    }
}

void GameWindow::Button::negativeColor() {
    backGroundColor.x = 1 - backGroundColor.x;
    backGroundColor.y = 1 - backGroundColor.y;
    backGroundColor.z = 1 - backGroundColor.z;
}

void GameWindow::Button::drawButton(GameWindow *gameWindow) {
    gameWindow->drawText(text, x, y, screenSize, color, backGroundColor);
}

void GameWindow::buttonListen() {
    glfwPollEvents();
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        for (size_t i = 0; i < buttonMap.size(); i++) {
            buttonMap[i].clickCheck(x / windowWidth, y / windowHeight, true);
        }
    } else {
        for (size_t i = 0; i < buttonMap.size(); i++) {
            buttonMap[i].clickCheck(x / windowWidth, y / windowHeight, false);
        }
    }
}

int GameWindow::addButton(float x, float y, float size, std::string text, vec3 color, vec4 backGroundColor) {
    buttonMap.push_back(Button(x, y, size, size / windowWidth, size / windowHeight, text, color, backGroundColor));
    return buttonMap.size() - 1;
}

void GameWindow::deleteUIElements() {
    buttonMap.clear();
    labelMap.clear();
}

void GameWindow::drawUIElements() {
    for (size_t i = 0; i < buttonMap.size(); i++) {
        buttonMap[i].drawButton(this);
    }
    for (size_t i = 0; i < labelMap.size(); i++) {
        labelMap[i].drawLabel(this);
    }
}

bool GameWindow::isButtonClicked(int buttonID) {
    if (buttonMap[buttonID].isClicked) {
        buttonMap[buttonID].isClicked = false;
        return true;
    }
    return false;
}

bool GameWindow::isWindowOpened() {
	return glfwWindowShouldClose(window) == 0;
}

void GameWindow::setCameraLocationTo(float x, float y) {
    cameraX = x;
    cameraY = y;
    viewMatrix = glm::lookAt(
            glm::vec3(cameraX, cameraY, cameraZ),
            glm::vec3(cameraX, cameraY, 0),
            glm::vec3(0, 1, 0)
    );
}

void GameWindow::setCursorTo(float x, float y) {
    glfwSetCursorPos(window, x * windowWidth, y * windowHeight);
}

bool GameWindow::changeButtonActive(int buttonID) {
    if (buttonID > buttonMap.size()) {
        return false;
    }
    if (buttonMap[buttonID].isActive) {
        buttonMap[buttonID].isActive = false;
    } else {
        buttonMap[buttonID].isActive = true;
    }
    buttonMap[buttonID].negativeColor();
    return true;
}

int GameWindow::getWidth() {
    return windowWidth;
}

int GameWindow::getHeight() {
    return windowHeight;
}

GameWindow::Label::Label(float x, float y, float screenSize, std::string text, vec3 color) {
    this->x = x;
    this->y = y;
    this->screenSize = screenSize;
    this->text = text;
    this->color = color;
}

void GameWindow::Label::drawLabel(GameWindow *gameWindow) {
    gameWindow->drawText(text, x, y, screenSize, color);
}

void GameWindow::addLabel(float x, float y, float size, std::string text, vec3 color) {
    labelMap.push_back(Label(x, y, size, text, color));
}
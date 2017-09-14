#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Loaders.h"
#include "TextView.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/transform.hpp>
#include "Model.h"
#include <map>

#if defined(_WIN32)
#define TIME_DIVISOR 1000
#else
#define TIME_DIVISOR 100000
#endif

using namespace glm;

class Model;
/**
 * Opengl 3d rendering class
 * Included simple UI like buttons and labels
 * Can handle user mouse and keyboard
 * Allows to draw text for all
 * Allows to draw 3d object only for Model.class
 */
class GameWindow {
public:
    /**
     * Construct and open window
     * \param width Window width
     * \param height Window height
     * \param cameraHeight Distance from the camera to OxOy plane
     * \param mouseBoard Camera sensitivity.
     * \param name Name of the window
     * \param fullScreen Window mode or Fullscreen
     */
    GameWindow(int width, int height, int cameraHeight, int mouseBoard, const char *name, bool fullScreen = false);
    /**
     * Added key listener
     * \param keyID Key iditentificator from GLFW_KEY_* context
     */
    void addKeyListener(int keyID);
    /**
     * \param keyID Key iditentificator from GLFW_KEY_* context
     * \return true if key clicked else false
     */
    bool isKeyClicked(int keyID);
    /**
     * \param keyID Key iditentificator from GLFW_KEY_* context
     * \param x Variable to write X coordinate of mouse when key was pressed
     * \param y Variable to write Y coordinate of mouse when key was pressed
     * \return true if key clicked else false
     */
    bool isKeyClicked(int keyID, double &x, double &y);
    /**
     * Starts draw with this func.
     */
    void drawStart();
    /**
     * Ends draw with this func.
     */
    void drawEnd();
    /**
     * \return Window width
     */
    int getWidth();
    /**
     * \return Window height
     */
    int getHeight();
    /**
     * Draws text.
     * Remember that first you need call drawStart(), and only when drawEnd() called, text were draws
     * \param s String to draw
     * \param x X coordinate of screen to draw (from 0 to 1)
     * \param y Y coordinate of screen to draw (from 0 to 1)
     * \param size Size of text height (in pixels)
     * \param color Color of text
     * \param backGroundColor Background color of text
     */
    void drawText(std::string s, float x, float y, float size, vec3 color, vec4 backGroundColor = vec4(0, 0, 0 ,0));
    /**
     * Close window.
     */
    void closeWindow();
    /**
     * \param clickX Variable to write X coordinate of mouse
     * \param clickY Variable to write Y coordinate of mouse
     * \param speed Value to move camera (in GL points)
     * \return true if right button is clicked
     */
    bool mouseHandler(double &clickX, double &clickY, float speed);
    /**
     * \param x X position on screen (from 0 to 1)
     * \param y Y position on screen (from 0 to 1)
     * \param size Size of button height (in pixels)
     * \param text String to draw
     * \param color Color of text
     * \param backGroundColor Background color of button
     */
    int addButton(float x, float y, float size, std::string text, vec3 color, vec4 backGroundColor);
    /**
     * \param x X position on screen (from 0 to 1)
     * \param y Y position on screen (from 0 to 1)
     * \param size Size of button height (in pixels)
     * \param text String to draw
     * \param color Color of text
     */
    void addLabel(float x, float y, float size, std::string text, vec3 color);
    /**
     * Changes button isActive.
     * If button marked Active you cant clicked them, it already cliked
     * \param buttonID ID of button
     * \return true if you give valid ID, else false
     */
    bool changeButtonActive(int buttonID);
    /**
     * Draws buttons and labels
     */
    void drawUIElements();
    /**
     * \param buttonID ID of button
     * \return true if button clicked, else false
     */
    bool isButtonClicked(int buttonID);
    /**
     * Deletes UI all elements, like buttons and labels
     */
    void deleteUIElements();
    /*
     * \return false if something request window to close, else true
     */
    bool isWindowOpened();
    /*
     * \param x X coordinate (in GL points)
     * \param y Y coordinate (in GL points)
     */
    void setCameraLocationTo(float x, float y);
    /**
     * relocate cursor to position
     * \param x X coordinate of screen (from 0 to 1)
     * \param y Y coordinate of screen (from 0 to 1)
     */
    void setCursorTo(float x, float y);
    /**
     * friend class
     * Allow this class to draw 3d models
     */
    friend class Model;
private:
    /**
     * Contains some key info
     */
    class KeyCallBack {
    public:
        bool isPressed, isClicked;
        float x, y;
        KeyCallBack();
    };
    /**
     * Contains button info
     */
    class Button {
    public:
        bool isPressed, isClicked, isActive;
        /**
         * \param x X coordinate of screen (from 0 to 1)
         * \param y Y coordinate of screen (from 0 to 1)
         * \param screenSize Size of text in pixels
         * \param sizeX Size of text width (from 0 to 1)
         * \param sizeY Size of text height (from 0 to 1)
         * \param text String to show
         * \param color Text color
         * \param backGroundColor Background color of text
         */
        Button(float x, float y, float screenSize, float sizeX, float sizeY, std::string text, vec3 color, vec4 backGroundColor);
        /**
         * Checks if button were clicked
         * \param x X coordinate of screen (from 0 to 1)
         * \param y Y coordinate of screen (from 0 to 1)
         * \param onPressed True if left mouse button is pressed
         */
        void clickCheck(double x, double y, bool onPressed);
        /**
         * Draws button
         * \param gameWindow window to draw
         */
        void drawButton(GameWindow *gameWindow);
        /**
         * Changes button backgroud color
         * Need to show that we press button
         */
        void negativeColor();
    private:
        float x, y, screenSize, sizeX, sizeY;
        std::string text;
        vec3 color;
        vec4 backGroundColor;
    };

    /**
     * Contain label info
     */
    class Label {
    public:
        /**
         * \param x X coordinate of screen (from 0 to 1)
         * \param y Y coordinate of screen (from 0 to 1)
         * \param screenSize Size of text in pixels
         * \param text String to show
         * \param color Text color
         */
        Label(float x, float y, float screenSize, std::string text, vec3 color);
        /**
         * Draws label
         * \param gameWindow Window to draw
         */
        void drawLabel(GameWindow *gameWindow);
    private:
        float x, y, screenSize;
        std::string text;
        vec3 color;
    };
    /**
     * Contains all button
     */
	std::vector<Button> buttonMap;
    /**
     * Contains all listened key
     * first arg is buttonID in GLFW_KEY_* context
     * second callback class for key
     */
    std::vector<std::pair<int, KeyCallBack>> keyMap;
    /**
     * Contains all labels
     */
    std::vector<Label> labelMap;
    /**
     * Text show program
     */
	TextView *textView;
    /**
     * GLFW window
     */
	GLFWwindow *window;
    /**
     * Matrix for draw 3d object
     */
	glm::mat4 projectionMatrix, viewMatrix, mvpMatrix;
    /**
     * Light position at scene
     */
	glm::vec3 lightPosition;
    /**
     * ID to Locations in our shader program
     */
	GLuint textureID, matrixID, programID, lightID, viewMatrixID,
            modelMatrixID, brightID, alphaID, myColorID, myFilterID;
    /**
     * Camera locations
     */
	float cameraX, cameraY, cameraZ;
    /**
     * Ratio of X pixels on screen to GL points
     */
	float pointTransformX, pointTransformY;
    /**
     * Window size
     */
	int windowHeight, windowWidth, windowBoard;
    /**
     * Is mouse pressed
     */
	bool mousePressed;

    /**
     * Moves camera
     * \param pointsX Points to move at X (in GL points)
     * \param pointsY Points to move at Y (in GL points)
     */
	void cameraMove(float pointsX, float pointsY);
    /**
     * Transforms pixels from screen into GL points
     * \param x X coordinate on screen
     * \param y Y copedinate on screen
     */
    void pointsTransform(double &x, double &y);
    /**
     * Draws 3d model
     * \param vertexBuffer ID to vertex buffer
     * \param uvBuffer Id to uv buffer
     * \param normalBuffer ID to normal buffer
     * \param indexBuffer ID to index buffer
     * \param texture ID to texture buffer
     * \param indexCount Size of IndexArray
     * \param x X coordinate (in GL points)
     * \param y Y coordinate (in GL points)
     * \param rotation Rotation of model (in radians)
     * \param brightness Brightness of model
     * \param alpha Transparency of object (0 - full transparent, 1 - non transparent)
     * \param filter Color filter
     * \param color Additional color to object texture
     */
    void drawModel(GLuint &vertexBuffer, GLuint &uvBuffer, GLuint &normalBuffer, GLuint &indexBuffer, GLuint &texture,
                   int indexCount, float x, float y, float rotation = 0, float brightness = 1, float alpha = 1,
                   vec3 filter = vec3(1, 1, 1), vec3 color = vec3(0, 0, 0));
    /**
     * Listens key pressed
     */
    void keyListen();
    /**
     * Listens button pressed
     */
    void buttonListen();
};
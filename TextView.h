#pragma once
#include <vector>
#include <cstring>

#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Loaders.h"
using namespace glm;


/**
 * Draws text in window
 */
class TextView {
	
public:
	/**
	 * Construcs program to draw text
	 * \param texturePath Path to texture with text (format square 16 symbols * 16 symbols in ASCII2)
	 * \param screenRes Screen resolution, need to correctly draw text
	 */
	TextView(const char *texturePath, vec2 screenRes);
    /**
     * Prints text on screen
     * \param text String to draw
     * \param x X coordinate (in pixels)
     * \param y Y coordinate (in pixels)
     * \param size Height of word (in pixels)
     * \param color Color of text
     * \param backGroundColor Background color of text
     */
	void printText(const char *text, float x, float y, float size, vec3 color, vec4 backGroundColor = vec4(0, 0, 0, 0));
    /**
     * Free all buffers
     */
	void clean();
private:
	GLuint textTextureID, textVertexID, textUVID, textProgramID, textUniformID, textColorID, screenResID, textBackGroundColorID;
	vec2 screen;
};
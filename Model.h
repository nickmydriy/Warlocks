#pragma once
#include "GameWindow.h"
#include "Loaders.h"
#include <vector>
#include <glm.hpp>
#include <glew.h>
#include <glfw3.h>

class GameWindow;
/**
 * Contains 3d object and info how to draw it
 */
class Model {
public:
	/**
	 * Constuct 3d object
	 * \param gameWindow Window to draw object
	 * \param objPath Path to object
	 * \param texturePath Path to texture
	 */
	Model(GameWindow *gameWindow, const char *objPath, const char *texturePath);
    /**
     * Draws object
     * \param x X coordinate (in GL points)
     * \param y Y coordinate (in GL points)
     * \param rotation Rotation of the object (in radians)
     * \param brightness Brightness of the object
     * \param alpha Transparency of the object (0 - full transparent, 1 - non transparent)
     * \param filter Color filter
     * \param color Additional color to texture
     */
    const void draw(float x, float y, float rotation = 0, float brightness = 1, float alpha = 1,
         vec3 filter = vec3(1, 1, 1), vec3 color = vec3(0, 0, 0));
    /**
     * Sets simple animation that relocate vertex that stay at (inRad, outRad) [polar coordinates] and under maxZ
     * Relocate vertex from original radius (OR) to (OR - subF, OR + addF)
     * \param inRad Inside radius of ring to mark vertex Animated
     * \param outRad Outside radius of ring to mark vertex Animated
     * \param maxZ high edge of Z coordinate to mark vertex Animated
     * \param subF Subtract animation
     * \param addF Additional animation
     * \param power Speed of animation (in GL points/sec)
     */
	void setSimpleRingAnimation(float inRad, float outRad, float maxZ, float subF, float addF, float power);
    /*
     * Updates animation
     * \param points if points != 0 then compress all vertices for points
     */
    void animationUpdate(float points = 0);
    /**
     * Need only for some UI objects, like hp bar
     * Sets all x coordinates of object vertices to points
     * \param points Points to set X
     */
    void setCompressX(float points);
    /**
     * Free all buffers
     */
	void clear();
    /**
     * Resets vertex to start position, when getVertexRadius() were first called
     */
	void resetVertexRadius();
    /**
     * \return min R (in polar system) coordinate of points
     */
    float getVertexRadius();

	~Model();

private:
    /**
     * Buffers ID
     */
    GLuint vertexBuffer, uvBuffer, normalBuffer, indexBuffer, texture;
    /**
     * Size of indices array
     */
    int indexCount;
    /**
     * Animation values
     */
    float subAnimation, addAnimation, time, power, vertexRadius;
    GameWindow *gameWindow;
    std::vector<glm::vec3> vertices;
    std::vector<std::pair<std::vector<int>, float>> animatedVertex;
    std::vector<bool> animationVec;

    /**
     * Updates buffer with new data
     * \param array Array to update
     * \param buffer Buffer to update
     * \return size of array binded
     */
    template <class T>
	size_t updateBuffer(std::vector<T> &array, GLuint &buffer) {
        glDeleteBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, array.size() * sizeof(T), &array[0], GL_STATIC_DRAW);
        return array.size();
    }
    /**
     * Compress all vertex R (in polar system) coordinate
     * \param points Points to compress (in GL points)
     */
	void vertexCompress(float points);
};
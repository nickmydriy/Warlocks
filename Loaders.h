#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <string.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844
/**
 * Loads program to draw, prints into consol if shader didnt compiled
 * \param vertexShaderPath Path to vertex shader
 * \param framentShaderPath Path to fragment shader
 * \return ID of compiled program
 */
GLuint loadProgram(const char *vertexShaderPath, const char *fragmentShaderPath, bool text);
/**
 * Loads texture from DDS image, creates mipmap
 * \param imagePath Path to image
 * \return ID of texture
 */
GLuint loadDDS(const char *imagePath);
/**
 * Loades 3d object from .obj file
 * File must includes verticies, uv, normals, and triangule faces, and nothing more, else object didnt loades
 * \param objPath Path to object
 * \param verticesArray Array to write verticies
 * \param uvsArray Array to write uv
 * \param normalsArray Array to write normals
 * \return false if object dont loaded
 */
bool loadOBJ(const char *objPath, std::vector<glm::vec3> &verticesArray,
             std::vector<glm::vec2> &uvsArray, std::vector<glm::vec3> &normalsArray);

/**
 * Class for indexing VBO (Vertex buffer object)
 */
class IndexVBO {
public:
	/**
	 * Indexing VBO
	 * \param vertices Input vertices array
	 * \param uvs Input UVs array
	 * \param normals Input normals array
	 * \param indices Output indices array
	 * \param indexedVertices Output vertices array
	 * \param indexedUVs Output UVs array
	 * \param indexedNormals Output normals array
	 */
	static void indexVBO (
		std::vector<glm::vec3> &vertices,
		std::vector<glm::vec2> &uvs,
		std::vector<glm::vec3> &normals,
		std::vector<unsigned short> &indices,
		std::vector<glm::vec3> &indexedVertices,
		std::vector<glm::vec2> &indexedUVs,
		std::vector<glm::vec3> &indexedNormals
		);

private:
	/**
	 * Contains vertex info
	 */
	struct PackedVertex {
    public:
        glm::vec3 position;
        glm::vec2 uv;
        glm::vec3 normal;
        bool operator < (const PackedVertex that) const;
        PackedVertex(glm::vec3 position, glm::vec2 uv, glm::vec3 normal);
	};
	/**
	 * \param packed Packed vertex
	 * \param vertexToOutIndex map to search
	 * \param result If we find similar vertex then we write this in this variable
	 * \return True if we found similar vertex
	 */
	static bool getSimilarVertexIndex(
			PackedVertex &packed,
			std::map<PackedVertex,unsigned short> &vertexToOutIndex,
			unsigned short &result
	);
};
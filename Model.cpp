#define GLEW_STATIC

#include <time.h>
#include "Model.h"

// TODO:
#define TIME_DIVISOR 100000

Model::Model(GameWindow *gameWindow,const char *objPath, const char *texturePath) {
    this->gameWindow = gameWindow;
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normal;
	loadOBJ(objPath, vertex, uv, normal);
    std::vector<glm::vec2> uvIndexed;
    std::vector<glm::vec3> normalIndexed;
    std::vector<unsigned short> indexV;
	IndexVBO::indexVBO(vertex, uv, normal, indexV, vertices, uvIndexed, normalIndexed);
	this->texture = loadDDS(texturePath);
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &normalBuffer);
    glGenBuffers(1, &uvBuffer);
    glGenBuffers(1, &indexBuffer);
	updateBuffer(vertices, vertexBuffer);
    updateBuffer(uvIndexed, uvBuffer);
    updateBuffer(normalIndexed, normalBuffer);
    this->indexCount = updateBuffer(indexV, indexBuffer);
    vertexRadius = -1;
}

void Model::clear() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &indexBuffer);
    glDeleteTextures(1, &texture);
}

float Model::getVertexRadius() {
    if (vertexRadius == -1) {
        vertexRadius = 1000;
        for (size_t i = 0; i < vertices.size(); i++) {
            float length = std::sqrt(vertices[i].x * vertices[i].x + vertices[i].y * vertices[i].y);
            if (length < vertexRadius) {
                vertexRadius = length;
            }
        }
    }
    return vertexRadius;
}


const void Model::draw(float x, float y, float rotation, float brightness, float alpha,
                vec3 filter, vec3 color) {
    gameWindow->drawModel(vertexBuffer, uvBuffer, normalBuffer, indexBuffer, texture, indexCount,
                          x, y, rotation, brightness, alpha, filter, color);
}

void Model::setSimpleRingAnimation(float inRad, float outRad, float maxZ, float subF, float addF, float power) {
    this->power = power;
    this->subAnimation = subF;
    this->addAnimation = addF;
    std::vector<std::pair<int, float>> animated;
    for (size_t i = 0; i < vertices.size(); i++) {
        if (vertices[i].z < maxZ) {
            float ox = vertices[i].x, oy = vertices[i].y;
            float length = std::sqrt(ox * ox + oy * oy);
            if (length > inRad && length < outRad) {
                animated.push_back(std::pair<int, float>(i, length));
            }
        }
    }

    while (animated.size() != 0) {
        animatedVertex.push_back(std::pair<std::vector<int>, float>());
        animatedVertex[animatedVertex.size() - 1].second = animated[0].second;
        animatedVertex[animatedVertex.size() - 1].first.push_back(animated[0].first);
        for (size_t i = 1; i < animated.size(); i++) {
            float cx = vertices[animated[i].first].x, cy = vertices[animated[i].first].y;
            if (vertices[animated[0].first] == vertices[animated[i].first]) {
                animatedVertex[animatedVertex.size() - 1].first.push_back(animated[i].first);
                animated.erase(animated.begin() + i);
                i--;
            }
        }
        animated.erase(animated.begin());
    }
    time = clock();
    animationVec.assign(animatedVertex.size(), false);
}

void Model::vertexCompress(float points) {
    for(size_t i = 0; i < vertices.size(); i++) {
        float radius = std::sqrt(vertices[i].x * vertices[i].x + vertices[i].y * vertices[i].y);
        if (radius < vertexRadius + 1) {
            float newRadius = radius - points;
            vertices[i].x = vertices[i].x / radius * newRadius;
            vertices[i].y = vertices[i].y / radius * newRadius;
        }
    }
    for(size_t i = 0; i < animatedVertex.size(); i++) {
        animatedVertex[i].second -= points;
    }
}

void Model::animationUpdate(float points) {
    float proctime = (clock() - time) / TIME_DIVISOR;
    time = clock();
    srand(time);
    for (size_t i = 0; i < animatedVertex.size(); i++) {
        float f = ((double)rand() / (double)RAND_MAX) * power;
        if (animationVec[i]) {
            f = -f;
        }
        float ox = vertices[animatedVertex[i].first[0]].x, oy = vertices[animatedVertex[i].first[0]].y;
        float abs = std::sqrt(ox * ox + oy * oy);
        float length = animatedVertex[i].second;
        if (abs > length + addAnimation) {
            animationVec[i] = true;
        }
        if (abs < length - subAnimation) {
            animationVec[i] = false;
        }
        float newLength = abs + f * proctime;
        for (size_t j = 0; j < animatedVertex[i].first.size(); j++) {
            vertices[animatedVertex[i].first[j]].x = ox / abs * newLength;
            vertices[animatedVertex[i].first[j]].y = oy / abs * newLength;
        }
    }

    if (points != 0) {
        vertexCompress(points);
    }
    updateBuffer(vertices, vertexBuffer);
}

void Model::setCompressX(float points) {
    if (points <= 0) {
        points = 0.03;
    }
    for (size_t i = 0; i < vertices.size(); i++) {
        if (vertices[i].x > 0) {
            vertices[i].x = points;
        } else {
            vertices[i].x = -points;
        }
    }
    updateBuffer(vertices, vertexBuffer);
}

void Model::resetVertexRadius() {
    for (size_t i = 0; i < vertices.size(); i++) {
        float radius = std::sqrt(vertices[i].x * vertices[i].x + vertices[i].y * vertices[i].y);
        if (radius < vertexRadius) {
            vertices[i].x = vertices[i].x / radius * vertexRadius;
            vertices[i].y = vertices[i].y / radius * vertexRadius;
        }
    }
    for(size_t i = 0; i < animatedVertex.size(); i++) {
        animatedVertex[i].second = vertexRadius;
    }
}

Model::~Model() {
    clear();
}


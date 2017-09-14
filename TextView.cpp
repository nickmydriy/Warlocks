#define GLEW_STATIC
#include "TextView.h"


TextView::TextView(const char *texturePath, vec2 screenRes){
	screen = screenRes;
	textTextureID = loadDDS(texturePath);

	glGenBuffers(1, &textVertexID);
	glGenBuffers(1, &textUVID);

	textProgramID = loadProgram("Shaders/TextVertex.vsh", "Shaders/TextFragment.fsh", true);
	textUniformID = glGetUniformLocation(textProgramID, "myTextSampler");
	textColorID = glGetUniformLocation(textProgramID, "textColor");
	screenResID = glGetUniformLocation(textProgramID, "screen");
	textBackGroundColorID = glGetUniformLocation(textProgramID, "backGroundColor");
}

void TextView::printText(const char *text, float x, float y, float size, vec3 color, vec4 backGroundColor) {
	unsigned int length = strlen(text);
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
    y = (float)screen.y * 2 - y;
	for (unsigned int i = 0 ; i < length; i++) {
		glm::vec2 vertexUpLeft = glm::vec2(x + i * size / 2, y);
		glm::vec2 vertexUpRight = glm::vec2(x + (i + 1) * size / 2, y);
		glm::vec2 vertexDownRight = glm::vec2(x + (i + 1) * size / 2, y - size);
		glm::vec2 vertexDownLeft = glm::vec2(x + i * size / 2, y - size);

		vertices.push_back(vertexUpLeft);
		vertices.push_back(vertexDownLeft);
		vertices.push_back(vertexUpRight);

		vertices.push_back(vertexDownRight);
		vertices.push_back(vertexUpRight);
		vertices.push_back(vertexDownLeft);

		char character = text[i];
		float uvX = (character % 16) / 16.0;
		float uvY = (character / 16) / 16.0;

		glm::vec2 uvUpLeft = glm::vec2(uvX + 1.0 / 64.0, uvY);
		glm::vec2 uvUpRight = glm::vec2(uvX + 1.0 / 16.0 - 1.0 / 64.0, uvY);
		glm::vec2 uvDownRight = glm::vec2(uvX + 1.0 / 16.0 - 1.0 / 64.0, uvY + 1.0 / 16.0);
		glm::vec2 uvDownLeft = glm::vec2(uvX + 1.0 / 64.0, uvY + 1.0 / 16.0);
		UVs.push_back(uvUpLeft);
		UVs.push_back(uvDownLeft);
		UVs.push_back(uvUpRight);

		UVs.push_back(uvDownRight);
		UVs.push_back(uvUpRight);
		UVs.push_back(uvDownLeft);
	}
	glDeleteBuffers(1, &textVertexID);
	glBindBuffer(GL_ARRAY_BUFFER, textVertexID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

	glDeleteBuffers(1, &textUVID);
	glBindBuffer(GL_ARRAY_BUFFER, textUVID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	glUseProgram(textProgramID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textTextureID);
	glUniform1i(textUniformID, 0);
	glUniform3f(textColorID, color.x, color.y, color.z);
	glUniform2f(screenResID, screen.x, screen.y);
    glUniform4f(textBackGroundColorID, backGroundColor.x, backGroundColor.y, backGroundColor.z, backGroundColor.a);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, textVertexID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, textUVID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void TextView::clean() {
	glDeleteBuffers(1, &textVertexID);
	glDeleteBuffers(1, &textUVID);
	glDeleteTextures(1, &textTextureID);
	glDeleteProgram(textProgramID);
}

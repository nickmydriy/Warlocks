#define GLEW_STATIC
#include "Loaders.h"


GLuint loadProgram(const char *vertexShaderPath, const char *fragmentShaderPath) {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertexShaderPath, std::ios::in);
	if(vertexShaderStream.is_open()) {
		std::string Line = "";
		while(getline(vertexShaderStream, Line))
			vertexShaderCode += "\n" + Line;
		vertexShaderStream.close();
	}

	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragmentShaderPath, std::ios::in);
	if(fragmentShaderStream.is_open()) {
		std::string Line = "";
		while(getline(fragmentShaderStream, Line))
			fragmentShaderCode += "\n" + Line;
		fragmentShaderStream.close();
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	char const *vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0){
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}



	char const *fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

GLuint loadDDS(const char *imagePath){
	unsigned char header[124];
	FILE *ddsPic;
	ddsPic = fopen(imagePath, "rb");

	char filecode[4];
	fread(filecode, 1, 4, ddsPic);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(ddsPic);
		return 0;
	}

	fread(&header, 124, 1, ddsPic);
	unsigned int height = *(unsigned int*) &(header[8]);
	unsigned int width = *(unsigned int*) &(header[12]);
	unsigned int linearSize	= *(unsigned int*) &(header[16]);
	unsigned int mipMapCount = *(unsigned int*) &(header[24]);
	unsigned int fourCC = *(unsigned int*) &(header[80]);


	unsigned char *buffer;
	unsigned int bufferSize;
	bufferSize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufferSize * sizeof(unsigned char));
	fread(buffer, 1, bufferSize, ddsPic);
	fclose(ddsPic);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch(fourCC) {
        case FOURCC_DXT1: {
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        } break;
        case FOURCC_DXT3: {
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        } break;
        case FOURCC_DXT5: {
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        } break;
        default: {
            free(buffer);
            return 0;
        }
    }

	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);
		offset += size;
		width /= 2;
		height /= 2;
		if (width < 1) {
            width = 1;
        }
		if (height < 1) {
            height = 1;
        }
	}
	free(buffer);
	return textureID;
}

bool loadOBJ(const char *objPath, std::vector<glm::vec3> &verticesArray, std::vector<glm::vec2> &uvsArray,
             std::vector<glm::vec3> &normalsArray) {
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;
	FILE *file = fopen(objPath, "r");
	while(1){
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {
            break;
        }
		if (strcmp(lineHeader, "v") == 0 ) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			tempVertices.push_back(vertex);
		} else if ( strcmp( lineHeader, "vt" ) == 0 ) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y;
			tempUVs.push_back(uv);
		} else if (strcmp(lineHeader, "vn") == 0 ) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			tempNormals.push_back(normal);
		} else if (strcmp( lineHeader, "f") == 0 ) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}


	for(unsigned int i = 0; i < vertexIndices.size(); i++){
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 vertex = tempVertices[vertexIndex - 1 ];
		glm::vec2 uv = tempUVs[uvIndex - 1 ];
		glm::vec3 normal = tempNormals[normalIndex - 1 ];
		verticesArray.push_back(vertex);
		uvsArray.push_back(uv);
		normalsArray.push_back(normal);
	}
	return true;
}

bool IndexVBO::PackedVertex::operator < (const PackedVertex that) const {
	return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
};

bool IndexVBO::getSimilarVertexIndex( 
	PackedVertex &packed,
	std::map<PackedVertex, unsigned short> &vertexToOutIndex,
	unsigned short &result) {
		std::map<PackedVertex,unsigned short>::iterator it = vertexToOutIndex.find(packed);
		if (it == vertexToOutIndex.end()){
			return false;
		} else {
			result = it->second;
			return true;
		}
}

IndexVBO::PackedVertex::PackedVertex(glm::vec3 position, glm::vec2 uv, glm::vec3 normal) {
	this->position = position;
	this->uv = uv;
	this->normal = normal;
}

void IndexVBO::indexVBO (
	std::vector<glm::vec3> &vertices,
	std::vector<glm::vec2> &uvs,
	std::vector<glm::vec3> &normals,
	std::vector<unsigned short> &indices,
	std::vector<glm::vec3> &indexedVertices,
	std::vector<glm::vec2> &indexedUVs,
	std::vector<glm::vec3> &indexedNormals) {
    std::map<PackedVertex, unsigned short> VertexToOutIndex;

    for (unsigned int i = 0; i < vertices.size(); i++) {
        PackedVertex packed(vertices[i], uvs[i], normals[i]);
        unsigned short index;
        bool found = getSimilarVertexIndex(packed, VertexToOutIndex, index);

        if (found) {
            indices.push_back(index);
        } else {
            indexedVertices.push_back(vertices[i]);
            indexedUVs.push_back(uvs[i]);
            indexedNormals.push_back(normals[i]);
            unsigned short newIndex = (unsigned short) indexedVertices.size() - 1;
            indices.push_back(newIndex);
            VertexToOutIndex[packed] = newIndex;
        }
    }
}
#include<vector>
#include<glm/glm.hpp>

class ImportedModel
{
private:
	int numVertices;
	std::vector<glm::vec3>vertices;
	std::vector<glm::vec2>texCoords;
	std::vector<glm::vec3>normalVecs;

public:
	ImportedModel(const char* filePath);
	int getNumVertices();
	std::vector<glm::vec3>getVertices();
	std::vector<glm::vec3>getTextureCoords();
	std::vector<glm::vec3>getNormals();
};

class ModelImporter
{
private:
	//从OBJ文件读取的数值
	std::vector<float>vertVals;
	std::vector<float>stVals;
	std::vector<float>normVals;

public:
	//保存为顶点属性，以供后续使用的数值
	ModelImporter();
	void parseOBJ(const char* filePath);
	int getNumVertices();
	std::vector<float>getVertices();
	std::vector<float>getTextureCoordinates();
	std::vector<float>getNormals();
};

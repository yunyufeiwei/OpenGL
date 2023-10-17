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
	ImportedModel();
	ImportedModel(const char* filePath);
	int getNumVertices();
	std::vector<glm::vec3>getVertices();
	std::vector<glm::vec2>getTextureCoords();
	std::vector<glm::vec3>getNormals();
};

class ModelImporter
{
private:
	//��OBJ�ļ���ȡ����ֵ
	std::vector<float>vertVals;
	std::vector<float>stVals;
	std::vector<float>normVals;

	//����Ϊ���������Թ�����ʹ�õ���ֵ
	std::vector<float> triangleVerts;
	std::vector<float> textureCoords;
	std::vector<float> normals;

public:
	ModelImporter();
	void parseOBJ(const char* filePath);
	int getNumVertices();
	std::vector<float>getVertices();
	std::vector<float>getTextureCoordinates();
	std::vector<float>getNormals();
};

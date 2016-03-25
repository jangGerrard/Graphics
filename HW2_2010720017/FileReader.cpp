#include "FileReader.h"

FileReader::FileReader(std::string path)
{
	this->path = path;
}
void FileReader::readText(std::vector<std::vector<float>>& vertexes, 
											std::vector<std::vector<int>>& triangles) {
	char temp[1000];
	std::string strTemp;
	int numOfVertex;
	int numOfTriangle;
	this->inputstream.open(this->path);

	inputstream >> numOfVertex; //numofVertex
	inputstream >> numOfTriangle; //numofTriangle

	for (int i = 0;i < numOfVertex;i++)
	{
		std::vector<float> vertex;
		for (int j = 0; j < 3;j++)
		{
			inputstream >> temp;
			std::string fs(temp);
			float f = std::stof(fs);  //string으로 읽은 값을 float로 변환해주는 stof()
			vertex.push_back(f);
		}
		vertexes.push_back(vertex);
	}

	for (int i = 0;i < numOfTriangle;i++)
	{
		std::vector<int> vertex;
		for (int j = 0; j < 3;j++)
		{
			inputstream >> temp;
			std::string is(temp);
			int point = std::stoi(is) - 1;
			vertex.push_back(point);
		}
		triangles.push_back(vertex);
	}
}
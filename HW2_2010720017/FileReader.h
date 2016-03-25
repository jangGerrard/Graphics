#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class FileReader {
public:
	FileReader(std::string path);
	void readText(std::vector<std::vector<float>>& vertexes, 
		std::vector<std::vector<int>>& triangles);
private:
	std::ifstream inputstream;
	std::string path;
};
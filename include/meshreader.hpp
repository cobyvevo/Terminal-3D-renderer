#pragma once
#include "vectorpoint.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

namespace meshreader 
{
	//void test() {
	//	std::cout << "test successful" << std::endl;
	//}
	struct mesh {
		int sz = 0;
		int st = 0;

		std::vector<vec3> verts;
		std::vector<vec3> normals;
		std::vector<std::vector<int>> tris;

	};

	void meshFromFile(const char* path,meshreader::mesh* currentmesh);
}
#include "meshreader.hpp"

using namespace meshreader;

void meshreader::meshFromFile(const char* path,meshreader::mesh* currentmesh) {
	std::cout << "getting " << path << " ..." << std::endl;
	
	std::ifstream meshfile;
	meshfile.open(path);

	if (meshfile.fail()) {
		std::cout << "failed" << std::endl;
	} else {
		std::cout << "success!" << std::endl;	
	}

	std::string line;

	std::vector<vec3> cverts;
	std::vector<vec3> cnormals;

	int i = 0;

	//v = vertex -
	//vt = texture
	//vn = normal -
	//f = polygon

	float cache[3] = {0,0,0};
	int intcache[3] = {0,0,0};

	int mode = 0; //0 none 1 normals 2 vertex

	int item = 0;
	int vertitem = 0;
	bool filestarted = false;

	while (!meshfile.eof() && i < 20) {
		meshfile >> line;
		std::cout << line << std::endl;
		//i++;

		int currentmode = 0;
		if (line=="v") {mode = 1; currentmode = 1;};
		if (line=="vn") {mode = 2; currentmode = 2;};
		if (line=="vt") {mode = 3; currentmode = 3;};
		if (line=="f") {mode = 4; currentmode = 4;};

		if (filestarted == false && mode == 1) filestarted = true;

		if (filestarted && currentmode != 3) {
			
			if (currentmode == 0 && item <= 2) {
				float value = std::atof(line.c_str());
				cache[item] = value;
				item++;
			}

			if (item > 2 || mode == 4) {
				if (mode == 1) {
					vec3 v(cache[0],cache[1],cache[2]);
					cverts.push_back(v);
					item = 0;
					
					
					std::cout << v.x << v.y << v.z << std::endl;

				} else if (mode == 2) {
					vec3 v(cache[0],cache[1],cache[2]);
					cnormals.push_back(v);
					item = 0;
				
				} else if (mode == 4) {
					//construct the mesh

					std::vector<std::string> split;
					std::string v = "";
					for (int i = 0; i < (int)line.size(); i++) {
						if (line[i] == '/') {
							split.push_back(v);
							v = "";
						} else {
							v += line[i];
						}					
					}
					split.push_back(v);

					std::cout << "sz" << split.size() << std::endl;
					
					if (split.size() == 3 && vertitem != 3) {
						int vertindex = std::atof(split[0].c_str());
						int normalindex = std::atof(split[2].c_str());

						std::cout << vertindex << std::endl;
						
						vec3 result1 = cverts[vertindex-1];
						currentmesh->verts.push_back(result1);
						

						std::cout << "verts push = " << currentmesh->sz << std::endl;
						vec3 result2 = cnormals[normalindex];
						currentmesh->normals.push_back(result2);


						if (currentmode == 0) {
							std::cout << "vertitem " << vertindex << " = " << currentmesh->sz << std::endl;
							intcache[vertitem] = currentmesh->sz;
							
							vertitem++;
						} else {
							vertitem = 0;
							std::cout << "vertitem end" << std::endl;
						}

						currentmesh->sz++;

					} else {
						if (vertitem != 0) {
							vertitem = 0;
							std::vector v{intcache[0],intcache[1],intcache[2]};
							//vec3 v(intcache[0],intcache[1],intcache[2]);
							currentmesh->tris.push_back(v);
							currentmesh->st++;
							std::cout << "or here" << std::endl;
						}
					}

				}
				
			}



		}

	}

	std::cout << "end of file" << std::endl;

	//currentmesh->verts = verts;
	//currentmesh->normals = normals;
}





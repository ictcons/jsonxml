/*
 * DataObject.cpp
 *
 *  Created on: 23/03/2024
 *      Author: gds
 */

#include "DataObject.h"
#include <fstream>
#include <sstream>


bool DataObject::fileExists(const std::string& _file) {
	std::stringstream err;
	std::ifstream fin(_file);
	if (! fin.is_open()) {
		err << "Failed to open " << _file;
		throw std::runtime_error(err.str());
	}
	fin.close();
	return true;
}


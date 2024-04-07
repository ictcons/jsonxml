/*
 * DataObject.cpp
 *
 *  Created on: 23/03/2024
 *      Author: gds
 */

#include "DataObject.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

bool isNumeric(const std::string& s) {
    std::stringstream ss(s);
    int val;
    ss >> val;
    return ! ss.fail() && ss.eof();
}

bool fileExists(const std::string& f) {
	std::stringstream err;
	std::ifstream fin(f);
	if (! fin.is_open()) {
		err << "Failed to open " << f;
		throw std::runtime_error(err.str());
	}
	fin.close();
	return true;
}

const std::string& removeChars(std::string& s, const std::string& chars) {
    s.erase(remove_if(s.begin(), s.end(),
    	[&chars](const char& c) {
        	return chars.find(c) != std::string::npos;
    	}), s.end());
    return s;
}


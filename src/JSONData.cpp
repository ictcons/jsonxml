/*
 * JSONObject.cpp
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#include "JSONData.h"
#include "XMLData.h"

#include <iostream>
#include <fstream>

#define DISPLAY_LIST

void JSONData::load(const std::string& _jsfile)  {
   // Load JSON data from a file
	std::cout << "Loading " << _jsfile << "...\n";
	m_file = _jsfile;

	// Open data file
	std::stringstream err;
    std::ifstream ifs(m_file);
    if (!ifs.is_open()) {
		err << "Failed to open " << m_file;
		throw std::runtime_error(err.str());
    }

    // Read the file into a string
    std::string tmp((std::istreambuf_iterator<char>(ifs)),
    				 std::istreambuf_iterator<char>());
    ifs.close();

    // Filter out asterisks which are prefixed to data
    // key by xidel in conversion of XML to JSON.
    std::stringstream ss;
    for (char ch : tmp) {
        if (ch != '@') {
            ss << ch;
        }
    }

    // Parse the string into a JSON object
    m_dataObj = json::parse(ss.str());

    std::cout << m_dataObj << std::endl;

    // Get the root name
    std::cout << "Root-name: " << this->rootName() << std::endl;

    return;
}

// Update XML data
void JSONData::save() {
	std::stringstream err;

    // Open a file for writing
    std::ofstream file(m_file);
    if (!file.is_open()) {
    	err << "Failed to open " << m_file << std::endl;
		throw std::runtime_error(err.str());
    }

    std::string jstr = m_dataObj.dump(2); // 2 spaces indentation
	file << jstr;
	file.close();

	std::cout << m_file << " saved\n";

    // Note: this app can be used update a local cached XML data file.
	//  : if a problem is detected with the updated XML file, the
    // original source XML file can be downloaded again from the device.
    return;
}

const std::string& JSONData::rootName() {
	// Top-level key is the root name
	if (m_root.empty()) {
		for (auto it = m_dataObj.begin(); it != m_dataObj.end(); ++it) {
			m_root = it.key();
			break;
		}
	}
	return m_root;
}

#if defined(DISPLAY_LIST)
void JSONData::list() {
	// Iterate over the JSON object
	for (auto it = m_dataObj.begin(); it != m_dataObj.end(); ++it) {
		std::cout << it.key() << " : " << it.value() << std::endl;
	}
}
#endif

int JSONData::merge(XMLData* _xmld) {
	assert(_xmld != nullptr);
	int numUpdates = 0;

	DataObject::DataElement brand {"PumpBrand", "Name", "ABCDE"};
	DataObject::DataElement errors {"Statistics", "Errors", 55};
	if (_xmld->update(brand)) numUpdates++;
	if (_xmld->update(errors)) numUpdates++;


	std::cout << "Total updates: " << numUpdates << "\n";
	return numUpdates;
}


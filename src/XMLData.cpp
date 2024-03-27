/*
 * XMLData.cpp
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#include "XMLData.h"

#include <iostream>
#include <fstream>

#define DISPLAY_LIST

void XMLData::load(const std::string& _xmlfile) {
	std::cout << "Loading " << _xmlfile << "...\n";
	m_file = _xmlfile;

	// Open data file
	std::stringstream err;
    std::ifstream ifs(m_file);
    if (!ifs.is_open()) {
		err << "Failed to open " << m_file;
		throw std::runtime_error(err.str());
    }

    // Parse XML file using pugixml
    if (!m_dataObj.load_file(m_file.c_str())) {
		err << "Failed to load " << m_file;
	    throw std::runtime_error(err.str());
	}

#if defined(DISPLAY_LIST)
    this->list();
#endif

    // Get the root name
    std::cout << "Root-name: " << this->rootName() << std::endl;
    return;
}

// Update XML data
void XMLData::save() {
	std::stringstream err;
    // Serialize updated XML data back to file
    if (!m_dataObj.save_file(m_file.c_str())) {
    	err << "Failed to save " << m_file << std::endl;
	    throw std::runtime_error(err.str());
    }

	std::cout << m_file << " saved\n";

    // Note: if a problem is detected with the updated XML file, the
    // original source XML file can be downloaded again from the device.
    return;
}

const std::string& XMLData::rootName() {
	if (m_root.empty()) {
		m_rootNode = m_dataObj.first_child();
		m_root = m_rootNode.value();
	}
	return m_root;
}

void XMLData::list() {
	// Iterate over the JSON object
//	for (auto it = m_dataObj.begin(); it != m_dataObj.end(); ++it) {
//		std::cout << it.key() << " : " << it.value() << std::endl;
//	}
	std::cout << "Not implemented\n";
}

bool XMLData::update(const DataElement& _data) {
    std::cout << "Find element " << _data.element << std::endl;
    pugi::xml_node root = m_dataObj.document_element();
    // Find the requested element
    pugi::xml_node node = root.child(_data.element.c_str());
    if (node.empty()) {
		std::cout << "element '" << _data.element << "' not found\n";
		return false;
    }
	pugi::xml_attribute attr = node.attribute(_data.attribute.c_str());
	if (!attr) {
		std::cout << "attribute '" << _data.attribute << "' not found\n";
		return false;
	}

	// If value is same as value expected, ignore attribute update
	if (std::string(attr.value()) == _data.value) {
		// If attribute is found, print the node name and attribute value
		std::cout << _data.attribute << "=\"" << attr.value() << "\" unchanged\n";
		return false;
	}

	// else update value
	// If attribute is found, print the node name and attribute value
	std::string oldValue(attr.value());
	attr.set_value(_data.value.c_str());
	std::cout << _data.attribute
		<< "=\"" << oldValue << "\" -> \"" << _data.value << "\"\n";
	return true;
}



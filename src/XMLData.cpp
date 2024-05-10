/*
 * jsonxml: XMLData.cpp
 *
 *  Created on: 10/05/2024
 *      Author: gds, Maran Consulting
 *      Version 1.0
 */

#include "XMLData.h"
#include "UpdateData.h"
#include "pugixml.hpp"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <cstdlib>
#include <algorithm>
#include <cctype>


void XMLData::load(const std::string& _xmlfile) {
	VERBOSE(std::cout << "Loading " << _xmlfile << "...\n";)
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
    // Report the root name
    VERBOSE(std::cout << "Root-name: " << this->rootName() << std::endl;)
    return;
}

void XMLData::backup() {
	std::string backupFile = m_file + ".bak";
    if (!m_dataObj.save_file(backupFile.c_str())) {
		std::stringstream err;
		err << "Failed to backup " << m_file << std::endl;
	    throw std::runtime_error(err.str());
    }
    VERBOSE(std::cout << m_file << " backed up\n";)
	return;
}

// Update XML data
void XMLData::save() {
    // Serialize updated XML data back to file
    if (!m_dataObj.save_file(m_file.c_str())) {
		std::stringstream err;
		err << "Failed to save " << m_file << std::endl;
	    throw std::runtime_error(err.str());
    }
    VERBOSE(std::cout << m_file << " saved\n";)
    // Note: if a problem is detected with the updated XML file, the
    // original source XML file can be downloaded again from the device.
    return;
}

const std::string& XMLData::rootName() {
	if (m_root.empty()) {
		m_rootNode = m_dataObj.document_element();
		if (m_rootNode)
			m_root = m_rootNode.name();
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

// Perform one or more updates
int XMLData::update(DataElements* _dataElements) {
	assert(_dataElements != nullptr);
    assert(_dataElements->root == rootName());

    VERBOSE(std::cout << "Updating xml: " << rootName() << std::endl;)

    int nUpdated = 0;
	m_results.clear();

    for (auto attrib : _dataElements->attributes) {
		std::string path = attrib.first;
		std::string newValue = attrib.second;

		// Add XML tag to most subordinate child
		std::size_t pos = path.rfind("/");
		if (pos != std::string::npos) {
			path.replace(pos, 1, "/@");
		}
		pugi::xpath_node_set nodes = m_dataObj.select_nodes(path.c_str());
	    if (nodes.empty()) {
	        std::cerr << "No matching node for " << path << std::endl;
	        continue;
	    }

	    // Iterate over the nodes in the document
	    for (const auto& node : nodes) {
			std::stringstream ss;
			// Update the attribute's value
			std::string currValue = node.attribute().value();
			if (currValue != newValue) {
				if (isNumeric(newValue)) {
					uint64_t ival = std::stoll(removeChars(newValue, "\""));
					node.attribute().set_value(ival);
				}
				else
					node.attribute().set_value(newValue.c_str());
				ss << "xml:" << path << "=" << newValue << " updated\n";
				nUpdated++;
			}
			else {
				ss << "xml:" << path << "=" << newValue << " unchanged\n";
			}
			VERBOSE(std::cout << ss.str();)
			m_results += ss.str();
		}
    }

	// Save the updates
	if (nUpdated) {
		backup();
		save();
	}

	return nUpdated;
}



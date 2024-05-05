/*
 * XMLData.cpp
 *
 *  Created on: 27/03/2024
 *      Author: gds
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

    // Get the root name
    VERBOSE(std::cout << "Root-name: " << this->rootName() << std::endl;)
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
	std::cout << m_file << " saved\n";
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

int XMLData::update(DataElements* _dataElements) {
	assert(_dataElements != nullptr);
    assert(_dataElements->root == rootName());
    VERBOSE(std::cout << "Updating xml " << rootName() << std::endl;)

    // Perform one or more updates
    int nUpdates = 0;
    for (auto attrib : _dataElements->attributes) {
		std::string path = attrib.first;
		std::string newValue = attrib.second;

		// Add XML tag to most subordinate child
		std::size_t pos = path.rfind("/");
		if (pos != std::string::npos) {
			path.replace(pos, 1, "/@");
		}

		VERBOSE(std::cout << "checking: " << path << " " << newValue << std::endl;)

		pugi::xpath_node_set nodes = m_dataObj.select_nodes(path.c_str());
	    if (nodes.empty()) {
	        std::cerr << "No matching node for " << path << std::endl;
	        continue;
	    }

	    // Iterate over the nodes in the document
	    for (const auto& node : nodes) {
	        // Access the node element
	    	VERBOSE(std::cout << "Node: " << node.attribute().name() << std::endl;)
			VERBOSE(std::cout << "Value: " << node.attribute().value() << std::endl;)

			// Update the attribute's value
			std::string currValue = node.attribute().value();
			if (currValue != newValue) {
				if (isNumeric(newValue)) {
					uint64_t ival = std::stoll(removeChars(newValue, "\""));
					node.attribute().set_value(ival);
				}
				else
					node.attribute().set_value(newValue.c_str());
				std::cout << "xml:" << path << "=" << newValue << " updated\n";
				nUpdates++;
			}
			else {
				std::cout << "xml:" << path << "=" << newValue << " unchanged\n";
			}
		}
    }
	// Save the updates
	if (nUpdates > 0) {
		if (! m_dataObj.save_file(m_file.c_str())) {
			std::stringstream ss;
			ss << "Failed to save " << m_file << std::endl;
			throw std::runtime_error(ss.str());
		}
	}

	return nUpdates;
}



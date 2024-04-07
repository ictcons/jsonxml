/*
 * JSONObject.cpp
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#include "JSONData.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

#define DEBUG

void JSONData::load(const std::string& _jsfile)  {
   // Load JSON data from a file
	std::cout << "Loading " << _jsfile << "...\n";
	m_file = _jsfile;
	std::stringstream ss;

	// Open data file
	std::stringstream err;
	m_jfs.open(m_file);
    if (!m_jfs.is_open()) {
		err << "Failed to open " << m_file;
		throw std::runtime_error(err.str());
    }

    // Read the file into a temp string, filter out asterisks
    // which may be prefixed to data keys (by xidel), then
    // stream the data into the JSON object.
    std::string tmp((std::istreambuf_iterator<char>(m_jfs)),
    				 std::istreambuf_iterator<char>());
    for (char ch : tmp) {
        if (ch != '@') {
            ss << ch;
        }
    }
    // Parse the string into a JSON object
    try {
    	m_dataObj = std::make_unique<json>();
    	ss >> *m_dataObj.get();
    	std::cout << "m_dataObj: " << *m_dataObj.get() << std::endl;

    } catch (json::parse_error& e) {
    	err << "JSON parse error: " << e.what() << std::endl;
		throw std::runtime_error(err.str());
    }
    return;
}

// Update XML data
void JSONData::save() {
    // Save the updated json data to the json file.
    //std::ofstream file(m_file);
    if (!m_jfs.is_open()) {
    	std::stringstream err;
    	err << "Failed to open " << m_file << std::endl;
		throw std::runtime_error(err.str());
    }

    m_jfs << std::setw(2) << m_dataObj << std::endl; // 2 spaces indentation

	std::cout << m_file << " updated\n";

    // Note: this app can be used to update a local cached XML data file.
	// if a problem is detected with the updated XML file, the original
    // source XML file can be downloaded again from the device.
    return;
}

const std::string& JSONData::rootName() {
	// Top-level key is the root name
	if (m_root.empty()) {
		for (auto it = m_dataObj->begin(); it != m_dataObj->end(); ++it) {
			m_root = it.key();
			break;
		}
	}
	return m_root;
}

void JSONData::list() {
	// Iterate over the JSON object
	for (auto it = m_dataObj->begin(); it != m_dataObj->end(); ++it) {
		std::cout << it.key() << " : " << it.value() << std::endl;
	}
}

bool JSONData::updateAttribute(const std::string& path,
							   const std::string& newValue) {
    // Get last segment in path
//	int nSegs = 0;
//    std::stringstream ss(path);
//    std::vector<std::string> segments;
//    std::string segment;
//    while(std::getline(ss, segment, '/')) {
//        if (!segment.empty()) {
//        	segments.push_back(segment);
//        	nSegs++;
//        }
//    }

    json* currentObject = m_dataObj.get();
    std::size_t pos = 0;
    while (pos != std::string::npos) {
        auto endPos = path.find('/', pos);
        if ((endPos - pos) <= 1) {
        	pos++;
        	continue;
        }
        std::string key = path.substr(pos, endPos - pos);
        if (!currentObject->is_object() || !currentObject->contains(key)) {
        	std::stringstream err;
        	err << "Invalid attribute path" << std::endl;
    		throw std::runtime_error(err.str());
        }
        currentObject = &((*currentObject)[key]);
        pos = (endPos == std::string::npos) ? endPos : endPos + 1;
    }

    // Access the root element
//    auto& root = m_dataObj[segments[0]];
//    for (size_t i = 1; i < segments.size(); ++i) {
//        root = root[segments[i]]; // Traverse through the nested objects
//	}
//    // Update the data if it is being changed
//	segment = segments[nSegs-1];
//	std::string currValue = m_dataObj[segments[nSegs-1]];
//
//	//std::cout << "m_segments.back(): " << m_segments.back() << std::endl;
//    if (newValue != currValue) {
//		root = newValue;
//		std::cout << "updated root: " << root << std::endl;
////	    std::cout << "m_dataObj:\n" << m_dataObj << std::endl;
//		this->save();
//		return true;
//    }

    // Update the numeric attribute
//     if (currentObject->is_number()) {
         auto oldValue = *currentObject;
         *currentObject = newValue;
         std::cout << "Updated attribute from " << oldValue << " to " << newValue << std::endl;
//     } else {
//         std::cerr << "Attribute is not a number." << std::endl;
//         return false;
//     }
     return true;
}


// Recurse-iterate through JSON data object to create
// the /root/a/b/c attribute path
void JSONData::iterRequest(const json& j, const std::string& prefix) {
	if (j.is_object()) {
		for (auto it = j.begin(); it != j.end(); ++it) {
			std::string newPrefix = prefix.empty() ?
				it.key() : prefix + "/" + it.key();
			iterRequest(it.value(), newPrefix);
		}
	} else if (j.is_array()) {
		for (size_t i = 0; i < j.size(); ++i) {
			std::string newPrefix = prefix.empty() ?
				std::to_string(i) : prefix + "/" + std::to_string(i);
			iterRequest(j[i], newPrefix);
		}
	} else {
		// "Return" a attribute path and value string
		m_iterRequestSS << "/" << prefix << " " << j << std::endl;
	}
}

// Recurse-iterate through JSON data object to create
// the /root/a/b/c attribute path
//void JSONData::iterUpdate(const json& j, const std::string& prefix,
//							DataElements* _dataElements) {
//	if (j.is_object()) {
//		for (auto it = j.begin(); it != j.end(); ++it) {
//			std::string newPrefix = prefix.empty() ?
//				it.key() : prefix + "/" + it.key();
//			std::string thisValue = it.value();
//			std::cout << "value: " << thisValue
//					<< ", prefix: " << newPrefix << std::endl;
//
//			if (_dataElements != nullptr) {
//				std::string newValue = _dataElements->getValue(newPrefix);
//				if (!newValue.empty()) {
//					if (newValue != thisValue) {
//						j[it.key()] = newValue;
//						nUpdates++;
//#ifdef DEBUG
//						std::cout << newPrefix << "=" << newValue << " updated\n";
//					} else {
//						std::cout << elemPath << ": " << newValue << "\" unchanged\n";
//#endif
//					}
//				}
//			}
//			iterUpdate(it.value(), newPrefix, _dataElements);
//		}
//	} else if (j.is_array()) {
//		for (size_t i = 0; i < j.size(); ++i) {
//			std::string newPrefix = prefix.empty() ?
//				std::to_string(i) : prefix + "/" + std::to_string(i);
//			iterUpdate(j[i], newPrefix);
//		}
//	} else {
//		m_ss << "/" << prefix << ": " << j << std::endl;
//	}
//}

// The JSON update request may contain more than one attribute.
// Argument #2 returns the update data content for XML updating.
int JSONData::update(const std::string& _jRequestUpdate,
					 DataElements* _dataElements) {
	assert(_dataElements != nullptr);
	assert(! _jRequestUpdate.empty());

	std::string requestUpdate = _jRequestUpdate;

	// Set root
	_dataElements->root = rootName();

	// If the JSON data update request is in a .json file,
	// insert the file's json data into the request string.
	if (requestUpdate.find(".") != std::string::npos) {
		std::ifstream ifs(requestUpdate);
		if (!ifs.is_open()) {
			std::stringstream ss;
			ss << "Failed to open " << requestUpdate;
			throw std::runtime_error(ss.str());
		}
		ifs >> requestUpdate;
		std::cout << "requestUpdate: " << requestUpdate << std::endl;
	}

	// Parse the json-formatted data update request.
	// Load the update request(s) in _dataElements.
	json j = json::parse(requestUpdate);
	iterRequest(j);
#ifdef DEBUG
	std::cout << m_iterRequestSS.str() << std::endl;
#endif
	// Prepare the DataElements object, to share the data update
	// request info with the XMLData object. The multiple update
	// requests are supported.
	_dataElements->attributes.clear();
    while (!m_iterRequestSS.eof()) {
    	std::string elemPath, value;
    	m_iterRequestSS >> elemPath >> value;
    	if (elemPath.empty() || value.empty()) {
    		continue;
    	}
#ifdef DEBUG
        std::cout << "Adding " << elemPath << " : " << value << std::endl;
#endif
    	_dataElements->add(elemPath, value);
	}

    // Perform the one or more updates
    int nUpdates = 0;
    for (auto attrib : _dataElements->attributes) {
		std::string elemPath = attrib.first;
		std::cout << "elemPath: " << elemPath << std::endl;
		std::string newValue = removeChars(attrib.second, "\"");
		// Have updateAttribute() do the complex JSON processing.
		if (updateAttribute(elemPath, newValue)) {
#ifdef DEBUG
			std::cout << elemPath << ": " << newValue << " updating\n";
#endif
			nUpdates++;
		}
		else {
			std::cout << elemPath << " unchanged\n";
		}
    }

    save();

    return nUpdates;
}


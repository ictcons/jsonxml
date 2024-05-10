/*
 * jsonxml: JSONData.cpp
 *
 *  Created on: 10/05/2024
 *      Author: gds, Maran Consulting
 *      Version 1.0
 */

#include "JSONData.h"
#include "UpdateData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

void JSONData::load(const std::string& _jsfile)  {
   // Load JSON data from a file
	VERBOSE(std::cout << "Loading " << _jsfile << "...\n";)
	m_jsonFile = _jsfile;

	// Open data file
	std::stringstream err;
	std::ifstream jfile(m_jsonFile);
    if (!jfile.is_open()) {
		err << "Failed to open " << m_jsonFile;
		throw std::runtime_error(err.str());
    }

    // Read the file into a temp string, filter out asterisks
    // which may be prefixed to data keys (by xidel), then
    // stream the data into the JSON object.
    std::string tmp((std::istreambuf_iterator<char>(jfile)),
    				 std::istreambuf_iterator<char>());
    std::stringstream ss;
    for (char ch : tmp) {
        if (ch != '@') {
            ss << ch;
        }
    }
    // Parse the string into a JSON object
    try {
    	ss >> m_dataObj;
    	VERBOSE(std::cout << "m_dataObj: " << m_dataObj << std::endl;)

    } catch (json::parse_error& e) {
    	err << "JSON parse error: " << e.what() << std::endl;
		throw std::runtime_error(err.str());
    }
    return;
}

// Update XML data
void JSONData::save() {
    // Save the updated json data to the json file.
    std::ofstream jfile(m_jsonFile);
    if (!jfile.is_open()) {
    	std::stringstream err;
    	err << "Failed to open " << m_jsonFile << std::endl;
		throw std::runtime_error(err.str());
    }

    std::ostringstream oss;
    oss << std::setw(2) << m_dataObj;
    VERBOSE(std::cout << oss.str() << std::endl;)

    jfile << oss.str() << std::endl;
    jfile.close();

    VERBOSE(std::cout << m_jsonFile << " saved\n";)

    // Note: this app can be used to update a local cached XML data file.
	// if a problem is detected with the updated XML file, the original
    // source XML file can be downloaded again from the device.
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

void JSONData::list() {
	// Iterate over the JSON object
	for (auto it = m_dataObj.begin(); it != m_dataObj.end(); ++it) {
		std::cout << it.key() << " : " << it.value() << std::endl;
	}
}

// Update the data in one attribute
bool JSONData::updateAttribute(const std::string& path,
							   const std::string& newValue) {
	json* currentObject = &m_dataObj;
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

    // Check if attribute needs to be updated

	if (currentObject->is_number()) {
		long long newInt = atoll(newValue.c_str()) ;
		long long oldValue = *currentObject;
		if (newInt == oldValue) {
			return false;
		}
		*currentObject = newValue;
	} else {
		std::string oldValue = *currentObject;
		if (newValue == oldValue) {
			return false;
		}
		*currentObject = newValue;
	}
	// Save the change
	save();

	return true;
}

// Recurse-iterate through JSON data object to create
// the /root/a/b/c attribute path
void JSONData::iterateRequest(const json& j, const std::string& prefix) {
	if (j.is_object()) {
		for (auto it = j.begin(); it != j.end(); ++it) {
			std::string newPrefix = prefix.empty() ?
				it.key() : prefix + "/" + it.key();
			iterateRequest(it.value(), newPrefix);
		}
	} else if (j.is_array()) {
		for (size_t i = 0; i < j.size(); ++i) {
			std::string newPrefix = prefix.empty() ?
				std::to_string(i) : prefix + "/" + std::to_string(i);
			iterateRequest(j[i], newPrefix);
		}
	} else {
		// "Return" a attribute path and value string
		m_iterRequestSS << "/" << prefix << " " << j << std::endl;
	}
}

// The JSON update request may contain more than one attribute.
// Argument #2 returns the update data content for XML updating.
int JSONData::update(const std::string& _jUpdates, DataElements* _dataElements) {
	assert(_dataElements != nullptr);
    assert(_dataElements->root == rootName());
	assert(! _jUpdates.empty());

    VERBOSE(std::cout << "Updating json: " << rootName() << std::endl;)

	std::string updates = _jUpdates;
	// If the JSON data update request is a .json file,
	// insert the file's json data into the request string.
	if ((updates.find(".json") != std::string::npos) ||
		(updates.find(".JSON") != std::string::npos)) {
		std::ifstream ifs(updates);
		if (!ifs.is_open()) {
			std::stringstream ss;
			ss << "Failed to open " << updates;
			throw std::runtime_error(ss.str());
		}
		ifs >> updates;
	}
	VERBOSE(std::cout << "Updates: " << updates << std::endl;)

	// Parse the json-formatted data update request.
	// The update request(s) are streamed into in m_iterRequestSS.
	json jRequest = json::parse(updates);
	iterateRequest(jRequest);

	// Prepare the DataElements object, to share the data update
	// request info with the XMLData object. The multiple update
	// requests are supported.
	_dataElements->attributes.clear();
    while (!m_iterRequestSS.eof()) {
    	std::string path, value;
    	m_iterRequestSS >> path >> value;
    	if (path.empty() || value.empty()) {
    		continue;
    	}
    	if (! _dataElements->add(path, value)) {
    		VERBOSE(std::cout << "Duplicate " << path << " not added\n";)
    	}
	}

    int nUpdated = 0;
	m_results.clear();

    // Perform each update
    for (auto attrib : _dataElements->attributes) {
		std::string path = attrib.first;
		std::string value = removeChars(attrib.second, "\"");
		// Have updateAttribute() do the complex JSON processing.
    	std::stringstream ss;
		if (updateAttribute(path, value)) {
			ss << "json:" << path << ":\"" << value << "\" updated\n";
			nUpdated++;
		}
		else {
			ss << "json:" << path << ":\"" << value << "\" unchanged\n";
		}
		VERBOSE(std::cout << ss.str();)
		m_results += ss.str();
    }

	// Save the updates
	if (nUpdated) {
		save();
	}

	return nUpdated;
}


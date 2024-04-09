/*
 * JSONData.h
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#ifndef JSONDATA_H_
#define JSONDATA_H_


#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "DataObject.h"
#include "jsonxml.h"

using json = nlohmann::json;

class XMLData;

class JSONData: public DataObject {
public:
	JSONData() = default;
	virtual ~JSONData() = default;

	void load(const std::string& _file) override;
	const std::string& rootName() override;
	void save() override;
#ifdef DEBUG
	void list() override;
#endif

	int update(const std::string& _jUpdateRequest,
				DataElements* _data);

private:
	void iterateRequest(const json& j,
						const std::string& prefix = "");
	bool updateAttribute(const std::string& path,
						 const std::string& newValue);

	json m_dataObj;
	std::stringstream m_iterRequestSS;
	std::string m_jsonFile;
    std::string m_root;
    int nUpdates = 0;
    std::vector<std::string> m_segments;

};

#endif /* JSONDATA_H_ */

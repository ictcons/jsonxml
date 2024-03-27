/*
 * JSONObject.h
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#ifndef JSONDATA_H_
#define JSONDATA_H_

#include <DataObject.h>

#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include "jsonxml.h"

using json = nlohmann::json;

class XMLData;

class JSONData: public DataObject {
public:
	JSONData() = default;
	virtual ~JSONData() = default;

	void load(const std::string& _file) override;
	const std::string& rootName() override;
	void list() override;
	//std::string keyValue(const std::string& _key) override;
	void save() override;

	bool update(const DataElement& data);

	int merge(XMLData* _xmld);

private:
	std::string m_file;
    std::string m_root;
	json m_dataObj;

};

#endif /* JSONDATA_H_ */

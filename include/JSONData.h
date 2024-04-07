/*
 * JSONObject.h
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#ifndef JSONDATA_H_
#define JSONDATA_H_


#include <iostream>
#include <memory>
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
	void list() override;
	void save() override;

	int update(const std::string& _jUpdateRequest,
				DataElements* _data);

	//	bool update(const DataElements& data);
	//int merge(XMLData* _xmld);

private:
	void iterRequest(const json& j,
					const std::string& prefix = "");
//	void iterUpdate(const json& j,
//					const std::string& prefix,
//					DataElements* _dataElements);
	bool updateAttribute(const std::string& path,
						 const std::string& newValue);

	std::unique_ptr<json> m_dataObj;
	std::stringstream m_iterRequestSS;
	std::string m_file;
    std::string m_root;
    int nUpdates = 0;
	std::fstream m_jfs;
    std::vector<std::string> m_segments;

};

#endif /* JSONDATA_H_ */

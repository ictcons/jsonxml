/*
 * XMLData.h
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#ifndef XMLDATA_H_
#define XMLDATA_H_

#include "pugixml.hpp"
#include "DataObject.h"

class XMLData: public DataObject {
public:
	XMLData() = default;
	virtual ~XMLData() = default;

	void load(const std::string& _file) override;
	const std::string& rootName() override;
	void save() override;
	void list() override;
	int update(DataElements* data);

private:
	std::string m_file;
    std::string m_root;
	pugi::xml_node m_rootNode;
    pugi::xml_document m_dataObj;
};

#endif /* XMLDATA_H_ */

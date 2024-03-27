/*
 * XMLData.h
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#ifndef XMLDATA_H_
#define XMLDATA_H_

#include <DataObject.h>
#include "pugixml.hpp"

class XMLData: public DataObject {
public:
	XMLData() = default;
	virtual ~XMLData() = default;

	void load(const std::string& _file) override;
	const std::string& rootName() override;
	void list() override;
	void save() override;

	bool update(const DataElement& data);

private:
	std::string m_file;
    std::string m_root;
	pugi::xml_node m_rootNode;
    pugi::xml_document m_dataObj;
};

#endif /* XMLDATA_H_ */

/*
 * jsonxml:  XMLData.h
 *
 *  Created on: 10/05/2024
 *      Author: gds, Maran Consulting
 *      Version 1.0
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

	void backup();

	int update(DataElements* data);

	const std::string& results() const {
		return m_results;
	}

private:
	std::string m_file;
    std::string m_root;
    std::string m_results;
	pugi::xml_node m_rootNode;
    pugi::xml_document m_dataObj;
};

#endif /* XMLDATA_H_ */

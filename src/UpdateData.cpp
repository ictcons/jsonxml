/*
 * UpdateData.cpp
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#include "UpdateData.h"
#include "JSONData.h"
#include "XMLData.h"
#include "DataObject.h"
typedef DataObject::DataElements DataElements;

#include <iostream>
#include <string>
#include <memory>

#include <fstream>
#include <sstream>

// Verbose logging
bool UpdateData::s_verbose = false;

//**** Do we want to have a XML-only update or JSON-only option? ****

int UpdateData::Process( const std::string& _jUpdates,
						 const std::string& _jsonfile,
						 const std::string& _xmlfile) {
	// Instantiate data objects
	std::unique_ptr<JSONData> jsd(new JSONData);
	std::unique_ptr<XMLData> xmld(new XMLData);
	std::unique_ptr<DataElements> dataElements;

	jsd->load(_jsonfile);

	dataElements = std::make_unique<DataElements>(jsd->rootName());
	int jsonUpdates = jsd->update(_jUpdates, dataElements.get());

	xmld->load(_xmlfile);
	VERBOSE(xmld->list());

	// The previous XML data file must be copied for a backup before processing.
	int xmlUpdates = xmld->update(dataElements.get());
	assert(jsonUpdates == xmlUpdates);

	return xmlUpdates;
}



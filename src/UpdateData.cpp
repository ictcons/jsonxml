/*
 * UpdateData.cpp
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#include <UpdateData.h>
#include "JSONData.h"
#include "XMLData.h"
#include "DataObject.h"
typedef DataObject::DataElements DataElements;

#include <iostream>
#include <string>
#include <memory>

#include <fstream>
#include <sstream>

#define DEBUG 0


//**** Do we want to have a XML-only update or JSON-only option? ****

int UpdateData( const std::string& _jUpdateRequest,	// JSON string or file
				const std::string& _jsonfile,
				const std::string& _xmlfile) {
	// Instantiate data objects
	std::unique_ptr<JSONData> jsd(new JSONData);
	std::unique_ptr<XMLData> xmld(new XMLData);
	std::unique_ptr<DataElements> dataElements;

	jsd->load(_jsonfile);
#if DEBUG
	jsd->list();
#endif
	dataElements = std::make_unique<DataElements>(jsd->rootName());
	int jsonUpdates = jsd->update(_jUpdateRequest, dataElements.get());

	xmld->load(_xmlfile);
#if DEBUG
	xmld->list();
#endif
	// The previous XML data file must be copied for a backup before processing.
	int xmlUpdates = xmld->update(dataElements.get());
	assert(jsonUpdates == xmlUpdates);

	return xmlUpdates;
}



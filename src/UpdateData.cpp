/*
 * UpdateXML.cpp
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#include "UpdateXML.h"
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

//#include "schema.hxx" // Include the generated header file

// The update process:
// 1) Input a JSON data file, its matching XML data file, and the client's
//    REST JSON data update request, a subset of the JSON file's data.
// 2) The JSON and related XML data is loaded into memory.
// 3) The JSONData and XMLData class objects are instantiated.
// 4) The JSONData object processes the client's JSON data update request.
//    Multiple updates can be processed, and the number is returned.
// 5) During JSON data processing, the attribute update info is added
//    to an instance of the DataElements structure.
// 6) The XMLData object then processes the data updates that are defined
//    in the DataElements object. The number of updates is returned.
// REST Response:
// The number of updates is returned, else an error message returned.

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

	//**** want to have a XML-only update option, so an already updated
	//**** json file can be used as the input to update the xml data.


//	jsd->merge(xmld.get());

	return xmlUpdates;
}



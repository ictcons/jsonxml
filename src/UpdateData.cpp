/*
 * jsonxml: UpdateData.cpp
 *
 *  Created on: 10/05/2024
 *      Author: gds, Maran Consulting
 *      Version 1.0
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

//*** Do we want to have a XML-only update or JSON-only option? ****

int UpdateData::process( const std::string& _jUpdates,
						 const std::string& _jsonfile,
						 const std::string& _xmlfile) {
	// Instantiate data objects
	std::unique_ptr<JSONData> jsd(new JSONData);
	std::unique_ptr<XMLData> xmld(new XMLData);
	std::unique_ptr<DataElements> dataElements;

	m_results ="No updates\n";

	// Load the JSON data object
	jsd->load(_jsonfile);
	// Update the JSON data file first; dataElements is also updated.
	dataElements = std::make_unique<DataElements>(jsd->rootName());
	int jsonUpdates = jsd->update(_jUpdates, dataElements.get());
	if (jsonUpdates) {
		m_results = jsd->results() ;
	}

	// Load the XML data object
	xmld->load(_xmlfile);
	// Update the XML data file using the structured updates in dataElements.
	// Process updates to the XML data file even when there are no JSON updates
	// to insure JSON:XML data equality.
	int xmlUpdates = xmld->update(dataElements.get());
	if (xmlUpdates) {
		m_results += xmld->results();
	}

	if (xmlUpdates != jsonUpdates) {
	    std::stringstream ss;
		ss << "Note that update totals differ: "
			"json="<< jsonUpdates << ", xml=" << xmlUpdates << std::endl;
		m_results += ss.str();
	}

	// return number of data updates - not used.
	return jsonUpdates;
}



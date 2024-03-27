/*
 * UpdateXML.cpp
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#include "UpdateXML.h"
#include "JSONData.h"
#include "XMLData.h"

#include <iostream>
#include <string>
#include <memory>

#include <fstream>
#include <sstream>

//#include "schema.hxx" // Include the generated header file

// The update process:
// Iterate through each key:value pair
// For each pair, iterate through XML object's key:value pairs to
// find the matching data element.
// Check if the JSON and XML objects' keys' values have been changed.
// If so, update the XML object's data value to equal JSON's update.
// Iterate trough all of the JSON object's data until finished.
// If there were updates, save the
void updateXML(const std::string& _jsonfile, const std::string& _xmlfile) {
	// Instantiate data objects
	std::unique_ptr<JSONData> jsd(new JSONData);
	std::unique_ptr<XMLData> xmld(new XMLData);

	jsd->load(_jsonfile);
	jsd->list();

	xmld->load(_xmlfile);
	xmld->list();

	jsd->merge(xmld.get());

	return;
}





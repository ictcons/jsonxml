/*
 * UpdateXML.h
 *
 *  Created on: 20/03/2024
 *      Author: gds
 */

#ifndef UPDATEXML_H_
#define UPDATEXML_H_

#include <string>

int UpdateData(const std::string& _jsonUpdates,
			   const std::string& _jsonfile,
			   const std::string& _xmlfile = std::string());


#endif /* UPDATEXML_H_ */

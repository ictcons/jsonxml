/*
 * UpdateXML.h
 *
 *  Created on: 20/03/2024
 *      Author: gds
 */

#ifndef UPDATEXML_H_
#define UPDATEXML_H_

#include <string>

class UpdateData;

#define VERBOSE(instr) if (UpdateData::showVerbose()) { instr ;}

class UpdateData {
public:
	UpdateData() = default;
	~UpdateData() = default;

	static int Process(const std::string& _jsonUpdates,
						const std::string& _jsonfile,
						const std::string& _xmlfile = std::string());

	static bool showVerbose() {
		return UpdateData::s_verbose;
	}
	static void setVerbose() {
		UpdateData::s_verbose = true;
	}

private:
	static bool s_verbose;

};


#endif /* UPDATEXML_H_ */

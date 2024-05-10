/*
 * jsonxml: UpdateXML.h
 *
 *  Created on: 10/05/2024
 *      Author: gds, Maran Consulting
 *      Version 1.0
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

	int process(const std::string& _jsonUpdates,
				const std::string& _jsonfile,
				const std::string& _xmlfile = std::string());

	static bool showVerbose() {
		return UpdateData::s_verbose;
	}
	static void setVerbose() {
		UpdateData::s_verbose = true;
	}

	const std::string& results() const {
		return m_results;
	}

private:
	static bool s_verbose;
	std::string m_results;

};


#endif /* UPDATEXML_H_ */

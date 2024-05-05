//============================================================================
// Name        : jsonxml
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include <UpdateData.h>
#include "Timestamp.h"

static const std::string version = "1.0";

static const std::string subpath = "//..\\cache\\";

#define DEBUG


int main(int argc, char *argv[]) {
	std::stringstream err;
	enum {upds, json, xml};		// Types in the argument sequence
	std::vector<std::string> file;

	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg.find("--version") != std::string::npos) {
			std::cout << "jsonxml, version " << version << ", "
				<< "built " << buildTime() << std::endl;
			return 0;
		}
		else if (arg.find("-help") != std::string::npos) {	// Can also be "--help"
			std::cout << "jsonxml.exe [-v] data-updates-file[.json] data-file[.json] [data-file[.xml]]\n\n";
			std::cout << "  [-v] Verbose output to console.\n";
			std::cout << "  update-request.json\n";
			std::cout << "		 A JSON data 'request' that contains updated values to be assigned\n";
			std::cout << "		 to attributes in the associated JSON and XML paired data sets. The\n";
			std::cout << "		 input must be a JSON subset text of the data file to be updated,\n";
			std::cout << "		 or the name of another JSON file that contains the JSON update text.\n\n";
			std::cout << "  data-file[.json]\n";
			std::cout << "		 The name of the JSON data file to be updated. If specified without\n";
			std::cout << "		 the \".json\" extension, the name will refer to both JSON and XML\n";
			std::cout << "		 files, and extensions wil be applied bu default.\n\n";
			std::cout << "  [data-file[.xml]]\n";
			std::cout << "		 The name of the XML file which is paired with the JSON data file.\n";
			std::cout << "		 By default, if not specified, the XML file name will be same as the\n"; ;
			std::cout << "		 JSON data file but with the \".xml\" extensions.\n";
			return 0;
		}
		else if (arg.find("-v") != std::string::npos) {
			UpdateData::setVerbose();
		}
		else {
			// Add the data file to the container, in the order of the enums
			file.push_back(arg);
		}
	}

	if (file.size() < 2) {
		std::cout << "jsonxml.exe [-v[erbose]] data-updates-file[.json] data-file[.json] [data-file[.xml]]\n";
		std::cout << "For information, run 'jsonxml.exe --help'\n";
		return 0;
	}

	try {
		std::string::size_type pos;

		// Check if first two files have JSON extension
		for (int i = 0; i < 2; i++) {
			// If JSON data file has no extension, then add ".json" extension
			pos = file[i].find(".json");
			if (pos == std::string::npos) {
				file[i].append(".json");
			}
		}

		// If the 3nd optional arg for the XML file name is missing,
		// push the base name of the JSON data file to the vector,
		// then append the .xml extension.
		if (file.size() == 2) {
			pos = file.back().find(".json");
			std::string xmlfile = file.back().substr(0, pos);
			// Add a copy the JSON data file base name to the vector
			file.push_back(xmlfile);
		}
		// Append the XML extension to the 3rd file name, if missing.
		if ((pos = file.back().find(".xml")) == std::string::npos) {
			file.back().append(".xml");
		}

		// Check existence of the two files
#ifdef DEBUG
		fs::path p = getenv("DEBUG_PATH");
		//p += "/";
#else
		fs::path p = fs::current_path();
		p += "/" + subpath;
#endif
		VERBOSE(std::cout << "exec path: " << p << std::endl;)

		// Fetch reference to filename can be expanded by path
		for ( std::string& f : file ) {
			std::string fp(p.string() + f);
			ifstream fin(fp);
			if (! fin.is_open()) {
				err << "File " << fp << " not found";
				throw std::runtime_error(err.str());
			}
			// Update file array with full path & list
			f = fp;
			VERBOSE(std::cout << f << std::endl;)
		}

		// Process update of XML data object with data changes
		// provided in the JSON data object
		UpdateData::Process(file[upds], file[json], file[xml]);
	}

	catch (const std::exception& e) {
	    std::cout << "Error: " << e.what() << std::endl;
	    return 1;
	}

    std::cout << "Data files updated\n";
    return 0;
}

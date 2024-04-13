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
static const std::string subpath = "\\Debug\\";

int main(int argc, char *argv[]) {
	std::stringstream err;
	enum {json, xml};
	std::vector<std::string> file;
	std::string jUpdateRequest;

	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if (arg.find("--version") != std::string::npos) {
			std::cout << "jsonxml, version " << version << ", "
				<< "built " << buildTime() << std::endl;
			return 0;
		}
		else if (arg.find("--help") != std::string::npos) {
			std::cout << "jsonxml.exe -u update-request datafile[.json] [datafile[.xml]]\n\n";
			std::cout << "  -u[pdate] update-request, or\n";
			std::cout << "            update-request.json\n";
			std::cout << "		A JSON data 'request' that contains updated values to be assigned\n";
			std::cout << "		to attributes in the associated JSON and XML paired data sets. The\n";
			std::cout << "		input must be a JSON subset text of the data file to be updated,\n";
			std::cout << "		or the name of another JSON file that contains the JSON update text.\n\n";
			std::cout << "  datafile[.json]\n";
			std::cout << "		The name of the JSON data file to be updated. If specified without\n";
			std::cout << "		the \".json\" extension, the name will refer to both JSON and XML\n";
			std::cout << "		files, and extensions wil be applied bu default.\n\n";
			std::cout << "  [datafile[.xml]]\n";
			std::cout << "		The name of the XML file which is paired with the JSON data file.\n";
			std::cout << "		By default, if not specified, the XML file name will be same as the\n"; ;
			std::cout << "		JSON data file but with the \".xml\" extensions.\n";
			return 0;
		}
		else if (arg.find("-u") != std::string::npos) {
			jUpdateRequest = argv[++i];
		}
		else {
			// Add the data file to the container
			file.push_back(arg);
		}
	}

	if (file.empty()) {
		std::cout << "jsonxml.exe -u[pdate] update-request datafile[.json] [datafile[.xml]]\n";
		std::cout << "For information, run 'jsonxml.exe --help'\n";
		return 0;
	}

	try {
		assert(! jUpdateRequest.empty());

		// Check extension of JSON file name
		std::string::size_type pos;
		// If JSON data file has no extension, then add ".json" extention
		if ((pos = file[json].find('.')) == std::string::npos) {
			pos = file[json].length();
			file[json].append(".json");
		}
		// If the 2nd optional arg for the XML file name is missing,
		// use the JSON file name with the .xml extension.
		if (file.size() == 1) {
			// Copy the JSON file name & append extension next instruction.
			file.push_back(file[json].substr(0, pos));
		}

		// Handle missing extension
		if ((pos = file[xml].find('.')) == std::string::npos) {
			file[xml].append(".xml");
		}

		// Check existence of the two files
		fs::path p = fs::current_path();
		p += subpath;
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
			std::cout << f << std::endl;
		}

		// Process update of XML data object with data changes
		// provided in the JSON data object
		UpdateData(jUpdateRequest, file[json], file[xml]);
	}

	catch (const std::exception& e) {
	    std::cout << "Error: " << e.what() << std::endl;
	    return 1;
	}

    std::cout << "Data files updated\n";
    return 0;
}

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

static const std::string version = "0.2";
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
			std::cout << "	   The update-request argument is a subset of the JSON data\n";
			std::cout << "	   file in the next argument, that has one or more attributes\n";
			std::cout << "	   with new data to replace existing data in the JSON file.\n";
			std::cout << "	   The input can either be a JSON data string, or the name of\n";
			std::cout << "	   another JSON data file that contains the JSON data string.\n\n";
			std::cout << "  datafile[.json]\n";
			std::cout << "	   The name of the JSON data file to be updated. The \".json\"\n";
			std::cout << "	   extension will be added if not specified.\n\n";
			std::cout << "  [datafile[.xml]], or\n";
			std::cout << "  -x[ml]\n";
			std::cout << "	   The name of the XML file which is paired with the JSON data\n";
			std::cout << "	   file. The XML file will also be updated with the same data.\n";
			std::cout << "	   By default, the two file names are identical, except for the\n";
			std::cout << "	   extensions. Alternatively, the \"-x\" option can be used,\n";
			std::cout << "	   instead of the XML file name, to update the paired XML file.\n";
			std::cout << "	   If neither option is specified, only the JSON data file will\n";
			std::cout << "	   be updated.\n";
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

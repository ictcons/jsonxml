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

#include "UpdateXML.h"
#include "Timestamp.h"

static const std::string version = "0.2";
static const std::string subpath = "\\Debug\\";

int main(int argc, char *argv[]) {
	std::stringstream err;
	enum {json, xml, upds};
	std::vector<std::string> file;
	std::string jUpdateRequest;

	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		std::cout << "arg: " << arg << std::endl;
		if (arg.find("--version") != std::string::npos) {
			std::cout << "jsonxml, version " << version << ", "
				<< "built " << buildTime() << std::endl;
			return 0;
		}
		else if (arg.find("--help") != std::string::npos) {
			std::cout << " > jsonxml.exe -upd <update-request> <datafile[.json]> [<datafile[.xml]>]\n";
			std::cout << " -upd <update-request>\n";
			std::cout << "      <update-request.json>\n";
			std::cout << "                     The update request without the .json extension indicates\n";
			std::cout << "                     that the input is a string, a subset of the JSON data file\n";
			std::cout << "                     contents, that contains attribute updates to be applied to\n";
			std::cout << "                     the JSON and XML data files\n";
			std::cout << "                     Alternatively, a JSON file that contains the JSON subset of\n";
			std::cout << "                     attribute updates can also specified.\n";
			std::cout << " <datafile[.json]>   The name of the target JSON data file, related to the XML\n";
			std::cout << "                     data file, that is to be updated. The \".json\" extension\n";
			std::cout << "                     is added if not specified.\n";
			std::cout << " [<datafile[.xml]>]  The name of the XML file, from which the the JSON file was\n";
			std::cout << "                     created. By default, the two names are the same, so the XML\n";
			std::cout << "                     file name will be defaulted if not entered.\n";
			return 0;
		}
		else if (arg.find("-upd") != std::string::npos) {
			jUpdateRequest = argv[++i];
		}
		else {
			// Add the data file to the container
			file.push_back(arg);
		}
	}

	if (file.empty()) {
		std::cout << " jsonxml.exe -upd <update-request> <datafile[.json]> [<datafile[.xml]>]\n";
		std::cout << " > run jsonxml.exe --help for more information.\n";
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

    std::cout << "Data files updated successfully.\n";
    return 0;
}

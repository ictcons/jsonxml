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
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "UpdateXML.h"
#include "Timestamp.h"

static const std::string version = "0.1";
static const std::string subpath = "\\Debug\\";

int main(int argc, char *argv[]) {
	std::stringstream err;
	enum {json, xml};
	std::vector<std::string> file;
	for (int i = 1; i < argc; i++)  {
		std::string arg = argv[i];
		if (arg.find("--version") != std::string::npos) {
			std::cout << "json-xml, version " << version << ", "
				<< "built " << buildTime() << std::endl;
			return 0;
		}
		else if (arg.find("--help") != std::string::npos) {
			std::cout << "> jsonxml.exe <datafile[.json]> [<datafile.xml>]\n";
			std::cout << "The first argument takes the name of a JSON data file that was created\n";
			std::cout << "by an XML->JSON conversion. By default, the extension is \".json\".\n";
			std::cout << "The second argument is the XML file name. This is an option since both\n";
			std::cout << "file names will initially be equal as the JSON was created from the XML.\n";
			std::cout << "However, alternate file names can be input and processed so long as the\n";
			std::cout << "RootNames in the two data files are equal.\n";
			return 0;
		}
		file.push_back(arg);
	}
	if (file[json].empty()) {
		std::cout << " json-xml.exe <datafile[.json]> [<datafile.xml>]\n";
		std::cout << " Run json-xml.exe --help for more information.\n";
		return 0;
	}

	try {
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
			// Copy file name; append extension next test
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
			fin.close();
			// Update file array with full path
			f = fp;
		}

		for (auto f : file)  {
			std::cout << f << std::endl;
		}
		// Process update of XML data object with data changes
		// provided in the JSON data object
		updateXML(file[json], file[xml]);
	}

	catch (const std::exception& e) {
	    std::cout << "Error: " << e.what() << std::endl;
	    return 1;
	}

    std::cout << "XML file updated successfully.\n";
    return 0;
}

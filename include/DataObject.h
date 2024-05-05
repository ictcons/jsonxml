/*
 * DataObject.h
 *
 *  Created on: 27/03/2024
 *      Author: gds
 */

#ifndef DATAOBJECT_H_
#define DATAOBJECT_H_

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

bool isNumeric(const std::string& s);
bool fileExists(const std::string& f);
const std::string& removeChars(std::string& s, const std::string& chars);


class DataObject {
public:
	DataObject() =  default;
	virtual ~DataObject() = default;

	virtual void load(const std::string& _file) = 0;
	virtual const std::string& rootName() = 0;
	virtual void save() = 0;
	virtual void list() = 0;

	struct DataElements {
		// pair<element-path ("/root/A/B/C"), value>
 		typedef std::pair<std::string,std::string> Attribute;
		DataElements(const std::string& _root,
					 std::string _elemPath,
					 // auto in argument made available in C++20!
					 auto _value) :
			root(_root)
		{
			add(_elemPath, _value);
		}

		DataElements(const std::string& _root) :
			root(_root)
		{
		}

		bool add(const std::string& _elemPath, auto _value) {
			// Use stream to handle both string and int attributes
			for (auto& attr : this->attributes) {
				if (attr.first == _elemPath) {
					return false;
				}
			}
			std::ostringstream oss;
			oss << _value;
			attributes.push_back( {_elemPath, oss.str()} );
			//std::cout << "Added: " << _elemPath << std::endl;
			return true;
		}

		const std::string& getValue(const std::string& _elemPath) const {
			static const std::string empty;
			for (auto& attr : this->attributes) {
				if (attr.first.find(_elemPath) != std::string::npos) {
					return attr.second;
				}
			}
			return empty;
		}
		std::string root;	// name
		// vector of multiple attribute updates
		std::vector<Attribute> attributes;
	};

};

typedef DataObject::DataElements DataElements;


#endif /* DATAOBJECT_H_ */

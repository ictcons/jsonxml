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

class DataObject {
public:
	DataObject() =  default;
	virtual ~DataObject() = default;

	virtual void load(const std::string& _file) = 0;
	virtual const std::string& rootName() = 0;
	virtual void list() = 0;
	//virtual std::string keyValue(const std::string& _key) = 0;
	virtual void save() = 0;

	struct DataElement {
		std::string element;	// name
		std::string attribute;	// name
		std::string value;		// data
		DataElement(const std::string& elem,
					const std::string& attr,
					// auto in argument made available in C++20!
					auto val) :
			element(elem),
			attribute(attr)
		{
			std::ostringstream oss;
			oss << val;
			value = oss.str();
		}
	};

protected:
	static bool fileExists(const std::string& _file);

};

#endif /* DATAOBJECT_H_ */

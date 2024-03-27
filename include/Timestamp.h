/*
 * Timestamp.h
 *
 *  Created on: 12/02/2024
 *      Author: gds
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <chrono>
#include <string>
#include <sstream>

 std::string timestamp() {
	 std::stringstream ss;
	 const auto p1 = std::chrono::system_clock::now();
	 ss << std::chrono::duration_cast<std::chrono::seconds>(
              p1.time_since_epoch()).count();
	 return ss.str();
 }

 std::string buildTime() {
	std::stringstream ss;
	ss << std::string(__DATE__) << "-" << std::string(__TIME__);
	return ss.str();
}


#endif /* TIMESTAMP_H_ */

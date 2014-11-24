/*
 * utils.cpp
 *
 *  Created on: Mar 12, 2010
 *      Author: gumer
 */

#include<cstdlib>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<list>
#include<string>
#include<sstream>
using namespace std;



/**
 * Splits string by delimiters.
 * @param str String to split
 * @param tokens Result is wrtten here
 * @param delimiters Delimiters to split by. e.g. "\n\t ,."
 */
vector<string> mb_str_split(const string& str,
			const string& delimiters = " ")
{
	vector<string> tokens;
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);

	// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}

	return tokens;
}

std::string mb_str_trim_right(const std::string &source , const std::string& t = " \r\n\t")
{
	std::string str = source;
	return str.erase( str.find_last_not_of(t) + 1);
}

std::string mb_str_trim_left( const std::string& source, const std::string& t = " \r\n\t")
{
	std::string str = source;
	return str.erase(0 , source.find_first_not_of(t) );
}

std::string mb_str_trim(const std::string& source, const std::string& t = " \r\n\t")
{
	std::string str = source;
	return mb_str_trim_left( mb_str_trim_right( str , t) , t );
}


/**
 * Converts string to double.
 * @param source Input string.
 * @param result Output value.
 * @return True if conversion went OK. False for ERROR.
 */
bool mb_str_to_double( const std::string& source, double& result )
{
  std::istringstream i(source);
  if (!(i >> result)){
	  return false;
  }
  return true;
}



#ifndef __MYMYMY_UTILS_H_
#define __MYMYMY_UTILS_H_

#include<cstdlib>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<list>
#include<string>
#include<sstream>

/**
 * Prints vector<T> to string
 * NOTE: T must implement operator<<. Compile time errors otherwise.
 * @param vec Vector to print
 * @return string in format '[ vec[0], vec[1], vec[2], ... , vec[n] ]'
 */
template< class T >
std::string mb_print_vec(  std::vector<T> vec ){

	std::ostringstream out;

	out << "[ " ;
	int size = vec.size();
	for ( int i = 0; i < size; i++ ){
		out << vec[i];
		// Don't print for last iteration
		if ( i != size-1 ){
			out << ", ";
		}
	}
	out << " ]";

	return out.str();

}


/**
 * Splits string by delimiters.
 * @param str String to split
 * @param tokens Result is wrtten here
 * @param delimiters Delimiters to split by. e.g. "\n\t ,."
 */
std::vector<std::string> mb_str_split(const std::string& str,
			const std::string& delimiters = " ");

std::string mb_str_trim_right(
		const std::string &source , const std::string& t = " \r\n\t");

std::string mb_str_trim_left(
		const std::string& source, const std::string& t = " \r\n\t");

std::string mb_str_trim(
		const std::string& source, const std::string& t = " \r\n\t");


/**
 * Converts string to double.
 * @param source Input string.
 * @param result Output value.
 * @return True if conversion went OK. False for ERROR.
 */
bool mb_str_to_double( const std::string& source, double& result );



#endif

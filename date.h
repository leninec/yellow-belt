/*
 * date.h
 *
 *  Created on: 23 дек. 2019 г.
 *      Author: Vasilii
 */

#ifndef DATE_H_
#define DATE_H_
#pragma once
#include <istream>
#include <memory>
#include <iomanip>


class Date{
public:
	  Date(int y,int m,int d):year(y),month(m),day(d){}
	  int GetYear() const{
		  return year;
	  }
	  int GetMonth() const{
		  return month;
	  }
	  int GetDay() const{
		  return day;
	  }
	  void PrintDate()const;
private:
	int year;
	int month;
	int day;
};

//std::shared_ptr<Date> ParseDate(std::istringstream&  is);
Date ParseDate(std::istream&  is);
Date ParseDate(std::istringstream&  is);
bool operator<(const Date& lhs, const Date& rhs);

bool operator<(const Date& lhs, const Date& rhs);
bool operator>(const Date& lhs, const Date& rhs);
bool operator<=(const Date& lhs, const Date& rhs);
bool operator>=(const Date& lhs, const Date& rhs);
bool operator==(const Date& lhs, const Date& rhs);
bool operator!=(const Date& lhs, const Date& rhs);



std::ostream& operator<<(std::ostream& stream,std::pair<Date,std::string> p);
std::ostream& operator<<(std::ostream& stream,const Date d);


#endif /* DATE_H_ */

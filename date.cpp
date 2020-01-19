/*
 * date.cpp
 *
 *  Created on: 23 дек. 2019 г.
 *      Author: Vasilii
 */

#include "date.h"
#include <iostream>
#include <vector>


using namespace std;

// определить сравнение для дат необходимо для использования их в качестве ключей словаря
bool operator<(const Date& lhs, const Date& rhs) {
  // воспользуемся тем фактом, что векторы уже можно сравнивать на <:
  // создадим вектор из года, месяца и дня для каждой даты и сравним их
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}
bool operator>(const Date& lhs, const Date& rhs) {
  // воспользуемся тем фактом, что векторы уже можно сравнивать на <:
  // создадим вектор из года, месяца и дня для каждой даты и сравним их
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} >
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}
bool operator<=(const Date& lhs, const Date& rhs) {
  // воспользуемся тем фактом, что векторы уже можно сравнивать на <:
  // создадим вектор из года, месяца и дня для каждой даты и сравним их
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <=
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}
bool operator>=(const Date& lhs, const Date& rhs) {
  // воспользуемся тем фактом, что векторы уже можно сравнивать на <:
  // создадим вектор из года, месяца и дня для каждой даты и сравним их
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} >=
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}
bool operator==(const Date& lhs, const Date& rhs) {
  // воспользуемся тем фактом, что векторы уже можно сравнивать на <:
  // создадим вектор из года, месяца и дня для каждой даты и сравним их
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} ==
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}
bool operator!=(const Date& lhs, const Date& rhs) {
  // воспользуемся тем фактом, что векторы уже можно сравнивать на <:
  // создадим вектор из года, месяца и дня для каждой даты и сравним их
  return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} !=
      vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

void EnsureNextSymbolAndSkip (istringstream& ss)
{
	if (ss.peek() != '-'){
				stringstream ssEr;
				ssEr << "4Wrong date format: " ;
				throw runtime_error(ssEr.str());
	}
	ss.ignore(1);
}
void EnsureNextSymbolAndSkip (istream& ss)
{
	if (ss.peek() != '-'){
				stringstream ssEr;
				ssEr << "4Wrong date format: " ;
				throw runtime_error(ssEr.str());
	}
	ss.ignore(1);
}
int ReadFromStreamToIntAndCheck(istringstream& ss){
	int n;
	if(!(ss>>n)){
		string test;
		getline(ss,test);
		cout<<test << " test"<<endl;
		cout<<n<<" test"<<endl;
		    stringstream ssEr;
			if(test == ""){
				 ssEr << "1Wrong date format:";
			}
			else{

				ssEr << "2Wrong date format: " + test;
			}
			throw runtime_error(ssEr.str());
		}else{
			//cout<<n<<" test"<<endl;
			return n;
		}
}
int ReadFromStreamToIntAndCheck(istream& ss){
	int n;
	if(!(ss>>n)){
		string test;
		getline(ss,test);
		cout<<test << " test"<<endl;
		cout<<n<<" test"<<endl;
		    stringstream ssEr;
			if(test == ""){
				 ssEr << "1Wrong date format:";
			}
			else{

				ssEr << "2Wrong date format: " + test;
			}
			throw runtime_error(ssEr.str());
		}else{
			//cout<<n<<" test"<<endl;
			return n;
		}
}

//shared_ptr<Date> ParseDate(istringstream&  is)
Date ParseDate(istringstream&  is)
{
	string sDate;
	//is>>sDate;
	int year = ReadFromStreamToIntAndCheck(is);
	EnsureNextSymbolAndSkip(is);
			int month = ReadFromStreamToIntAndCheck(is);
			EnsureNextSymbolAndSkip(is);
			int day  = ReadFromStreamToIntAndCheck(is);
	/*		if( is.peek() != ' '){    // проверял что дальше есть пробел. в команде del может не быть
				stringstream ssEr;
				string s;
				getline(is,s);
				ssEr<< "6Wrong date format: " + s;
				throw runtime_error(ssEr.str());
			}
			*/
			if ((month>12)||(month<1)){
				stringstream ssEr;
				ssEr << "Month value is invalid: " <<month;
				throw runtime_error(ssEr.str());
			}
			if ((day>31)||(day<1)){
						stringstream ssEr;
						ssEr << "Day value is invalid: " <<day;
						throw runtime_error(ssEr.str());
			}

			return Date(year, month, day);
			//return make_shared<Date>(year, month, day);
}


//shared_ptr<Date> ParseDate(istream&  is)
Date ParseDate(istream&  is)
{
	string sDate;
	//is>>sDate;
	int year = ReadFromStreamToIntAndCheck(is);
	EnsureNextSymbolAndSkip(is);
			int month = ReadFromStreamToIntAndCheck(is);
			EnsureNextSymbolAndSkip(is);
			int day  = ReadFromStreamToIntAndCheck(is);
	/*		if( is.peek() != ' '){
				stringstream ssEr;
				string s;
				getline(is,s);
				ssEr<< "6Wrong date format: " + s;
				throw runtime_error(ssEr.str());
			}
			*/
			if ((month>12)||(month<1)){
				stringstream ssEr;
				ssEr << "Month value is invalid: " <<month;
				throw runtime_error(ssEr.str());
			}
			if ((day>31)||(day<1)){
						stringstream ssEr;
						ssEr << "Day value is invalid: " <<day;
						throw runtime_error(ssEr.str());
			}

			//return make_shared<Date>(year, month, day);
			return Date(year, month, day);
}

void Date::PrintDate() const{
	cout<<GetYear()<<"-"<<GetMonth()<<"-"<<GetDay()<<endl;
}
ostream& operator<<(ostream& stream, pair<Date,string> p)
{
	stream<<setfill('0')<<setw(4)<<p.first.GetYear()<<"-"<<setw(2)<<p.first.GetMonth()<<"-"<<setw(2)<<p.first.GetDay()<<" ";
	stream<<p.second;
	return stream;
}
ostream& operator<<(ostream& stream,const Date d)
{
	stream<<setfill('0')<<setw(4)<<d.GetYear()<<"-"<<setw(2)<<d.GetMonth()<<"-"<<setw(2)<<d.GetDay();
	return stream;
}



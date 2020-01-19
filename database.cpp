/*
 * database.cpp
 *
 *  Created on: 23 дек. 2019 г.
 *      Author: Vasilii
 */


#include "database.h"
#include <iostream>
using namespace std;


void Database::Add(std::shared_ptr<Date> d,std::string event)
{
	auto r = storage[*d].insert(event);
		if (r.second){
			storageV[*d].push_back(event);
		}
}
void Database::Add(Date d,std::string event)
{
	auto r = storage[d].insert(event);
	if (r.second){
		storageV[d].push_back(event);
	}

}
void Database::Print(ostream& s) const
{
	for (auto c : storageV)
	{
		for (auto it = c.second.begin();it!=c.second.end();it++)
		{
			s<<setfill('0')<<setw(4)<<c.first.GetYear()<<"-"<<setw(2)<<c.first.GetMonth()<<"-"<<setw(2)<<c.first.GetDay()<<" ";
			s<<*it<<endl;
		}
	}
}
string Database::Last(shared_ptr<Date> d) const
{

	stringstream stream;
	auto it = storageV.lower_bound(*d); // получил равный или больший чем искомое
	if(it == storageV.begin()){
		if (it->first == *d){
			stream<<setfill('0')<<setw(4)<<it->first.GetYear()<<"-"<<setw(2)<<it->first.GetMonth()<<"-"<<setw(2)<<it->first.GetDay()<<" "<<it->second.back();
			//s = to_string(it->first.GetYear())+'-'+to_string(it->first.GetMonth())+'-'+to_string(it->first.GetDay())+' ';
			//s = s+ it->second.back();
			//return s;
			return stream.str();
		}
		else{
			return "No entries";
		}
	}
		else{
			if (it->first != *d){
				it--;
			}
			stream<<setfill('0')<<setw(4)<<it->first.GetYear()<<"-"<<setw(2)<<it->first.GetMonth()<<"-"<<setw(2)<<it->first.GetDay()<<" "<<it->second.back();
			return stream.str();
		}
}
string Database::Last(Date d) const
{
	stringstream stream;
	auto it = storageV.lower_bound(d); // получил равный или больший чем искомое
	if(it == storageV.begin()){
		if (it->first == d){
			stream<<setfill('0')<<setw(4)<<it->first.GetYear()<<"-"<<setw(2)<<it->first.GetMonth()<<"-"<<setw(2)<<it->first.GetDay()<<" "<<it->second.back();
			return stream.str();
		}
		else{
			return "No entries";
		}
	}
		else{
			if (it->first != d){
				it--;
			}
			stream<<setfill('0')<<setw(4)<<it->first.GetYear()<<"-"<<setw(2)<<it->first.GetMonth()<<"-"<<setw(2)<<it->first.GetDay()<<" "<<it->second.back();
			return stream.str();

		}

}


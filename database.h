/*
 * database.h
 *
 *  Created on: 23 дек. 2019 г.
 *      Author: Vasilii
 */

#ifndef DATABASE_H_
#define DATABASE_H_
#pragma once

#include <string>
#include <istream>
#include <memory>
#include <map>
#include <vector>
#include "date.h"
#include <utility>
#include "node.h"
#include <set>
#include <algorithm>
#include <iostream>


class Database{
public:
	void Add(std::shared_ptr<Date> d,std::string s);
	void Add(Date d,std::string s);
	void Print(std::ostream& s) const;
	template <typename Func>
	int RemoveIf(Func predicate){
		std::vector<Date> remD;
		int count = 0;
		for (auto &c:storageV){
			Date space = c.first;
			auto bound = std::stable_partition(c.second.begin(),c.second.end(),
					[predicate, space](std::string str){return (!predicate(space, str));});
			count += c.second.end() - bound;

						// не удал€ю файлы из set

			if (bound == c.second.begin() ){
				remD.push_back(c.first);
				storage.erase(c.first);
			}else{
				c.second.erase(bound,c.second.end());
			}
		}
		for (auto& it: remD){
			storageV.erase(it);
		}

		return count;
	}
	template <typename Func>
	std::vector<std::pair<Date,std::string>> FindIf(Func predicate) const{
		std::vector<std::pair<Date,std::string>> f;
			for (auto c:storage){
				Date space = c.first;
				std::vector<std::string> temp(c.second.size());
				auto it = copy_if(c.second.begin(), c.second.end(), temp.begin(), [predicate, space](std::string s){
								return predicate(space, s);});
				for (auto j = temp.begin(); j<it; j++)
				{
					f.push_back(std::make_pair(c.first,*j));
				}


				/*
						for (auto b: c.second){
							if (predicate(c.first,b)){
								f.push_back(std::make_pair(c.first,b));
							}
						}
						*/
					}
			return f;
		}
	std::string Last(std::shared_ptr<Date> d) const;
	std::string Last(Date d) const;




private:
	std::map<Date, std::set<std::string>> storage;
	std::map<Date, std::vector<std::string>> storageV;

};



#endif /* DATABASE_H_ */


#ifndef NODE_H_
#define NODE_H_
#pragma once

#include "date.h"
#include <string>
#include <memory>
#include <iostream>


enum class Comparison {
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	NotEqual
};
enum class LogicalOperation{
	Or,
	And
};


struct Node{
	virtual bool Evaluate(const Date& date, const std::string& event) const = 0;
	virtual std::string PrintNode() const = 0;
};

class EmptyNode :public Node{
public:
	 bool Evaluate(const Date& date, const std::string& event) const override{
		 return false;
	 }
	 std::string PrintNode() const override{
		 std::string s;
		 s = "Empty node";
		 return s;
	 }
};
class DateComparisonNode :public Node{
public:
	//DateComparisonNode(const Comparison& cmp, std::shared_ptr<Date> date);
	DateComparisonNode(const Comparison& cmp, Date date);
	bool Evaluate(const Date& date, const std::string& event) const override{
		switch (cmp_){
		case Comparison::Less:
			return(date<date_);
			break;
		case Comparison::Greater:
				return(date>date_);
				break;
		case Comparison::GreaterOrEqual:
				return(date>=date_);
				break;
		case Comparison::LessOrEqual:
				return(date<=date_);
				break;
		case Comparison::NotEqual:
				return(date!=date_);
				break;
		case Comparison::Equal:
				return(date==date_);
				break;
		default:
			return false;
		  break;
		}
	}
	std::string PrintNode() const override{
		std::string s;
		s = std::to_string(date_.GetYear())+'-'+ std::to_string(date_.GetMonth())+ '-' + std::to_string(date_.GetDay())+' ';
		return s;
	}
private:
	const Comparison cmp_;
	const Date date_;

};
class EventComparisonNode :public Node{
public:
	EventComparisonNode(const Comparison& cmp, const std::string& value);
	bool Evaluate(const Date& date, const std::string& event) const override
	{
		switch (cmp_){
				case Comparison::Less:
					return(event < value_);
					break;
				case Comparison::Greater:
					return(event > value_);
					break;
				case Comparison::GreaterOrEqual:
					return(event >= value_);
					break;
				case Comparison::LessOrEqual:
					return(event <= value_);
					break;
				case Comparison::NotEqual:
					return(event != value_);
					break;
				case Comparison::Equal:
					return(event == value_);
					break;
		}
		return false;
	}
	std::string PrintNode() const override{
		std::string s;
		s = "Event node";
		return s;
	}
	private:
	    const Comparison cmp_;
	    const std::string value_;

};
class LogicalOperationNode :public Node{
public:
	LogicalOperationNode(const LogicalOperation& op, const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs);
	bool Evaluate(const Date& date, const std::string& event) const override
	{
		if (operation == LogicalOperation::And){
			return ((lhs_->Evaluate(date,event))&&(rhs_->Evaluate(date,event)));
		}
		if (operation == LogicalOperation::Or){
			return ((lhs_->Evaluate(date,event))||(rhs_->Evaluate(date,event)));
		}
	}
	std::string PrintNode() const override{
		std::string s;
		s = "Logic node: l = ";
		if (operation == LogicalOperation::And){
			s = s + lhs_->PrintNode() + "AND r = "+ rhs_->PrintNode();
		}
		else{
			s = s + lhs_->PrintNode() + "OR r = "+ rhs_->PrintNode();
		}
		return s;
	}
	private:
	    const LogicalOperation operation;
	    const std::shared_ptr<Node> lhs_, rhs_;
};


#endif /* NODE_H_ */

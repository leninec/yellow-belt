/*
 * node.cpp
 *
 *  Created on: 23 дек. 2019 г.
 *      Author: Vasilii
 */

#include "node.h"
using namespace std;


//DateComparisonNode::DateComparisonNode(const Comparison& cmp, shared_ptr<Date> date)
DateComparisonNode::DateComparisonNode(const Comparison& cmp, Date date)
	:cmp_(cmp),date_(date.GetYear(),date.GetMonth(),date.GetDay())
	{
		//cout<< "Конструктор DateComparisonNode "<<PrintNode() <<endl;;
	}
EventComparisonNode::EventComparisonNode(const Comparison& cmp, const string& value)
	:cmp_(cmp),value_(value)
	{
		//cout<< "Конструктор EventComparisonNode"<<endl;;
	}
LogicalOperationNode::LogicalOperationNode(const LogicalOperation& op, const shared_ptr<Node>& lhs, const shared_ptr<Node>& rhs)
	: operation(op),lhs_(lhs), rhs_(rhs)
	{
		//cout<< "Конструктор LogicalOperationNode "<<PrintNode()<<endl;;
	}



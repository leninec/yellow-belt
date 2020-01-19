/*
 * condition_parser.h
 *
 *  Created on: 4 џэт. 2020 у.
 *      Author: Vasilii
 */

#include "node.h"

#include <memory>
#include <iostream>

using namespace std;

shared_ptr<Node> ParseCondition(istream& is);

void TestParseCondition();

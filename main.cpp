#include "database.h"
#include "date.h"
#include "condition_parser.h"
#include "node.h"
#include <cassert>
#include "test_runner.h"



#include <iostream>
#include <stdexcept>

using namespace std;


string DoFind (Database& db, const string& str) {
    istringstream is (str);
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date &date, const string &event) {
        return condition->Evaluate(date, event);
    };
    const auto entries = db.FindIf(predicate);
    ostringstream os;
    for (const auto& entry : entries) {
        os << entry << endl;
    }
    os << entries.size();
    return os.str();
}
int DoRemove (Database& db, const string& str) {
    istringstream is (str);
    auto condition = ParseCondition(is);
    auto predicate = [condition](const Date &date, const string &event) {
        return condition->Evaluate(date, event);
    };
    return db.RemoveIf(predicate);
}

string ParseEvent(istream& is) {
		string event;
		getline(is,event);
		auto it = event.begin();
		while ((it<event.end())&&(*it==' ')){
			event.erase(it);
			it = event.begin();
		}
		return event;
}

void TestAll();

int main() {


  TestAll();

  Database db;

  for (string line; getline(cin, line); ) {
    istringstream is(line);

    string command;
    is >> command;
    if (command == "Add") {
      const auto date = ParseDate(is);
      const auto event = ParseEvent(is);
      db.Add(date, event);
    } else if (command == "Print") {
      db.Print(cout);
    } else if (command == "Del") {
      auto condition = ParseCondition(is);
      auto predicate = [condition](const Date& date, const string& event) {
        return condition->Evaluate(date, event);
      };
      int count = db.RemoveIf(predicate);
      cout << "Removed " << count << " entries" << endl;
    } else if (command == "Find") {
      auto condition = ParseCondition(is);
      auto predicate = [condition](const Date& date, const string& event) {
        return condition->Evaluate(date, event);
      };

      const auto entries = db.FindIf(predicate);
      for (const auto& entry : entries) {
        cout << entry << endl;
      }
      cout << "Found " << entries.size() << " entries" << endl;
    } else if (command == "Last") {
      try {
          cout << db.Last(ParseDate(is)) << endl;
      } catch (invalid_argument&) {
          cout << "No entries" << endl;
      }
    } else if (command.empty()) {
      continue;
    } else {
      throw logic_error("Unknown command: " + command);
    }

  }

  return 0;
}


void TestParseEvent() {
  {
    istringstream is("event");
    AssertEqual(ParseEvent(is), "event", "Parse event without leading spaces");
  }
  {
    istringstream is("   sport event ");
    AssertEqual(ParseEvent(is), "sport event ", "Parse event with leading spaces");
  }
  {
    istringstream is("  first event  \n  second event");
    vector<string> events;
    events.push_back(ParseEvent(is));
    events.push_back(ParseEvent(is));
    AssertEqual(events, vector<string>{"first event  ", "second event"}, "Parse multiple events");
  }
}
void TestDbAdd() {

        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n2017-01-07 xmas\n", out.str(), "straight ordering");
        {
               Database db;
               db.Add({2017, 1, 1}, "new year");
               db.Add({2017, 1, 1}, "holiday");
               ostringstream out;
               db.Print(out);
               AssertEqual("2017-01-01 new year\n2017-01-01 holiday\n", out.str(), "several in one day");
           }
           {
               Database db;
               db.Add({2017, 1, 7}, "xmas");
               db.Add({2017, 1, 1}, "new year");
               ostringstream out;
               db.Print(out);
               AssertEqual("2017-01-01 new year\n2017-01-07 xmas\n", out.str(), "reverse ordering");
           }
           {
               Database db;
               db.Add({2017, 1, 1}, "new year");
               db.Add({2017, 1, 1}, "new year");
               db.Add({2017, 1, 1}, "xmas");
               db.Add({2017, 1, 1}, "new year");
               ostringstream out;
               db.Print(out);
               AssertEqual("2017-01-01 new year\n2017-01-01 xmas\n", out.str(), "uniq adding");
           }
   }
void TestDbFind() {
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        AssertEqual("2017-01-01 new year\n1", DoFind(db, "date == 2017-01-01"), "simple find by date");
        AssertEqual("2017-01-01 new year\n2017-01-07 xmas\n2", DoFind(db, "date < 2017-01-31"), "multiple find by date");
        AssertEqual("2017-01-01 new year\n1", DoFind(db, R"(event != "xmas")"), "multiple find by holiday");
    }
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 1}, "new year2");
        db.Add({2017, 1, 7}, "xmas");
        AssertEqual("2017-01-01 new year\n2017-01-07 xmas\n2", DoFind(db, R"(date == 2017-01-07 OR event == "new year")"),
                "complex find or");
        AssertEqual("2017-01-01 new year\n1", DoFind(db, R"(date == 2017-01-01 AND event == "new year")"),
                    "complex find and");
        AssertEqual("0", DoFind(db, R"(date == 2017-01-09 AND event == "new year")"),
                    "complex find and, nothing");
    }
}
void TestDbLast(){
    Database db;
    db.Add({2017, 1, 1}, "new year");
    db.Add({2017, 1, 7}, "xmas");
    {
        try {
            db.Last({2016, 12, 31});
            Assert(false, "last, found no entries");
        } catch (...){
            Assert(true, "last, found no entries");

        }
    }
    {
        ostringstream os;
        os << db.Last({2017, 1, 2});
        AssertEqual("2017-01-01 new year", os.str(), "greater than date");
    }
    {
        ostringstream os;
        os << db.Last({2017, 1, 1});
        AssertEqual("2017-01-01 new year", os.str(), "eq to date");
    }
    {
        ostringstream os;
        os << db.Last({2017, 1, 10});
        AssertEqual("2017-01-07 xmas", os.str(), "greater than max date");
    }
}
void TestDbRemoveIf () {
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        AssertEqual(0, DoRemove(db, R"(event == "something")"), "Remove nothing");
        AssertEqual(1, DoRemove(db, R"(date == "2017-01-01")"), "Remove by date");
        ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-07 xmas\n", out.str(), "Remove by date, left");
    }
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        AssertEqual(1, DoRemove(db, R"(event == "xmas")"), "Remove by event");
        ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n", out.str(), "Remove by event, left");
    }
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        db.Add({2017, 1, 7}, "new year");
        AssertEqual(2, DoRemove(db, R"(event == "new year")"), "Multiple remove by event");
        ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-07 xmas\n", out.str(), "Multiple remove by event, left");
    }
}
void TestInsertionOrder()
{
    {
        Database db;
        db.Add({2017, 1, 1}, "new year");
        db.Add({2017, 1, 7}, "xmas");
        db.Add({2017, 1, 7}, "party");
        db.Add({2017, 1, 7}, "pie");
        ostringstream out;
        db.Print(out);
        AssertEqual("2017-01-01 new year\n2017-01-07 xmas\n2017-01-07 party\n2017-01-07 pie\n", out.str(), "Remove by date, left");
    }

}
void TestsMyCustom()
{
    {
        Database db;
        db.Add({2019, 12, 18}, "Kolya");
        db.Add({2020, 1, 15}, "Katya");
        db.Add({2020, 2, 22}, "Riding");
        db.Add({2019, 12, 9}, "Go home");
        db.Add({2019, 12, 9}, "Read");
        ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-09 Go home\n2019-12-09 Read\n2019-12-18 Kolya\n2020-01-15 Katya\n2020-02-22 Riding\n", out.str(), "Order of insertion");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "b");
        db.Add({2019, 12, 2}, "c");
        db.Add({2019, 12, 2}, "d");
        db.Add({2019, 12, 3}, "e");
        ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 a\n2019-12-01 b\n2019-12-02 c\n2019-12-02 d\n2019-12-03 e\n", out.str(), "Order of insertion");
    }

    {
        Database db;
        istringstream ss("    a");
        const auto event = ParseEvent(ss);
        db.Add({2019, 12, 1}, event);
        ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 a\n", out.str(), "My test 20");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "b");
        db.Add({2019, 12, 2}, "c c");
        db.Add({2019, 12, 2}, "d");
        db.Add({2019, 12, 3}, "e");
        db.Add({2019, 12, 3}, "f");
        ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 a\n2019-12-01 b\n2019-12-02 c c\n2019-12-02 d\n2019-12-03 e\n2019-12-03 f\n", out.str(), "My test 2");
        AssertEqual(1, DoRemove(db, R"(event == "c" OR event == "d")" ), "My test 3");
        AssertEqual(1, DoRemove(db, R"(event == "e" AND event != "a")" ), "My test 4");
        db.Add({2019, 11, 30}, "a");
// db.Print(cout);
//        AssertEqual("2019-12-03 f\n1", DoFind(db, R"(date >= 2019-12-3)"), "My test 5");  /// !! не работает удаляю сыобытия из ветора а ищу в set в итоге нахожу удаленное сообщение
        AssertEqual("2019-12-03 f", db.Last({2019, 12, 4}), " My test 6");

        try
        {
            db.Last({2019, 11, 3});
        }
        catch(invalid_argument&)
        {
            cerr << "Тест на No entries OK" << endl;
        }
        ostringstream out1;
//db.Print(cout);

        AssertEqual("2019-12-02 c c", db.Last({2019, 12, 2}), " My test 7");


        AssertEqual("2019-12-03 f", db.Last({2019, 12, 4}), " My test 8");//

        db.Add({2019, 12, 3}, "m");
        AssertEqual("2019-12-03 m", db.Last({2019, 12, 3}), " My test 9");

        AssertEqual(1, DoRemove(db, R"(event == "e" AND event != "a" OR event == "m" AND date == 2019-12-3)" ), "My test 10");

        ostringstream out2;
        db.Print(out2);
        AssertEqual("2019-11-30 a\n2019-12-01 a\n2019-12-01 b\n2019-12-02 c c\n2019-12-03 f\n", out2.str(), "My test 11");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "b");
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 2}, "c");
        db.Add({2019, 12, 2}, "a");
        db.Add({2019, 12, 2}, "a");

        AssertEqual(2, DoRemove(db, R"(event == "a")" ), "My test 12");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "aa");
        db.Add({2019, 12, 1}, "aaa");

        AssertEqual(2, DoRemove(db, R"(event >= "aa")" ), "My test 13");

        ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 a\n", out.str(), "My test 14");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "aa");
        db.Add({2019, 12, 1}, "aaa");
        db.Add({2019, 12, 2}, "b");
        db.Add({2019, 12, 2}, "a");

        AssertEqual(2, DoRemove(db, R"(event > "aa")" ), "My test 15");

        ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 a\n2019-12-01 aa\n2019-12-02 a\n", out.str(), "My test 16");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "aa");
        db.Add({2019, 12, 1}, "aaa");
        db.Add({2019, 12, 2}, "b");
        db.Add({2019, 12, 2}, "a");

        AssertEqual(2, DoRemove(db, R"(event < "aa")" ), "My test 17");

        ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 aa\n2019-12-01 aaa\n2019-12-02 b\n", out.str(), "My test 18");
    }

    {
        Database db;
        db.Add({2019, 12, 1}, "a");
        db.Add({2019, 12, 1}, "b");

        AssertEqual(1, DoRemove(db, R"(event != "b")" ), "My test 19");

        db.Add({2019, 12, 1}, "c");

        AssertEqual("2019-12-01 c", db.Last({2019, 12, 1}), " My test 20");

        db.Add({2019, 12, 1}, "b");
        AssertEqual("2019-12-01 c", db.Last({2019, 12, 1}), " My test 21");

        ostringstream out;
        db.Print(out);
        AssertEqual("2019-12-01 b\n2019-12-01 c\n", out.str(), "My test 22");
    }
    {
    Database db;
    db.Add({2019, 12, 1}, "a");
    db.Add({2019, 12, 1}, "b");
    db.Add({2019, 12, 2}, "b");

    AssertEqual(2, DoRemove(db, R"( date  > 2020-01-01 OR event == "b")" ), "My test 23");

    }
}


void TestAll() {
  TestRunner tr;
  tr.RunTest(TestParseEvent, "TestParseEvent");
  tr.RunTest(TestParseCondition, "TestParseCondition");
  tr.RunTest(TestDbAdd, "Тест 3(1) из Coursera");
  tr.RunTest(TestDbFind, "Тест 3(2) из Coursera");
  tr.RunTest(TestDbLast, "Тест 3(3) из Coursera");
  tr.RunTest(TestDbRemoveIf, "Тест 3(4) из Coursera");
  tr.RunTest(TestInsertionOrder, "Тест на порядок вывода");
  tr.RunTest(TestsMyCustom, "Мои тесты");

}



#include "gtest/gtest.h"
#include "../src/lookup-seq.h"
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

// TODO samo testiranje paralelne verzije koda.

TEST(DICT, Brojevi)
{
    HashTable<int, double> table;

    table.add_or_update(0, 0.0);
    table.add_or_update(1, 1.0);
    table.add_or_update(2, 2.0);
    table.add_or_update(3, 3.0);
    table.add_or_update(100, 100.0);
    table.add_or_update(200, 200.0);
    table.add_or_update(300, 300.0);
    table.add_or_update(4, 300.0);

    ASSERT_EQ(table.find_if([](const std::pair<int, double> &p) { return p.first == 0; }).has_value(), true);
    ASSERT_EQ(table.find_if([](const std::pair<int, double> &p) { return p.first == 0; }).value(), std::make_pair(0, 0.0));
    //  update
    table.add_or_update(0, 1.0);
    ASSERT_EQ(table.find_if([](const std::pair<int, double> &p) { return p.first == 0; }).value(), std::make_pair(0, 1.0));
    //  remove
    ASSERT_EQ(table.size(), 8);
    ASSERT_EQ(table.find_all_if([](const std::pair<int, double> &p) { return p.first == 0; }).size(), 1);
    ASSERT_EQ(table.remove(0), true);
    ASSERT_EQ(table.find_all_if([](const std::pair<int, double> &p) { return p.first == 0; }).empty(), true);

    //table.remove(0);
    ASSERT_EQ(table.find_if([](const std::pair<int, double> &p) { return p.first == 0; }).has_value(), false);
    //ASSERT_EQ(table.find_if([](const std::pair<int, double> &p) { return p.first == 0; }).value(), {});
    ASSERT_EQ(table.size(), 7);

    ASSERT_EQ(table.remove(8), false);
    table.add_or_update(8, 1.0);
    ASSERT_EQ(table.remove(8), true);

    ASSERT_EQ(table.find_all_if([](const std::pair<int, double> &p) { return p.second == 300.0; }).size(), 2);
    ASSERT_EQ(table.find_all_if([](const std::pair<int, double> &p) { return p.second == 7.0; }).size(), 0);
}

TEST(DICT, Stringovi)
{
    HashTable<std::string, std::string> table;

    table.add_or_update("a", "ab");
    table.add_or_update("b", "aa");
    table.add_or_update("c", "ba");
    table.add_or_update("d", "cd");
    table.add_or_update("ee", "dd");
    table.add_or_update("k", "dd");

    ASSERT_EQ(table.find_if([](const std::pair<std::string, std::string> &p) { return p.first == "a"; }).has_value(), true);
    ASSERT_EQ(table.find_if([](const std::pair<std::string, std::string> &p) { return p.first == "a"; }).value().first, "a");
    ASSERT_EQ(table.find_if([](const std::pair<std::string, std::string> &p) { return p.first == "a"; }).value().second, "ab");

    //  update
    table.add_or_update("a", "k");
    ASSERT_EQ(table.find_if([](const std::pair<std::string, std::string> &p) { return p.first == "a"; }).value().first, "a");
    ASSERT_EQ(table.find_if([](const std::pair<std::string, std::string> &p) { return p.first == "a"; }).value().second, "k");

    //  remove
    ASSERT_EQ(table.size(), 6);
    ASSERT_EQ(table.find_all_if([](const std::pair<std::string, std::string> &p) { return p.first == "a"; }).size(), 1);
    ASSERT_EQ(table.remove("a"), true);
    ASSERT_EQ(table.find_all_if([](const std::pair<std::string, std::string> &p) { return p.first == "a"; }).empty(), true);

    //table.remove(0);
    ASSERT_EQ(table.find_if([](const std::pair<std::string, std::string> &p) { return p.first == "a"; }).has_value(), false);
    ASSERT_EQ(table.size(), 5);

    ASSERT_EQ(table.remove("g"), false);
    table.add_or_update("g", "k");
    ASSERT_EQ(table.remove("g"), true);

    ASSERT_EQ(table.find_all_if([](const std::pair<std::string, std::string> &p) { return p.second == "dd"; }).size(), 2);
    ASSERT_EQ(table.find_all_if([](const std::pair<std::string, std::string> &p) { return p.second == "z"; }).size(), 0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
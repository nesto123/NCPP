/* 
 *             Testiranje sekvencijalne verzije rječnika  
 *                  na bazi hash tablice
 *                                                           @FV
 */

#include "lookup-seq.h"

#include <string>
#include <iostream>

int pred(const std::pair<int, double> &p)
{
    return p.first >= 8;
}
// TODO samo testiranje sekvencijalne verzije koda.
int main()
{

    std::pair<int, double> p(1, 1.0);
    std::hash<int> hash;
    HashTable<int, double> table;
    table.add_or_update(0, 0.0);

    table.add_or_update(1, 1.2);

    //std::cout<<table.remove(0)<<std::endl;
    table.print(std::cout);

    if (table.find_if([](const std::pair<int, double> &p) { return p.first == 1; }).has_value())
        std::cout << table.find_if([](const std::pair<int, double> &p) { return p.first == 1; }).value().second << std::endl;
    std::cout << table.find_all_if([](const std::pair<int, double> &p) { return p.first >= 0; }).size();

    if (table.value(1).has_value())
        std::cout << "ima jedinicu" << table.value(1).value() << std::endl;
    table.print(std::cout);

    return 0;
}

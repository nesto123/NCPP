/* 
 *             Testiranje paralene verzije rječnika  
 *                  na bazi hash tablice
 *                                                           @FV
 */

#include "ts-lookup.h"

#include <iostream>
#include <thread>

std::hash<int> hash;
HashTable<int, int> table;


void f1()
{
    table.add_or_update(0, 0);
    table.add_or_update(1, 1);
    table.add_or_update(2, 2);
    table.add_or_update(3, 3);
    table.add_or_update(5, 4);
    table.print(std::cout);
    std::cout << "veličina rječnika: " << table.size() << std::endl;
    if (table.find_if([](const std::pair<int, int> &p) { return p.first == 6; }).has_value())
        std::cout <<"postoji element s ključem 6 i vrijednost mu je " << 
            table.find_if([](const std::pair<int, int> &p) { return p.first == 6; }).value().second
            << std::endl;
    table.remove(8);
    table.print(std::cout);
    std::cout << "broj elemenata kojima je vrijednost jednaka 5: " 
        << table.find_all_if([](const std::pair<int, int> &p) { return p.second == 5; }).size()
        << std::endl;
    table.remove(7);
    std::cout << "veličina rječnika: " << table.size() << std::endl;


}

void f2()
{
    table.add_or_update(5, 5);
    table.add_or_update(6, 5);
    table.add_or_update(7, 5);
    table.add_or_update(0, 3);
    table.add_or_update(8, 8);
    table.print(std::cout);
    std::cout << "veličina rječnika: " << table.size() << std::endl;
    if (table.find_if([](const std::pair<int, int> &p) { return p.first == 0; }).has_value())
        std::cout <<"postoji element s ključem 0 i vrijednost mu je " << 
            table.find_if([](const std::pair<int, int> &p) { return p.first == 0; }).value().second
            << std::endl;
    table.remove(2);
    table.print(std::cout);
    if ( table.find_if([](const std::pair<int, int> &p) { return p.first == 7; }).has_value() )
        std::cout <<"postoji element s ključem 7 i vrijednost mu je " << 
            table.find_if([](const std::pair<int, int> &p) { return p.first == 7; }).value().second
            << std::endl;
    std::cout << "broj elemenata kojima je vrijednost jednaka 5: " 
        << table.find_all_if([](const std::pair<int, int> &p) { return p.second == 5; }).size()
        << std::endl;
}
/*
int pred(const std::pair<int, int> &p)
{
    return p.first >= 8;
}
*/

// TODO samo testiranje paralelne verzije koda.
int main()
{

   
    std::thread t1(f1);
    std::thread t2(f2);


    t1.join();
    t2.join();
   



    return 0;
}
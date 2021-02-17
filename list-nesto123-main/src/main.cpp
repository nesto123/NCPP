/* 
 *                      std::list 
 *              Paralelna verzija - testovi
 *                                                           @nesto123
 */

#include "tslist.h"
#include <thread>
#include <iostream>
#include <atomic>

//  Makro za veličinu spremnika kod testiranja - može se podesiti
//  na proizvojan broj veći od 100, da bi program imao smisla.
//  Izračnavanje za veće brojeve traje malo duže, predlažem da se za N stavi
//  2000,4000, ... do maksimalno 10 000, inaće izračunavanje traje dosta dugo.
#define N 1000

List<int> lista;
std::atomic<bool> stop{false}; //flag kada je zaustavljeno ubacivanje

void printList()
{
    std::cout << "\tIspis liste:" << std::endl;
    lista.for_each([](int &p) { std::cout << p << ", "; });
    std::cout << std::endl;
}

// punimo listu sa brojevima do N jednom dretvom, paralelno worker briše elemente dok se puni lista
void generator()
{
    int n = 1;
    while (n <= N)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        if (n % 2)
            lista.push_front(n++);
        else
            lista.push_back(n++);
    }
    stop.store(true);
}

//  dretve brišu vrijednosti iz liste na sljedeći način
//  Svaka dretva briše elemente koji su djeljivi sa in a nisu djeljivi sa 100.
//  Također nakon toga gledamo jesmo li gotovi, a gotovi smo kada smo sve elemente izbacili po
//  prvom i drugom pravilu. Druo pravilo je izbacivanje elemenata koji su djeljivi sa N
//
void workerFill(int in)
{
    while (true)
    {
        // miče sve elemente djeljive sa in koji nisu djeljivi sa 100
        lista.remove_if([in](int const &p) { return (p % in == 0) && (p % 100 != 0); });

        // gledamo ima li još elemenata koji su djeljivi sa 2 ili 3
        if (lista.find_first_if([](int const &p) { return (p % 2 == 0 || p % 3 == 0) && (p % 100 != 0); }) == nullptr)
            if (lista.find_first(N) == nullptr) // provjeravamo je li još N u listi
                if (stop.load())                // ako je završilo ubacivanje i izacili smo sve što smo trebali izlazimo
                    return;

        //  Brišemo elemente koji su djeljivi sa 100 pomoću remove-a
        for (int n = 100; n <= N; n += 100)
            lista.remove(n);
    }
}

// TODO testiranje paralelne verzije koda.

int main()
{
    ///  Paralelno testiranje containera
    std::cout << "-------------\tParalelno testiranje containera-------------" << std::endl;

    std::thread t1(generator);

    std::thread t2(workerFill, 2);
    std::thread t3(workerFill, 3);

    t1.join();
    t2.join();
    t3.join();

    printList();

    ///  Sekvencionalno testiranje containera
    std::cout << "-------------\tSekvencijalno testiranje containera-------------" << std::endl;
    // Prvo pobrišemo listu odprije
    lista.remove_if([](int const &p) { return true; });

    lista.push_back(-1);
    lista.push_front(5);

    for (size_t i = 1; i < 10; i++)
        lista.push_front((int)i);

    lista.push_back(-2);

    printList();

    std::cout << "Prvi parni:" << *lista.find_first_if([](int const &p) { return p % 2 == 0; }) << std::endl;

    std::cout << "Nađi prvu pojavu 5:" << *lista.find_first(5) << std::endl;

    std::cout << "Broj maknutih parnih brojeva: " 
                << lista.remove_if([](int const &p) { return p % 2 == 0; }) << std::endl;
    std::cout << "Broj maknutih instanci broja 5: " << lista.remove(5) << std::endl;

    printList();

    return 0;
}
/* 
 *           Usporedba std::accumulate i parallel_accumulate
 *           te        std::transform i parallel_transform
 *                                                           @FV
 */

#include "clock.h"
#include "paraccumulate.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <iomanip>

// Glavni program koji testira paralelnu verziju accumulate() algoritma.
// Potrebno je na par primjera usporediti serijsku i paralelnu verziju
// algoritma i usporediti njihova vremena izvršavanja kao funkciju veličine
// spremnika na kojem rade,

//        Printing vector function
template <typename T>
void printVector(std::vector<T> vect)
{
    for (auto x : vect)
        std::cout << x << " ";
    std::cout << std::endl
              << "Size=" << vect.size() << std::endl;
}

//        Random number generator function
template <class T>
T generateRandomNumber(T startRange, T endRange)
{
    return startRange + (double)(rand()) / (RAND_MAX) * (endRange - startRange);
}

//        Vector resize & random fill function
template <class T>
void VecResizeFill(std::vector<T> &vec, T min_value, T max_value, unsigned size = 20)
{
    vec.resize(size);
    std::generate(vec.begin(), vec.end(), [min_value, max_value]() mutable { return generateRandomNumber(min_value, max_value); });
}
int increment2(int element)
{
    return ++element;
}
//        Vector compare function for accumulate
template <class T>
void Compare(std::vector<T> &vec, bool alg = false)
{
    Clock time;
    T sum;
    double time_para, time_seq;

    if (!alg)
    {
        time.start();
        sum = std::accumulate(vec.begin(), vec.end(), 0);
        time_seq = time.stop(Clock::millisec);

        time.start();
        sum = parallel_accumulate(vec.begin(), vec.end(), 0);
        time_para = time.stop(Clock::millisec);
    }
    else
    {
        std::vector<T> vecOut;

        vecOut.resize(vec.size());

        time.start();
        std::transform(vec.begin(), vec.end(), vecOut.begin(), increment2);

        time_seq = time.stop(Clock::millisec);

        time.start();
        parallel_transform(vec.begin(), vec.end(), vecOut.begin(), increment2);
        time_para = time.stop(Clock::millisec);
    }

    std::cout << "n=" << std::setw(5) << vec.size()
              << " Seq=" << std::setw(9) << time_seq
              << "ms\t Para=" << std::setw(9) << time_para
              << "ms Difference= " << std::setw(9) << std::abs(time_para - time_seq) << "ms";

    if (time_para < time_seq)
        std::cout << " Better one: "
                  << "para" << std::endl;
    else
        std::cout << " Better one: "
                  << "seq" << std::endl;
    //std::cout<< time.stop(Clock::millisec) << "ms Sum=" << sum << std::endl;
}

int main()
{

    std::vector<double> vec;
    std::vector<int> vec2, vecOut;

    srand(time(NULL));

    std::cout << "COMPARING accumulate" << std::endl;
    VecResizeFill(vec, 0.0, 100.0, 200);
    Compare(vec);
    VecResizeFill(vec2, 0, 100, 200);
    Compare(vec);
    VecResizeFill(vec2, 0, 100, 50000);
    Compare(vec2);

    std::cout << "---------------------------\tCOMPARE TO n = 1000000\t---------------------------" << std::endl;

    for (size_t i = 50; i <= 5000; i += 500)
    {
        VecResizeFill(vec, 0.0, 100.0, i);
        Compare(vec);
    }
    for (size_t i = 50; i <= 1000000; i += 50000)
    {
        VecResizeFill(vec, 0.0, 100.0, i);
        Compare(vec);
    }

    std::cout << "COMPARING transform" << std::endl;

    //      Testiranje      parallel transforma     i    std::transforma
    VecResizeFill(vec2, 0, 100);
    Compare(vec2, true);

    VecResizeFill(vec2, 0, 500);
    Compare(vec2, true);

    VecResizeFill(vec2, 0, 1000);
    Compare(vec2, true);

    std::cout << "---------------------------\tCOMPARE TO n = 1000000\t---------------------------" << std::endl;

    for (size_t i = 50; i <= 5000; i += 500)
    {
        VecResizeFill(vec2, 0, 100, i);
        Compare(vec2, true);
    }
    for (size_t i = 50; i <= 1000000; i += 50000)
    {
        VecResizeFill(vec2, 0, 100, i);
        Compare(vec2, true);
    }

    return 0;
}

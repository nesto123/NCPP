/* Kreirati paralelni min_element() algoritam koristeći programske niti. 
 * Vidi https://en.cppreference.com/w/cpp/algorithm/min_element
 */

/* 
 *           Imlementacija par_min_element 
 *                   
 *                                                           @FV
 */

#include <vector>
#include <thread>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <shared_mutex>

template <typename ForwardIt, typename Compare>
void block_min(ForwardIt first, ForwardIt last, ForwardIt &min, Compare comp)
{
    min = std::min_element(first, last, comp);
}

template <typename ForwardIt, typename Compare>
ForwardIt par_min_element(ForwardIt first, ForwardIt last, Compare comp)
{
    auto length = std::distance(first, last);
    if (!length)
        return first;

    int phard = std::thread::hardware_concurrency();
    if (!phard)
        phard = 2;
    int n = 1000;
    int pmax = (length > n) ? length / n : 1;
    int p = std::min(pmax, phard);

    std::vector<ForwardIt> results(p);       // mjesto za rezultat izračunavanja
    std::vector<std::thread> threads(p - 1); // paralelne programske niti

    auto block_size = length / p;
    auto block_first = first;
    auto block_last = block_first;
    for (int i = 0; i < p - 1; ++i)
    {
        std::advance(block_last, block_size);
        results[i] = block_first; //     dodano
        threads[i] = std::thread(block_min<ForwardIt, Compare>,
                                 block_first, block_last, std::ref(results[i]), comp);
        block_first = block_last;
    }
    // Ostatak sumiramo u glavnoj programskoj niti.
    results[p - 1] = std::min_element(block_first, last, comp);

    for (int i = 0; i < p - 1; ++i)
        threads[i].join();
    auto min = results[0];
    for (auto temp : results)
        if (comp(*temp, *min))
            min = temp;
    //     std::cout<<*temp;
    // Kad sve dretve završe glavna će obaviti konačno sumiranje.
    return min;
}

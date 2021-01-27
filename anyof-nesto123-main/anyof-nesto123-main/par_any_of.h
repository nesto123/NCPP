/* Paralelni any_of() algoritam baziran na promise
   mehanizmu. 
*/
/* 
 *           Imlementacija parany_of 
 *                   
 *                                                           @FV
 */

#include <iterator>
#include <thread>
#include <algorithm>
#include <future>
#include <atomic>
#include <vector>
#include <exception>

#include <iostream>
#include <chrono>

std::atomic<bool> atom_flag = false; //    za komunikaciju između dretvi -- ako jedna dretva nađe element, ostale stanu

template <typename InputIt, typename UnaryPredicate>
void block_any_of(InputIt first, InputIt last, std::promise<bool> &&promise, UnaryPredicate p)
{
   for (; first != last  && !atom_flag; ++first)
   {
      if (p(*first) && !atom_flag)
      {
         atom_flag = true;
         try
         {
            promise.set_value(true);
         }
         catch (const std::exception &e)  // kada se dvije dretve probaju postaviti vrijednost
         {
            //promise.set_exception(std::current_exception());
         }
         return;
      }
   }
}

template <typename InputIt, typename UnaryPredicate>
bool parany_of(InputIt first, InputIt last, UnaryPredicate p)
{
   auto length = std::distance(first, last);
   if (!length)
      return false;

   int phard = std::thread::hardware_concurrency();
   if (!phard)
      phard = 2;
   int n = 10;
   int pmax = (length > n) ? length / n : 1;
   int p_ = std::min(pmax, phard);

   std::vector<std::thread> threads(p_ - 1); // paralelne programske niti

   std::promise<bool> promise;
   std::future<bool> future = promise.get_future();
   atom_flag = false; //    za komunikaciju između dretvi -- ako jedna dretva nađe element, ostale stanu

   auto block_size = length / p_;
   auto block_first = first;
   auto block_last = block_first;
   for (int i = 0; i < p_ - 1; ++i)
   {
      std::advance(block_last, block_size);
      threads[i] = std::thread(block_any_of<InputIt, UnaryPredicate>,
                               block_first, block_last, std::move(promise), p);
      // GREŠKA, NAKON PRVOG STD::MOVE() PROMISE OSTAJE "PRAZAN", NEPOVEZAN SA DIJELJENOM VARIJABLOM (-10)
      block_first = block_last;
   }
   
   // Ostatak sumiramo u glavnoj programskoj niti.
   block_any_of(block_first, last, std::move(promise), p);


   for (int i = 0; i < p_ - 1; ++i)
      threads[i].join();

   bool result = false;
   if ( atom_flag == true )// future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)   // provjera jeli postavljen objekt
   {
      try
      {
         result = future.get();
      }
      catch (...)// nije postavljen future.get() pa nema elemenata
      {
         result = true;
      }
   }

   return (result); //atom_flag;   IZUZETAK MOR PROPAGIRATI U POZIVNI PROGRAM, INAČE JE OK
}

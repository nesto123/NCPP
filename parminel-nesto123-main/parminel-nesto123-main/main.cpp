/* 
 *           Testiranje RandomInt, par_min_element, Clock
 *                   
 *                                                           @FV
 */

#include "minelement.h"
#include "random_int.h"
#include "clock.h"
#include <algorithm>
#include <iterator>
#include <iomanip>



//  Comparing operator
bool less(const int &a, const int &b)
{
  return (a < b);
}

//  Comparing function
template <typename T, typename Compare>
void CompareFunction(std::vector<T> &vec, Compare comp)
{
  Clock time;
  double time_para, time_seq;

  time.start();
  std::min_element(vec.begin(), vec.end(), comp);
  time_seq = time.stop(Clock::millisec);

  time.start();
  par_min_element(vec.begin(), vec.end(), comp);
  time_para = time.stop(Clock::millisec);

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
}

int main(int argc, char *argv[])
{

  //  -- test
  int N = 10000;
  if (argc > 1)
    N = std::atoi(argv[1]);

  RandomInt ri(37, 3000); // na primjer
  RandomInt ri2(ri);
  std::vector<int> source(N), s2(N); // testirati na tom vektoru
  std::generate(std::begin(source), std::end(source), ri);
  std::generate(std::begin(s2), std::end(s2), ri2);

  // ...

  RandomInt ir(1, 100); // konstrukcija
  std::vector<int> AA(5);
  std::generate(AA.begin(), AA.end(), ir);                                     // kopiranje u algoritam generate()
  std::copy(AA.begin(), AA.end(), std::ostream_iterator<int>(std::cout, ",")); // ispis
  std::cout << "\n";
  std::generate(AA.begin(), AA.end(), ir);
  std::copy(AA.begin(), AA.end(), std::ostream_iterator<int>(std::cout, ","));
  std::cout << "\n";
  std::generate(AA.begin(), AA.end(), ir);
  std::copy(AA.begin(), AA.end(), std::ostream_iterator<int>(std::cout, ","));
  std::cout << "\n";

  std::cout << "Serial min at: " << std::distance(AA.begin(), std::min_element(AA.begin(), AA.end()))
            << "Parallel min at: " << std::distance(AA.begin(), par_min_element(AA.begin(), AA.end(), less))
            << std::endl;

  for (int i = 0; i < 10; ++i)
    std::cout << ir() << ((i == 9) ? "\n" : ",");

  RandomInt ir2(1, 1000); // konstrukcija
  std::vector<int> vec;
  for (size_t i = 50; i <= 100000; i += 1000)
  {
    vec.resize(i);
    std::generate(vec.begin(), vec.end(), ir2);
    CompareFunction(vec, less);
  }

  return 0;
}

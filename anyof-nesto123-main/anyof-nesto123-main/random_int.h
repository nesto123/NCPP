/* 
 *           Imlementacija RandomInt za slučanih brojeva
 *                   
 *                                                           @FV
 */

#include <random>

// Klasa koja predstavlja generator slučajnih brojeva tipa int uniformno distribuiranih
// u zadanom rasponu.  Sučelje klase je dano ovdje:
class RandomInt
{
public:
  //std::random_device device;
  std::mt19937 generator;
  std::uniform_int_distribution<int> distribution;

  RandomInt(int a, int b);
  RandomInt(RandomInt const &rhs);
  RandomInt &operator=(RandomInt const &rhs) = delete;
  int operator()();
};
/*
- a, b su granice distribuiranih int-ova;
- konstruktor kopije (i konstruktor) mora postaviti novi _seed_ kako kopija ne bi 
  generirala isti niz kao i original;
- operator() generira novu pseudoslučajnu vrijednost.
*/

RandomInt::RandomInt(int a, int b) : generator{std::random_device{}()}, distribution(a, b)
{}

RandomInt::RandomInt(RandomInt const &rhs) : generator{std::random_device{}()}, distribution(rhs.distribution)
{}

int RandomInt::operator()()
{
  return distribution(generator);
}

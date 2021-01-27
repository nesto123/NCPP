/* 
 *           Testing std::any_of()
 *                   
 *                                                           @FV
 */

#include "par_any_of.h"
#include <vector>




bool negative(const int &a)
{
    return a<0;
}

// IZUZETAK NIJE ISPITAN !!!
int main(){
    // Ovdje u tri primjera treba pokazati korektnost paralelnog algoritma usporedbom
    // sa std::any_of() algoritmom.  U trećem primjeru demonstrirati 
    // propagiranje izuzetka.

    std::vector<int> vec{1,2,1,1,-1,1,1,1,1,1,1,-1,1,1,1,1,1,1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                    vec2{-1},
                    vec3{1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};// u ovom slučaju se neće postaviti future objekt

     std::cout<<"Sekvencijalni: "<<std::any_of(vec.begin(), vec.end(), negative)<<" "
            <<"Paralelni: "<<parany_of(vec.begin(), vec.end(), negative)<<" "
             <<std::endl;

     std::cout<<"Sekvencijalni: "<<std::any_of(vec2.begin(), vec2.end(), negative)<<" "
            <<"Paralelni: "<<parany_of(vec2.begin(), vec2.end(), negative)<<" "
             <<std::endl;
      std::cout<<"Sekvencijalni: "<<std::any_of(vec3.begin(), vec3.end(), negative)<<" "
            <<"Paralelni: "<<parany_of(vec3.begin(), vec3.end(), negative)<<" "
             <<std::endl;

    return 0;
}

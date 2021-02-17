**Zadatak**.
Treba implementirati jednostruko povezanu list sigurnu za 
pristup iz različitih programskih niti. Zaključavanje se izvodi na
razini svakog pojedinog čvora tako da se maksimizira mogućnost 
paralelizma. 

U implementaciji ne kreiramo iteratore budući da to nije jednostavno. 
Naime, iteratori drže referencu na listu koja se u višenitnom 
programu može promijeniti neovisno o samim iteratorima i na taj način 
ih obezvrijediti. Stoga je implementacija iteratora složena. 
Umjesto iteratora implementirat ćemo `for_each()` metodu koja će biti 
dio liste. Ona će pozivati korisničku funkciju što predstavlja 
rizik, no pretpostavljamo da se korisnik ponaša razumno.

Najzanimljivije je kretanje kroz listu pri čemu važnu ulogu ima 
premještanje mutexa.

U svakom čvoru liste podatak držimo kao `shared_ptr<>` tako da možemo 
referirati na podatak u listi izvan liste (ako još postoji). 

Potrebno je implementirati sve metode u `tslist.h` datoteci i zatim
napisati `main` program koji će pokušati testirati sve paralelne 
aspekte liste. 

Kod detaljno komentirati. 

Literatura: Williams ....

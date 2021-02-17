/* 
 *                      std::list 
 *                  Paralelna verzija
 *                                                           @nesto123
 */

#pragma once

#include <memory>
#include <mutex>

template <typename T>
class List
{
    struct Node
    {
        std::mutex m;
        std::shared_ptr<T> data;
        std::unique_ptr<Node> next;
        Node() : next() {}
        Node(T const &value) : data(std::make_shared<T>(value)), next() {}
    };

    Node head;

public:
    List() {}
    ~List();
    List(List const &) = delete;
    List &operator=(List const &) = delete;

    void push_front(T const &value);

    void push_back(T const &value);

    template <typename Func>
    void for_each(Func f);

    // Vraća nullptr kada ne nađe element.
    std::shared_ptr<T> find_first(T const &t);

    // Vraća nullptr kada ne nađe element.
    template <typename Predicate>
    std::shared_ptr<T> find_first_if(Predicate p);

    // Ukloni sve elemente jednake value i vrati broj uklonjenih elemenata.
    int remove(T value);

    // Ukloni sve elemente koji zadovoljavaju i vrati broj uklonjenih elemenata.
    template <typename Predicate>
    int remove_if(Predicate p);
};

////////////////////////////////////////////////////////////////////////////
//*                 IMPLEMENTATION

template <typename T>
void List<T>::push_front(T const &value)
{
    // stvorimo novi čvor
    std::unique_ptr<Node> novi(new Node(value));

    // kritični odsječak -- mjenjamo head čvor
    std::lock_guard<std::mutex> lokot(head.m);

    // potavljamo next novog čvora na next head-a(pošto je on prazan)  te dodjeljujemo data novom čvoru
    novi->next = std::move(head.next);
    head.next = std::move(novi);
}

template <typename T>
void List<T>::push_back(T const &value)
{
    // stvorimo novi čvor
    std::unique_ptr<Node> novi(new Node(value));

    // krećemo od head-a
    Node *element = &head;

    // prolazimo po listi pa zaključamo trenutni element te ga otkuljučamo kad idemo dalje
    std::unique_lock<std::mutex> lokot(head.m);

    if (!element->next)
    { // na headu smo pa tu ubacujemo -- samo ako je lista prazna
        element->next = std::move(novi);
        lokot.unlock();
        return;
    }

    while (Node *const sljedeci = element->next.get())
    {
        // zaključamo pristup sljedećem elemntu
        std::unique_lock<std::mutex> lokot2(sljedeci->m);
        // sada otključamo prethodni lokot jer nam više ne treba
        lokot.unlock();
        
        if (!sljedeci->next) // na zadnjem smo
        {
            sljedeci->next = std::move(novi);
            lokot2.unlock();
            break;
        }
        //prebacimo sljedeći u trenutni
        element = sljedeci;
        lokot = std::move(lokot2);
    }
}

template <typename T>
template <typename Func>
void List<T>::for_each(Func f)
{
    // krećemo od head-a
    Node *element = &head;

    // prolazimo po listi pa zaključamo trenutni element te ga otkuljučamo kad idemo dalje
    std::unique_lock<std::mutex> lokot(head.m);
    while (Node *const sljedeci = element->next.get())
    {
        // zaključamo pristup sljedećem elemntu
        std::unique_lock<std::mutex> lokot2(sljedeci->m);
        // sada otključamo prethodni lokot jer nam više ne treba
        lokot.unlock();
        // izvrši se uvjet
        f(*sljedeci->data);
        //prebacimo sljedeći u trenutni
        element = sljedeci;
        lokot = std::move(lokot2);
    }
}

// Vraća nullptr kada ne nađe element.
template <typename T>
std::shared_ptr<T> List<T>::find_first(T const &t)
{
    return find_first_if([t](T const &p) { return p == t; });
}

// Vraća nullptr kada ne nađe element.
template <typename T>
template <typename Predicate>
std::shared_ptr<T> List<T>::find_first_if(Predicate p)
{
    // krećemo od head-a
    Node *element = &head;

    // prolazimo po listi pa zaključamo trenutni element te ga otkuljučamo kad idemo dalje
    std::unique_lock<std::mutex> lokot(head.m);
    while (Node *const sljedeci = element->next.get())
    {
        // zaključamo pristup sljedećem elemntu
        std::unique_lock<std::mutex> lokot2(sljedeci->m);
        // sada otključamo prethodni lokot jer nam više ne treba
        lokot.unlock();
        // vraćamo ako je uvijet zadovoljen
        if (p(*sljedeci->data))
            return sljedeci->data;
        //prebacimo sljedeći u trenutni
        element = sljedeci;
        lokot = std::move(lokot2);
    }
    return nullptr;
}

// Ukloni sve elemente jednake value i vrati broj uklonjenih elemenata.
template <typename T>
int List<T>::remove(T value)
{
    return remove_if([value](T const &p) { return p == value; });
}

// Ukloni sve elemente koji zadovoljavaju i vrati broj uklonjenih elemenata.
template <typename T>
template <typename Predicate>
int List<T>::remove_if(Predicate p)
{
    size_t broj_uklonjenih = 0;

    // krećemo od head-a
    Node *element = &head;

    // prolazimo po listi pa zaključamo trenutni element te ga otkuljučamo kad idemo dalje
    std::unique_lock<std::mutex> lokot(head.m);
    while (Node *const sljedeci = element->next.get())
    {
        // zaključamo pristup sljedećem elemntu
        std::unique_lock<std::mutex> lokot2(sljedeci->m);

        if (p(*sljedeci->data)) // ako je zadovoljen predikat brišemo element
        {
            // spremamo pokazivac na sljedeci element koji ne brisemo da ga kasnije stavimo na element prije brisanog
            // -- stari se sam obriše, te otkljucamo lokot
            std::unique_ptr<Node> sljedci_stari = std::move(element->next);
            element->next = std::move(sljedeci->next);
            lokot2.unlock();
            broj_uklonjenih++;
        }
        else
        { // nije taj element pa idemo dalje -- otkljucamo trenutni i postaimo se za sljedeci element
            lokot.unlock();
            element = sljedeci;
            lokot = std::move(lokot2);
        }
    }
    return broj_uklonjenih;
}

template <typename T>
List<T>::~List()
{
    remove_if([](T const &p) { return true; });
}

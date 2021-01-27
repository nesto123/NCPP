/* 
 *              Rječnik na bazi hash tablice 
 *                  Sekvencijalna verzija
 *                                                           @FV
 */
#pragma once

#include <vector>
#include <list>
#include <ostream>
#include <optional>
#include <algorithm>
#include <string>

// Rječnik na bazi hash tabele
template <typename Key, typename Value, typename Hash = std::hash<Key>>
class HashTable
{
private:
    class Bucket
    {
    public:
        using BucketValue = std::pair<Key, Value>;

        // TODO ...
        std::optional<Value> find(const Key &key) const;

        void add_value(const Key &key, const Value &value);

        bool remove(const Key &key);

        std::string print() const;

        template <typename Predicat>
        std::optional<BucketValue> find_if(Predicat p) const;

        template <typename Predicat>
        std::list<BucketValue> find_all_if(Predicat p) const;

        // možda ne treba
        size_t size() const;

    private:
        std::list<BucketValue> mData;
    };
    // Podaci.
    std::vector<Bucket> mBuckets;
    using BucketValue = typename Bucket::BucketValue;
    Hash mHasher;

public:
    HashTable(size_t N = 101, Hash hasher = Hash()) : mBuckets(N), mHasher(hasher)
    {
    }
    HashTable(HashTable const &) = delete;
    HashTable &operator=(HashTable const &) = delete;

    //  Vrati vrijednost koja odgovara ključu
    std::optional<Value> value(Key key);
    // Ukloni ključ ako je prisutan i vrati true ako je uklonjen i false inače.
    bool remove(Key key);
    // Dodaj par "(key, value)" ako nije u spremniku.
    // U suprotnom ključu "key" daj vrijednost "value".
    void add_or_update(Key key, Value value);
    // Ispis čitave strukture (pretinci i povezane liste) radi kontrole.
    void print(std::ostream &out);
    // Broj elemenaa u tabeli.
    size_t size() const;
    // Nađi prvi element na koje predikat vraća "true".
    template <typename Predicat>
    std::optional<BucketValue> find_if(Predicat p) const;
    // Vrati listu svih elemenata na kojima predikat vraća "true".
    template <typename Predicat>
    std::list<BucketValue> find_all_if(Predicat p) const;
};

////////////////////////////////////////////////////////////////////////////
//*                 IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////
//**                Bucket IMPLEMENTATION

template <typename Key, typename Value, typename Hash>
std::optional<Value> HashTable<Key, Value, Hash>::Bucket::find(const Key &key) const
{
    auto it = std::find_if(mData.begin(), mData.end(),
                           [&key](const BucketValue &record) {
                               return record.first == key;
                           });
    if (it != mData.end())
        return it->second;
    else
        return {};
}

template <typename Key, typename Value, typename Hash>
void HashTable<Key, Value, Hash>::Bucket::add_value(const Key &key, const Value &value)
{
    auto it = std::find_if(mData.begin(), mData.end(),
                           [&key](const BucketValue &record) {
                               return record.first == key;
                           });
    if (it != mData.end())
        it->second = value;
    else
        mData.emplace_back(key, value);
}

template <typename Key, typename Value, typename Hash>
bool HashTable<Key, Value, Hash>::Bucket::remove(const Key &key)
{
    auto it = std::find_if(mData.begin(), mData.end(),
                           [&key](const BucketValue &record) {
                               return record.first == key;
                           });
    if (it == mData.end())
        return false;
    else
    {
        mData.erase(it);
        return true;
    }
}

template <typename Key, typename Value, typename Hash>
std::string HashTable<Key, Value, Hash>::Bucket::print() const
{
    std::string str;
    for (auto element : mData)
        str += "{" + std::to_string(element.first) + "->" + std::to_string(element.second) + "},";
    return str.substr(0, (str.size() - 1) > 0 ? (str.size() - 1) : 0);
}

////        !!!!!! možda ne treba
template <typename Key, typename Value, typename Hash>
size_t HashTable<Key, Value, Hash>::Bucket::size() const
{
    return mData.size();
}

template <typename Key, typename Value, typename Hash>
template <typename Predicat>
std::optional<typename HashTable<Key, Value, Hash>::Bucket::BucketValue> HashTable<Key, Value, Hash>::Bucket::find_if(Predicat p) const
{
    const auto it = std::find_if(mData.begin(), mData.end(), p);
    if (it != mData.end())
        return (*it);
    else
        return {};
}

template <typename Key, typename Value, typename Hash>
template <typename Predicat>
std::list<typename HashTable<Key, Value, Hash>::Bucket::BucketValue> HashTable<Key, Value, Hash>::Bucket::find_all_if(Predicat p) const
{
    std::list<typename HashTable<Key, Value, Hash>::Bucket::BucketValue> list;
    for (const auto pair : mData)
        if (p(pair))
            list.push_back(pair);
    return list;
}

////////////////////////////////////////////////////////////////////////////
//**                HashTable IMPLEMENTATION
template <typename Key, typename Value, typename Hash>
std::optional<Value> HashTable<Key, Value, Hash>::value(Key key)
{
    return mBuckets[mHasher(key) % mBuckets.size()].find(key);
}

template <typename Key, typename Value, typename Hash>
void HashTable<Key, Value, Hash>::add_or_update(Key key, Value value)
{
    mBuckets[mHasher(key) % mBuckets.size()].add_value(key, value);
}

template <typename Key, typename Value, typename Hash>
bool HashTable<Key, Value, Hash>::remove(Key key)
{
    return mBuckets[mHasher(key) % mBuckets.size()].remove(key);
}

template <typename Key, typename Value, typename Hash>
void HashTable<Key, Value, Hash>::print(std::ostream &out)
{
    std::string str("(");
    for (auto element : mBuckets)
        str += "[" + element.print() + "],";
    str.substr(0, str.size() - 1);
    out << str + ")\n";
}

///         ?????????????? smo u vektoru popunjenih ili svih el(svi dosada pridruženi u svim listama po svim elementima vektora)
template <typename Key, typename Value, typename Hash>
size_t HashTable<Key, Value, Hash>::size() const
{
    size_t size = 0;
    for (const auto &element : mBuckets)
        size += element.size();
    return size;
}

template <typename Key, typename Value, typename Hash>
template <typename Predicat>
std::optional<typename HashTable<Key, Value, Hash>::BucketValue> HashTable<Key, Value, Hash>::find_if(Predicat p) const
{
    std::optional<typename HashTable<Key, Value, Hash>::BucketValue> optional;
    for (auto element : mBuckets)
    {
        optional = element.find_if(p);
        if (optional.has_value())
            return optional.value();
    }
    return {};
}

template <typename Key, typename Value, typename Hash>
template <typename Predicat>
std::list<typename HashTable<Key, Value, Hash>::BucketValue> HashTable<Key, Value, Hash>::find_all_if(Predicat p) const
{
    std::list<typename HashTable<Key, Value, Hash>::BucketValue> list, temp;
    for (auto element : mBuckets)
    {

        temp = element.find_all_if(p);
        temp.sort();
        list.merge(temp);
    }
    return list;
}
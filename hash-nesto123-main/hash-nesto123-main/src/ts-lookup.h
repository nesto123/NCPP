/* 
 *              Rječnik na bazi hash tablice 
 *                  Paralelna verzija
 *                                                           @FV
 */

#pragma once

#include <vector>
#include <list>
#include <ostream>
#include <optional>
#include <shared_mutex>
#include <algorithm>
#include <string>

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
        mutable std::shared_mutex mMutex;
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
////////////////////////////////////////////////////////////////////////////
//**                Bucket IMPLEMENTATION

template <typename Key, typename Value, typename Hash>
std::optional<Value> HashTable<Key, Value, Hash>::Bucket::find(const Key &key) const
{
    std::optional<Value> val;
    typename std::list<HashTable<Key, Value, Hash>::Bucket::BucketValue>::const_iterator iterator;

    {
        std::lock_guard<std::shared_mutex> loc(mMutex);
        iterator = std::find_if(mData.begin(), mData.end(),
                                [&key](const BucketValue &record) {
                                    return record.first == key;
                                });
        if (iterator != mData.end())
            val = iterator->second;
    }
    return val;
}

template <typename Key, typename Value, typename Hash>
void HashTable<Key, Value, Hash>::Bucket::add_value(const Key &key, const Value &value)
{
    typename std::list<HashTable<Key, Value, Hash>::Bucket::BucketValue>::const_iterator iterator;
    HashTable<Key, Value, Hash>::Bucket::BucketValue element(key, value);

    {
        std::lock_guard<std::shared_mutex> loc(mMutex);
        auto iterator = std::find_if(mData.begin(), mData.end(),
                                     [&key](const BucketValue &record) {
                                         return record.first == key;
                                     });
        if (iterator != mData.end())
            iterator->second = value;
        else
            mData.push_back(element);
    }
}

template <typename Key, typename Value, typename Hash>
bool HashTable<Key, Value, Hash>::Bucket::remove(const Key &key)
{
    typename std::list<HashTable<Key, Value, Hash>::Bucket::BucketValue>::const_iterator iterator;
    bool flag = false;
    {
        std::lock_guard<std::shared_mutex> loc(mMutex);
        auto it = std::find_if(mData.begin(), mData.end(),
                               [&key](const BucketValue &record) {
                                   return record.first == key;
                               });
        if (it != mData.end())
        {
            mData.erase(it);
            flag = true;
        }
    }
    return flag;
}

template <typename Key, typename Value, typename Hash>
std::string HashTable<Key, Value, Hash>::Bucket::print() const
{
    std::string str;
    {
        std::lock_guard<std::shared_mutex> loc(mMutex);
        for (auto element : mData)
            str += "{" + std::to_string(element.first) + "->" + std::to_string(element.second) + "},";
    }
    return str.substr(0, (str.size() - 1) > 0 ? (str.size() - 1) : 0);
}

////        !!!!!! možda ne treba
template <typename Key, typename Value, typename Hash>
size_t HashTable<Key, Value, Hash>::Bucket::size() const
{
    size_t size = 0;
    {
        std::lock_guard<std::shared_mutex> loc(mMutex);
        size = mData.size();
    }
    return size;
}

template <typename Key, typename Value, typename Hash>
template <typename Predicat>
std::optional<typename HashTable<Key, Value, Hash>::Bucket::BucketValue> HashTable<Key, Value, Hash>::Bucket::find_if(Predicat p) const
{
    typename std::list<HashTable<Key, Value, Hash>::Bucket::BucketValue>::const_iterator iterator;
    std::optional<typename HashTable<Key, Value, Hash>::Bucket::BucketValue> value;
    {
        std::lock_guard<std::shared_mutex> loc(mMutex);
        iterator = std::find_if(mData.begin(), mData.end(), p);
        if (iterator != mData.end())
            value = (*iterator);
    }
    return value;
}

template <typename Key, typename Value, typename Hash>
template <typename Predicat>
std::list<typename HashTable<Key, Value, Hash>::Bucket::BucketValue> HashTable<Key, Value, Hash>::Bucket::find_all_if(Predicat p) const
{
    std::list<typename HashTable<Key, Value, Hash>::Bucket::BucketValue> list;

    {
        std::lock_guard<std::shared_mutex> loc(mMutex);
        for (const auto &pair : mData)
            if (p(pair))
                list.push_back(pair);
    }

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
    for (const auto &element : mBuckets)
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
    for (const auto &element : mBuckets)
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
    for (const auto &element : mBuckets)
    {

        temp = element.find_all_if(p);
        temp.sort();
        list.merge(temp);
    }
    return list;
}
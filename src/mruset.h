// Copyright (c) 2012 The curecoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef curecoin_MRUSET_H
#define curecoin_MRUSET_H

#include <unordered_set>
#include <deque>

/** STL-like set container that only keeps the most recent N elements. */
template <typename T, typename set_type = std::unordered_set<T>> class mruset
{
public:
    using key_type = T;
    using value_type = T;
    using iterator = typename set_type::iterator;
    using const_iterator = typename set_type::const_iterator;
    using size_type = typename set_type::size_type;

protected:
    set_type set;
    std::deque<const_iterator> queue;
    size_type nMaxSize;

public:
    mruset(size_type nMaxSizeIn = 0) : nMaxSize(nMaxSizeIn) {}
    iterator begin() const { return set.begin(); }
    iterator end() const { return set.end(); }
    size_type size() const { return set.size(); }
    bool empty() const { return set.empty(); }
    iterator find(const key_type& k) const { return set.find(k); }
    bool contains(const key_type& k) const { return find(k) != end(); }
    size_type count(const key_type& k) const { return set.count(k); }
    bool inline operator==(const mruset<T, set_type>& other) { return set == other.set; }
    bool inline operator==(const set_type& other) { return set == other; }
    // not instantiated (i.e. no error) until used
    bool inline operator<(const mruset<T, set_type>& other) { return set < other.set; }
    std::pair<iterator, bool> insert(const key_type& x)
    {
        std::pair<iterator, bool> ret = set.insert(x);
        if (ret.second)
        {
            if (nMaxSize && queue.size() == nMaxSize)
            {
                set.erase(queue.front());
                queue.pop_front();
            }
            queue.push_back(ret.first);
        }
        return ret;
    }
    size_type max_size() const { return nMaxSize; }
    size_type set_max_size(size_type s)
    {
        if (s)
            while (queue.size() > s)
            {
                set.erase(queue.front());
                queue.pop_front();
            }
        nMaxSize = s;
        return nMaxSize;
    }
};

#endif

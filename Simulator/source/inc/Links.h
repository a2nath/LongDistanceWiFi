#pragma once
#include <vector>
#include <map>
typedef unsigned int uint;

template<class T, class U>
std::vector<U> mltimap2vector(std::multimap<T, U>& map, T key)
{
	auto i = map.equal_range(key);
	std::vector<U> intlist;

	for (auto it2 = i.first; it2 != i.second; ++it2)
	{
		intlist.push_back(it2->second);
	}
	return intlist;
}

class Link
{
private:
	std::multimap<uint, uint> hashlink;
public:
	void add(uint source, uint destin)
	{
		hashlink.insert(std::pair<uint, uint>(source, destin));
	}
	void add2way(uint source, uint destin)
	{
		add(source, destin);
		add(destin, source);
	}
	std::vector<uint> dest(uint source)
	{
		return mltimap2vector(hashlink, source);
	}
};
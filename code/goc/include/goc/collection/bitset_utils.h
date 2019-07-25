//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef GOC_COLLECTION_BITSET_UTILS_H
#define GOC_COLLECTION_BITSET_UTILS_H

#include <bitset>

namespace goc
{
// Returns: A bitset with the numbers given.
template<unsigned long N>
std::bitset<N> create_bitset(const std::initializer_list<int>& numbers)
{
	std::bitset<N> b;
	for (size_t n: numbers) b.set(n);
	return b;
}

// Returns: The intersection of bitsets b1 and b2.
template<unsigned long N>
std::bitset<N> intersection(const std::bitset<N>& b1, const std::bitset<N>& b2)
{
	return b1 & b2;
}

// Returns: The intersection of bitset b1 and the numbers in the second parameter.
template<unsigned long N>
std::bitset<N> intersection(const std::bitset<N>& b1, const std::initializer_list<size_t>& numbers)
{
	return b1 & create_bitset<N>(numbers);
}

// Returns: The union of bitsets b1 and b2.
template<unsigned long N>
std::bitset<N> unite(const std::bitset<N>& b1, const std::bitset<N>& b2)
{
	return b1 | b2;
}

// Returns: The union of bitset b1 and the numbers in the second parameter.
template<unsigned long N>
std::bitset<N> unite(const std::bitset<N>& b1, const std::initializer_list<size_t>& numbers)
{
	return b1 | create_bitset<N>(numbers);
}
} // namespace goc

#endif //GOC_COLLECTION_BITSET_UTILS_H

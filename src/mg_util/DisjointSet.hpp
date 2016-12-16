//
// Created by Arthur Eubanks on 20/12/15.
//

#ifndef MAP_GEN_DISJOINTSET_H
#define MAP_GEN_DISJOINTSET_H

#include "mapgen/mapgen_global.hpp"

#include <unordered_map>

namespace mg_util {
	// a generic disjoint set (or union find) implementation
	template<typename T>
	class DisjointSet {
	public:
		DisjointSet() : nodes_() { }

		// add an item or multiple items
		void add(const T && t);
		template<typename Itr>
		void add(Itr first, Itr last);

		// return if two items are part of the same set
		bool same_set(const T && a, const T && b);

		// merge two items so they are in the same set
		void merge(const T && a, const T && b);

	private:
		// the node class needed for DisjointSet
		class Node {
		public:
			Node & parent;
			int rank;

			Node() : parent(*this), rank(0) { }

			bool operator==(const Node & other) const {
				return this == &other;
			}

			bool operator!=(const Node & other) const {
				return !operator==(other);
			}
		};

		// returns the root of the tree the node is part of
		Node & root(Node & node);

		// the map of all the nodes
		std::unordered_map<T, Node> nodes_;
	};
}


#endif //MAP_GEN_DISJOINTSET_H

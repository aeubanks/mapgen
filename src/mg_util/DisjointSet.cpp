//
// Created by Arthur Eubanks on 20/12/15.
//

#include "DisjointSet.hpp"

namespace mg_util {
	template<typename T>
	void DisjointSet<T>::add(const T && t) {
		nodes_.insert({t, Node()});
	}

	template<typename T>
	template<typename Itr>
	void DisjointSet<T>::add(Itr first, Itr last) {
		for (; first != last; ++first) {
			add(*first);
		}
	}

	template<typename T>
	bool DisjointSet<T>::same_set(const T && a, const T && b) {
		if (nodes_.find(a) == nodes_.end()) {
			throw mg_error("DisjointSet::same_set: first argument is not yet added");
		}
		if (nodes_.find(b) == nodes_.end()) {
			throw mg_error("DisjointSet::same_set: second argument is not yet added");
		}
		return root(nodes_[a]) == root(nodes_[b]);
	}

	template<typename T>
	void DisjointSet<T>::merge(const T && a, const T && b) {
		if (nodes_.find(a) == nodes_.end()) {
			throw mg_error("DisjointSet::same_set: first argument is not yet added");
		}
		if (nodes_.find(b) == nodes_.end()) {
			throw mg_error("DisjointSet::same_set: second argument is not yet added");
		}
		auto root1 = root(nodes_[a]);
		auto root2 = root(nodes_[b]);
		if (root1 != root2) {
			if (root1.rank < root2.rank) {
				root1.parent = root2;
			} else if (root1.rank > root2.rank) {
				root2.parent = root1;
			} else {
				root2.parent = root1;
				++root1.rank;
			}
		}
	}

	template<typename T>
	auto DisjointSet<T>::root(Node & node) -> Node & {
		if (node.parent != node) {
			node.parent = root(node.parent);
		}
		return node.parent;
	}
}

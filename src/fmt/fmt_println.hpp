//
// Created by Arthur Eubanks on 30/12/15.
//

#ifndef MAPGEN_FMT_PRINTLN_HPP
#define MAPGEN_FMT_PRINTLN_HPP

#include "format.hpp"

#include <utility>

namespace fmt {
	template<typename Arg, typename... Args>
	void println(Arg && arg, Args &&... args) {
		print(std::forward<Arg>(arg), std::forward<Args>(args)...);
		print("\n");
	}

	void println();

	template<typename Arg>
	void print_obj(Arg && arg) {
		print("{}", std::forward<Arg>(arg));
	}

	template<typename Arg>
	void println_obj(Arg && arg) {
		print("{}\n", std::forward<Arg>(arg));
	}
}

#endif //MAPGEN_FMT_PRINTLN_HPP

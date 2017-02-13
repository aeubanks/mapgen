#pragma once

#include "mg_util_include.hpp"

#include <vector>
#include <map>
#include <tuple>

#include "Random.hpp"
#include "WeightedSet.hpp"

namespace mg_util {

class GenerativeGrammar {
public:
    using ElementType = int16_t;
    using WeightType = int16_t;

    GenerativeGrammar(): curElements(), rules(), rand(), curSymbol(1), curTerminal(-1) {}

    ElementType createSymbol() { return curSymbol++; }
    ElementType createTerminal() { return curTerminal--; }
    void addRule(ElementType symbol, std::vector<ElementType> rhs, WeightType weight = 1);

    void setCurrent(std::vector<ElementType> elements) { curElements = std::move(elements); }
    void step();

    const std::vector<ElementType> & getCurrent() const { return curElements; }

private:
    // current elements to operate on
    std::vector<ElementType> curElements;
    // list of rules
    // map from a symbol to a list of possible rules
    // each rule includes the rhs (a list of elements) and the cdf of the weight (used for randomly selecting rules)
    std::map<ElementType, WeightedSet<std::vector<ElementType>>> rules;
    // std::map<ElementType, std::vector<std::tuple<std::vector<ElementType>, WeightType>>> rules;
    // rng for choosing rules
    mg_util::Random rand;
    // keep track of the next symbol/terminal to hand out
    ElementType curSymbol, curTerminal;
};

}


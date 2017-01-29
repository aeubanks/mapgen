#include "GenerativeGrammar.hpp"

namespace mg_util {

// true if element is symbol, false if is terminal
static bool elementIsSymbol(GenerativeGrammar::ElementType element) {
    return element > 0;
}

void GenerativeGrammar::addRule(ElementType symbol, std::vector<ElementType> rhs, WeightType weight) {
    rules[symbol].emplace(weight, std::move(rhs));
}

void GenerativeGrammar::step() {
    std::vector<ElementType> newElements;

    for (auto elem : curElements) {
        if (!elementIsSymbol(elem)) {
            newElements.push_back(elem);
            continue;
        }
        auto find = rules.find(elem);
        if (find == rules.end()) {
            // no rules for current symbol
        }
        const auto & rule = find->second.random(rand);
        newElements.insert(newElements.end(), rule.begin(), rule.end());
    }

    setCurrent(std::move(newElements));
}

}

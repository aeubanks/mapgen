#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <boost/spirit/include/qi.hpp>

template<typename It1, typename It2>
bool parse(It1 first, It2 last) {
    namespace qi = boost::spirit::qi;

    unsigned long long count = 0;
    auto action = [&count](int i) {
        std::cout << "int: " << i << "\n";
        count += i * i;
    };

    auto parser = qi::char_('(') >> qi::int_[action] >> *(qi::char_(',') >> qi::int_[action]) >> qi::char_(')');

    bool success = qi::phrase_parse(first, last, parser, qi::space);

    std::cout << "count: " << count << "\n";

    return success && first == last;
}

int main(int argc, char ** argv) {
    bool res;
    if (argc >= 2) {
        std::ifstream inFile(argv[1]);
        std::istreambuf_iterator<char> begin(inFile), end;
        res = parse(begin, end);
    } else {
        std::string input = "(22)";
        res = parse(input.begin(), input.end());
    }
    return res ? 0 : 1;
}

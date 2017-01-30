#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

template<typename It>
bool parse(It itFirst, It itLast, const std::string & itName) {
    namespace qi = boost::spirit::qi;
    using PosItType = boost::spirit::classic::position_iterator2<It>;
    PosItType posFirst(itFirst, itLast, itName), posLast;

    std::vector<std::vector<std::vector<char>>> chars;
    auto finishMapAction = [&chars] (std::vector<std::vector<char>> & map) {
        chars.push_back(std::move(map));
    };
    auto mapChars = qi::char_('#') | qi::char_('.') | qi::char_('+') | qi::char_('~');
    auto oneMap = (+((+mapChars) >> qi::eol))[finishMapAction];

    auto parser = *(oneMap | qi::eol);

    bool success = qi::parse(posFirst, posLast, parser) && posFirst == posLast;

    if (success) {
        for (auto & map : chars) {
            std::cout << "vec\n";
            for (auto & row : map) {
                for (auto c : row) {
                    std::cout << c;
                }
                std::cout << '\n';
            }
            std::cout << '\n';
        }
    } else {
        auto failPos = posFirst.get_position();
        std::cerr << "bad parse in " << failPos.file << " at " << failPos.line << ":" << failPos.column << "\n";
        std::cerr << posFirst.get_currentline() << '\n';
        for (int i = 1; i < failPos.column; ++i) {
            std::cerr << ' ';
        }
        std::cerr << "^ around here\n";
    }

    return success;
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        std::cerr << "need file name\n";
        return 1;
    }
    using file_in_iterator = std::istreambuf_iterator<char>;
    std::ifstream inFile(argv[1]);
    bool res = parse(boost::spirit::make_default_multi_pass(file_in_iterator(inFile)), boost::spirit::make_default_multi_pass(file_in_iterator()), argv[1]);
    return res ? 0 : 1;
}

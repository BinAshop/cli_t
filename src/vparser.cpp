#include <iostream>
#include <list>
#include <string>
#include <variable.hpp>
#include <boost/algorithm/string.hpp>
#include <var_parser.hpp>
#include <commandline.hpp>
using namespace std;
using namespace boost::algorithm;
string VParser::vparser(string str, CLI& cli){
    replace_all(str, "%{?PATH}", cli.clib.getstrv("PATH"));
    replace_all(str, "%{?HOME}", cli.clib.getstrv("HOME"));
    replace_all(str, "%{?PS1}", cli.clib.getstrv("PS1"));
    replace_all(str, "%{?LIB}", cli.clib.getstrv("LIB"));
    replace_all(str, "%{?user}", cli.clib.getstrv("USER"));
    replace_all(str, "%{?hostname}", cli.clib.getstrv("HOSTNAME"));
    return str;
}
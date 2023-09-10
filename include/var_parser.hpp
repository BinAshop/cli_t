#pragma once

#include <iostream>
#include <string> 
#include <variable.hpp>
#include <commandline.hpp>
using namespace std;

class VParser{
    public:
    string vparser(string str, CLI& cli);
};
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <cliba.hpp>
#include <var_parser.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <commandline.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/array.hpp>
#include <cstdio>
#include <boost/algorithm/string.hpp>
using namespace boost::algorithm;
int cliba::varstr(string key, string value)
{
    if (value == "" || value.empty())
    {
        perr(1, "Value can't be empty.");
        return 1;
    }
    for (const auto &it : var_str)
    {
        std::string rep = "%{?" + it.first + "}";
        if (value.find(rep) != std::string::npos)
        {
            replace_all(value, rep, it.second);
        }
    }
    std::cout << "var: " << key << ", contents: " << value << std::endl;
    cliba::var_str[key] = value;
    return 0;
}
string cliba::getstrv(string key)
{
    if (key == " " || key == "" || key.empty())
    {
        perr(1, "Key can't be empty.");
        return "null";
    }
    if (var_str[key].empty())
    {
        return "null";
    }
    return var_str[key];
}
int cliba::perr(int code, string str)
{
    if (getstrv("HOSTNAME") == "null")
    {
        std::cout << "HOSTNAME hasn't set yet" << std::endl;
        return 1;
    }
    std::cout << getstrv("HOSTNAME") << ": Error(" + std::to_string(code) + "): " << str << std::endl;
    return code;
}
bool isValidPrintF(const std::string &input)
{
    if (starts_with(input, "@printf|"))
    {
        if (ends_with(input, "|end"))
        {
            return true;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
    return true;
}
bool isValidPrintP(const std::string &input)
{
    if (starts_with(input, "@printp|"))
    {
        if (ends_with(input, "|end"))
        {
            return true;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
    return true;
}
int cliba::printpf(string str)
{
    for (const auto &it : var_str)
    {
        std::string rep = "%{?" + it.first + "}";
        if (str.find(rep) != std::string::npos)
        {
            replace_all(str, rep, it.second);
        }
    }
    if (isValidPrintF(str))
    {
        replace_first(str, "@printf", "");
        //std::cout << str << std::endl;
        int i = 0;
        if (str == "" || str.empty())
        {
            perr(1, "String can't empty ");
            return 1;
        }
        //std::cout << str << std::endl;

        if (ends_with(str, "|end"))
        {
            replace_last(str, "|end", "");
            std::vector<std::string> tokens;
            boost::split(tokens, str, boost::is_any_of("|"));

            if (tokens.size() >= 2)
            {
                std::string result = boost::algorithm::join(std::vector<std::string>(tokens.begin() + 1, tokens.end()), "|");
                std::cout << result << std::endl;
            }
        }
        else
        {
            std::vector<std::string> tokens;
            boost::split(tokens, str, boost::is_any_of("|"));

            if (tokens.size() >= 2)
            {
                std::string result = boost::algorithm::join(std::vector<std::string>(tokens.begin() + 1, tokens.end()), "|");
                std::cout << result;
            }
        }
    }
    else
    {
        perr(1, "Invalid usage!.");
        return 1;
    }
    return 0;
}
int cliba::printp(string str)
{
    if (isValidPrintP(str))
    {
        replace_first(str, "@printp", "");
        //std::cout << str << std::endl;
        int i = 0;
        if (str == "" || str.empty())
        {
            perr(1, "String can't empty ");
            return 1;
        }
        //std::cout << str << std::endl;

        if (ends_with(str, "|end"))
        {
            replace_last(str, "|end", "");
            std::vector<std::string> tokens;
            boost::split(tokens, str, boost::is_any_of("|"));

            if (tokens.size() >= 2)
            {
                std::string result = boost::algorithm::join(std::vector<std::string>(tokens.begin() + 1, tokens.end()), "|");
                std::cout << result << std::endl;
            }
        }
        else
        {
            std::vector<std::string> tokens;
            boost::split(tokens, str, boost::is_any_of("|"));

            if (tokens.size() >= 2)
            {
                std::string result = boost::algorithm::join(std::vector<std::string>(tokens.begin() + 1, tokens.end()), "|");
                std::cout << result;
            }
        }
    }
    else
    {
        perr(1, "Invalid usage!.");
        return 1;
    }

    return 0;
}
int cliba::import(string path){
    
    if (path == "" || path.empty()){
        return perr(1, "IMPORT: Invalid usage.");
    }
    if (!ends_with(path, ".clb")){
        path.append(".clb");
    }
    std::string pimport_default = getstrv("LIB") + "/" + path;
    std::string pimport = path;
    if (access(pimport_default.c_str(), F_OK) != 0){
        if (access(pimport.c_str(), F_OK) != 0){
            return perr(1, "IMPORT: Module '" + path + "' not found.");
        }else{
            pimport = path;
        }
    }else{
        pimport = pimport_default;
    }
    //pimport.append(".clb");
    std::string line;
    // reading files
    std::ifstream file(pimport);
    if (!file){
        return perr(1, "IMPORT: Module '" + path + "' error.");
    }
    std::string sources;
    while (std::getline(file, line)){
        sources.append(line + "\n");
    }
    int result = CompileCliba(sources);
    if (result != 0)
    {
        return perr(1, "IMPORT: Module '" + path + "' error.");
    }
    return 0;
}
int cliba::CompileCliba(string sources)
{
    // Membuat streambuf dan stream dari string input

    std::istringstream iss(sources);

    std::string line;

    // Membaca baris per baris
    int line_n = 0;
    int c_result = 0;
    while (std::getline(iss, line))
    {
        line_n++;
        std::vector<std::string> args;
        std::istringstream iss(line);
        std::string word;

        while (iss >> word)
        {
            args.push_back(word);
        }
        if (starts_with(line, "@printp"))
        {
            //std::cout << line << std::endl;
            c_result = printp(line);
        }
        else if (starts_with(line, "@printf"))
        {
            c_result = printpf(line);
        }
        else if (starts_with(line, "(str)"))
        {
            std::string var;
            char V[255];
            std::string value;
            replace_first(line, "(str)", "");
            int k = std::sscanf(line.c_str(), "%254[^=]", V);
            if (k != 1){
                return perr(1, "Key invalid");
            }
            var.append(V);
            replace_all(var, " ", "_");
            replace_first(line, V, "");
            std::vector<std::string> tokens;
            split(tokens, line, boost::is_any_of("="));

            if (tokens.size() >= 2)
            {
                std::string result = boost::algorithm::join(std::vector<std::string>(tokens.begin() + 1, tokens.end()), "=");
                value = result;
                
            }

            if (!var.empty() && !value.empty() && var != "" && var != " ")
            {
                return varstr(var, value);
            }
            else
           
            {
                return perr(1, "Invalid format!.");
            }
        }
        else if (starts_with(line, "lvar"))
        {
            for (auto it = var_str.begin(); it != var_str.end(); it++)
			{
				cout << it->first << ": " << it->second << endl;
			}
        }
        else if (starts_with(line, "@import"))
        {
            replace_first(line, "@import", "");
            replace_first(line, ">", "");
            replace_all(line, " ", "");
            c_result = import(line);
        }
        else
        {
            if (getstrv("PATH") == "null")
            {
                perr(1, "PATH variable has not set yet.");
                return 1;
            }
            if (getstrv("PS") == "null")
            {
                perr(1, "PS variable has not set yet.");
                return 1;
            }
            if (getstrv("HOME") == "null")
            {
                perr(1, "HOME variable has not set yet.");
                return 1;
            }
            if (getstrv("LIB") == "null")
            {
                perr(1, "LIB variable has not set yet.");
                return 1;
            }
            std::string bin_access_str = getstrv("PATH") + "/" + args.front();
            std::string command_runner = getstrv("PATH") + "/" + line;
            const char *cmdr = command_runner.c_str();
            const char *binac = bin_access_str.c_str();
            if (access(binac, F_OK) != 0)
            {
                return perr(127, "Command not found.");
            }
            if (access(binac, X_OK) != 0)
            {
                return perr(126, "Permission denied.");
            }
            int exec = system(cmdr);
            c_result = exec;
        }
        switch (c_result)
        {
        case 0:
            break;
        case 1:
            
            return perr(1, "Error on line " + std::to_string(line_n) + ".");
        default:
            return 1;
        }
        /*switch (c_result){
            case 1:
            std::cout << ""
        }*/
        // int Parse = std::sscanf(line.c_str(), "");
    }
    return 0;
}
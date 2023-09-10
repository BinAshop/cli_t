#pragma once
#include <string>
#include <list>
#include <iostream> 
using Text = std::list<std::string>;
using Input = std::string;
using CPath = unsigned char *;
using Death = signed int;
using Proccess = signed int;
//color
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define BD_BLACK "\033[1;30m"
#define BD_RED "\033[1;31m"
#define BD_GREEN "\033[1;32m"
#define BD_YELLOW "\033[1;33m"
#define BD_BLUE "\033[1;34m"
#define BD_MAGENTA "\033[1;35m"
#define BD_CYAN "\033[1;36m"
#define BD_WHITE "\033[1;37m"
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

struct Host {
  std::string hostname;
};
class Output {
public:
    int err = 0;
    std::string str;
    std::string f;
};
class User {
public:
  int id;
  std::string name;
private:
  std::string pass;
};
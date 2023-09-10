#ifndef CMD
#define CMD
#include <iostream>
#include <string>
#include <variable.hpp>
#include <cliba.hpp>
class CLI
{
    //path
  public:
    std::string PS1 = "%{?user}@%{?hostname} $ ";
    std::string PATH = "bin";
    std::string HOME = "root";
    std::string LIB = "lib";
    std::string LIB_DEFAULT = "libcli.so";
    // func
    
    Text text;
    Input input = "null";
    User user;
    bool isSet = false;
    CPath path;
    Host host;
    Death d;
    Proccess p;
    cliba clib;
    void Set(std::string name, int id, Host &hosts)
    {
        host.hostname = hosts.hostname;
        user.id = id;
        user.name = name;
        isSet = true;
    }
    void Initialise(CLI &cli, cliba &clis, std::string (*func)(std::string cmd, CLI &cli, Output& out, cliba& clis));
    void Exit()
    {
        d = 1;
    }
    void Clear();
    
};
#endif
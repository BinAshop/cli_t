#ifndef CLIBA
#define CLIBA
#include <iostream>
#include <map>
#include <string>
using namespace std;
/*class c_cliba{
    public:
    string[] c_name;
    string[] c_func;
};
class 32u{
    public:
    unsigned int i32;
};*/
class cliba{
    public:
     int perr(int code, string str);
     string getstrv(string key);
     int varstr(string key, string value);
     int printp(string str);
     int printpf(string str);
     int import(string path);
     //int s_import(c_cliba& __class);
     //int func(void (*args), ...);

     
     int CompileCliba(string sources);
    private:
        map<string, string> var_str;
};
#endif
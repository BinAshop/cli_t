#include <iostream>
#include <list>
#include <string>
#include <stdlib.h>
#include <unistd.h>

#include <variable.hpp>
#include <sstream>
#include <vector>
#include <commandline.hpp>
#include <cliba.hpp>
#include <boost/algorithm/string.hpp>
#ifndef HOSTS
#define HOSTS "NULL"
#endif
std::string F(std::string cmd, CLI &cli, Output &out, cliba& clis)
{
    std::string cmds;

    
    try
    {
        if (cmd == "" || cmd.empty())
        {
            return "";
        }
        else if (cmd == "exit")
        {
            cli.Exit();
            return "Exited";
        }
        else
        {
        /*else if (cmds == "c")
        {
            boost::algorithm::replace_all(cmd, "c", "");
            
        }
        else
        {
            /*std::string bin_access_str = cli.PATH + "/" + cmds;
            const char *binac = bin_access_str.c_str();
            if (access(binac, F_OK) != 0)
            {
                throw 127;
            }
            if (access(binac, X_OK) != 0)
            {
                throw 126;
            */
            int exec = clis.CompileCliba(cmd);
            throw exec;
        }
    }
    catch (int err)
    {
        std::string code;
        out.err = err;
        out.f = cmds;
        switch (err)
        {
        case 127:
            code = "127";
            out.str = "ERROR: " + cmds + ": Command not found";
            return "";
        case 126:
            code = "126";
            out.str = "ERROR: " + cmds + ": can't execute: permission denied";
            return "";
        case 0:
            code = "0";
            out.str = "\nCLI/success -code- 0";
            return "";
        default:
            return "Code: " + std::to_string(err);
        }
    }
    return "";
};
int main()
{
    CLI cli_term;
    Host host;
    cliba clis;
    clis.varstr("PATH", cli_term.PATH);
    clis.varstr("HOME", cli_term.HOME);
    clis.varstr("PS", cli_term.PS1);
    clis.varstr("LIB", cli_term.LIB);
    cli_term.Set("help", 0, host);
    clis.varstr("USER", cli_term.user.name);
    clis.varstr("HOSTNAME", cli_term.host.hostname);
    cli_term.clib = clis;
    cli_term.Initialise(cli_term, clis, F);
    return 0;
}

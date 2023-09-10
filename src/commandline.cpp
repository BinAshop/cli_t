#include <iostream>
#include <string>
#include <ncurses.h>
#include <list>
#include <variable.hpp>
#include <commandline.hpp>
#include <signal.h>
#include <var_parser.hpp>
#include <cliba.hpp>
#include <string.h>
volatile sig_atomic_t stop = 0;
void sigint_handler(int sig)
{
    stop = 1;
}
/*static int *b = NULL;
static int width;
static int height;
static int size;
static void sizechanged()
{
    getmaxyx(stdscr, height, width);
    size = width * height;
    b = (int *)realloc(b, (size + width + 1) * sizeof(int));
    memset(b, 0, (size + width + 1) * sizeof(int));
}*/
void CLI::Initialise(CLI &cli, cliba& clis, std::string (*func)(std::string cmd, CLI &cli, Output &out, cliba& clis))
{
    if (!isSet)
    {
        std::cout << "Cli is not set yet." << std::endl;
        exit(1);
    }
    //nodelay(stdscr, TRUE);
    Output o;
    VParser vp;
    std::string PS1_tmp = vp.vparser(clis.getstrv("PS"), cli);
    signal(SIGINT, sigint_handler);
    while (d != 1)
    {
        //int ch = getch();
        std::cout << PS1_tmp;
        
        std::getline(std::cin, input);
        //std::cout << input.empty();
        if (input != "" || input != "\n")
        {
            text.push_front(input);
            std::string result = func(text.front(), cli, o, clis);
            if (result != "")
            {
                result.append("\n");
            }
            std::cout << result.c_str();
            
            PS1_tmp = std::to_string(o.err) + "|" + vp.vparser(PS1, cli);
        }
        else
        {
            PS1_tmp = vp.vparser(PS1, cli);
        }
    }
}
void CLI::Clear()
{
    text.clear();
    system("cls");
};
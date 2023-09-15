#include <iostream>
#include <string>
#include <vector>
#include <map>
std::map<std::string, std::string> VAR_STR;
std::map<std::string, int> VAR_INT;
std::map<std::string, bool> VAR_BOOL;
enum TokenType
{

    identifier,
    keyword,
    op,
    vs,
    vi,
    vb,
    colon,
    Int,
    String,
    Bool,
    literal,
    at,
    eq,
    d_eq,
    coma,
    ob,
    cb,
    pp,
    add,
    ticks,
    minus,
    divide,
    percent,
    TypeError,
    
    error,
    open_round_brackets,
    close_round_brackets,
    comment,
    end,

};
std::map<TokenType, std::string> TOKEN_TYPE_MAP = {
    {colon, ":"},
    {at, "@"},
    {eq, "="},
    {d_eq, "eq"},
    {coma, ","},
    {ob, "{"},
    {cb, "}"},
    {pp, "()"},
    {open_round_brackets, "("},
    {close_round_brackets, ")"},
    {add, "+"},
    {minus, "-"},
    {divide, "/"},
    {percent, "%"},

    {comment, "#"},
    {end, "\n"},
    
    
    
    {ticks, "\""}};
enum Expression {

};
/*enum StringInterpolation{
    STRING_INTERPOLATION,
    STRING_INTERPOLATION_END
};
std::map<StringInterpolation, std::string> STRING_INTERPOLATION_MAP = {
    {STRING_INTERPOLATION, "{"},
    {STRING_INTERPOLATION_END, "}"}
};*/
enum KeywordType
{
    null,
    False,
    True,
    kw_String,
    kw_Int,
    kw_Bool,
    kw_TypeError,
   
    error_func_not_implemented,
    error_invalid_operator,
    error_expected_identifier,
    error_expected_expression,
    error_expected_colon,
    error_expected_operator,
    error_expected_end_of_string,
    out,
    Func,
    var,

};
std::map<KeywordType, std::string> KEYWORD = {
    {out, "out"},
    {Func, "Func"},
    {var, "var"},
    {null, "Null"},
    {False, "false"},
    {True, "true"}
};
struct Token
{
    TokenType type;
    TokenType type2;
    KeywordType keyword;
    std::string lexeme;
};

class Lexer
{
public:
    Lexer(const std::string &input) : input_(input), position_(0) {}

    Token getNextToken()
    {
        if (position_ >= input_.size())
        {
            return {end, end, null, " "};
        }
        // Handle identifiers and keywords.
        char currentChar = input_[position_];
        if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n')
        {
            ++position_;
            getNextToken();
        }
        else if (currentChar == TOKEN_TYPE_MAP[at][0])
        {
            ++position_;
            return {op, at, null, "@"};
        }
        else if (isalpha(currentChar))
        {
            std::string identifier;
            while (position_ < input_.size() && isalnum(input_[position_]))
            {
                identifier += input_[position_];
                ++position_;
            }
            // check if identifier is a keyword
            Token token;
            // token = {kw_error_func_not_implemented, " Func: " + identifier + " not implemented"};
            token = {TokenType::identifier, TokenType::identifier, null, identifier};
            for (auto &i : KEYWORD)
            {
                if (identifier == i.second)
                {
                    token = {keyword, keyword, i.first, i.second};
                    break;
                }
            }
            for (auto &i : TOKEN_TYPE_MAP)
            {
                if (identifier == i.second)
                {
                    token = {op, i.first, null ,i.second};
                    break;
                }
            }
            for (auto &i : VAR_STR)
            {
                if (identifier == i.first)
                {
                    token = {keyword, vs, KeywordType::kw_String, i.second};
                    break;
                }
            }
            for (auto &i : VAR_INT)
            {
                if (identifier == i.first)
                {
                    token = {keyword, vi, KeywordType::kw_Int, std::to_string(i.second)};
                    break;
                }
            }
            for (auto &i : VAR_BOOL)
            {
                if (identifier == i.first)
                {
                    if (i.second == true)
                    {
                        token = {keyword, vb, KeywordType::kw_Bool, KEYWORD[True]};
                        break;
                    }
                    else if (i.second == false)
                    {
                        token = {keyword, vb, KeywordType::kw_Bool, KEYWORD[False]};
                        break;
                    }
                }
            }
            return token;
            /*if (identifier == KEYWORD[out]){
                return {keyword, KEYWORD[out]};
            }
            else if (identifier == KEYWORD[Func]){
                return {keyword, KEYWORD[Func]};
            }else if (identifier == KEYWORD[var]){
                return {keyword, KEYWORD[var]};
            }else if (identifier == TOKEN_TYPE_MAP[tr]){
                return {tr, TOKEN_TYPE_MAP[tr]};
            }else if (identifier == )*/
        }
        /*else if (currentChar == TOKEN_TYPE_MAP[])
        {
            ++position_;
            return {colon, ":"};
        }*/

        else if (isdigit(currentChar))
        {
            int num = 0;
            while (position_ < input_.size() && isdigit(input_[position_]))
            {
                num = num * 10 + (input_[position_] - '0');
                ++position_;
            }
            return {literal, TokenType::Int, KeywordType::kw_Int, std::to_string(num)};
        }
        else
        {
            // Handle other characters as needed.
            // For simplicity, we'll just skip unknown characters.
            Token token;
            token = {error, TokenType::TypeError, error_invalid_operator,  " Invalid operator: " + std::string(1, currentChar)};
            for (auto &i : TOKEN_TYPE_MAP)
            {
                if (currentChar == i.second[0])
                {
                    ++position_;
                    token = {op, i.first, null, i.second};
                    break;
                }
            }
            if (token.type == ticks)
            {
                std::string str;
                ++position_;
                while (position_ < input_.size() && input_[position_] != '\"')
                {
                    str += input_[position_];
                    ++position_;
                }
                if (position_ < input_.size() && input_[position_] == '\"')
                {
                    ++position_; // Advance past the closing quote.
                }
                else
                {
                    return {error, TokenType::TypeError, error_expected_end_of_string, "Expected end of string"};
                }
                return {literal, TokenType::String, KeywordType::kw_String, str};
            }
            return token;
        }
    }

private:
    std::string input_;
    size_t position_;
};

class Parser
{
public:
    Parser(const std::string &input) : lexer_(input) {}

    void parse()
    {
        Token token;
        while (true)
        {
            token = lexer_.getNextToken();
            if (token.type == end)
            {
                break; // End of input, exit loop.
            }
            else if (token.type == at)
            {
                // Handle built-in command.
                std::cout << "Built-in command: " << token.lexeme << "\n ";
            }
            else if (token.keyword == out)
            {
                std::cout << "out: " << token.lexeme << "\n";
            }
            else if (token.type == identifier)
            {
                // Handle identifier.
                std::cout << "Identifier: " << token.lexeme << "\n ";
            }
            else if (token.type == colon)
            {
                // Handle colon.
                std::cout << "Colon " << token.lexeme << "\n";
            }
            else if (token.type == literal)
            {
                // Handle string.
                std::cout << "L: " << token.lexeme << " \n";
            }
            /*else if (token.type == int_literal)
            {
                std::cout << "int: " << token.lexeme << "\n";
            }*/
            else
            {
                std::cout << "Elze" << token.lexeme << " \n";
            }
        }
        std::cout << std::endl;
    }

private:
    Lexer lexer_;
};

int main()
{
    std::string input = "out: \"Hello";
    Parser parser(input);
    parser.parse();
    return 0;
}

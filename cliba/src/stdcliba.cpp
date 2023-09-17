#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
std::map<std::string, std::string> VAR_STR;
std::map<std::string, int> VAR_INT;
std::map<std::string, bool> VAR_BOOL;

enum TokenType
{

    identifier,
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
    space,
    tab,
    nextl,
    error,
    open_round_brackets,
    close_round_brackets,
    comment,
    end,
    ors,
    times,
    ands,
    nots,
    nteq
};
std::map<TokenType, std::string> TOKEN_TYPE_MAP = {
    {colon, ":"},
    {at, "@"},
    {eq, "="},
    {d_eq, "=="},
    {times, "*"},
    {pp, "++"},
    {ors, "||"},
    {ands, "&&"},
    {nots, "!"},
    {nteq, "!="},
    {coma, ","},
    {ob, "{"},
    {cb, "}"},
    {open_round_brackets, "("},
    {close_round_brackets, ")"},
    {add, "+"},
    {minus, "-"},
    {divide, "/"},
    {percent, "%"},

    {comment, "#"},
    {end, "\n"},
    {ticks, "\""}};
enum Expression
{
    str,
    integer,
    boolean,
    nulls
};
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
    {True, "true"}};
struct Error
{
    KeywordType type;
    std::string message;
};
struct VariableNode
{
    KeywordType type = null;
    std::string name;
    std::string value_str;
    int value_int;
    bool value_bool;
};
struct Token
{
    TokenType type;
    TokenType type2;
    KeywordType keyword;
    std::string lexeme;
    int line;
    int column = 0;
};
struct ExpressionNode
{
    Expression type = nulls;
    std::string lexeme = "null";
};
class Lexer
{
public:
    Lexer(const std::string &input) : input_(input), position_(0) {}

    Token getNextToken()
    {

        if (position_ >= input_.size())
        {
            return {end, end, null, " ", line_, column_};
        }
        // Handle identifiers and keywords.
        char currentChar = input_[position_];
        if (currentChar == ' ')
        {
            column_++;
            ++position_;
            return getNextToken();
        }
        else if (currentChar == '\t')
        {
            column_ += 4;
            ++position_;
            return getNextToken();
        }
        else if (currentChar == '\n')
        {
            column_ = 0;
            line_++;
            ++position_;
            return {nextl, nextl, null, " ", line_, column_};
        }
        else if (currentChar == TOKEN_TYPE_MAP[at][0])
        {
            column_++;
            ++position_;
            return {op, at, null, "@", line_, column_};
        }
        else if (isalpha(currentChar))
        {
            std::string identifier;
            while (position_ < input_.size() && isalnum(input_[position_]))
            {
                identifier += input_[position_];
                column_++;
                ++position_;
            }

            Token token;
            // 0 = not found, 1 = found
            token = {TokenType::identifier, TokenType::identifier, null, identifier, line_, column_};
            if (identifier == "false")
            {
                token = {literal, TokenType::Bool, KeywordType::False, "false", line_, column_};
            }
            else if (identifier == "true")
            {
                token = {literal, TokenType::Bool, KeywordType::True, "true", line_, column_};
            }

            return token;
        }

        else if (isdigit(currentChar))
        {
            std::string literal;
            while (position_ < input_.size() && isdigit(input_[position_]))
            {
                literal += input_[position_];
                column_++;
                ++position_;
            }
            int num = std::stoi(literal);
            return {TokenType::literal, TokenType::Int, KeywordType::kw_Int, std::to_string(num), line_, column_};
        }
        /*
         *  Parse string literals and if not valid like "string" return error
         */
        else if (currentChar == '\"')
        {
            std::string str;
            ++position_;
            while (position_ < input_.size() && input_[position_] != '\"')
            {
                str += input_[position_];
                column_++;
                ++position_;
            }
            // Check if string is valid
            if (position_ >= input_.size() || input_[position_] != '\"')
            {
                return {error, TokenType::TypeError, error_expected_end_of_string, " Expected end of string", line_, column_};
            }
            if (position_ < input_.size() && input_[position_] == '\"')
            {
                column_++;
                ++position_; // Advance past the closing quote.
            }
            return {literal, TokenType::String, KeywordType::kw_String, str, line_, column_};
        }
        else
        {
            // Handle other characters as needed.
            // For simplicity, we'll just skip unknown characters.
            Token token;
            token = {
                error,
                TokenType::TypeError,
                error_invalid_operator,
                " Invalid operator: " + std::string(1, currentChar),
            };
            for (auto &i : TOKEN_TYPE_MAP)
            {
                if (currentChar == i.second[0])
                {
                    column_++;
                    ++position_;
                    token = {op, i.first, null, i.second, line_, column_};
                    break;
                }
            }

            return token;
        }
    };

private:
    int line_ = 1;
    int column_ = 0;
    std::string input_;
    size_t position_;
};

class Parser
{
public:
    Parser(const std::string &input) : lexer_(input) {}

    void parse()
    {
        while (true)
        {
            token = lexer_.getNextToken();
            if (token.type == end)
            {
                break;
            }
            /*
             *  Handle
             */

            if (token.type == identifier)
            {
                if (token.lexeme == KEYWORD[out])
                {
                    token = lexer_.getNextToken();
                    std::string str;
                    if (token.type == op && token.type2 == colon)
                    {
                        ExpressionNode expression = expression_();
                        std::cout << expression.lexeme;
                    }else{
                        std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected colon" << std::endl;
                        return;
                    }
                }
            }
        }
    }

private:
    Lexer lexer_;
    Token token;
    /*
     * Parse Expression and return the value
     */
    ExpressionNode expression_()
    {
        ExpressionNode node;
        std::vector<Token> tokens;
        while (true)
        {
            Token token_b = token;

            /*
             * Add into vector
             */
            tokens.push_back(token);
            token = lexer_.getNextToken();
            if (token.type == end || token.type == nextl)
            {
                break;
            }
        }
        int Num = 0;
        TokenType type = tokens[Num].type2;
        while (tokens.size() == Num)
        {
           
            Token token = tokens[Num];
            /*
             *
             */
            if (token.type == literal || token.type == identifier)
            {
                /*
                 * Cek jika saat ini masih di 0 maka tidak akan cek jika sebelumnya adalah operator
                 * Jika saat ini adalah bukan 0 maka cek jika sebelumnya adalah operator
                 * Dan juga cek tipe yg ke 0 jika tipe ke 0 adalah string maka akan di proses seperti string interpolation
                 * Jika tipe ke 0 adalah integer maka akan di proses seperti integer
                 */
                if (token.type2 == identifier)
                {
                    if (VAR_STR.find(token.lexeme) != VAR_STR.end())
                    {
                        token.type2 = String;
                        token.lexeme = VAR_STR[token.lexeme];
                    }
                    else if (VAR_INT.find(token.lexeme) != VAR_INT.end())
                    {
                        token.type2 = Int;
                        token.lexeme = std::to_string(VAR_INT[token.lexeme]);
                    }
                    else if (VAR_BOOL.find(token.lexeme) != VAR_BOOL.end())
                    {
                        if (type != Bool)
                        {
                            std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Cannot expression Boolean with string or integer" << std::endl;
                            return node;
                        }
                    }
                    

                    else
                    {
                        for (auto &i : KEYWORD)
                        {
                            if (token.lexeme == i.second)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Cannot expression keyword" << std::endl;
                                return node;
                            }
                        }
                        std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Variable " << token.lexeme << " undefined" << std::endl;
                        return node;
                    }
                }
                if (Num == 1)
                {
                    if (type == String)
                    {
                        node.type = str;
                        node.lexeme = token.lexeme;
                    }
                    else if (type == Int)
                    {
                        node.type = integer;
                        node.lexeme = token.lexeme;
                    }else if (type == Bool)
                    {
                        node.type = boolean;
                        node.lexeme = token.lexeme;
                    }
                }
                else
                {
                    if (tokens[Num - 1].type == op && tokens[Num - 1].type2 == add)
                    {
                        if (type == String)
                        {
                            if (token.type2 != String)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected string" << std::endl;
                                return node;
                            }
                            node.type = str;
                            node.lexeme = node.lexeme + token.lexeme;
                        }
                        else if (type == Int)
                        {
                            if (token.type2 != Int)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected integer" << std::endl;
                                return node;
                            }
                            node.type = integer;
                            node.lexeme = std::to_string(std::stoi(node.lexeme) + std::stoi(token.lexeme));
                        }
                    }
                    else if (tokens[Num - 1].type == op && tokens[Num - 1].type2 == minus)
                    {
                        if (type != Int)
                        {
                            std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected integer" << std::endl;
                            return node;
                        }
                        else if (type == Int)
                        {
                            if (token.type2 != Int)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected integer" << std::endl;
                                return node;
                            }
                            node.type = integer;
                            node.lexeme = std::to_string(std::stoi(node.lexeme) - std::stoi(token.lexeme));
                        }
                    }
                    else if (tokens[Num - 1].type == op && tokens[Num - 1].type2 == divide)
                    {
                        if (type != Int)
                        {
                            std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected integer" << std::endl;
                            return node;
                        }
                        else if (type == Int)
                        {
                            if (token.type2 != Int)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected integer" << std::endl;
                                return node;
                            }
                            node.type = integer;
                            node.lexeme = std::to_string(std::stoi(node.lexeme) / std::stoi(token.lexeme));
                        }
                    }
                    else if (tokens[Num - 1].type == op && tokens[Num - 1].type2 == percent)
                    {
                        if (type != Int)
                        {
                            std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected integer" << std::endl;
                            return node;
                        }
                        else if (type == Int)
                        {
                            if (token.type2 != Int)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected integer" << std::endl;
                                return node;
                            }
                            node.type = integer;
                            node.lexeme = std::to_string(std::stoi(node.lexeme) % std::stoi(token.lexeme));
                        }
                    }
                    else if (tokens[Num - 1].type == op && tokens[Num - 1].type2 == times)
                    {
                        if (type != Int)
                        {
                            std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected integer" << std::endl;
                            return node;
                        }
                        else if (type == Int)
                        {
                            if (token.type2 != Int)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected integer" << std::endl;
                                return node;
                            }
                            node.type = integer;
                            node.lexeme = std::to_string(std::stoi(node.lexeme) * std::stoi(token.lexeme));
                        }
                    }
                    // for expression || && == !=
                    else if (tokens[Num - 1].type == op && tokens[Num - 1].type2 == d_eq)
                    {
                        if (type != Int && type != String && type != boolean)
                        {
                            std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected literal or boolean" << std::endl;
                            return node;
                        }
                        // cek jika yg di compare adalah sama tidak seperti ini 1 == "string" atau "string" == 1 atau seterusnya
                        if (type == Int)
                        {
                            if (token.type2 != Int)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Cannot compare integer with string or boolean" << std::endl;
                                return node;
                            }
                            node.type = boolean;
                            if (std::stoi(node.lexeme) == std::stoi(token.lexeme))
                            {
                                node.lexeme = "true";
                            }
                            else
                            {
                                node.lexeme = "false";
                            }
                        }
                        else if (type == String)
                        {
                            if (token.type2 != String)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Cannot compare string with integer or boolean" << std::endl;
                                return node;
                            }
                            node.type = boolean;
                            if (node.lexeme == token.lexeme)
                            {
                                node.lexeme = "true";
                            }
                            else
                            {
                                node.lexeme = "false";
                            }
                        }
                        else if (type == boolean)
                        {
                            if (token.type2 != boolean)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Cannot compare boolean with integer or string" << std::endl;
                                return node;
                            }
                            node.type = boolean;
                            if (node.lexeme == token.lexeme)
                            {
                                node.lexeme = "true";
                            }
                            else
                            {
                                node.lexeme = "false";
                            }
                        }
                        
                    }
                    else if (tokens[Num - 1].type == op && tokens[Num - 1].type2 == nteq)
                    {
                        if (type != Int && type != String && type != boolean)
                        {
                            std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected literal or boolean" << std::endl;
                            return node;
                        }
                        // cek jika yg di compare adalah sama tidak seperti ini 1 == "string" atau "string" == 1 atau seterusnya
                        if (type == Int)
                        {
                            if (token.type2 != Int)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Cannot compare integer with string or boolean" << std::endl;
                                return node;
                            }
                            node.type = boolean;
                            if (std::stoi(node.lexeme) != std::stoi(token.lexeme))
                            {
                                node.lexeme = "true";
                            }
                            else
                            {
                                node.lexeme = "false";
                            }
                        }
                        else if (type == String)
                        {
                            if (token.type2 != String)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Cannot compare string with integer or boolean" << std::endl;
                                return node;
                            }
                            node.type = boolean;
                            if (node.lexeme != token.lexeme)
                            {
                                node.lexeme = "true";
                            }
                            else
                            {
                                node.lexeme = "false";
                            }
                        }
                        else if (type == boolean)
                        {
                            if (token.type2 != boolean)
                            {
                                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Cannot compare boolean with integer or string" << std::endl;
                                return node;
                            }
                            node.type = boolean;
                            if (node.lexeme != token.lexeme)
                            {
                                node.lexeme = "true";
                            }
                            else
                            {
                                node.lexeme = "false";
                            }
                        }
                        
                    }
                    
                    else
                    {
                        std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Unknown operator " << "(" << token.lexeme << ")" << std::endl;
                        return node;
                    }
                }
            }
            ++Num;
            ++Num;
            
        }
        return node;
    }
    /* Variable */
    VariableNode var_()
    {
        VariableNode node;
        Token token_b = token;
        token = lexer_.getNextToken();
        if (token_b.type == identifier)
        {
            if (token.type == op && token.type2 == eq){
                ExpressionNode expression = expression_();
                if (expression.type == str)
                {
                    node.type = kw_String;
                    node.name = token_b.lexeme;
                    node.value_str = expression.lexeme;
                    VAR_STR[token_b.lexeme] = expression.lexeme;
                }
                else if (expression.type == integer)
                {
                    node.type = kw_Int;
                    node.name = token_b.lexeme;
                    node.value_int = std::stoi(expression.lexeme);
                    VAR_INT[token_b.lexeme] = std::stoi(expression.lexeme);
                }
                else if (expression.type == boolean)
                {
                    node.type = kw_Bool;
                    node.name = token_b.lexeme;
                    if (expression.lexeme == "true")
                    {
                        node.value_bool = true;
                    }
                    else
                    {
                        node.value_bool = false;
                    }
                    VAR_BOOL[token_b.lexeme] = node.value_bool;
                }else{
                    std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected expression" << std::endl;
                    return node;
                }
            }else{
                std::cout << "Error at line " << std::to_string(token.line) << ":" << std::to_string(token.column) << " : Expected operator" << std::endl;
                return node;
            }
        }
        return node;
    }
};

int main()
{
    std::string input = "out: \"yoo, \" + \"Hello\"";
    Parser parser(input);
    parser.parse();
    return 0;
}

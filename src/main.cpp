#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <ctype.h>

enum TokenType {
    _return,
    _int,
    semi
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& str) {
    std::vector<Token> tokens;
    std::string buf; 

    for (int i = 0; i < str.length(); i++) {
        // std::cout << i << std::endl;
        if (std::isalnum(str.at(i))) {
            buf.push_back(str.at(i));
            i++;

            while (isalnum(str.at(i)) && i < str.length()) {
                buf.push_back(str.at(i));
                i++;
            }
            i--;          
            if (buf == "return") {
                Token token = {.type =  _return, .value = ""};
                tokens.push_back(token);
                buf.clear();
            } 
        continue;
        }
        // TODO: debug to remove hardcoding
        else if (i == 6) {
            buf.push_back(str.at(i));
            i++;

            while (std::isdigit(str.at(i)) && i < str.length()) {
                buf.push_back(str.at(i));
                i++;
            }
            i--;

            Token token = {.type = _int, .value = buf};
            tokens.push_back(token);
            buf.clear();
            continue;
        } 
        else if (str.at(i) == ';') {
            Token token = {.type = semi, .value = ""};
            tokens.push_back(token);
            continue;
        }
        else if (isspace(str.at(i))) {
            continue;
        }
    }
    return tokens;
}


std::string tokens_to_asm(const std::vector<Token>& tokens) {
    std::stringstream out;
    out <<  "global _start\nstart:\n";
    for (int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens[i];
        if (token.type == _return) {
            if (i+ 1 < tokens.size() && tokens.at(i+1).type == _int) {
                if (i+ 2 < tokens.size() && tokens.at(i+2).type == semi) {
                    out << "    mov rax, 60 \n";
                    out << "    mov rdi, " << tokens.at(i+1).value << "\n"; 
                    out << "    syscall";
                }
            }
        }
    }
    return out.str();
}



int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "specify custom compiler" << std::endl;
        return EXIT_FAILURE;
    } 

    std::fstream input(argv[1], std::ios::in);
    std::stringstream file_stream;
    file_stream << input.rdbuf();
    std::string f = file_stream.str();

    std::vector<Token> t = tokenize(f);

    std::fstream outfile ("../out.asm", std::ios::out);
    outfile << tokens_to_asm(t);

    std::cout << "Hello World!" << std::endl;
    return EXIT_SUCCESS;
}
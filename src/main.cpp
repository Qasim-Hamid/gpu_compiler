#include "../include/lexer.h"

int main(int argc, char* argv[]) {

    std::string input_string;

    for (int i = 1; i < argc; i++) {
        input_string += argv[i];
        input_string += " ";
    }

    // std::string test_str = "kernel@";
    // std::vector<Token> token_list = process_input(test_str);
    std::vector<Token> token_list = process_input(input_string);
    printList(token_list);

    return 0;
}

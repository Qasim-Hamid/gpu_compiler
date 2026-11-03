#include "../include/lexer.h"

int main(int argc, char* argv[]) {

    std::string input_string;

    for (int i = 1; i < argc; i++) {
        input_string += argv[i];
        input_string += " ";
    }

    // std::vector<std::string> all_args = {"123", "3.14", "0", "1+2", 
    // "-5", "3.14.5", "12abc", "1.", "@123", ""};

    std::vector<std::string> all_args = {"1+2", "-5", "a + b * 2", "12+3.5*2", "1%2", "2/1.3"};

    for (const std::string& arg : all_args) {
        std::cout << "\n";
        std::cout << "arg: " << arg << "\n";
        std::vector<Token> token_list = process_input(arg);
        printList(token_list);
    }
    // std::string arg = "1+2";
    // std::vector<Token> token_list = process_input(arg);
    // printList(token_list);

    // std::string test_str = "123.";
    // std::vector<Token> token_list = process_input(test_str);
    // std::vector<Token> token_list = process_input(input_string);
    // printList(token_list);

    return 0;
}

#include "../include/lexer.h"

int main(int argc, char* argv[]) {

    TokenValue v = true;

    std::string input_string;

    for (int i = 1; i < argc; i++) {
        input_string += argv[i];
        if ((i + 1) < argc) {
            input_string += " ";
        }
    }
    std::vector<Token> token_list = process_input(input_string);
    printList(token_list);
    // std::vector<std::string> str_list = {"int x;", "float* ptr;", 
    // "void foo()", "shared float data[256];", "global int* output;",
    // "kernel", "kernel123", "float32", "vector_add(a,b);", 
    // "()", "{}", ""};

    // for (const std::string& arg: str_list) {
    //     std::cout << "\narg: " << arg << "\n";
    //     std::vector<Token> token_list = process_input(arg);
    //     printList(token_list);
    // }
    // std::string test_str = "123.";
    // std::vector<Token> token_list = process_input(test_str);

    return 0;
}

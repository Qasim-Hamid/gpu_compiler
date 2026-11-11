#include "../include/lexer.h"
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]) {

    TokenValue v = true;

    std::string input_string;

    for (int i = 1; i < argc; i++) {
        input_string += argv[i];
        if ((i + 1) < argc) {
            input_string += " ";
        }
    }
    std::string filename = "cmdline_input";

    std::vector<Token> token_list = process_input(input_string);
    bool has_error = false;
    printList(token_list, filename, &has_error);

    //
    // std::string filename = argv[1];
    // std::ifstream file(filename);

    // if (!file.is_open()) {
    //     std::cerr << "Error: Couldn't open file:  '" << filename << "'\n";
    // }

    // std::stringstream buffer;
    // buffer << file.rdbuf();

    // std::string raw_text = buffer.str();
    
    // std::vector<Token> token_list = process_input(raw_text);
    // bool has_error = false;
    // printList(token_list, filename, &has_error);


    if (has_error) {
        return 1;
    }
    return 0;
}

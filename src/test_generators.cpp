#include <iostream>
#include "generators.h"

void print_some_output(BruteforceGenerator bg)
{
    StaticVector<char, MAX_PW_LEN>  pw;
    uint32_t                        same_chars;

    while (!bg.isDone()) {
        bg.next(&pw, &same_chars);
        for (uint32_t j = 0; j < pw.size(); j++) {
            std::cout << pw[j];
        }
        std::cout << " " << same_chars << "\n";
    }
    std::cout << "\n";
}

StaticVector<char, 256> string_to_array(const char *s)
{
    StaticVector<char, 256>  r;

    uint32_t           i;
    for (i = 0; s[i] != '\0'; s++) {
        r.push_back(s[i]);
    }
    return r;
}

int main(int argc, char *argv[])
{
    std::cout << "bg(ABC, 0, 10)\n";
    print_some_output(BruteforceGenerator(string_to_array("ABC"), 0, 10));

    std::cout << "bg(ABC, 2, 4)\n";
    print_some_output(BruteforceGenerator(string_to_array("ABC"), 2, 4));

    return 0;
}


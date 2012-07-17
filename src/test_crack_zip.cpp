#include "zip_crack.h"
#include <iostream>


int main (int   argc,
          char *argv[])
{
    ZipStreamCipher zsc = ZipStreamCipher::initialState();

    std::cout << "Initial state: " << zsc << "\n";

    zsc.advanceOneByte('A');

    std::cout << "State after character A processed: " << zsc << "\n";
    std::cout << "Keystream byte: " << (uint32_t)zsc.getKeystreamByte() << "\n";

    return 0;
}

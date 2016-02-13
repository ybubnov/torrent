#include <vector>
#include <iostream>

#include "bencode.h"


int main(int argc, char **argv)
{
    std::vector<char> vect;
    auto provider = new bencode::provider::vector();
    auto consumer = new bencode::consumer::vector();

    std::cout << provider << std::endl;

    return 0;
}

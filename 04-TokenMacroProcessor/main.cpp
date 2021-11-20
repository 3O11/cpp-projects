#include <iostream>

#include "MacroProcessor.h"

int main(int argc, char ** argv)
{
    MacroMap m;
    if (argc > 1)
    {
        std::string token = argv[1];
        std::string body = "";
        for(int i = 2; i < argc; ++i)
        {
            body.append(argv[i]);
            if (i != argc - 1) body.append(" ");
        }
        m[token] = body;
    }

    MacroProcessor::Process(std::cin, std::cout, std::move(m));
}
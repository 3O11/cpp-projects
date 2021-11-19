#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "MacroProcessor.h"

int main(int argc, char ** argv)
{
    MacroProcessor p;

    if (argc > 1)
    {
        std::string identifier = argv[1];
        std::string body = "";
        for(int i = 2; i < argc; ++i)
        {
            body.append(argv[i]);
            if (i != argc - 1) body.append(" ");
        }
    }

    char ch;
	while ((ch = getchar()) != EOF) p << ch;
    p.Finish();
}
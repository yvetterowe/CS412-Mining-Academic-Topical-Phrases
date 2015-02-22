//
//  main.cpp
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 10/18/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#include <iostream>
#include <string>
#include "TextFpMining.h"

int main(int argc, const char * argv[])
{
    std::string data = "paper.txt";
    std::string dict= "vocab.txt";
    std::string title = "title.txt";
    std::string lda = "word-assignments.dat";

    TextFpMining test(data, dict, title, lda, 5);
    test.Run();
    
    return 0;
}


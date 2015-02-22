//
//  FrequentMining.c
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 11/3/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#include "TextFpMining.h"

#include "FPTree.h"
#include "FPGrowthAlgorithm.h"


//Step 4: Mining Frequent Patterns for Each Topic
//Step 5: Mining Maximal/Closed Patterns
void TextFpMining::MineFrequentPatterns() {
    
    //int minSupport = 100;
    
    for(int i = 0; i < topicNum; ++i) {
        std::string topicFile = "topic-" + std::to_string(i)+ ".txt";
        std::string patternFile = "patterns/pattern-" + std::to_string(i)+ ".txt";
        std::string closedFile = "closed/closed-" + std::to_string(i) + ".txt";
        std::string maxFile = "max/max-" + std::to_string(i) + ".txt";
        int minSupport = 0.01 * TextFpMining::D1d[i];
        FPGrowthAlgorithm fpGrowth(minSupport, topicFile, patternFile,closedFile, maxFile);
        fpGrowth.Run();
    }
    
}

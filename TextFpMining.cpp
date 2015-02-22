//
//  TextFpMining.cpp
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 10/24/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#include "TextFpMining.h"

#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>

#include "FPTree.h"
#include "FPGrowthAlgorithm.h"
#include "RankAlgorithm.h"


//initialize static member variables
std::map<std::string,int> TextFpMining::dict;
std::map<int, std::string> TextFpMining::reverseDict;
int TextFpMining::D2d[MAX][MAX];
int TextFpMining::D1d[MAX];


void TextFpMining::Run() {
    
    //Step 2: Preprocessing
    Preprocess();
    
    //Step 3: Partitioning
    //OrganizeTerms();
    
    //Step 4: Mining Frequent Patterns for Each Topic
    //Step 5: Mining Maximal/Closed Patterns
   // MineFrequentPatterns();
    
    //Step 6: Re-rank by Purity of Patterns
    //Step 7: Bonus(improve ranking quality)
   // Rank();
}
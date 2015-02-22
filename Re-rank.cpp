//
//  Re-rank.c
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 11/3/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#include "TextFpMining.h"

#include "RankAlgorithm.h"


//Step 6: Re-rank by Purity of Patterns
//Step 7: Bonus (improve ranking quality)
void TextFpMining::Rank() {
    
    RankAlgorithm rankAlgo(topicNum);
    
    //just purity
    rankAlgo.Run(0.0f, 0.0f, true, true);
    
    /**************tests below are for BONUS***************/
    /******please run it to see the comparison result******/
    //purity, phraseness
    rankAlgo.Run(0.0f, 0.5f);
    
    //purity, completeness
    rankAlgo.Run(0.5f, 0.0f);
    
    //purity, completeness, phraseness
    rankAlgo.Run(0.5f, 0.5f);
}


//
//  FPTree.h
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 10/21/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#ifndef __cs412_text_fp_mining__FPTree__
#define __cs412_text_fp_mining__FPTree__

#include <iostream>
#include <string>
#include <map>

#include "FPNode.h"


class FPTree {
public:
    
    FPNode*                             root;
    std::vector<std::pair<int, int>>    headerList;
    std::map<int, FPNode*>              headerNodeLink;
    bool                                containSinglePath;
    
    FPTree():containSinglePath(true){
        root = new FPNode();
    }
    
    void AddTransaction(std::vector<std::pair<int, int>>& transaction);
    void AddPrefixPath(std::vector<FPNode*>& prefixPath, std::map<int,int>& oneItemSetBeta, int minSupport);
    void CreateHeaderList(std::map<int,int> oneItemSet);
};

#endif /* defined(__cs412_text_fp_mining__FPTree__) */

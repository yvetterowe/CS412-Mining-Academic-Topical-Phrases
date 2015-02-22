//
//  FPNode.h
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 10/21/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#ifndef __cs412_text_fp_mining__FPNode__
#define __cs412_text_fp_mining__FPNode__

#include <iostream>
#include <vector>

class FPNode {
public:
    FPNode*                 parent;
    std::vector<FPNode*>    children;
    FPNode*                 nodeLink;
    int                     count;
    int                     data;
    
    FPNode():parent(NULL), nodeLink(NULL), count(1){
        children.clear();
    }
    
    FPNode* GetChildByData(int _data);
};

#endif /* defined(__cs412_text_fp_mining__FPNode__) */

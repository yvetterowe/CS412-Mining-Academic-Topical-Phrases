//
//  FPNode.cpp
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 10/21/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#include "FPNode.h"

FPNode* FPNode::GetChildByData(int _data) {
    for(int i = 0; i < children.size(); ++i) {
        if(children[i]->data == _data){
            return children[i];
        }
    }
    return NULL;
}
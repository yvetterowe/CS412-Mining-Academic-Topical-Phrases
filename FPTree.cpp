//
//  FPTree.cpp
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 10/21/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#include "FPTree.h"

void FPTree::AddTransaction(std::vector<std::pair<int, int>>& transaction){
    
    FPNode* currParent = root;
    
    for(int i = 0; i < transaction.size(); ++i) {
        int item = transaction[i].first;
        FPNode* currNode = currParent->GetChildByData(item);
        
        //if currNode doesn't exist, insert it into the tree
        if(currNode == NULL){
            
            //create a new node and set data & parent
            FPNode* newNode = new FPNode();
            newNode->data = item;
            newNode->parent = currParent;
            
            //link new node to its parent
            currParent->children.push_back(newNode);
            
            //check single/multiple paths
            if(containSinglePath && currParent->children.size() > 1) {
                containSinglePath = false;
            }
            
            //replace parent with the new node for next iteration
            currParent = newNode;
            
            //update headerlist
            if(headerNodeLink.find(item) == headerNodeLink.end()) {
                //if not, create new pair (int node) and insert it into map
                headerNodeLink[item] = newNode;
            } else{
                //otherwise, update the corresponding link
                FPNode* currLink = headerNodeLink[item];
                while(currLink->nodeLink != NULL) {
                    currLink = currLink->nodeLink;
                }
                currLink->nodeLink = newNode;
            }
        } else {
            ++currNode->count;
            currParent = currNode;
        }
    }
}

void FPTree::AddPrefixPath(std::vector<FPNode*>& prefixPath,
                           std::map<int,int>& oneItemSetBeta,
                           int minSupport) {
    FPNode* currParent = root;
    int pathSupport = prefixPath[0]->count;
    
    for(int i = prefixPath.size() - 1; i >= 1; --i) {
        int item = prefixPath[i]->data;
        
        if(oneItemSetBeta[item] < minSupport) {
            continue;
        }
        
        FPNode* currNode = currParent->GetChildByData(item);
        
        if(currNode == NULL) {
            
            FPNode* newNode = new FPNode();
            newNode->data = item;
            newNode->parent = currParent;
            newNode->count = pathSupport;
            
            currParent->children.push_back(newNode);
            
            //check single/multiple paths
            if(containSinglePath && currParent->children.size() > 1) {
                containSinglePath = false;
            }
            
            currParent = newNode;
            
            //update headerlist
            if(headerNodeLink.find(item) == headerNodeLink.end()) {
                headerNodeLink[item] = newNode;
            } else {
                FPNode* currLink = headerNodeLink[item];
                while(currLink->nodeLink != NULL) {
                    currLink = currLink->nodeLink;
                }
                currLink->nodeLink = newNode;
            }
        } else {
            currNode->count += pathSupport;
            currParent = currNode;
        }
    }
}


bool decreasingByValue(std::pair<int, int> a, std::pair<int, int> b){
    return a.second > b.second;
}

void FPTree::CreateHeaderList(std::map<int,int> oneItemSet) {
    headerList = std::vector<std::pair<int,int>>(oneItemSet.begin(), oneItemSet.end());
    sort(headerList.begin(), headerList.end(), decreasingByValue);
}





//
//  FPGrowthAlgorithm.cpp
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 10/21/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#include "FPGrowthAlgorithm.h"

#include <sstream>
#include <fstream>

#include "TextFpMining.h"

//scan database for the first time
//find all item-support pairs
void FPGrowthAlgorithm::createOneItemSet() {
    std::ifstream inputStream(dataFile);
    std::string transaction;
    
    while(getline(inputStream, transaction)) {
        std::stringstream tranStream(transaction);
        int item;
        
        while(tranStream >> item) {
            ++oneItemSet[item];
        }
    }
}

bool decreasingByValue1(std::pair<int, int> a, std::pair<int, int> b){
    return a.second > b.second;
}

void FPGrowthAlgorithm::addFPtoOutput(int support, std::vector<int>& fq) {
    //sort for later subset use
    std::sort(fq.begin(),fq.end());
    
    if(outputFPs.find(support) == outputFPs.end()) {
        std::vector<std::vector<int>> fqs;
        fqs.push_back(fq);
        outputFPs[support] = fqs;
    } else {
        outputFPs[support].push_back(fq);
    }
}

void FPGrowthAlgorithm::fpGrowthSinglePath(FPNode* node, std::vector<int>& alpha) {
    // Concatenate the node item to the current prefix
    std::vector<int> beta(alpha);
    beta.push_back(node->data);
    
    //output pattern
    addFPtoOutput(node->count, beta);
        
    //create all combinations recursively 
    if(node->children.size() != 0) {
        fpGrowthSinglePath(node->children[0], beta);
        fpGrowthSinglePath(node->children[0], alpha);
    }
}

void FPGrowthAlgorithm::fpGrowth(FPTree* tree, std::vector<int>& alpha, int minSupport, std::map<int, int>& itemSupport){
    if(tree->containSinglePath) {
        fpGrowthSinglePath(tree->root->children[0], alpha);
    } else {
        //fpGrowth(tree, alpha, minSupport, itemSupport);
        std::vector<std::pair<int, int>> headerList = tree->headerList;
        
        //in reversing order (from the minimum support)
        for(int i = headerList.size() - 1; i >=0; --i) {
            int item = headerList[i].first;
            int support = headerList[i].second;
            
            if(support < minSupport) {
                continue;
            }
            
            //for each item ai in headerlist generate pattern beta = ai U alpha
            std::vector<int> beta(alpha);
            beta.push_back(item);
            
            //output pattern
            addFPtoOutput(support, beta);
            
            //construct beta's conditional pattern base
            std::vector< std::vector<FPNode*> > prefixPaths;
            FPNode* suffixItem = tree->headerNodeLink[item];
            while(suffixItem != NULL) {
                
                if(suffixItem->parent->parent != NULL) {
                    std::vector<FPNode*> prefixPath;
                    
                    FPNode* curr = suffixItem;
                    while(curr->parent != NULL) {
                        prefixPath.push_back(curr);
                        curr = curr->parent;
                    }
                    prefixPaths.push_back(prefixPath);
                }
                suffixItem = suffixItem->nodeLink;
            }
            
            //compute each item's frequency in each conditional base
            std::map<int, int> oneItemSetBeta;
            for(int i = 0; i < prefixPaths.size(); ++i) {
                std::vector<FPNode*> prefixPath = prefixPaths[i];
                int pathSupport = prefixPath[0]->count;
                
                for(int j = 1; j < prefixPath.size(); ++j) {
                    FPNode* curr = prefixPath[j];
                    if(oneItemSetBeta.find(curr->data) == oneItemSetBeta.end()) {
                        oneItemSetBeta[curr->data] = pathSupport;
                    } else {
                        oneItemSetBeta[curr->data] += pathSupport;
                    }
                }
            }
            
            //construct conditional fp-tree
            FPTree treeBeta;
            for(int i = 0; i < prefixPaths.size(); ++i) {
                treeBeta.AddPrefixPath(prefixPaths[i],oneItemSetBeta, minSupport);
            }
            treeBeta.CreateHeaderList(oneItemSetBeta);
            
            //mine the conditional fp-tree recursively
            if(treeBeta.root->children.size() > 0) {
                fpGrowth(&treeBeta, beta, minSupport, oneItemSetBeta);
            }
        }
    }
}

void FPGrowthAlgorithm::createFPTree(FPTree& tree){
    std::ifstream inputStream(dataFile);
    std::string transaction;
    
    while(getline(inputStream, transaction)) {
        std::vector<std::pair<int,int>> transVec;
        std::stringstream tranStream(transaction);
        int item;
        
        while(tranStream >> item) {
            //directly prune item whose support < min_support
            if(oneItemSet[item] >= minSupport){
                std::pair<int,int> currItem (item, oneItemSet[item]);
                transVec.push_back(currItem);
            }
        }
        
        //sort words by decreasing order
        std::sort(transVec.begin(), transVec.end(), decreasingByValue1);
        
        //add each sorted title into titles
        tree.AddTransaction(transVec);
    }

    //generate header list
    tree.CreateHeaderList(oneItemSet);
}

void FPGrowthAlgorithm::outputResult() {
    
    //output fqs and closed patterns
    std::ofstream os(outputFpFile); std::ofstream osp(outputFpFile + ".phrase");
    std::ofstream os1(outputClosedFpFile); std::ofstream os1p(outputClosedFpFile + ".phrase");
    
    typedef std::map<int, std::vector<std::vector<int>>>::iterator iter;
    
    for(iter it = outputFPs.begin(); it != outputFPs.end(); ++it) {
       
        int support = it->first;
        std::vector<std::vector<int>> fps = it->second;
        
        for(int i = 0; i < fps.size(); ++i) {
            std::vector<int> fp = fps[i];
            bool isClosedFp = (!isSubset(fp, fps));
            
            if(isClosedFp) {
                if(outputClosedFPs.find(support) == outputFPs.end()) {
                    std::vector<std::vector<int>> closedFps;
                    closedFps.push_back(fp);
                    outputClosedFPs[support] = closedFps;
                } else {
                    outputClosedFPs[support].push_back(fp);
                }
                closedFPs.push_back(fp);
            }
            
            osp<<support<<" "; os<<support<<" ";
            if(isClosedFp) {
                os1p<<support<<" ";
                os1<<support<<" ";
            }
            
            for(int j = 0; j < fp.size(); ++j) {
                osp<<TextFpMining::reverseDict[fp[j]]<<" ";
                os<<fp[j]<<" ";
                if(isClosedFp) {
                    os1p<<TextFpMining::reverseDict[fp[j]]<<" ";
                    os1<<fp[j]<<" ";
                }
            }
            osp<<std::endl; os<<std::endl;
            if(isClosedFp) {
                os1p<<std::endl;
                os1<<std::endl;
            }
        }
    }
    
    //output max patterns
    std::ofstream osMax(outputMaxFpFile);
    std::ofstream osMaxp(outputMaxFpFile + ".phrase");
    for(iter it = outputClosedFPs.begin(); it != outputClosedFPs.end(); ++it) {
        
        int support = it->first;
        std::vector<std::vector<int>> fps = it->second;
        
        for(int i = 0; i < fps.size(); ++i) {
            std::vector<int> fp = fps[i];
            bool isMaxFp = (!isSubset(fp, closedFPs));
            if(isMaxFp) {
                osMaxp<<support<<" "; osMax<<support<<" ";
                for(int j = 0; j < fp.size(); ++j) {
                    osMaxp<<TextFpMining::reverseDict[fp[j]]<<" ";
                    osMax<<fp[j]<<" ";

                }
                osMaxp<<std::endl;
                osMax<<std::endl;

            }
        }
    }
}

void FPGrowthAlgorithm::Run() {
    
    createOneItemSet();
    
    FPTree tree;
    createFPTree(tree);
        
    //mining fp-tree recursively
    std::vector<int> alpha;
    fpGrowth(&tree, alpha, minSupport, oneItemSet);
    
    //output result patterns
    outputResult();
}

bool FPGrowthAlgorithm::isSubset(std::vector<int>& fq, std::vector<std::vector<int>>& fqs) {
    for(int i = 0; i < fqs.size(); ++i) {
        if(std::includes(fqs[i].begin(), fqs[i].end(), fq.begin(), fq.end())) {
            if(fq != fqs[i]) {
                return true;
            }
        }
    }
    return false;
}

//
//  FPGrowthAlgorithm.h
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 10/21/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#ifndef __cs412_text_fp_mining__FPGrowthAlgorithm__
#define __cs412_text_fp_mining__FPGrowthAlgorithm__

#include <iostream>

#include "FPTree.h"

struct cmpByDecreasingSupport {
    bool operator()(int a, int b) const {
        return a > b;
        }
};
        

class FPGrowthAlgorithm {
public:
    FPGrowthAlgorithm(int _minSupport, std::string& _dataFile,
                      std::string& _outputFpFile, std::string& _outputClosedFpFile,
                      std::string& _outputMaxFpFile) {
        minSupport = _minSupport;
        dataFile = _dataFile;
        outputFpFile = _outputFpFile;
        outputClosedFpFile = _outputClosedFpFile;
        outputMaxFpFile = _outputMaxFpFile;
    }
    
    void Run();
    
    std::map<int,int> oneItemSet;

private:
    int             minSupport;
    std::string     dataFile;
    std::string     outputFpFile;
    std::string     outputClosedFpFile;
    std::string     outputMaxFpFile;
    
    std::map<int, std::vector<std::vector<int>>, cmpByDecreasingSupport> outputFPs;
    std::map<int, std::vector<std::vector<int>>, cmpByDecreasingSupport> outputClosedFPs;
    std::vector<std::vector<int>> closedFPs;
    
    void createOneItemSet();
    
    void createFPTree(FPTree& tree);
    
    void fpGrowth(FPTree* tree, std::vector<int>& alpha, int minSupport, std::map<int, int>& itemSupport);
    
    void fpGrowthSinglePath(FPNode* node, std::vector<int>& alpha);
        
    void addFPtoOutput(int support, std::vector<int>& fq);
    
    bool isSubset(std::vector<int>& fq, std::vector<std::vector<int>>& fqs);
    
    void outputResult();
    
};

#endif /* defined(__cs412_text_fp_mining__FPGrowthAlgorithm__) */

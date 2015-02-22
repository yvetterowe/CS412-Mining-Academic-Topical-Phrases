//
//  PurityRankAlgorithm.h
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 10/26/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#ifndef __cs412_text_fp_mining__RankAlgorithm__
#define __cs412_text_fp_mining__RankAlgorithm__

#include <iostream>
#include <vector>
#include <string>
#include <map>

struct cmpByDecreasingSupport2 {
    bool operator()(int a, int b) const {
        return a > b;
    }
};

class RankAlgorithm {
    
public:
    RankAlgorithm(int _topicNum): topicNum(_topicNum) {}
    
    void Run(float r, float _w, bool optimizeRanking = false, bool outputPurity = false);
    
private:
    
    typedef std::vector<int> pattern;
    typedef std::map<pattern, int> patternSupportMap;
    
    typedef std::map<int, std::vector<pattern>> dimPatternsMap;
    typedef std::map<int, dimPatternsMap, cmpByDecreasingSupport2> supportDimMapMap;
    
    int topicNum;

    std::vector<patternSupportMap> patternSupportMaps;
    std::vector<supportDimMapMap> supportDimPatternsMaps;
    
    void createSupportMaps(bool optimizeRanking);
    
    //ranking measures
    double coverage(pattern p, int t);
    double purity(pattern p, int t);
    double phraseness(pattern p, int t);
    double completeness(pattern p, int t);
    
    bool existSuperPattern(pattern p, std::vector<pattern>& patterns);
    
    struct rankOutput {
        double purity;
        double rankMeasure;
        pattern p;
        
        rankOutput(double _pu, double _r, pattern& _p):
        purity(_pu), rankMeasure(_r){
            p = _p;
        }
    };
    
    struct {
        bool operator()(rankOutput& a, rankOutput& b)
        {
            return a.rankMeasure > b.rankMeasure;
        }
    } cmpMeasure;
};

#endif /* defined(__cs412_text_fp_mining__PurityRankAlgorithm__) */

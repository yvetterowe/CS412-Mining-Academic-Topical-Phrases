//
//  PurityRankAlgorithm.cpp
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 10/26/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#include "RankAlgorithm.h"

#include <fstream>
#include <sstream>
#include <cmath>

#include "TextFpMining.h"


void RankAlgorithm::createSupportMaps(bool optimizeRanking) {
    
    //just generate maps once :)
    if(!patternSupportMaps.empty()) return;
    if(!supportDimPatternsMaps.empty()) return;
    
    for(int i = 0; i < topicNum; ++i) {
        patternSupportMap currMap;
        supportDimMapMap currReverseMap;
        
        std::string patternFile = "patterns/pattern-" + std::to_string(i)+ ".txt";
        std::ifstream inputStream(patternFile);
        
        std::string patternSupport;
        while(getline(inputStream, patternSupport)) {
            pattern currPattern;
            int support;
            
            std::stringstream psStream(patternSupport);
            int item;
            
            psStream >> support;
            while(psStream >> item) {
                currPattern.push_back(item);
            }
            
            //construct pattern -> support map
            currMap[currPattern] = support;
            
            if(optimizeRanking) {
                
                //construct support -> dim -> pattern map
                //sort in support descending order,
                //so that we can find prune more potential superpatterns efficiently
                int dim = currPattern.size();
                if(currReverseMap.find(support) == currReverseMap.end()) {
                    dimPatternsMap newDpm;
                    
                    std::vector<pattern> newPatterns;
                    newPatterns.push_back(currPattern);
                    newDpm[dim] = newPatterns;
                    
                    currReverseMap[support] = newDpm;
                    
                } else{
                    dimPatternsMap& currDpm = currReverseMap[support];
                    
                    if(currDpm.find(dim) == currDpm.end()) {
                        std::vector<pattern> currDimPatterns;
                        currDimPatterns.push_back(currPattern);
                        
                        currDpm[dim] = currDimPatterns;
                    } else {
                        std::vector<pattern>& dpmv = currDpm[dim];
                        dpmv.push_back(currPattern);
                    }
                }
            }
        }
    
        patternSupportMaps.push_back(currMap);
        supportDimPatternsMaps.push_back(currReverseMap);
    }
}

double RankAlgorithm::coverage(pattern p, int t) {
    return (double) patternSupportMaps[t][p] /  TextFpMining::D1d[t];
}

double RankAlgorithm::purity(pattern p, int t) {
    
    //purity(p,t)=log [ f(t,p) / | D(t) | ] - log (max [ ( f(t,p) + f(t’,p) ) / | D(t,t’) | ] )
    
    std::vector<double> probInOtherTopics;
    probInOtherTopics.clear();
    int ftp = patternSupportMaps[t][p];
    
    for(int i = 0; i < topicNum; ++i) {
        if(i != t) {
            int dtt = i < t ? TextFpMining::D2d[i][t] : TextFpMining::D2d[t][i];
            int ft1p = patternSupportMaps[i].find(p) == patternSupportMaps[i].end()?0:patternSupportMaps[i][p];
            double prob = (double)(ftp + ft1p) / dtt;
            
            probInOtherTopics.push_back(prob);
        }
    }
    
    return (log(coverage(p, t)) - log(*std::max_element(probInOtherTopics.begin(), probInOtherTopics.end()))) / log(2);
}

double RankAlgorithm::phraseness(pattern p, int t){
    
    //phrasesness(p,t) = log [ f(t,p) / | D(t) | ] - sum(log[ f(t,w) / | D(t) | ])
   
    double indieSum = 0;
    
    for(int i = 0; i < p.size(); ++i) {
        pattern w;
        w.push_back(p[i]);
        if(patternSupportMaps[t].find(w) != patternSupportMaps[t].end()) {
            indieSum += log(coverage(w, t)) / log(2);
        }
    }
    
    return log(coverage(p,t)) / log(2) - indieSum;
}


bool RankAlgorithm::existSuperPattern(pattern p, std::vector<pattern> &patterns){
    for(int i = 0; i < patterns.size(); ++i) {
        if(std::includes(patterns[i].begin(), patterns[i].end(), p.begin(), p.end())) {
            if(p != patterns[i]) {
                return true;
            }
        }
    }
    return false;
}


double RankAlgorithm::completeness(pattern p, int t) {
    
    //completeness(p,t) = 1 - max(f(t, (pUw))) / f(t,p)
    
    double completeness = 1.0;
    int dim = p.size();
    int support = patternSupportMaps[t][p];
    supportDimMapMap currReverseMap = supportDimPatternsMaps[t];
    
    for(supportDimMapMap::iterator itt = currReverseMap.find(support); itt != currReverseMap.end(); ++itt){
        
        std::map<int, std::vector<pattern>>& dimPatterns = itt->second;
        
        typedef std::map<int, std::vector<pattern>>::iterator iter;
        
        bool find = false;
        
        //search for super-patterns
        for(iter it = dimPatterns.find(dim + 1); it != dimPatterns.end(); ++it) {
            std::vector<pattern> sameDimPatterns = it->second;
            
            if(existSuperPattern(p, sameDimPatterns)){
                completeness = 1 - (double)itt->first / support;
                find = true;
                break;
            }
        }
        if(find){
            break;
        }
    }
    
    return completeness; 
}

void RankAlgorithm::Run(float r, float w, bool optimizeRanking, bool outputPurity) {
    
    createSupportMaps(optimizeRanking);
    
    for(int i = 0; i < topicNum; ++i) {
        
        patternSupportMap currMap = patternSupportMaps[i];
        std::vector<rankOutput> output;
        
        for(patternSupportMap::iterator it = currMap.begin(); it != currMap.end(); ++it) {
            pattern currPattern = it->first;
            int support = it->second;
            
            //compute completeness
            double currCompleteness = 1.0;
            if(r){
                currCompleteness = completeness(currPattern,i);
            }
            
            //compute coverage
            double currCoverage = coverage(currPattern, i);

            //compute purity
            double currPurity = purity(currPattern, i);
            
            //compute phraseness
            double currPhraseness = 0;
            if(w) {
                currPhraseness = phraseness(currPattern, i);
            }
            

            //first, filter by completeness
            if(currCompleteness >= r) {
                
                //then compute combined ranking measure
                double currRankMeasure = currCoverage * ((1 - w) * currPurity + w * currPhraseness);
                
                rankOutput currOutput(currPurity, currRankMeasure, currPattern);

                output.push_back(currOutput);
            }
        }
        
        //sort by combined measures
        std::sort(output.begin(), output.end(), cmpMeasure);
        
        //test differnet ranking & filtering methods
        std::string outputFile;
        if(r == 0.0f) {
            if(w == 0.0f) {
                outputFile = "purity/purity-" + std::to_string(i) + ".txt";
            } else {
                outputFile = "purity-phraseness/purity-phraseness-" + std::to_string(i) + ".txt";
            }
        } else{
            if(w == 0.0f) {
                outputFile = "purity-completeness/purity-completeness-" + std::to_string(i) + ".txt";
            } else {
                outputFile = "purity-phraseness-completeness/purity-phraseness-completeness" + std::to_string(i) + ".txt";
            }
        }
       
        std::ofstream prStreamp(outputFile + ".phrase");
        std::ofstream prStream(outputFile);
        for(int j = 0; j < output.size(); ++j) {
            if(outputPurity) {
                prStreamp << output[j].purity<<" ";
                prStream << output[j].purity<<" ";
                
            } else {
                prStreamp << output[j].rankMeasure<<" ";
                prStream << output[j].rankMeasure<<" ";
            }
            pattern p = output[j].p;
            for(int k = 0; k < p.size(); ++k) {
                prStreamp <<TextFpMining::reverseDict[p[k]]<<" ";
                prStream << p[k]<<" ";
            }
            prStreamp<<std::endl;
            prStream<<std::endl;
        }
    }
}


//
//  Re-organizing.c
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 11/3/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//


#include "TextFpMining.h"

#include <sstream>
#include <fstream>
#include <vector>


void TextFpMining::OrganizeTerms() {
    
    //generate topic-i file output stream;
    std::vector<std::ofstream> topicFileStreams;
    for(int i = 0; i < topicNum; ++i) {
        std::string topicFile = "topic-" + std::to_string(i)+ ".txt";
        topicFileStreams.push_back(std::ofstream(topicFile));
    }
    
    std::ifstream inputStream(ldaFile);
    std::string titleStat;
    
    //scan each title line
    while(getline(inputStream, titleStat)) {
        std::stringstream titleStream(titleStat);
        std::string termStat;
        titleStream >> termStat;
        
        bool hasTopic[topicNum];
        for(int i = 0; i < topicNum; ++i) {
            hasTopic[i] = false;
        }
        
        //organize each term in the current title
        while(titleStream >> termStat) {
            int dpos = termStat.find(":");
            int termId = atoi(termStat.substr(0,dpos).c_str());
            int topicId = atoi(termStat.substr(dpos+1).c_str());
            hasTopic[topicId] = true;
            topicFileStreams[topicId] << termId <<" ";
        }
        
        for(int i = 0; i < topicNum; ++i) {
            if(hasTopic[i]) {
                topicFileStreams[i]<<std::endl;
                
                //Preprocess |D(t)| for step 6
                ++TextFpMining::D1d[i];
            }
        }
        
        //Preprocess |D(t, t')| for step 6
        for(int i = 0; i < topicNum; ++i) {
            for(int j = i + 1; j < topicNum; ++j) {
                if(hasTopic[i] || hasTopic[j]) {
                    ++TextFpMining::D2d[i][j];
                }
            }
        }
    }
}


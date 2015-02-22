//
//  TextFpMining.h
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 10/24/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#ifndef __cs412_text_fp_mining__TextFpMining__
#define __cs412_text_fp_mining__TextFpMining__

#include <iostream>
#include <map>
#include <string>

const int MAX = 5;

class TextFpMining {
    
public:
    TextFpMining(std::string& _dataFile,
                 std::string& _dictFile,
                 std::string& _titleFile,
                 std::string& _ldaFile,
                 int _topicNum):topicNum(_topicNum) {
        dataFile =  _dataFile;
        dictFile = _dictFile;
        titleFile = _titleFile;
        ldaFile = _ldaFile;
    }
    
    void Run();
    
    static std::map<std::string,int> dict;
    static std::map<int, std::string> reverseDict; //for later index->term mapping
    
    static int TopicNum;
    static int D2d[MAX][MAX];
    static int D1d[MAX];
    
private:
    
    void Preprocess();                                          //Step 2: Preprocessing
    void GenerateDict(std::map<std::string,int>& dict);         //Step 2.1 Generate a Dictionary
    void Tokenize(std::map<std::string, int>& dict);            //Step 2.2 Tokenize Plain Text by Dictionary
    
    void OrganizeTerms();                                       //Step 3: Partitioning
    
    void MineFrequentPatterns();                                //Step 4: Mining Frequent Patterns for Each Topic
                                                                //Step 5: Mining Maximal/Closed Patterns
    
    void Rank();                                                //Step 6: Re-rank by Purity of Patterns
                                                                //Step 7: Bonus(improve ranking quality)
    
    
    std::string dataFile;
    std::string dictFile;
    std::string titleFile;
    std::string ldaFile;
    int         topicNum;
};

#endif /* defined(__cs412_text_fp_mining__TextFpMining__) */

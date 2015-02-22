//
//  Preprocessing.c
//  cs412_text-fp-mining
//
//  Created by Yvette Luo on 11/3/13.
//  Copyright (c) 2013 Yvette Luo. All rights reserved.
//

#include "TextFpMining.h"

#include <sstream>
#include <fstream>
#include <iostream>

//Step 2: Preprocessing
void TextFpMining::Preprocess() {
    
    GenerateDict(dict);
    
    Tokenize(dict);
}

//Step 2.1 Generate a Dictionary
void TextFpMining::GenerateDict(std::map<std::string,int>& dict) {
    
    std::ifstream inputStream(dataFile);
    typedef std::map<std::string,int>::iterator map_iter;
    
    std::string title;
    while(getline(inputStream, title)) {
        std::stringstream titleStream(title);
        std::string word;
        titleStream >> word;
        while(titleStream >> word) {
            std::cout << word <<std::endl;
            ++dict[word];
        }
    }
    
    //output dictionary and generate inverse index->term dictionary
    int id = 0;
    std::ofstream outputStream(dictFile);
    
    for(map_iter it = dict.begin(); it != dict.end(); ++it) {
        //outputStream << it -> first << std::endl;
        it->second = id;
        outputStream << it -> first <<" "<<it->second<<std::endl;
        reverseDict[id] = it->first;
        
        ++id;
    }
}

//Step 2.2 Tokenize Plain Text by Dictionary
void TextFpMining::Tokenize(std::map<std::string, int>& dict) {
    std::ifstream inputStream(dataFile);
    std::string title;
    std::ofstream outputStream(titleFile);
    typedef std::map<int,int>::iterator map_iter;
    
    while(getline(inputStream, title)) {
        std::stringstream titleStream(title);
        std::string word;
        titleStream >> word;
        
        std::map<int,int> titleBuffer;
        titleBuffer.clear();
        
        bool isBlank = true;
        while(titleStream >> word) {
            isBlank = false;
            int wordId = dict[word];
            ++titleBuffer[wordId];
        }
        
        //delete blank lines
        if(isBlank) continue;
        
        outputStream << titleBuffer.size();
        
        for(map_iter it = titleBuffer.begin(); it != titleBuffer.end(); ++it) {
            outputStream<<" "<<it->first<<":"<<it->second;
        }
        outputStream<<std::endl;
    }
}

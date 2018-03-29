#include "provided.h"
#include <iostream>
#include <fstream>
#include "MyHash.h"
#include <string>
#include <algorithm>
#include <vector>
#include <functional>
using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    MyHash<string, string> wordMap;
    MyHash<string, vector<string> > patternMap;
    
    string letterPattern(string word) const {
        // create table with mapping
        MyHash<char, char> temp;
        char marker = 'A';
        //
        for (int i = 0; i < word.size(); i++) {
            if (temp.find(word[i]) == nullptr) {
                temp.associate(word[i], marker);
                marker++;
            }
        }
        string toReturn;
        for (int j = 0; j < word.size(); j++) {
            toReturn += *(temp.find(word[j]));
        }
        return toReturn;
    }
};

bool WordListImpl::loadWordList(string filename)
{
    wordMap.reset();
    patternMap.reset();
    
    ifstream infile(filename);
    if ( ! infile)
        return false;
    else {
        string s;
        while (getline(infile, s)) {
            // transform it to lower case
            string lower = s;
            transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            wordMap.associate(lower, lower);
            
            string pattern = letterPattern(lower);
            vector<string> temp;
            if (patternMap.find(pattern) == nullptr) {
                temp.push_back(lower);
            }
            else {
                temp = *patternMap.find(pattern);
                temp.push_back(lower);
            }
            patternMap.associate(pattern, temp);
        }
        return true;
    }
}

bool WordListImpl::contains(string word) const
{
    string lower = word;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (wordMap.find(lower) == nullptr)
        return false;
    return true;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    vector<string> matches;
    
    if (cipherWord.size() != currTranslation.size())
        return matches;
    for (int i = 0; i < cipherWord.size(); i++) {
        if (! isalpha(cipherWord[i]) && cipherWord[i] != '\'')
            return matches;
        if (! isalpha(currTranslation[i]) && currTranslation[i] != '?' && currTranslation[i] != '\'')
            return matches;
    }
    
    string lowerCipher = cipherWord;
    transform(lowerCipher.begin(), lowerCipher.end(), lowerCipher.begin(), ::tolower);
    
    string lowerTrans = currTranslation;
    transform(lowerTrans.begin(), lowerTrans.end(), lowerTrans.begin(), ::tolower);
    
    vector<string> possibilities =  *patternMap.find(letterPattern(lowerCipher));
    // loop throughout matches vector
    for (int i = 0; i < possibilities.size(); i++) {
        bool ok = true;
        // loop throughout letters of each word in matches
        for (int j = 0; j < possibilities[i].size(); j++) {
            if (possibilities[i][j] == lowerTrans[j] || lowerTrans[j] == '?')
                continue;
            else {
                ok = false;
                break;
            }
        }
        if (ok)
            matches.push_back(possibilities[i]);
    }
    return matches;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
    return m_impl->findCandidates(cipherWord, currTranslation);
}

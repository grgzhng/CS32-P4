#include "provided.h"
#include <string>
#include <vector>
#include <set>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
    set<char> m_separators;
};

TokenizerImpl::TokenizerImpl(string separators)
{
    for  (int i = 0; i < separators.size(); i++) {
        m_separators.insert(separators[i]);
    }
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    vector<string> list;
    string temp;
    for (int i = 0; i < s.size(); i++) {
        if (m_separators.find(s[i]) == m_separators.end())
            temp += s[i];
        else {
            if (temp != "") {
                list.push_back(temp);
                temp = "";
            }
        }
    }
    if (temp != "")
        list.push_back(temp);
    return list;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}


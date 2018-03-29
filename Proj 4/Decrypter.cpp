#include "provided.h"
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

class DecrypterImpl
{
public:
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    Translator m_translator;
    WordList m_wordList;
    Tokenizer m_tokenizer;
    
//    string Crack(string ciphertext_message, vector<string>& output);
};

DecrypterImpl::DecrypterImpl()
: m_tokenizer(" ,;:.!()[]{}-\"#$%^&")
{
}

bool DecrypterImpl::load(string filename)
{
    return m_wordList.loadWordList(filename);
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    vector<string> messageTokens = m_tokenizer.tokenize(ciphertext);
    
    // max holds index of token with most '?' and the number of ?'s
    pair<int, int> max (0, 0);
    for (int i = 0; i < messageTokens.size(); i++) {
        string iTranslated = m_translator.getTranslation(messageTokens[i]);
        int countQ = 0;
        for (int j = 0; j < iTranslated.size(); j++) {
            if (iTranslated[j] == '?')
                countQ++;
        }
        if (countQ > max.second)
            max = make_pair(i, countQ);
    }
    // this is the translated w/ the current map
    string translated = m_translator.getTranslation(messageTokens[max.first]);
    
    vector<string> possibleTrans = m_wordList.findCandidates(messageTokens[max.first], translated);
    
    //    for (int i = 0; i < possibleTrans.size(); i++)
    //        cout << possibleTrans[i] << endl;
    
    if (possibleTrans.empty()) {
        // instruction number 5
        m_translator.popMapping();
        return vector<string>();
    }
    
    vector<string> ans;
    
    // start of step 6
    for (int i = 0; i < possibleTrans.size(); i++) {
        bool newMapping = m_translator.pushMapping(messageTokens[max.first], possibleTrans[i]);
        if (newMapping == true) {
            string newTrans = m_translator.getTranslation(ciphertext);
            vector<string> newTokens = m_tokenizer.tokenize(newTrans);
            int numComplete = 0;
            bool completeGood = true;
            for (int j = 0; j < newTokens.size(); j++) {
                if (newTokens[j].find('?') == string::npos) {
                    if (! m_wordList.contains(newTokens[j])) {
                        m_translator.popMapping();
                        completeGood = false;
                        break;
                    }
                    numComplete++;
                }
            }
            if (completeGood) {
                if (numComplete == newTokens.size()) {
                    ans.push_back(newTrans);
                    m_translator.popMapping();
                }
                else {
                    // go back to step 2 with new mapping
                    vector<string> otherAns = crack(ciphertext);
                    ans.insert(ans.end(), otherAns.begin(), otherAns.end());
                }
            }
            else
                continue;
            //            if (numComplete == newTokens.size()) {
            //                ans.push_back(newTrans);
            //                m_translator.popMapping();
            //            }
            //            else {
            //                // go back to step 2 with new mapping
            //                vector<string> otherAns = crack(ciphertext);
            //                ans.insert(ans.end(), otherAns.begin(), otherAns.end());
            //            }
        }
        else {
            continue;
        }
    }
    m_translator.popMapping();
    sort(ans.begin(), ans.end());
    return ans;  // This compiles, but may not be correct
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
    return m_impl->crack(ciphertext);
}


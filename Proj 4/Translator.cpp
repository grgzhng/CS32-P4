#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <set>
#include <cctype>
using namespace std;

class TranslatorImpl
{
public:
    TranslatorImpl();
    ~TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    char* m_cipher;
    vector<char*> m_plain;
    vector<set<char>*> m_plainContained;
    int m_counter;
};

TranslatorImpl::TranslatorImpl()
: m_counter(1)
{
    m_cipher = new char[26];
    char* plain= new char[26];
    char z = 'A';
    for (int i = 0; i < 26; i++) {
        m_cipher[i] = z;
        plain[i] = '?';
        z++;
    }
    m_plain.push_back(plain);
    m_plainContained.push_back(new set<char>);
}

TranslatorImpl::~TranslatorImpl()
{
    delete[] m_cipher;
    for (int i = 0; i < m_counter; i++) {
        delete[] m_plain[i];
        delete m_plainContained[i];
    }
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    // compare sizes of two inputs
    if (ciphertext.size() != plaintext.size())
        return false;
    // check that all inputs are only letters of alphabet
    // also check that the set doesn't already contain each of the plaintext members
    for (int i = 0; i < ciphertext.size(); i++) {
        if (!isalpha(ciphertext[i]) || !isalpha(plaintext[i]))
            return false;
        if (m_plainContained[m_counter-1]->find(toupper(plaintext[i])) != m_plainContained[m_counter-1]->end()) {
            
            // GOTTA CHECK IF THE MAPPING IS ALREADY THERE FROM THAT CIPHER LETTER TO THE PLAIN LETTER
            // I.E. IF IT'S THE SAME ALREADY
            int indexFound = toupper(ciphertext[i]) - 'A';
            if (m_plain[m_counter-1][indexFound] == toupper(plaintext[i]))
                continue;
            else
                return false;
            
            //            if (*(m_plainContained[m_counter-1]->find(plaintext[i])) != plaintext[i]) {
            //                return false;
            //            }
        }
    }
    
    // copy over new mapping for plain map
    char* newPlain = new char[26];
    for (int i = 0; i < 26; i++) {
        newPlain[i] = m_plain[m_counter-1][i];
    }
    // copy over set
    set<char>* newPlainSet = new set<char>;
    for (set<char>::iterator it = m_plainContained[m_counter-1]->begin(); it != m_plainContained[m_counter-1]->end(); it++) {
        newPlainSet->insert(*it);
    }
    
    for (int i = 0; i < ciphertext.size(); i++){
        char upperIndex = toupper(plaintext[i]);
        newPlainSet->insert(upperIndex);
        // temp is the index to be changed
        int temp = toupper(ciphertext[i]) - 'A';
        // this is to check that the cipher letter isn't mapped to a different letter already
        if (newPlain[temp] == '?' || newPlain[temp] == upperIndex)
            newPlain[temp] = upperIndex;
        else {
            delete[] newPlain;
            delete newPlainSet;
            return false;
        }
        
    }
    m_plain.push_back(newPlain);
    m_plainContained.push_back(newPlainSet);
    
    //    for (int i = 0; i < 26; i++) {
    //        cout << m_plain[m_counter][i] << " ";
    //    }
    //    cout << endl;
    //    for (set<char>::iterator it = m_plainContained[m_counter]->begin(); it != m_plainContained[m_counter]->end(); it++) {
    //        cout << *it << " ";
    //    }
    //    cout << endl;
    
    m_counter++;
    return true;
}

bool TranslatorImpl::popMapping()
{
    if (m_counter <= 1)
        return false;
    m_counter--;
    delete[] m_plain[m_counter];
    delete m_plainContained[m_counter];
    m_plain.pop_back();
    m_plainContained.pop_back();
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string translated;
    for (int i = 0; i < ciphertext.size(); i++) {
        if (! isalpha(ciphertext[i])) {
            translated += ciphertext[i];
        }
        else {
            int index = toupper(ciphertext[i]) - 'A';
            if (m_plain[m_counter-1][index] == '?')
                translated += '?';
            else if (isupper(ciphertext[i])) {
                //                int index = ciphertext[i] - 'A';
                translated += m_plain[m_counter-1][index];
            }
            else {
                index = toupper(ciphertext[i]) - 'A';
                translated += tolower(m_plain[m_counter-1][index]);
            }
        }
    }
    return translated;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}


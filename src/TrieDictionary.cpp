#include "TrieDictionary.h"
#include <fstream>
#include <iostream>

using namespace std;
TrieDictionary::TrieDictionary() : wordCount(0)
{
    root = make_unique<TrieNode>();
}

void TrieDictionary::insert(const string &word)
{
    TrieNode *current = root.get();
    for (char c : word)
    {
        char lowerC = std::tolower(c);
        if (lowerC < 'a' || lowerC > 'z')
            continue;
        int indx = lowerC - 'a';
        if (!current->children[indx])
        {
            current->children[indx] = make_unique<TrieNode>();
        }
        current = current->children[indx].get();
    }
    if (!current->isEndOfWord)
    {
        current->isEndOfWord = true;
        wordCount++;
    }
}

TrieNode *TrieDictionary::searchPrefixNode(const string &prefix) const
{
    TrieNode *current = root.get();
    for (char c : prefix)
    {
        char lowerC = std::tolower(c);
        int indx = lowerC - 'a';
        if (lowerC < 'a' || lowerC > 'z')
            return nullptr;
        else if (!current->children[indx])
            return nullptr;
        current = current->children[indx].get();
    }
    return current;
}

bool TrieDictionary::contains(const string &word) const
{
    TrieNode *node = searchPrefixNode(word);
    return (node != nullptr && node->isEndOfWord);
}

void TrieDictionary::collectWords(TrieNode *node, const string &prefix, vector<string> &results, int limit) const
{
    if (!node || results.size() >= limit)
        return;
    if (node->isEndOfWord)
        results.push_back(prefix);
    for (int i = 0; i < 26; i++)
    {
        if (node->children[i])
        {
            char c = 'a' + i;
            collectWords(node->children[i].get(), prefix + c, results, limit);
        }
    }
}
vector<string> TrieDictionary::suggest(const string &prefix, int limit) const
{
    vector<string> results;
    TrieNode *prefixnode = searchPrefixNode(prefix);
    if (prefixnode != nullptr)
    {
        collectWords(prefixnode, prefix, results, limit);
    }
    return results;
}

void TrieDictionary::loadFromFile(const string &filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        cout << "Error: Could not open dictionary file:";
        return;
    }
    string word;
    while (file >> word)
    {
        insert(word);
    }
    file.close();
}
// ============================================================
// TrieDictionary.h
// Words dictionary and auto-complete using Trie tree
// ============================================================
#pragma once
#include <memory>
#include <string>
#include <vector>

// Trie tree node (stores only 26 English letters)
class TrieNode
{
public:
    std::unique_ptr<TrieNode> children[26];
    bool isEndOfWord;
    TrieNode() : isEndOfWord(false) {}
};

class TrieDictionary
{
private:
    std::unique_ptr<TrieNode> root;
    size_t wordCount;

    // Helper function to collect all words under a specific node (DFS)
    void collectWords(TrieNode *node, const std::string &prefix,
                      std::vector<std::string> &results, int limit) const;

    // Search for the node ending with a specific prefix
    TrieNode *searchPrefixNode(const std::string &prefix) const;

public:
    TrieDictionary();

    // Load dictionary from a text file (one word per line)
    void loadFromFile(const std::string &filepath);

    // Add a new word
    void insert(const std::string &word);

    // Suggest a list of words starting with the given prefix (max number = limit)
    std::vector<std::string> suggest(const std::string &prefix, int limit = 5) const;

    // Check for the existence of a complete word
    bool contains(const std::string &word) const;

    size_t size() const { return wordCount; }
};
// ============================================================
// SearchEngine.h
// Search and replace engine using KMP algorithm
// ============================================================
#pragma once
#include <vector>
#include <string>

class SearchEngine
{
private:
    // Build the Longest Prefix Suffix (LPS) table for the searched pattern
    std::vector<int> computeLPS(const std::string &pattern) const;

public:
    // Find all occurrences of the pattern inside the text
    std::vector<size_t> searchAll(const std::string &text, const std::string &pattern) const;

    // Find the next match starting from a specific position (for Find Next feature)
    size_t searchNext(const std::string &text, const std::string &pattern, size_t startPos) const;
};
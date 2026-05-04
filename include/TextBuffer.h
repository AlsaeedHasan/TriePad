// ============================================================
// TextBuffer.h
// The internal text editor using a doubly linked list of lines
// ============================================================
#pragma once
#include <list>
#include <string>
#include <utility>
#include "TriePadCommon.h"

class TextBuffer
{
private:
    std::list<std::string> lines; // Doubly linked list: each node = a line
    size_t totalChars;            // Total number of characters (for faster performance)

    // Helper functions to convert between logical index (character 157) to (line number, character index in line)
    std::pair<size_t, size_t> getLineAndIndex(size_t globalPos) const;

    // Split the inserted text into lines upon encountering '\n'
    std::list<std::string> splitIntoLines(const std::string &text) const;

public:
    TextBuffer();
    // Helper functions for search and replace
    size_t length() const { return totalChars; }

    // Load full text (e.g., from a file)
    void load(const std::string &content);

    // Save current text into a single string (for saving to a file)
    std::string save() const;

    // Extract word boundaries at cursor position (used with search and replace feature)
    std::pair<size_t, size_t> getWordBoundaries(size_t pos) const;

    // Insert text at cursor position -> returns an Action that can be undone
    TriePad::Action insert(size_t pos, const std::string &text);

    // Erase a specific number of characters starting from cursor position -> returns an Action
    TriePad::Action erase(size_t pos, size_t length);
};
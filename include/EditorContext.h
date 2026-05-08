#pragma once
#include <memory>
#include <string>
#include <vector>
#include "TextBuffer.h"
#include "ActionHistory.h"
#include "TrieDictionary.h"
#include "SearchEngine.h"

struct EditorContext
{
    // Core engine components
    std::shared_ptr<TextBuffer> buffer;
    std::shared_ptr<ActionHistory> history;
    std::shared_ptr<TrieDictionary> dictionary;
    std::shared_ptr<SearchEngine> searchEngine;

    // Editor state and cursor
    size_t cursorPos = 0;
    bool isInsertMode = false;
    std::string statusMessage = "Ready";

    // Modal state
    bool showSearch = false;
    bool showOpen = false;
    bool showSave = false;

    // User input in modals
    std::string searchQuery;
    std::string fileNameInput;
    std::string savePathInput;

    // Search results
    std::vector<size_t> searchResults;
    size_t searchLen = 0;

    EditorContext();

    // Control logic functions
    void handleUndo();
    void handleRedo();
    void handleTabComplete();
    void handleBackspace();
    void handleEnter();
    void handleCharacterInput(std::string ch);
    void moveCursorLeft();
    void moveCursorRight();
};
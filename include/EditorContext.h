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
    // الأجزاء الأساسية للمحرك
    std::shared_ptr<TextBuffer> buffer;
    std::shared_ptr<ActionHistory> history;
    std::shared_ptr<TrieDictionary> dictionary;
    std::shared_ptr<SearchEngine> searchEngine;

    // حالة الأديتور والمؤشر
    size_t cursorPos = 0;
    bool isInsertMode = false;
    std::string statusMessage = "Ready";

    // حالة القوائم المنبثقة (Modals)
    bool showSearch = false;
    bool showOpen = false;
    bool showSave = false;

    // مدخلات المستخدم في القوائم
    std::string searchQuery;
    std::string fileNameInput;
    std::string savePathInput;

    // نتائج البحث
    std::vector<size_t> searchResults;
    size_t searchLen = 0;

    EditorContext();

    // دوال التحكم المنطقي
    void handleUndo();
    void handleRedo();
    void handleTabComplete();
    void handleBackspace();
    void handleEnter();
    void handleCharacterInput(std::string ch);
    void moveCursorLeft();
    void moveCursorRight();
};
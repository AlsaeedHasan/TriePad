#include "EditorContext.h"
#include "FileManager.h"

using namespace std;

EditorContext::EditorContext()
{
    buffer = make_shared<TextBuffer>();
    history = make_shared<ActionHistory>();
    dictionary = make_shared<TrieDictionary>();
    searchEngine = make_shared<SearchEngine>();
    dictionary->loadFromFile("resources/english_words.txt");
}

void EditorContext::handleUndo()
{
    if (history->canUndo())
    {
        TriePad::Action undo = history->undo();
        if (undo.type == TriePad::ActionType::INSERT)
        {
            buffer->insert(undo.position, undo.text);
            cursorPos = undo.position + undo.text.length();
        }
        else if (undo.type == TriePad::ActionType::DELETE)
        {
            buffer->erase(undo.position, undo.text.length());
            cursorPos = undo.position;
        }
        searchResults.clear();
        searchLen = 0;
    }
}

void EditorContext::handleRedo()
{
    if (history->canRedo())
    {
        TriePad::Action redo = history->redo();
        if (redo.type == TriePad::ActionType::INSERT)
        {
            buffer->insert(redo.position, redo.text);
            cursorPos = redo.position + redo.text.length();
        }
        else if (redo.type == TriePad::ActionType::DELETE)
        {
            buffer->erase(redo.position, redo.text.length());
            cursorPos = redo.position;
        }
        searchResults.clear();
        searchLen = 0;
    }
}

void EditorContext::handleTabComplete()
{
    auto bounds = buffer->getWordBoundaries(cursorPos);
    size_t start = bounds.first;
    size_t length = cursorPos - start;

    if (length > 0)
    {
        string fullText = buffer->save();
        string prefix = fullText.substr(start, length);
        vector<string> suggestions = dictionary->suggest(prefix, 10);

        for (const string &s : suggestions)
        {
            if (s.length() > prefix.length())
            {
                string remainingPart = s.substr(prefix.length());
                TriePad::Action act = buffer->insert(cursorPos, remainingPart);
                history->recordAction(act);
                cursorPos += remainingPart.length();
                break;
            }
        }
    }
    searchResults.clear();
    searchLen = 0;
}

void EditorContext::handleBackspace()
{
    if (cursorPos > 0)
    {
        TriePad::Action act = buffer->erase(cursorPos - 1, 1);
        history->recordAction(act);
        cursorPos--;
        searchResults.clear();
        searchLen = 0;
    }
}

void EditorContext::handleEnter()
{
    TriePad::Action act = buffer->insert(cursorPos, "\n");
    history->recordAction(act);
    cursorPos++;
    searchResults.clear();
    searchLen = 0;
}

void EditorContext::handleCharacterInput(string ch)
{
    TriePad::Action act = buffer->insert(cursorPos, ch);
    history->recordAction(act);
    cursorPos++;
    searchResults.clear();
    searchLen = 0;
}

void EditorContext::moveCursorLeft()
{
    if (cursorPos > 0)
        cursorPos--;
}

void EditorContext::moveCursorRight()
{
    if (cursorPos < buffer->length())
        cursorPos++;
}
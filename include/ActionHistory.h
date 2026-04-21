// ============================================================
// ActionHistory.h
// Undo and Redo system using stacks
// ============================================================
#pragma once
#include <stack>
#include "TriePadCommon.h"

class ActionHistory
{
private:
    std::stack<TriePad::Action> undoStack;
    std::stack<TriePad::Action> redoStack;

public:
    // Record a new action (called after every Insert/Erase)
    void recordAction(const TriePad::Action &action);

    // Request undo -> returns the reverse action ready to be applied
    TriePad::Action undo();

    // Request redo
    TriePad::Action redo();

    bool canUndo() const { return !undoStack.empty(); }
    bool canRedo() const { return !redoStack.empty(); }

    // Clear redo log when a new operation is made after a series of undos
    void clearRedo();
};
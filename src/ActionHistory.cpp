#include "ActionHistory.h"
#include "TriePadCommon.h"

using namespace TriePad;

void ActionHistory::recordAction(const Action &action)
{
    undoStack.push(action);
    clearRedo();
}

void ActionHistory::clearRedo()
{
    while (!redoStack.empty())
    {
        redoStack.pop();
    }
}

Action ActionHistory::undo()
{
    if (undoStack.empty())
    {
        return Action{ActionType::INSERT, 0, ""};
    }

    Action lastAction = undoStack.top();
    undoStack.pop();

    redoStack.push(lastAction);

    Action reverseAction;
    reverseAction.position = lastAction.position;
    reverseAction.text = lastAction.text;

    if (lastAction.type == ActionType::INSERT)
    {
        reverseAction.type = ActionType::DELETE;
    }
    else
    {
        reverseAction.type = ActionType::INSERT;
    }

    return reverseAction;
}

Action ActionHistory::redo()
{
    if (redoStack.empty())
    {
        return Action{ActionType::INSERT, 0, ""};
    }

    Action actionToRedo = redoStack.top();
    redoStack.pop();
    undoStack.push(actionToRedo);

    return actionToRedo;
}
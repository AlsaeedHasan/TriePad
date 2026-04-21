// ============================================================
// TriePadCommon.h
// Common definitions shared across all project modules
// ============================================================
#pragma once
#include <string>

namespace TriePad
{
    // Action type (Insert or Delete)
    enum class ActionType
    {
        INSERT,
        DELETE
    };

    // Action object: records every text modification operation
    struct Action
    {
        ActionType type;
        size_t position;  // Cursor position at the time of operation
        std::string text; // The inserted or deleted text
    };
}
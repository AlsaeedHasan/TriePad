// ============================================================
// FileManager.h
// Helper functions to open and save files
// ============================================================
#pragma once
#include <memory>
#include <string>

#include "TextBuffer.h"

bool OpenFile(std::shared_ptr<TextBuffer> &buffer, const std::string &filepath);
bool SaveToFile(const std::shared_ptr<TextBuffer> &buffer, const std::string &filepath);
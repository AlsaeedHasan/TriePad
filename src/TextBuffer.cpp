#include "TextBuffer.h"
#include <iostream>
#include <sstream>
#include <iterator>

TextBuffer::TextBuffer()
{
    totalChars = 0;
    lines.push_back("");
}

void TextBuffer::load(const std::string &content)
{
    lines.clear();
    totalChars = 0;
    if (content.empty())
    {
        lines.push_back("");
        return;
    }

    std::stringstream ss(content);
    std::string line;
    while (std::getline(ss, line))
    {
        lines.push_back(line);
        totalChars += line.length();
    }

    // عشان لو الملف بينتهي بسطر جديد (Enter)، نحافظ عليه
    if (!content.empty() && content.back() == '\n')
    {
        lines.push_back("");
    }

    totalChars += (lines.size() - 1); // حساب الـ '\n' المخفية بين السطور
}

std::string TextBuffer::save() const
{
    std::ostringstream oss;
    if (lines.empty())
    {
        return oss.str();
    }

    for (auto i = lines.begin(); i != lines.end(); ++i)
    {
        oss << *i;
        if (std::next(i) != lines.end())
        {
            oss << '\n';
        }
    }
    return oss.str();
}

std::pair<size_t, size_t> TextBuffer::getLineAndIndex(size_t globalPos) const
{
    size_t currentIndex = 0;
    size_t lineIndex = 0;

    for (auto i = lines.begin(); i != lines.end(); i++, lineIndex++)
    {
        size_t lineLength = i->length() + (std::next(i) != lines.end() ? 1 : 0);
        if (globalPos < (currentIndex + lineLength))
        {
            return {lineIndex, (globalPos - currentIndex)};
        }
        currentIndex += lineLength;
    }
    return {lines.size() - 1, lines.back().length()};
}

std::list<std::string> TextBuffer::splitIntoLines(const std::string &text) const
{
    std::list<std::string> result;
    if (text.empty())
    {
        result.push_back("");
        return result;
    }

    std::stringstream ss(text);
    std::string line;
    while (std::getline(ss, line))
    {
        result.push_back(line);
    }

    if (!text.empty() && text.back() == '\n')
    {
        result.push_back("");
    }

    return result;
}

std::pair<size_t, size_t> TextBuffer::getWordBoundaries(size_t pos) const
{
    auto [lineIdx, charIdx] = getLineAndIndex(pos);
    auto i = lines.begin();
    std::advance(i, lineIdx);
    const std::string &line = *i;

    size_t wordStart = charIdx;
    while (wordStart > 0 && line[wordStart - 1] != ' ')
    {
        wordStart--;
    }

    size_t wordEnd = charIdx;
    while (wordEnd < line.length() && line[wordEnd] != ' ')
    {
        wordEnd++;
    }

    // Convert back from character index to global position
    size_t globalLineStart = pos - charIdx;
    return {globalLineStart + wordStart, globalLineStart + wordEnd};
}

TriePad::Action TextBuffer::insert(size_t pos, const std::string &text)
{
    auto [lineIdx, charIdx] = getLineAndIndex(pos);
    std::list<std::string> newLines = splitIntoLines(text);
    auto i = lines.begin();
    std::advance(i, lineIdx);
    std::string originalText = text;

    if (newLines.size() == 1)
    { // if text is one line
        std::string currentLine = *i;
        std::string newLine = currentLine.substr(0, charIdx) + newLines.front() + currentLine.substr(charIdx);
        *i = newLine;
        totalChars += text.length();
    }
    else
    {
        std::string currentLine = *i;
        std::string leftpart = currentLine.substr(0, charIdx);
        std::string rightpart = currentLine.substr(charIdx);

        std::string firstnewline = leftpart + newLines.front();
        std::string lastnewline = newLines.back() + rightpart;

        i = lines.erase(i);
        i = lines.insert(i, firstnewline);
        ++i;

        size_t numNewLines = newLines.size();
        if (numNewLines > 1)
        {
            auto newi = newLines.begin();
            ++newi;
            while (numNewLines > 2)
            {
                i = lines.insert(i, *newi);
                ++i;
                ++newi;
                --numNewLines;
            }
            lines.insert(i, lastnewline);
        }
        totalChars += text.length();
    }
    return {TriePad::ActionType::INSERT, pos, originalText};
}

TriePad::Action TextBuffer::erase(size_t pos, size_t length)
{
    auto [lineIdx, charIdx] = getLineAndIndex(pos);
    auto i = lines.begin();
    std::advance(i, lineIdx);
    std::string deletedText = "";

    if (charIdx + length <= i->length())
    { // erasing from the same line
        deletedText = i->substr(charIdx, length);
        i->erase(charIdx, length);
        totalChars -= length;
    }
    else
    {
        size_t remaining = length;
        std::string firstPart = i->substr(charIdx);
        deletedText += firstPart;
        i->erase(charIdx);

        if (remaining > firstPart.length())
        {
            remaining -= (firstPart.length() + 1);
            deletedText += '\n';
        }
        else
        {
            remaining -= firstPart.length();
        }

        ++i;

        // delete complete lines if there is remaining
        while (i != lines.end() && remaining > i->length())
        {
            deletedText += *i;
            deletedText += '\n';
            remaining -= (i->length() + 1);
            i = lines.erase(i);
        }

        // delete the remaining chars from the beginning of the last line
        if (remaining > 0 && i != lines.end())
        {
            std::string lastPart = i->substr(0, remaining);
            deletedText += lastPart;
            i->erase(0, remaining);
        }

        if (i != lines.begin() && i != lines.end())
        {
            auto prev = i;
            --prev;
            *prev = *prev + *i;
            lines.erase(i);
        }
        totalChars -= length;
    }
    return {TriePad::ActionType::DELETE, pos, deletedText};
}

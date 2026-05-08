#include "UIComponents.h"
#include "FileManager.h"

using namespace std;
using namespace ftxui;

Element RenderDocument(const EditorContext &ctx)
{
    string fullText = ctx.buffer->save();
    Elements lines;
    Elements currentLineElements;

    size_t i = 0;
    while (i <= fullText.size())
    {
        bool isCursor = (i == ctx.cursorPos);
        bool isSearchResult = false;

        if (ctx.searchLen > 0)
        {
            for (size_t pos : ctx.searchResults)
            {
                if (i >= pos && i < pos + ctx.searchLen)
                {
                    isSearchResult = true;
                    break;
                }
            }
        }

        if (i == fullText.size() || fullText[i] == '\n')
        {
            if (isCursor)
                currentLineElements.push_back(text("█") | focus);
            if (currentLineElements.empty())
                currentLineElements.push_back(text(""));
            lines.push_back(hbox(move(currentLineElements)));
            currentLineElements.clear();
            if (i == fullText.size())
                break;
        }
        else
        {
            string ch(1, fullText[i]);
            Element el = isCursor ? text("█") | focus : text(ch);
            if (isSearchResult && !isCursor)
            {
                el = el | bgcolor(Color::Yellow) | color(Color::Black);
            }
            currentLineElements.push_back(el);
        }
        i++;
    }
    return vbox(lines) | frame | vscroll_indicator | borderEmpty | border | flex;
}

Element RenderStatusLine(const EditorContext &ctx)
{
    return hbox(
        {text(" Mode: " + string(ctx.isInsertMode ? "INSERT" : "READ")) | bold,
         separatorEmpty(),
         text(" | Undo: " + string(ctx.history->canUndo() ? "Available" : "N/A")) | dim,
         separatorEmpty(),
         text(" | Dictionary: " + to_string(ctx.dictionary->size()) + " words") | color(Color::Blue),
         text(" " + ctx.statusMessage + " ") | color(Color::LightGreen) | bold});
}

Element RenderFooter()
{
    return hbox({text(" [Ctrl+U] Undo  [Ctrl+R] Redo  [Ctrl+F] Find  [Ctrl+O] Open  [Ctrl+S] Save  [Ctrl+Q] Quit ") | dim});
}

Component CreateSearchModal(EditorContext &ctx, const InputOption &opt)
{
    auto input = Input(&ctx.searchQuery, "Search word...", opt);
    return Renderer(
        input,
        [&ctx, input]
        {
            return window(text(" Find [KMP Engine] ") | bold | color(Color::Cyan) | center,
                          vbox({text(" Enter word:"),
                                input->Render() | border,
                                text(" [Enter] Find | [Esc] Close ") | dim | center})) |
                   clear_under | center;
        });
}

Component CreateOpenModal(EditorContext &ctx, const InputOption &opt)
{
    auto input = Input(&ctx.fileNameInput, "File path...", opt);
    return Renderer(
        input,
        [&ctx, input]
        {
            return window(text(" Open File ") | bold | color(Color::Green) | center,
                          vbox({input->Render() | border,
                                text(" [Enter] Open | [Esc] Cancel ") | dim | center})) |
                   clear_under | center;
        });
}

Component CreateSaveModal(EditorContext &ctx, const InputOption &opt)
{
    auto input = Input(&ctx.savePathInput, "Save as...", opt);
    return Renderer(
        input,
        [&ctx, input]
        {
            return window(text(" Save Document ") | bold | color(Color::Blue) | center,
                          vbox({input->Render() | border,
                                text(" [Enter] Save | [Esc] Cancel ") | dim | center})) |
                   clear_under | center;
        });
}
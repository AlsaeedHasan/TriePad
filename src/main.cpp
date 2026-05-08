#include <ftxui/component/screen_interactive.hpp>
#include "EditorContext.h"
#include "UIComponents.h"
#include "FileManager.h"

using namespace ftxui;
using namespace std;

int main()
{
    auto ctx = make_shared<EditorContext>();
    auto screen = ScreenInteractive::Fullscreen();

    InputOption modalOpt;
    modalOpt.transform = [](InputState s)
    {
        if (s.focused)
            s.element |= bgcolor(Color::Black) | color(Color::White) | bold;
        return s.element;
    };

    auto main_view = Renderer(
        [&]
        { return vbox({RenderStatusLine(*ctx),
                       separator(),
                       RenderDocument(*ctx) | flex,
                       separator(),
                       RenderFooter()}) |
                 border | bgcolor(Color::Default); });

    main_view |= CatchEvent(
        [&](Event event)
        {
        if (ctx->showSearch || ctx->showOpen || ctx->showSave) return false;
        
        if (event == Event::Insert) { ctx->isInsertMode = !ctx->isInsertMode; return true; }
        if (event == Event::Escape) { ctx->isInsertMode = false; ctx->searchResults.clear(); return true; }

        if (ctx->isInsertMode) {
            if (event == Event::Backspace) { ctx->handleBackspace(); return true; }
            if (event == Event::CtrlU) { ctx->handleUndo(); return true; }
            if (event == Event::CtrlR) { ctx->handleRedo(); return true; }
            if (event == Event::Tab)   { ctx->handleTabComplete(); return true; }
            if (event == Event::Return) { ctx->handleEnter(); return true; }
            if (event == Event::ArrowLeft) { ctx->moveCursorLeft(); return true; }
            if (event == Event::ArrowRight) { ctx->moveCursorRight(); return true; }
            if (event.is_character()) { ctx->handleCharacterInput(event.character()); return true; }
        }

        if (event == Event::CtrlF) { ctx->showSearch = true; return true; }
        if (event == Event::CtrlO) { ctx->showOpen = true; ctx->fileNameInput = ""; return true; }
        if (event == Event::CtrlS) { ctx->showSave = true; ctx->savePathInput = ""; return true; }
        if (event == Event::CtrlQ) { screen.ExitLoopClosure()(); return true; }
        
        return false; });

    auto final_ui = Container::Stacked(
        {Maybe(CreateSearchModal(*ctx, modalOpt), &ctx->showSearch),
         Maybe(CreateOpenModal(*ctx, modalOpt), &ctx->showOpen),
         Maybe(CreateSaveModal(*ctx, modalOpt), &ctx->showSave),
         main_view});

    final_ui |= CatchEvent(
        [&](Event event)
        {
        if (event == Event::Return) {
            if (ctx->showSearch) { 
                ctx->searchResults = ctx->searchEngine->searchAll(ctx->buffer->save(), ctx->searchQuery);
                ctx->searchLen = ctx->searchQuery.length();
                ctx->showSearch = false; 
                return true; 
            }
            if (ctx->showOpen) {
                if (OpenFile(ctx->buffer, ctx->fileNameInput)) { ctx->cursorPos = 0; ctx->showOpen = false; }
                return true;
            }
            if (ctx->showSave) {
                if (!ctx->savePathInput.empty() && SaveToFile(ctx->buffer, ctx->savePathInput)) {
                    ctx->statusMessage = "Saved to " + ctx->savePathInput;
                    ctx->showSave = false;
                }
                return true;
            }
        }
        if (event == Event::Escape) {
            ctx->showSearch = ctx->showOpen = ctx->showSave = false;
            return true;
        }
        return false; });

    screen.Loop(final_ui);
    return 0;
}
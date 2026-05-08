#pragma once
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include "EditorContext.h"

using namespace ftxui;

Element RenderDocument(const EditorContext &ctx);
Element RenderStatusLine(const EditorContext &ctx);
Element RenderFooter();

// Modal components
Component CreateSearchModal(EditorContext &ctx, const InputOption &opt);
Component CreateOpenModal(EditorContext &ctx, const InputOption &opt);
Component CreateSaveModal(EditorContext &ctx, const InputOption &opt);
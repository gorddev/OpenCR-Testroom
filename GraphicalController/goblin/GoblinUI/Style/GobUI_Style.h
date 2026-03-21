#pragma once

/* Created by Gordie Novak on 3/21/26.
 * Purpose: 
 */

#include "gobdef.h"

namespace gobin {

    class GobUI_Style {
    private:
        ImFont* header_font;
        ImFont* body_font;
        ImFont* console_font;
        ImFont* input_font;

        friend class GoblinController;

    public:
        GobUI_Style() = default;

        /* -------------- default ------------- */

        inline void pushGobDefault() {
            ImGui::PushFont(body_font);
            // generic
            ImGui::PushStyleColor(ImGuiCol_WindowBg,        col::windowBG);
            ImGui::PushStyleColor(ImGuiCol_FrameBg,         col::panelBG);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,  col::panelBGHover);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive,   col::panelBGActive);
            // dropdown menu
            ImGui::PushStyleColor(ImGuiCol_Header,          col::dropdown);
            ImGui::PushStyleColor(ImGuiCol_HeaderActive,    col::dropdownActive);
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered,   col::dropdownHover);
            // buttons
            ImGui::PushStyleColor(ImGuiCol_Button,          col::button);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   col::buttonHovered);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    col::buttonActive);
            ImGui::PushStyleColor(ImGuiCol_TextDisabled,    col::buttonSelected);
            // checkmark
            ImGui::PushStyleColor(ImGuiCol_CheckMark,       col::buttonExaggerated);
            // slider
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive,col::buttonActive);
            ImGui::PushStyleColor(ImGuiCol_SliderGrab,      col::button);
            // title
            ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.118, 0.2, 0.024, 1.f));
            ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.208, 0.361, 0.035, 1.f));
            ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.075, 0.09, 0.059, 0.8f));

        }

        inline void popGobDefault() {
            ImGui::PopStyleColor(17);
            ImGui::PopFont();
        }

        /* ----------- buttons ----------- */

        inline void pushSelectedButton(const bool push_it = true) {
            if (push_it)
            ImGui::PushStyleColor(ImGuiCol_Button, col::buttonSelected);;
        }

        inline void popSelectedButton(const bool pop_it = true) {
            if (pop_it)
            ImGui::PopStyleColor(1);
        }

        inline void pushSpecialButton() {
            ImGui::PushStyleColor(ImGuiCol_Button,          col::buttonSpecial);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   col::buttonSpecialHover);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    col::buttonSpecialActive);
        }

        inline void popSpecialButton() {
            ImGui::PopStyleColor(3);
        }

        /* ----------- headers --------------- */

        inline void pushHeaderFont() {
            ImGui::PushFont(header_font);
        }

        inline void popHeaderFont() {
            ImGui::PopFont();
        }

        /* ----------- console --------------- */

        inline void pushConsoleFont() {
            ImGui::PushFont(console_font);
        }

        inline void popConsoleFont() {
            ImGui::PopFont();
        }

        /* ----------- input --------------- */

        inline void pushInputFont() {
            ImGui::PushFont(input_font);
        }

        inline void popInputFont() {
            ImGui::PopFont();
        }

    public:
    } inline g_style;

}

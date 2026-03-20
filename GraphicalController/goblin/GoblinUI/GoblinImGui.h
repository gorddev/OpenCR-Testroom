#pragma once

/* Created by Gordie Novak on 3/20/26.
 * Purpose: 
 */
#include <goblin/gobdef.h>

#define GOB_GUI_BUTTON_DISABLE(button, cond, id) \
{ \
const bool gob_gui_disabled_##id = (cond); \
/* Pass the bool directly: ImGui handles the 'if' internally */ \
ImGui::BeginDisabled(gob_gui_disabled_##id); \
{ \
button; \
} \
/* EndDisabled MUST be called if BeginDisabled was called */ \
ImGui::EndDisabled(); \
}

namespace gobin {

    namespace col {
        // generic
        constexpr ImVec4 windowBG{0.025, 0.045, 0.025, 0.8};
        constexpr ImVec4 panelBG{0.15, 0.2, 0.13, 0.9};
        constexpr ImVec4 panelBGHover{0.20, 0.25, 0.2, 0.9};
        constexpr ImVec4 panelBGActive{0.4, 0.3, 0.4, 0.9};

        // dropdown
        constexpr ImVec4 dropdown{0.118, 0.2, 0.024, 1.f};
        constexpr ImVec4 dropdownActive{0.306, 0.439, 0.149, 1.f};
        constexpr ImVec4 dropdownHover{0.280, 0.450, 0.230, 1.f};

        // buttons
        constexpr ImVec4 button{0.240, 0.430, 0.130, 1};
        constexpr ImVec4 buttonHovered{0.650, 0.2, 0.50, 1};
        constexpr ImVec4 buttonActive{0.9250, 0.5, 0.80, 1};
        constexpr ImVec4 buttonSelected{0.5f, 0.2f, 0.35f, 1.0f};
        constexpr ImVec4 buttonExaggerated{0.40, 0.80, 0.20, 1};



    }

    inline void pushGoblinStyle() {
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

    inline void popGoblinStyle() {
        ImGui::PopStyleColor(17);
    }

    inline void pushGoblinSelectedButton(const bool do_it = true) {
        if (do_it)
        ImGui::PushStyleColor(ImGuiCol_Button,          col::buttonSelected);
    }
    inline void popGoblinSelectedButton(const bool do_it = true) {
        if (do_it)
        ImGui::PopStyleColor(1);
    }

}

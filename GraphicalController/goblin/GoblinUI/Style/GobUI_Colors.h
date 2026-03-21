#pragma once

/* Created by Gordie Novak on 3/20/26.
 * Purpose: 
 */

#include "../../gobdef.h"

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
        constexpr ImVec4 buttonSpecial{0.5, 0.3, 0.25, 1};
        constexpr ImVec4 buttonSpecialHover{0.7, 0.4, 0.35, 1};
        constexpr ImVec4 buttonSpecialActive{0.9, 0.3, 0.5, 1};
    }

}

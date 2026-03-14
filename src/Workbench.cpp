#include "Workbench.h"
/* Created by Gordie Novak on 3/14/26.
 * Purpose: 
 */

namespace dxl {
    namespace workbench{
        DynamixelWorkbench wb;
        bool initialized = false;
        uint16_t status_num = 0;
        const char* cr_log = nullptr;
    }
}
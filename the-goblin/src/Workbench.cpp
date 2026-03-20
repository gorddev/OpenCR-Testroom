#include "goblin/Workbench.h"
/* Created by Gordie Novak on 3/14/26.
 * Purpose: 
 */

namespace gobin {
    namespace workbench{
        DynamixelWorkbench wb;
        b8          initialized = false;
        u16         status_num  = 0;
        const char* cr_log      = nullptr;
    }
}
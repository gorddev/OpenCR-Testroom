/* Created by Gordie Novak on 3/19/26.
 * Purpose: 
 */

#include <iostream>
#include "../src/core/crstream.hpp"
#include "../src/command/command-io.h"

int main(void) {

    std::cout << "Eghe\n";
    auto def = gobin::com::bitcast("ejfiefo");
    gobin::com::comprint(def.bits);

    return 0;
}

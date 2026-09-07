#include "Box.h"
#include "ShippingContainer.h"
#include "InsuredShipping.h"
#include "RefrigeratedShipping.h"
#include "Exceptions.h"

#include <cstdio>
#include <iostream>
#include <string>

namespace {

void printManifest(const std::string& title, CargoIterator& it) {
    std::cout << "-- " << title << " --\n";
    int count = 0;
    while (it.hasNext()) {
        Shippable* item = it.next();
        std::printf("  %-60s weight=%7.2fkg  cost=$%7.2f%s\n",
                    item->getName().c_str(),
                    item->getWeight(),
                    item->getCost(),
                    item->isHazardous() ? "  [HAZMAT]" : "");
        ++count;
    }
    std::cout << "  (" << count << " item(s) in this manifest)\n\n";
}

template <typename ActionFunc>
void tryTransition(const std::string& label, ActionFunc action) {
    try {
        action();
        std::cout << "  OK: " << label << "\n";
    } catch (const InvalidStateTransition& ex) {
        std::cout << "  REJECTED (" << label << "): " << ex.what() << "\n";
    }
}

}
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

int main() {
    std::cout << "==================================================\n";
    std::cout << " TaskForge: Logistics & Shipping\n";
    std::cout << "==================================================\n\n";

    ShippingContainer* root = new ShippingContainer("Global Shipment GS-01");

    ShippingContainer* containerA = new ShippingContainer("Container MSCU-1122");
    ShippingContainer* palletA1 = new ShippingContainer("Pallet A1");
    ShippingContainer* palletA2 = new ShippingContainer("Pallet A2");
    ShippingContainer* containerB = new ShippingContainer("Container MSCU-2233");

    Box* box1 = new Box("BX-1001 Electronics Crate", 120.0, 300.0, false);
    Box* box3 = new Box("BX-1003 Frozen Seafood", 60.0, 250.0, false);

    palletA1->addItem(box1);
    palletA1->addItem(new Box("BX-1002 Industrial Chemicals", 80.0, 500.0, true));

    Shippable* decoratedBox3 = new RefrigeratedShipping(box3, -18.0);
    decoratedBox3 = new InsuredShipping(decoratedBox3, 75.0);
    palletA2->addItem(decoratedBox3);

    containerA->addItem(palletA1);
    containerA->addItem(palletA2);

    containerB->addItem(new Box("BX-2001 Lithium Batteries", 15.0, 120.0, true));

    root->addItem(containerA);
    root->addItem(containerB);

    delete root;

    return 0;
}
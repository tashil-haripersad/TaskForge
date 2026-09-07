#include "Box.h"
#include "ShippingContainer.h"
#include "InsuredShipping.h"
#include "RefrigeratedShipping.h"
#include "Exceptions.h"

#include <iostream>
#include <string>

namespace {
    std::string padRight(const std::string& s, std::string::size_type width) {
        std::string padded = s;
        while (padded.size() < width) padded += ' ';
        return padded;
    }

    void printManifest(const std::string& title, CargoIterator& it) {
        std::cout << "-- " << title << " --\n";
        std::cout << std::fixed;
        std::cout.precision(2);
        int count = 0;
        while (it.hasNext()) {
            Shippable* item = it.next();
            std::cout << "  " << padRight(item->getName(), 48)
                    << " weight=" << item->getWeight() << "kg"
                    << "  cost=$" << item->getCost()
                    << (item->isHazardous() ? "  [HAZMAT]" : "") << "\n";
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

    std::cout << "### Scenario 1: manifests and lifecycle ###\n\n";

    {
        CargoIterator* full = root->createFullIterator();
        printManifest("Full manifest (FullInventoryIterator)", *full);
        delete full;
    }
    
    {
        CargoIterator* hazmat = root->createHazardousIterator();
        printManifest("Dangerous goods only (HazardousMaterialsIterator)", *hazmat);
        delete hazmat;
    }

    std::cout << "Driving BX-1001 through its lifecycle (state: " << box1->getStateName() << ")\n";
    tryTransition("load onto truck", [&]() { box1->loadOntoTruck(); });
    std::cout << "  now: " << box1->getStateName() << "\n";
    tryTransition("load onto truck again (invalid)", [&]() { box1->loadOntoTruck(); });
    tryTransition("arrive at customs", [&]() { box1->arriveAtCustoms(); });
    std::cout << "  now: " << box1->getStateName() << "\n";
    tryTransition("clear customs", [&]() { box1->clearCustoms(); });
    std::cout << "  now: " << box1->getStateName() << "\n";
    tryTransition("clear customs again (invalid, already delivered)", [&]() { box1->clearCustoms(); });
    std::cout << "\n";

    std::cout << "### Scenario 2: runtime changes vs. an in-flight traversal ###\n\n";

    CargoIterator* inFlight = root->createFullIterator();
    std::cout << "Started an iterator before making changes. First item seen: "
              << (inFlight->hasNext() ? inFlight->next()->getName() : std::string("<none>")) << "\n\n";

    std::cout << "(structural change) Re-consolidating cargo: moving BX-2001 from Container "
                 "MSCU-2233 into Pallet A1, and receiving a new box directly into MSCU-2233...\n";
    Shippable* movedBox = containerB->releaseItem("BX-2001 Lithium Batteries");
    if (movedBox) palletA1->addItem(movedBox);
    containerB->addItem(new Box("BX-2002 Spare Parts", 10.0, 50.0, false));

    std::cout << "(decoration change) Re-wrapping BX-1002 with insurance after a hazard review...\n";
    Shippable* hazBox = palletA1->releaseItem("BX-1002 Industrial Chemicals");
    if (hazBox) {
        hazBox = new InsuredShipping(hazBox, 150.0);
        palletA1->addItem(hazBox);
    }
    std::cout << "\n";

    std::cout << "Finishing the traversal that started BEFORE the changes above:\n";
    int remainingBefore = 0;
    while (inFlight->hasNext()) { inFlight->next(); ++remainingBefore; }
    std::cout << "  " << remainingBefore
              << " more item(s) were left in the OLD snapshot -- it still reflects the "
                 "hierarchy exactly as it was when it was created, unaffected by the later "
                 "moves, additions, and re-decoration.\n\n";
    delete inFlight;

    {
        CargoIterator* afterFull = root->createFullIterator();
        printManifest("Full manifest AFTER changes (freshly created iterator)", *afterFull);
        delete afterFull;
    }
    {
        CargoIterator* afterHazmat = root->createHazardousIterator();
        printManifest("Dangerous goods AFTER changes", *afterHazmat);
        delete afterHazmat;
    }

    std::cout << "Total shipment weight: " << root->getWeight() << "kg, total cost: $"
              << root->getCost() << "\n";

    delete root;

    return 0;
}
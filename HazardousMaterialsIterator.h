#ifndef HAZARDOUSMATERIALSITERATOR_H
#define HAZARDOUSMATERIALSITERATOR_H

#include "CargoIterator.h"
#include <vector>

class ShippingContainer;

// ConcreteIterator: same snapshot policy as FullInventoryIterator, but its
// selection rule differs -- it only exposes leaf items tagged as dangerous
// goods, skipping everything else. Having two ConcreteIterator classes with
// different selection rules over the same ConcreteAggregate demonstrates
// that independent traversal strategies can coexist over one structure.
class HazardousMaterialsIterator : public CargoIterator {
public:
    explicit HazardousMaterialsIterator(ShippingContainer& root);

    bool hasNext() const override;
    Shippable* next() override;

private:
    std::vector<Shippable*> items_;
    unsigned int position_;
};

#endif

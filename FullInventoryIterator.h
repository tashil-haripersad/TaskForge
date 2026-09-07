#ifndef FULLINVENTORYITERATOR_H
#define FULLINVENTORYITERATOR_H

#include "CargoIterator.h"
#include <vector>

class ShippingContainer;

// ConcreteIterator: visits every leaf item (a Box, or a decorated Box)
// reachable from the container it was built from, in pre-order (depth-first)
// sequence, to print a full manifest.
//
// Traversal-modification policy: the sequence of items is captured as a
// SNAPSHOT at construction time (see the constructor). A structural change
// made to the hierarchy after an iterator has been created does not
// retroactively affect that iterator; a new iterator must be requested to
// observe the updated hierarchy. This keeps an in-progress traversal safe
// and deterministic even if other code mutates the hierarchy at the same
// time, at the cost of that traversal not seeing very-latest changes -- a
// deliberate, documented trade-off for this system.
class FullInventoryIterator : public CargoIterator {
public:
    explicit FullInventoryIterator(ShippingContainer& root);

    bool hasNext() const override;
    Shippable* next() override;

private:
    std::vector<Shippable*> items_;
    unsigned int position_;
};

#endif

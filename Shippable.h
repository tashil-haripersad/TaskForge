#ifndef SHIPPABLE_H
#define SHIPPABLE_H

#include <string>
#include <vector>

// Component (abstract) of the Composite pattern, and simultaneously the
// Component (abstract) of the Decorator pattern. The two patterns
// deliberately share one abstraction: a decorated Box is still a Shippable
// and can be stored inside a ShippingContainer exactly like a plain Box
class Shippable {
    public:
        virtual ~Shippable() {}

        virtual double getWeight() const = 0;       // kg; recursive for composites
        virtual double getCost() const = 0;         // currency units; recursive for composites
        virtual std::string getName() const = 0;
        virtual bool isHazardous() const = 0;       // true if this item (or anything inside it) is dangerous goods

        // Traversal hook used only by CargoIterator implementations (see
        // CargoIterator.h) so that client code never needs direct access to a
        // container's internal storage. Leaves and decorated leaves append
        // themselves; composites recurse into their children instead
        virtual void collectLeaves(std::vector<Shippable*>& out) = 0;
};

#endif
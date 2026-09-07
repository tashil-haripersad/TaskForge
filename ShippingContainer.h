#ifndef SHIPPINGCONTAINER_H
#define SHIPPINGCONTAINER_H

#include "Shippable.h"
#include "CargoIterator.h"
#include <string>
#include <vector>

// Composite (concrete), and ConcreteAggregate for the Iterator pattern. Owns
// its children exclusively via raw pointers stored in children_: when a
// ShippingContainer is destroyed, every Box, decorated Box, and nested
// ShippingContainer beneath it is explicitly deleted too, recursively (see
// the destructor in ShippingContainer.cpp). A child can only exist inside
// one container at a time - moving it elsewhere is an explicit
// releaseItem()/addItem() pair (ownership transfer, never a shared or
// aliased pointer), and copying a ShippingContainer is disabled outright so
// two containers can never end up thinking they own the same children
class ShippingContainer : public Shippable, public CargoAggregate {
    public:
        explicit ShippingContainer(std::string name);
        ~ShippingContainer() override;

        ShippingContainer(const ShippingContainer&) = delete;
        ShippingContainer& operator=(const ShippingContainer&) = delete;

        // Takes ownership of item; it will be deleted when this container is
        // destroyed, unless it is released first via releaseItem()
        void addItem(Shippable* item);

        // Removes and returns the named item, transferring ownership to the
        // caller (e.g. so it can be handed to addItem() on a different
        // container, or rewrapped in a decorator and put back - the caller is
        // now responsible for eventually deleting it, or handing it to another
        // owner). Returns nullptr if no direct child has that name - this
        // method intentionally does not search recursively, matching the "no
        // reaching into a group merely to find something" spirit of the rules.
        Shippable* releaseItem(const std::string& itemName);

        double getWeight() const override;
        double getCost() const override;
        std::string getName() const override;
        bool isHazardous() const override;
        void collectLeaves(std::vector<Shippable*>& out) override;

        CargoIterator* createFullIterator() override;
        CargoIterator* createHazardousIterator() override;

        unsigned int childCount() const;

    private:
        std::string name_;
        std::vector<Shippable*> children_;
};

#endif
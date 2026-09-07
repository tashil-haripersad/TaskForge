#include "HazardousMaterialsIterator.h"
#include "ShippingContainer.h"

HazardousMaterialsIterator::HazardousMaterialsIterator(ShippingContainer& root) : position_(0) {
    std::vector<Shippable*> all;
    root.collectLeaves(all);
    for (Shippable* item : all) {
        if (item->isHazardous()) {
            items_.push_back(item);
        }
    }
}

bool HazardousMaterialsIterator::hasNext() const { return position_ < items_.size(); }

Shippable* HazardousMaterialsIterator::next() {
    if (!hasNext()) return nullptr;
    return items_[position_++];
}

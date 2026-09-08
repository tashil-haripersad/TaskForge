#include "FullInventoryIterator.h"
#include "ShippingContainer.h"

FullInventoryIterator::FullInventoryIterator(ShippingContainer& root) : position_(0) {
    root.collectLeaves(items_);
}

bool FullInventoryIterator::hasNext() const { return position_ < items_.size(); }

Shippable* FullInventoryIterator::next() {
    if (!hasNext()) return nullptr;
    return items_[position_++];
}

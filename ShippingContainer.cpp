#include "ShippingContainer.h"
#include "FullInventoryIterator.h"
#include "HazardousMaterialsIterator.h"

ShippingContainer::ShippingContainer(std::string name) : name_(std::move(name)) {}

ShippingContainer::~ShippingContainer() {
    for (Shippable* child : children_) {
        delete child;
    }
}

void ShippingContainer::addItem(Shippable* item) {
    children_.push_back(item);
}

Shippable* ShippingContainer::releaseItem(const std::string& itemName) {
    for (auto it = children_.begin(); it != children_.end(); ++it) {
        if ((*it)->getName() == itemName) {
            Shippable* released = *it;
            children_.erase(it);
            return released;
        }
    }
    return nullptr;
}

double ShippingContainer::getWeight() const {
    double total = 0.0;
    for (const Shippable* child : children_) total += child->getWeight();
    return total;
}

double ShippingContainer::getCost() const {
    double total = 0.0;
    for (const Shippable* child : children_) total += child->getCost();
    return total;
}

std::string ShippingContainer::getName() const { return name_; }

bool ShippingContainer::isHazardous() const {
    for (const Shippable* child : children_) {
        if (child->isHazardous()) return true;
    }
    return false;
}

void ShippingContainer::collectLeaves(std::vector<Shippable*>& out) {
    for (Shippable* child : children_) child->collectLeaves(out);
}

CargoIterator* ShippingContainer::createFullIterator() {
    return new FullInventoryIterator(*this);
}

CargoIterator* ShippingContainer::createHazardousIterator() {
    return new HazardousMaterialsIterator(*this);
}

unsigned int ShippingContainer::childCount() const {
    return static_cast<unsigned int>(children_.size());
}
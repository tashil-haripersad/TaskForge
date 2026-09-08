#include "ShippableDecorator.h"

ShippableDecorator::ShippableDecorator(Shippable* wrapped) : wrapped_(wrapped) {}

ShippableDecorator::~ShippableDecorator()
{
    delete wrapped_;
}

double ShippableDecorator::getWeight() const
{
    return wrapped_->getWeight();
}

double ShippableDecorator::getCost() const
{
    return wrapped_->getCost();
}

std::string ShippableDecorator::getName() const
{
    return wrapped_->getName();
}

bool ShippableDecorator::isHazardous() const
{
    return wrapped_->isHazardous();
}

void ShippableDecorator::collectLeaves(std::vector<Shippable*>& out)
{
    out.push_back(this);
}

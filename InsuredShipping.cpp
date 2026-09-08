#include "InsuredShipping.h"

InsuredShipping::InsuredShipping(Shippable* wrapped, double premium) : ShippableDecorator(wrapped), premium_(premium) {}

double InsuredShipping::getCost() const
{
    return wrapped_->getCost() + premium_;
}

std::string InsuredShipping::getName() const
{
    return wrapped_->getName() + " [Insured]";
}
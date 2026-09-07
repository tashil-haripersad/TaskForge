#include "RefrigeratedShipping.h"
#include <sstream>

const double RefrigeratedShipping::kInsulationWeightKg = 2.5;
const double RefrigeratedShipping::kRefrigerationSurcharge = 45.0;

RefrigeratedShipping::RefrigeratedShipping(Shippable* wrapped, double targetCelsius) : ShippableDecorator(wrapped), targetCelsius_(targetCelsius) {}

double RefrigeratedShipping::getWeight() const
{
    return wrapped_->getWeight() + kInsulationWeightKg;
}

double RefrigeratedShipping::getCost() const
{
    return wrapped_->getCost() + kRefrigerationSurcharge;
}

std::string RefrigeratedShipping::getName() const
{
    std::ostringstream oss;
    oss << wrapped_->getName() << " [Refrigerated @ " << targetCelsius_ << "C]";
    return oss.str();
}

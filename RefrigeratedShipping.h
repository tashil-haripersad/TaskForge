#ifndef REFRIGERATEDSHIPPING_H
#define REFRIGERATEDSHIPPING_H

#include "ShippableDecorator.h"

class RefrigeratedShipping : public ShippableDecorator
{
    private:
        double targetCelsius_;
        static const double kInsulationWeightKg;
        static const double kRefrigerationSurcharge;

    public:
        RefrigeratedShipping(Shippable* wrapped, double targetCelsius);

        double getWeight() const override;
        double getCost() const override;
        std::string getName() const override;
};

#endif

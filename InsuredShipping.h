#ifndef INSUREDSHIPPING_H
#define INSUREDSHIPPING_H

#include "ShippableDecorator.h"

class InsuredShipping : public ShippableDecorator
{
    private:
        double premium_;

    public:
        InsuredShipping(Shippable* wrapped, double premium);

        double getCost() const override;
        std::string getName() const override;
};

#endif

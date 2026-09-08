#ifndef SHIPPABLEDECORATOR_H
#define SHIPPABLEDECORATOR_H

#include "Shippable.h"

class ShippableDecorator : public Shippable
{
    protected:
        Shippable* wrapped_;

    public:
        explicit ShippableDecorator(Shippable* wrapped);
        ~ShippableDecorator() override;

        ShippableDecorator(const ShippableDecorator&) = delete;
        ShippableDecorator& operator=(const ShippableDecorator&) = delete;

        double getWeight() const override;
        double getCost() const override;
        std::string getName() const override;
        bool isHazardous() const override;

        void collectLeaves(std::vector<Shippable*>& out) override;
};

#endif

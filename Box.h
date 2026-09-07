#ifndef BOX_H
#define BOX_H

#include "Shippable.h"
#include "BoxState.h"
#include <string>
#include <vector>

class Box : public Shippable {
    private:
        std::string name_;
        double weight_;
        double baseCost_;
        bool hazardous_;
        BoxState* state_;

    public:
        Box(std::string name, double weight, double baseCost, bool hazardous = false);
        ~Box() override;

        Box(const Box&) = delete;
        Box& operator=(const Box&) = delete;

        double getWeight() const override;
        double getCost() const override;

        std::string getName() const override;

        bool isHazardous() const override;
        void collectLeaves(std::vector<Shippable*>& out) override;
        void loadOntoTruck();
        void arriveAtCustoms();
        void clearCustoms();

        std::string getStateName() const;

        void setState(BoxState* newState);
};

#endif

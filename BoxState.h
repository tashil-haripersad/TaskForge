#ifndef BOXSTATE_H
#define BOXSTATE_H

#include <string>

class Box;

class BoxState
{
    public:
        virtual ~BoxState() {}

        virtual void loadOntoTruck(Box& box) = 0;
        virtual void arriveAtCustoms(Box& box) = 0;
        virtual void clearCustoms(Box& box) = 0;
        virtual std::string name() const = 0;
};

class InWarehouseState : public BoxState
{
    public:
        void loadOntoTruck(Box& box) override;
        void arriveAtCustoms(Box& box) override;
        void clearCustoms(Box& box) override;
        std::string name() const override { return "InWarehouse"; }
};

class InTransitState : public BoxState
{
    public:
        void loadOntoTruck(Box& box) override;
        void arriveAtCustoms(Box& box) override;
        void clearCustoms(Box& box) override;
        std::string name() const override { return "InTransit"; }
};

class CustomsClearanceState : public BoxState {
    public:
        void loadOntoTruck(Box& box) override;
        void arriveAtCustoms(Box& box) override;
        void clearCustoms(Box& box) override;
        std::string name() const override { return "CustomsClearance"; }
};

class DeliveredState : public BoxState {
    public:
        void loadOntoTruck(Box& box) override;
        void arriveAtCustoms(Box& box) override;
        void clearCustoms(Box& box) override;
        std::string name() const override { return "Delivered"; }
};

#endif

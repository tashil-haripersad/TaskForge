#include "Box.h"

Box::Box(std::string name, double weight, double baseCost, bool hazardous) : name_(std::move(name)), weight_(weight), baseCost_(baseCost), hazardous_(hazardous), state_(new InWarehouseState()) {}

Box::~Box()
{
    delete state_;
}

double Box::getWeight() const
{
    return weight_;
}

double Box::getCost() const
{ 
    return baseCost_; 
}

std::string Box::getName() const
{
    return name_;
}

bool Box::isHazardous() const
{
    return hazardous_;
}

void Box::collectLeaves(std::vector<Shippable*>& out)
{
    out.push_back(this);
}

void Box::loadOntoTruck()
{
    state_->loadOntoTruck(*this);
}

void Box::arriveAtCustoms()
{
    state_->arriveAtCustoms(*this);
}

void Box::clearCustoms()
{
    state_->clearCustoms(*this);
}

std::string Box::getStateName() const
{
    return state_->name();
}

void Box::setState(BoxState* newState)
{
    delete state_;
    state_ = newState;
}

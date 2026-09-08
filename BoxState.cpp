#include "BoxState.h"
#include "Box.h"
#include "Exceptions.h"

void InWarehouseState::loadOntoTruck(Box& box)
{
    box.setState(new InTransitState());
}

void InWarehouseState::arriveAtCustoms(Box&)
{
    throw InvalidStateTransition("Box cannot arrive at customs while still InWarehouse.");
}

void InWarehouseState::clearCustoms(Box&)
{
    throw InvalidStateTransition("Box cannot clear customs while still InWarehouse.");
}

void InTransitState::loadOntoTruck(Box&)
{
    throw InvalidStateTransition("Box is already InTransit and cannot be loaded again.");
}

void InTransitState::arriveAtCustoms(Box& box)
{
    box.setState(new CustomsClearanceState());
}

void InTransitState::clearCustoms(Box&)
{
    throw InvalidStateTransition("Box must arrive at customs before it can be cleared.");
}

void CustomsClearanceState::loadOntoTruck(Box&)
{
    throw InvalidStateTransition("Box cannot be loaded onto a truck while in customs clearance.");
}

void CustomsClearanceState::arriveAtCustoms(Box&)
{
    throw InvalidStateTransition("Box has already arrived at customs.");
}

void CustomsClearanceState::clearCustoms(Box& box)
{
    box.setState(new DeliveredState());
}

void DeliveredState::loadOntoTruck(Box&)
{
    throw InvalidStateTransition("Delivered boxes cannot be loaded onto a truck.");
}

void DeliveredState::arriveAtCustoms(Box&)
{
    throw InvalidStateTransition("Delivered boxes cannot arrive at customs again.");
}

void DeliveredState::clearCustoms(Box&)
{
    throw InvalidStateTransition("Delivered boxes have already cleared customs.");
}

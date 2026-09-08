#ifndef CARGOITERATOR_H
#define CARGOITERATOR_H

#include "Shippable.h"

// Iterator (abstract). A minimal external-iterator interface: the client
// asks hasNext()/next() and never sees how the underlying hierarchy is
// actually stored or walked.
class CargoIterator {
public:
    virtual ~CargoIterator() {}
    virtual bool hasNext() const = 0;
    virtual Shippable* next() = 0;
};

// Aggregate (abstract). Anything that can hand out iterators over its cargo.
// ShippingContainer is the ConcreteAggregate.
//
// Ownership note: createFullIterator()/createHazardousIterator() hand back a
// heap-allocated CargoIterator that the CALLER now owns and must delete once
// finished with it (iterators are meant to be short-lived, throwaway
// objects). This mirrors the manual-ownership style used throughout the
// project -- see README.md.
class CargoAggregate {
public:
    virtual ~CargoAggregate() {}
    virtual CargoIterator* createFullIterator() = 0;
    virtual CargoIterator* createHazardousIterator() = 0;
};

#endif

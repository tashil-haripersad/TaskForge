# TaskForge - Logistics & Shipping

A C++11 implementation of the TaskForge hierarchical work-processing brief,
instantiated in the **logistics & shipping** domain.

## Building and running

```
make        # builds ./taskforge
./taskforge # runs the two demo scenarios
make clean  # removes build artefacts
```

Everything lives in one flat directory, built with a single `Makefile`
target (`taskforge`), compiled with `-std=c++11`.

A typical memory check, once inside the supplied Docker environment:

```
valgrind --leak-check=full --show-leak-kinds=all ./taskforge
```

Ownership is managed manually with raw pointers. Every class that owns
something defines an explicit destructor that deletes it, and disables
copying (`= delete` on the copy constructor and copy assignment operator) so
two objects can never end up thinking they own the same resource — see
section 3 below for the full policy. All base classes with virtual
functions (`Shippable`, `BoxState`, `CargoIterator`, `CargoAggregate`)
declare a `virtual` destructor, so deleting through a base pointer (e.g.
destroying a `ShippingContainer` full of mixed `Shippable` children) runs
the correct derived destructor in every case.

## 1. Domain and problem

A freight forwarder manages **shipments**: nested groupings of cargo that
must be inspected in different ways, whose individual boxes move through a
customs/delivery lifecycle, and which sometimes need extra handling
(insurance, refrigeration) bolted on after the fact — sometimes several
kinds of handling stacked on the same box. A shipment is a tree: a global
shipment contains **containers**, containers contain **pallets**, pallets
contain **boxes** (and, in principle, further nested groupings) — so
treating "one box" and "a whole pallet" uniformly when asking "how much
does this weigh?" or "how much will this cost to ship?" is genuinely
useful, not just a textbook excuse for polymorphism.

## 2. GoF participants

### Composite

| Participant | Role in TaskForge |
|---|---|
| Component | `Shippable` |
| Leaf | `Box` |
| Composite | `ShippingContainer` |
| Client | `main.cpp` |

`ShippingContainer::getWeight()` / `getCost()` sum over their children,
recursing into any nested `ShippingContainer` automatically because every
child is accessed only through the `Shippable` interface.

### Iterator

| Participant | Role in TaskForge |
|---|---|
| Aggregate | `CargoAggregate` |
| ConcreteAggregate | `ShippingContainer` |
| Iterator | `CargoIterator` |
| ConcreteIterator | `FullInventoryIterator`, `HazardousMaterialsIterator` |

Both concrete iterators are built from the same `Shippable::collectLeaves`
traversal hook, but apply a different **selection rule**:
`FullInventoryIterator` keeps everything, `HazardousMaterialsIterator`
keeps only items where `isHazardous()` is true. Two independent iterators
can be live over the same `ShippingContainer` at once (demonstrated in
`main.cpp`, Scenario 2) because each one owns its own snapshot list and
position counter — they share no mutable state.

### State

| Participant | Role in TaskForge |
|---|---|
| Context | `Box` |
| State | `BoxState` |
| ConcreteState | `InWarehouseState`, `InTransitState`, `CustomsClearanceState`, `DeliveredState` |

`Box` never contains an `if (state == ...)` chain: it just forwards
`loadOntoTruck()` / `arriveAtCustoms()` / `clearCustoms()` to whichever
`BoxState` it currently holds, and that state decides whether the
transition is legal. An illegal transition throws `InvalidStateTransition`
(a `std::logic_error`), which `main.cpp` catches and reports — the "sensible
handling" the brief asks for, rather than a silent no-op or a crash.

### Decorator

| Participant | Role in TaskForge |
|---|---|
| Component | `Shippable` |
| ConcreteComponent | `Box` |
| Decorator | `ShippableDecorator` |
| ConcreteDecorator | `InsuredShipping`, `RefrigeratedShipping` |

`ShippableDecorator` forwards every `Shippable` operation to the object it
wraps by default; each concrete decorator overrides only what it changes
(`InsuredShipping` adds a premium to `getCost()`; `RefrigeratedShipping`
adds insulation weight to `getWeight()` and a surcharge to `getCost()`).
Because the Composite pattern's Component and the Decorator pattern's
Component are the *same* interface (`Shippable`), a decorated `Box` — even
one wrapped twice, as `BX-1003` is in the demo — can sit inside a
`ShippingContainer` and be picked up by either iterator with no special
casing anywhere in the system.

## 3. Ownership and destruction policy

Every owning relationship uses a raw pointer plus an explicit destructor
that deletes what it owns, and disables copying so ownership can never
become ambiguous or shared:

- A `ShippingContainer` owns its children exclusively via
  `std::vector<Shippable*>`. `~ShippingContainer()` loops over that vector
  and `delete`s every child, so destroying a container recursively destroys
  everything beneath it, including further nested containers (whose own
  destructors then do the same thing one level down). Its copy constructor
  and copy assignment operator are `= delete`d — copying a container by
  value would otherwise produce two containers whose `children_` vectors
  point at the same objects, and both destructors would try to delete them.
- A `Box` owns its current `BoxState*`; transitioning state
  (`Box::setState`) deletes the old state object before installing the new
  one, and `~Box()` deletes whatever state is left. `Box`'s copy operations
  are likewise deleted.
- A `ShippableDecorator` owns the `Shippable*` it wraps, deleting it in
  `~ShippableDecorator()`, so a stack of decorators forms a single
  ownership chain (outermost decorator → … → innermost `Box`) with no
  aliasing. Its copy operations are deleted too; every concrete decorator
  inherits that restriction automatically.
- There is exactly one owner for every object at every point in time. To
  move an item between containers, code must explicitly call
  `releaseItem(name)` on the source (which removes the pointer from
  `children_` *without* deleting it and hands it back to the caller) and
  then `addItem(...)` on the destination — there is never a moment where
  two containers hold the same object.
- `releaseItem` only searches **direct** children, deliberately — reaching
  deep inside a nested group "merely to find something" is exactly what
  the brief prohibits; a caller that needs to act on a deeply-nested box
  gets there by holding (or being handed) a pointer to the specific
  container that owns it, which is how `main.cpp` does it.
- The two `CargoIterator` factory methods (`createFullIterator()`,
  `createHazardousIterator()`) are the one place the project deliberately
  hands back a raw, caller-owned pointer rather than storing it inside
  another owner: iterators are meant to be short-lived, throwaway objects,
  so `main.cpp` `delete`s each one as soon as it is finished with it (see
  the `delete full;`, `delete hazmat;`, `delete inFlight;` etc. calls).

## 4. Traversal without exposing internals

`ShippingContainer` never exposes its `vector<Shippable*>`.
Client code only ever gets a `CargoIterator` from `createFullIterator()` /
`createHazardousIterator()`, and only ever calls `hasNext()` / `next()` on
it. The recursive walk itself happens inside `Shippable::collectLeaves`,
which is a traversal-support hook rather than a general-purpose accessor —
a `ShippingContainer` recurses into its children, a `Box` or
`ShippableDecorator` appends itself, and neither exposes storage.

## 5. Traversal-modification policy

**Policy: snapshot iteration.** Both `FullInventoryIterator` and
`HazardousMaterialsIterator` compute their full sequence of `Shippable*`
pointers once, in their constructor, by calling `collectLeaves` on the
container they were built from. After that point they are entirely
independent of the container's internal storage — they just walk their own
`std::vector<Shippable*>`.

Consequences, demonstrated in `main.cpp` Scenario 2:

- An iterator created *before* a structural change (moving `BX-2001`
  between containers), a decoration change (re-wrapping `BX-1002` in
  `InsuredShipping`), and an addition (`BX-2002`) continues to report
  exactly the objects that existed when it was created, in the same order,
  even though the underlying hierarchy has since changed shape.
- A *new* iterator created afterwards reflects the updated hierarchy (five
  items instead of four; `BX-1002` now shows its `[Insured]` tag and
  updated cost; `BX-2001` now appears under `Pallet A1` instead of
  `Container MSCU-2233`).
- This is safe by construction — an in-progress traversal can never be
  invalidated by another part of the system reshaping the tree, and never
  observes a torn/half-updated view — at the cost of that traversal not
  reflecting the very latest state. That trade-off is the deliberate,
  documented choice for this system, and is why iterators are cheap,
  short-lived, throwaway objects here rather than long-lived cursors into
  live container state.

## 6. Object, State, and Activity Diagrams

The class diagram follows directly from the headers in this directory; the 
participant tables above map 1:1 onto it. A meaningful object diagram is the 
tree built at the top of `main()` (`GS-01` → two containers → pallets/boxes, 
with the two stacked decorators around `BX-1003` shown as wrapper objects). 
The state diagram is the four `BoxState` classes and the transition methods 
in `BoxState.cpp`. Good activity-diagram candidates from this program: 
printing a manifest (traversal made visible, with a loop over `hasNext()`/`next()`), 
driving a box through its lifecycle (decisions and guards, including the 
rejected-transition branches), and the Scenario 2 re-consolidation workflow 
(fork/join between the structural change and the decoration change, both merging 
back before the "after" manifests are printed).

## 7. GDB/Valgrind
Every owning class pairs its `new` calls with an explicit `delete` in its 
destructor (or in `setState`, for the one case where an owned object is 
replaced rather than only released), copying is disabled everywhere ownership 
exists, and every polymorphic base has a virtual destructor, a 
`valgrind --leak-check=full` run against `./taskforge` is expected to report 
zero leaks and zero invalid frees. Good GDB breakpoints for the investigation: 
`BoxState.cpp` transition functions (to step through exactly which concrete state 
handles a given call and inspect `box`'s fields), and `FullInventoryIterator`'s 
constructor (to inspect the snapshot vector being built and confirm its contents 
before any later structural change).

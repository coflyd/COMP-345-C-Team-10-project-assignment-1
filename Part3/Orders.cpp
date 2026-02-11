// ========================= Orders.cpp =========================
// (All implementations are here)

#include "Orders.h"

// Small helper so validate() checks look simpler
static bool nonEmpty(const std::string& s) {
    return !s.empty();
}

// ------------------------------------------------------------
// Order (base)
// ------------------------------------------------------------

Order::Order()
    : description(new std::string("Order")),
    effect(new std::string("")) {
}

Order::Order(const std::string& d)
    : description(new std::string(d)),
    effect(new std::string("")) {
}

Order::Order(const Order& other)
    : description(new std::string(*other.description)),
    effect(new std::string(*other.effect)) {
}

Order& Order::operator=(const Order& other) {
    if (this != &other) {
        // We don't delete/re-new here because strings already exist.
        // Just copy the values.
        *description = *other.description;
        *effect = *other.effect;
    }
    return *this;
}

Order::~Order() {
    delete description;
    delete effect;
    description = nullptr;
    effect = nullptr;
}

std::string Order::getDescription() const { return *description; }
std::string Order::getEffect() const { return *effect; }

void Order::setDescription(const std::string& d) { *description = d; }
void Order::setEffect(const std::string& e) { *effect = e; }

std::ostream& operator<<(std::ostream& os, const Order& o) {
    os << o.getDescription();
    // Only show effect after execute() sets it
    if (!o.getEffect().empty()) {
        os << " | Effect: " << o.getEffect();
    }
    return os;
}

// ------------------------------------------------------------
// Deploy
// ------------------------------------------------------------

Deploy::Deploy()
    : Order("Deploy"),
    targetTerritory(new std::string("")),
    armies(0) {
}

Deploy::Deploy(const std::string& target, int a)
    : Order("Deploy to " + target + " (" + std::to_string(a) + ")"),
    targetTerritory(new std::string(target)),
    armies(a) {
}

Deploy::Deploy(const Deploy& other)
    : Order(other),
    targetTerritory(new std::string(*other.targetTerritory)),
    armies(other.armies) {
}

Deploy& Deploy::operator=(const Deploy& other) {
    if (this != &other) {
        Order::operator=(other);
        *targetTerritory = *other.targetTerritory;
        armies = other.armies;
    }
    return *this;
}

Deploy::~Deploy() {
    delete targetTerritory;
    targetTerritory = nullptr;
}

bool Deploy::validate() const {
    // Basic check: needs a name + positive armies
    return nonEmpty(*targetTerritory) && armies > 0;
}

void Deploy::execute() {
    // Always validate first
    if (!validate()) {
        setEffect("Invalid Deploy.");
        return;
    }

    // Real game would change territory armies.
    // For assignment now, just store a readable effect string.
    setEffect("Deployed " + std::to_string(armies) + " to " + *targetTerritory + ".");
}

Order* Deploy::clone() const {
    // Polymorphic deep copy
    return new Deploy(*this);
}

// ------------------------------------------------------------
// Advance
// ------------------------------------------------------------

Advance::Advance()
    : Order("Advance"),
    fromTerritory(new std::string("")),
    toTerritory(new std::string("")),
    armies(0) {
}

Advance::Advance(const std::string& from, const std::string& to, int a)
    : Order("Advance " + from + " -> " + to + " (" + std::to_string(a) + ")"),
    fromTerritory(new std::string(from)),
    toTerritory(new std::string(to)),
    armies(a) {
}

Advance::Advance(const Advance& other)
    : Order(other),
    fromTerritory(new std::string(*other.fromTerritory)),
    toTerritory(new std::string(*other.toTerritory)),
    armies(other.armies) {
}

Advance& Advance::operator=(const Advance& other) {
    if (this != &other) {
        Order::operator=(other);
        *fromTerritory = *other.fromTerritory;
        *toTerritory = *other.toTerritory;
        armies = other.armies;
    }
    return *this;
}

Advance::~Advance() {
    delete fromTerritory;
    delete toTerritory;
    fromTerritory = nullptr;
    toTerritory = nullptr;
}

bool Advance::validate() const {
    // Basic: from/to not empty, not same, armies > 0
    return nonEmpty(*fromTerritory)
        && nonEmpty(*toTerritory)
        && armies > 0
        && (*fromTerritory != *toTerritory);
}

void Advance::execute() {
    if (!validate()) {
        setEffect("Invalid Advance.");
        return;
    }

    // Real game would move/attack depending on ownership.
    setEffect("Advanced " + std::to_string(armies) + " from " + *fromTerritory
        + " to " + *toTerritory + ".");
}

Order* Advance::clone() const {
    return new Advance(*this);
}

// ------------------------------------------------------------
// Bomb
// ------------------------------------------------------------

Bomb::Bomb()
    : Order("Bomb"),
    targetTerritory(new std::string("")) {
}

Bomb::Bomb(const std::string& target)
    : Order("Bomb " + target),
    targetTerritory(new std::string(target)) {
}

Bomb::Bomb(const Bomb& other)
    : Order(other),
    targetTerritory(new std::string(*other.targetTerritory)) {
}

Bomb& Bomb::operator=(const Bomb& other) {
    if (this != &other) {
        Order::operator=(other);
        *targetTerritory = *other.targetTerritory;
    }
    return *this;
}

Bomb::~Bomb() {
    delete targetTerritory;
    targetTerritory = nullptr;
}

bool Bomb::validate() const {
    return nonEmpty(*targetTerritory);
}

void Bomb::execute() {
    if (!validate()) {
        setEffect("Invalid Bomb.");
        return;
    }

    // Real game would halve armies on target.
    setEffect("Bombed " + *targetTerritory + ".");
}

Order* Bomb::clone() const {
    return new Bomb(*this);
}

// ------------------------------------------------------------
// Blockade
// ------------------------------------------------------------

Blockade::Blockade()
    : Order("Blockade"),
    targetTerritory(new std::string("")) {
}

Blockade::Blockade(const std::string& target)
    : Order("Blockade " + target),
    targetTerritory(new std::string(target)) {
}

Blockade::Blockade(const Blockade& other)
    : Order(other),
    targetTerritory(new std::string(*other.targetTerritory)) {
}

Blockade& Blockade::operator=(const Blockade& other) {
    if (this != &other) {
        Order::operator=(other);
        *targetTerritory = *other.targetTerritory;
    }
    return *this;
}

Blockade::~Blockade() {
    delete targetTerritory;
    targetTerritory = nullptr;
}

bool Blockade::validate() const {
    return nonEmpty(*targetTerritory);
}

void Blockade::execute() {
    if (!validate()) {
        setEffect("Invalid Blockade.");
        return;
    }

    // Real game would double armies + make neutral.
    setEffect("Blockaded " + *targetTerritory + ".");
}

Order* Blockade::clone() const {
    return new Blockade(*this);
}

// ------------------------------------------------------------
// Airlift
// ------------------------------------------------------------

Airlift::Airlift()
    : Order("Airlift"),
    fromTerritory(new std::string("")),
    toTerritory(new std::string("")),
    armies(0) {
}

Airlift::Airlift(const std::string& from, const std::string& to, int a)
    : Order("Airlift " + from + " -> " + to + " (" + std::to_string(a) + ")"),
    fromTerritory(new std::string(from)),
    toTerritory(new std::string(to)),
    armies(a) {
}

Airlift::Airlift(const Airlift& other)
    : Order(other),
    fromTerritory(new std::string(*other.fromTerritory)),
    toTerritory(new std::string(*other.toTerritory)),
    armies(other.armies) {
}

Airlift& Airlift::operator=(const Airlift& other) {
    if (this != &other) {
        Order::operator=(other);
        *fromTerritory = *other.fromTerritory;
        *toTerritory = *other.toTerritory;
        armies = other.armies;
    }
    return *this;
}

Airlift::~Airlift() {
    delete fromTerritory;
    delete toTerritory;
    fromTerritory = nullptr;
    toTerritory = nullptr;
}

bool Airlift::validate() const {
    return nonEmpty(*fromTerritory)
        && nonEmpty(*toTerritory)
        && armies > 0
        && (*fromTerritory != *toTerritory);
}

void Airlift::execute() {
    if (!validate()) {
        setEffect("Invalid Airlift.");
        return;
    }

    // Real game: move armies anywhere (ignore adjacency).
    setEffect("Airlifted " + std::to_string(armies) + " from " + *fromTerritory
        + " to " + *toTerritory + ".");
}

Order* Airlift::clone() const {
    return new Airlift(*this);
}

// ------------------------------------------------------------
// Negotiate
// ------------------------------------------------------------

Negotiate::Negotiate()
    : Order("Negotiate"),
    playerA(new std::string("")),
    playerB(new std::string("")) {
}

Negotiate::Negotiate(const std::string& a, const std::string& b)
    : Order("Negotiate " + a + " <-> " + b),
    playerA(new std::string(a)),
    playerB(new std::string(b)) {
}

Negotiate::Negotiate(const Negotiate& other)
    : Order(other),
    playerA(new std::string(*other.playerA)),
    playerB(new std::string(*other.playerB)) {
}

Negotiate& Negotiate::operator=(const Negotiate& other) {
    if (this != &other) {
        Order::operator=(other);
        *playerA = *other.playerA;
        *playerB = *other.playerB;
    }
    return *this;
}

Negotiate::~Negotiate() {
    delete playerA;
    delete playerB;
    playerA = nullptr;
    playerB = nullptr;
}

bool Negotiate::validate() const {
    // Basic: players exist and can't negotiate with themselves
    return nonEmpty(*playerA) && nonEmpty(*playerB) && (*playerA != *playerB);
}

void Negotiate::execute() {
    if (!validate()) {
        setEffect("Invalid Negotiate.");
        return;
    }

    // Real game: both players can't attack each other this turn.
    setEffect("Negotiated peace between " + *playerA + " and " + *playerB + ".");
}

Order* Negotiate::clone() const {
    return new Negotiate(*this);
}

// ------------------------------------------------------------
// OrdersList
// ------------------------------------------------------------

OrdersList::OrdersList()
    : orders(new std::vector<Order*>()) {
}

void OrdersList::clearAndDelete() {
    // delete all orders we own
    for (std::size_t i = 0; i < orders->size(); ++i) {
        delete (*orders)[i];
    }
    orders->clear();
}

OrdersList::OrdersList(const OrdersList& other)
    : orders(new std::vector<Order*>()) {

    // Deep copy: each order clones itself (keeps the real derived type)
    orders->reserve(other.orders->size());
    for (std::size_t i = 0; i < other.orders->size(); ++i) {
        orders->push_back((*other.orders)[i]->clone());
    }
}

OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (this != &other) {
        // free old stuff first so we don't leak
        clearAndDelete();

        // then deep copy
        orders->reserve(other.orders->size());
        for (std::size_t i = 0; i < other.orders->size(); ++i) {
            orders->push_back((*other.orders)[i]->clone());
        }
    }
    return *this;
}

OrdersList::~OrdersList() {
    clearAndDelete();
    delete orders;
    orders = nullptr;
}

void OrdersList::add(Order* o) {
    // Caller passes new OrderType(...)
    // After adding, OrdersList owns it and will delete it later.
    if (o != nullptr) {
        orders->push_back(o);
    }
}

void OrdersList::remove(int index) {
    if (index < 0 || index >= static_cast<int>(orders->size())) return;

    // Must delete the object first (otherwise memory leak)
    delete (*orders)[index];

    // Then remove the pointer from the vector
    orders->erase(orders->begin() + index);
}

void OrdersList::move(int from, int to) {
    // bounds checks
    if (from < 0 || to < 0) return;
    if (from >= static_cast<int>(orders->size())) return;
    if (to >= static_cast<int>(orders->size())) return;
    if (from == to) return;

    // pull pointer out and insert it at new spot
    Order* tmp = (*orders)[from];
    orders->erase(orders->begin() + from);
    orders->insert(orders->begin() + to, tmp);
}

int OrdersList::size() const {
    return static_cast<int>(orders->size());
}

Order* OrdersList::at(int index) const {
    // non-owning access (OrdersList still owns memory)
    if (index < 0 || index >= static_cast<int>(orders->size())) return nullptr;
    return (*orders)[index];
}

std::ostream& operator<<(std::ostream& os, const OrdersList& ol) {
    os << "OrdersList (" << ol.size() << " orders)\n";
    for (int i = 0; i < ol.size(); ++i) {
        os << "  [" << i << "] " << *ol.at(i) << "\n";
    }
    return os;
}

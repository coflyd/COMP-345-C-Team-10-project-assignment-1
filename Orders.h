// ========================= Orders.h =========================
// (Declarations only. All function bodies are in Orders.cpp)

#pragma once

#include <iostream>
#include <string>
#include <vector>

// ------------------------------------------------------------
// Order (abstract base class)
//
// Why abstract?
// - We never create a plain "Order".
// - We only create specific orders (Deploy, Advance, etc.).
// - validate/execute depend on the type, so they are pure virtual.
// ------------------------------------------------------------
class Order {
public:
    Order();
    Order(const std::string& description);

    // Rule of 3: we use new/delete -> we need copy ctor, assignment, destructor
    Order(const Order& other);
    Order& operator=(const Order& other);
    virtual ~Order();

    // Each specific order decides if it is valid and what happens on execute
    virtual bool validate() const = 0;
    virtual void execute() = 0;

    // OrdersList stores Order*.
    // clone() lets us deep-copy the REAL derived object (Deploy/Bomb/etc.).
    virtual Order* clone() const = 0;

    // Simple getters so driver and << can print info
    std::string getDescription() const;
    std::string getEffect() const;

protected:
    // Derived classes use these to update their base text/effect
    void setDescription(const std::string& d);
    void setEffect(const std::string& e);

private:
    // Using pointers to follow the course “class-type members as pointers” style.
    // This also forces us to practice deep copying.
    std::string* description;
    std::string* effect;
};

// Print format: "description | Effect: ..."
std::ostream& operator<<(std::ostream& os, const Order& o);

// ------------------------------------------------------------
// Concrete Orders (6 types required)
// For now, validate() is “basic checks” because i dont have the full game logic yet 
// ------------------------------------------------------------

class Deploy : public Order {
public:
    Deploy();
    Deploy(const std::string& targetTerritory, int armies);

    Deploy(const Deploy& other);
    Deploy& operator=(const Deploy& other);
    ~Deploy() override;

    bool validate() const override;
    void execute() override;
    Order* clone() const override;

private:
    std::string* targetTerritory;
    int armies;
};

class Advance : public Order {
public:
    Advance();
    Advance(const std::string& from, const std::string& to, int armies);

    Advance(const Advance& other);
    Advance& operator=(const Advance& other);
    ~Advance() override;

    bool validate() const override;
    void execute() override;
    Order* clone() const override;

private:
    std::string* fromTerritory;
    std::string* toTerritory;
    int armies;
};

class Bomb : public Order {
public:
    Bomb();
    Bomb(const std::string& targetTerritory);

    Bomb(const Bomb& other);
    Bomb& operator=(const Bomb& other);
    ~Bomb() override;

    bool validate() const override;
    void execute() override;
    Order* clone() const override;

private:
    std::string* targetTerritory;
};

class Blockade : public Order {
public:
    Blockade();
    Blockade(const std::string& targetTerritory);

    Blockade(const Blockade& other);
    Blockade& operator=(const Blockade& other);
    ~Blockade() override;

    bool validate() const override;
    void execute() override;
    Order* clone() const override;

private:
    std::string* targetTerritory;
};

class Airlift : public Order {
public:
    Airlift();
    Airlift(const std::string& from, const std::string& to, int armies);

    Airlift(const Airlift& other);
    Airlift& operator=(const Airlift& other);
    ~Airlift() override;

    bool validate() const override;
    void execute() override;
    Order* clone() const override;

private:
    std::string* fromTerritory;
    std::string* toTerritory;
    int armies;
};

class Negotiate : public Order {
public:
    Negotiate();
    Negotiate(const std::string& playerA, const std::string& playerB);

    Negotiate(const Negotiate& other);
    Negotiate& operator=(const Negotiate& other);
    ~Negotiate() override;

    bool validate() const override;
    void execute() override;
    Order* clone() const override;

private:
    std::string* playerA;
    std::string* playerB;
};

// ------------------------------------------------------------
// OrdersList
//
// Owns the Order* pointers.
// That means:
// - destructor must delete them
// - remove() must delete one
// - copy/assignment must deep copy (clone each order)
// ------------------------------------------------------------
class OrdersList {
public:
    OrdersList();
    OrdersList(const OrdersList& other);
    OrdersList& operator=(const OrdersList& other);
    ~OrdersList();

    // Takes ownership of the pointer (caller should NOT delete after adding)
    void add(Order* o);

    // Deletes the object at index, then removes it from the vector
    void remove(int index);

    // Reorders an existing order
    void move(int from, int to);

    int size() const;

    // Returns raw pointer for access (OrdersList still owns it)
    Order* at(int index) const;

private:
    // Pointer to vector to match “class-type members as pointers” style
    std::vector<Order*>* orders;

    // Helper to avoid repeating deletion loops
    void clearAndDelete();
};

std::ostream& operator<<(std::ostream& os, const OrdersList& ol);

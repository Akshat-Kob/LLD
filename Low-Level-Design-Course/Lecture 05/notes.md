# Liskov Substitution Principle (LSP)

## SOLID Principles

* **S** → Single Responsibility Principle
* **O** → Open/Closed Principle
* **L** → Liskov Substitution Principle
* **I** → Interface Segregation Principle
* **D** → Dependency Inversion Principle

## Definition

> Objects of a subclass should be replaceable with objects of the parent class without breaking the correctness of the program.

### Simple Terms

If `B` is a child of `A`, then anywhere you use `A`, you should be able to use `B` without unexpected behavior.

## Example

```cpp
Account* acc = new FixedTermAccount();
acc->withdraw(500);
```

The client expects every `Account` to support withdrawal.

But:

```cpp
void withdraw(double amount) {
    throw logic_error("Withdrawal not allowed");
}
```

So `FixedTermAccount` cannot behave like a normal `Account`.

##
========================================================================================================================
##

# LSP Fix

## Problem

`FixedTermAccount` cannot support:

```cpp
withdraw()
```

but its parent expects it to.

## Fix

Separate contracts:

```text
DepositOnlyAccount
├── FixedTermAccount
└── WithdrawableAccount
    ├── SavingAccount
    └── CurrentAccount
```

## Result

* Child honors parent contract.
* No exceptions.
* No type checks.

##
========================================================================================================================
##

# Open/Closed Principle (OCP)

## Definition

> Software entities should be open for extension but closed for modification.

### Simple Terms

You should be able to add new functionality without changing existing code.

## Example

Persistence is defined as an abstraction:

```cpp
class Persistence {
public:
    virtual void save(ShoppingCart* cart) = 0;
};
```

Implementations:

```cpp
class SQLPersistence : public Persistence
```

```cpp
class MongoPersistence : public Persistence
```

```cpp
class FilePersistence : public Persistence
```

## Why OCP is Followed

To support a new storage type:

```cpp
class RedisPersistence : public Persistence
```

we only add a new class.

No changes are required in:

* `ShoppingCart`
* `ShoppingCartPrinter`
* Existing persistence classes

➡️ Existing code remains unchanged.

## Result

* Easy to extend.
* Existing code is not modified.
* New behavior added through inheritance/abstraction.

➡️ OCP Satisfied ✅

##
========================================================================================================================
##

# Single Responsibility Principle (SRP)

## Definition

> A class should have only one reason to change.

### Simple Terms

One class = One responsibility.

## Example

```text
Product              → Product data
ShoppingCart         → Cart logic
ShoppingCartPrinter  → Invoice printing
ShoppingCartStorage  → Database storage
```

## Result

* Each class has a single responsibility.
* Changes are isolated.
* Easier to maintain and test.

➡️ SRP Satisfied ✅

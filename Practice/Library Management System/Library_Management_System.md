# Library Management System - Design Notes

## Step 1: Clarify Requirements

### Who uses the system?

* Librarian
* Member
* Admin

---

## Step 2: Find Nouns

### What can they do?

* Search book
* Borrow book
* Return book
* View borrowed books
* Add book
* Remove book

---

## Step 3: Ask

### Circle nouns

Example:

> Member borrows Book from Library.

#### Nouns

* Member
* Book
* Library

These often become classes.

### For each noun

#### Does it have state?

Example: Book

##### State

* title
* author
* isbn
* availability

Yes → likely a class.

#### Does it have behavior?

Example: Member

##### Behavior

* borrowBook()
* returnBook()

Yes → class.

---

## Step 4: First Draft Entities

* Library
* Book
* Member
* Librarian

---

## Step 5: Relationships

### One library contains?

* Many books
* Many members

### One member has?

* Many borrowed books

### One book belongs to?

* One library

---

# Other Future Extensions

## 1. BookCopy

Most important.

---

## 2. Fine

Late returns.

---

## 3. IssueRecord

Track:

* issueDate
* returnDate
* member
* book

Very common interview extension.

---

## 4. NotificationManager

Support:

* Email
* SMS
* WhatsApp

simultaneously.

---

## 5. SearchStrategy

Search by:

* Title
* Author
* ISBN

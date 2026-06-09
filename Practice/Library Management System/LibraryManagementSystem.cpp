#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// What information does a Book store?
// What actions can happen on a Book?
class Book {
private:
    string bookId;
    string title;
    string author;
    bool isAvailable;

public:
    // Constructor)
    Book(string id, string bookTitle, string bookAuthor){
        this->bookId = id;
        this->title = bookTitle;
        this->author = bookAuthor;
        this->isAvailable = true; // Assuming a new book is available by default
    }
    // another way to initialise constructor
    // Book(string bookId, string title, string author)
    //     : bookId(bookId),
    //       title(title),
    //       author(author),
    //       isAvailable(true) {}

    // Getters
    string getBookId() const {
        return bookId;
    }

    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    bool getAvailability() const {
        return isAvailable;
    }

    // Setters
    void setAvailability(bool available) {
        isAvailable = available;
    }

    // borrowBook()
    void borrowBook() {
        if (isAvailable) {
            isAvailable = false;
        }
    }

    // returnBook()
    void returnBook() {
        isAvailable = true;
    }
};

class Member {
private:
    string memberId;
    string name;

public:
    Member(string id, string memberName) {
        this->memberId = id;
        this->name = memberName;
    }

    string getMemberId() {
        return memberId;
    }

    string getName() {
        return name;
    }
};

class Library {
private:
    vector<Book*> books;
    vector<Member*> members;

public:
    // addBook()
    void addBook(Book* book) {
        if (book) {
            books.push_back(book);
        }
    }

    // removeBook()
    void removeBook(Book* book) {
        if (book) {
            books.erase(std::remove(books.begin(), books.end(), book), books.end());
        }
    }

    // registerMember()
    void registerMember(Member* member) {
        if (member) {
            members.push_back(member);
        }
    }

    // searchBook()
    Book* searchBook(const string& title, const string& author) {
        for (Book* book : books) {
            if (book->getTitle() == title && book->getAuthor() == author) {
                return book;
            }
        }
        return nullptr; // Return nullptr if the book is not found
    }
};

class Notification {
public:
    virtual void sendNotification(string message) = 0;
};

class EmailService : public Notification {
public:
    void sendNotification(string message) override {
        cout << "[EMAIL] " << message << endl;
    }
};

// Future extensions:
// class SMSService : public Notification
// class WhatsAppService : public Notification

class Librarian {
private:
    string librarianId;
    string name;
    Notification* notification;


public:
    Librarian(string librarianId, string name, Notification* notification) {
        this->librarianId = librarianId;
        this->name = name;
        this->notification = notification;
    }

    void issueBook(Member* member, Book* book) {
        if (member && book && book->getAvailability()) {
            book->borrowBook();
            notification->sendNotification("Book issued successfully");
        }
    }

    void acceptReturn(Member* member, Book* book) {
        if (member && book) {
            book->returnBook();
            notification->sendNotification("Book returned successfully");
        }
    }
};

class Admin {
private:
    string adminId;
    string name;
    vector<Librarian*> librarians; // but librarian is a different class so how can we have a vector of librarians in admin class?
    // we can have a vector of pointers to Librarian objects in the Admin class. This allows the Admin class to manage multiple Librarian instances without needing to know the details of their implementation. The Admin class can add or remove Librarian instances from this vector as needed, and it can call methods on the Librarian instances through the pointers. This design promotes separation of concerns and allows for more flexible management of Librarians within the Library Management System.


public:
    Admin(string adminId, string name) {
        this->adminId = adminId;
        this->name = name;
    }

    void addLibrarian(Librarian* librarian) {
        if (librarian) {
            librarians.push_back(librarian);
        }
    }

    void removeLibrarian(Librarian* librarian) {
        if (librarian) {
            librarians.erase(std::remove(librarians.begin(), librarians.end(), librarian), librarians.end());
        }
    }

    void manageMembers() {
        cout << "Managing members...\n";
    }
};

class Reservation {
private:
    string reservationId;
    Member* member;
    Book* book;
    bool isReserved;

public:
    Reservation(string reservationId, Member* member, Book* book) {
        this->reservationId = reservationId;
        this->member = member;
        this->book = book;
        this->isReserved = false;
    }

    void reserveBook() {
        if(book == nullptr) {
            cout << "Invalid book\n";
            return;
        }
        if(book->getAvailability()) {
            cout << "Book is available. No reservation needed.\n";
            return;
        }
        isReserved = true;
        cout << "Book reserved successfully\n";
        book->setAvailability(false);
    }
};

class LibraryRepository {
public:
    virtual void saveBook(Book* book) = 0;

    virtual void saveMember(Member* member) = 0;
};

class SQLRepository : public LibraryRepository {

public:
    void saveBook(Book* book) override {
        cout<<"Saving book to SQL database...\n";
    }

    void saveMember(Member* member) override {
        cout<<"Saving member to SQL database...\n";
    }
};

class MongoRepository : public LibraryRepository {

public:
    void saveBook(Book* book) override {
        cout<<"Saving book to MongoDB...\n";
    }

    void saveMember(Member* member) override {
        cout<<"Saving member to MongoDB...\n";
    }
};

int main() {

    Notification* emailService = new EmailService();

    Library library;

    Book* b1 = new Book("B101", "Atomic Habits", "James Clear");
    Book* b2 = new Book("B102", "Clean Code", "Robert Martin");

    library.addBook(b1);
    library.addBook(b2);

    Member* m1 = new Member("M101", "Akshat");

    library.registerMember(m1);

    Librarian* librarian =
        new Librarian("L101", "Rahul", emailService);

    Admin* admin =
        new Admin("A101", "Admin");

    admin->addLibrarian(librarian);

    cout << "\n--- Issue Book ---\n";

    librarian->issueBook(m1, b1);

    cout << "\n--- Return Book ---\n";

    librarian->acceptReturn(m1, b1);

    cout << "\n--- Reserve Book ---\n";

    librarian->issueBook(m1, b2);

    Reservation* reservation =
        new Reservation("R101", m1, b2);

    reservation->reserveBook();

    cout << "\n--- Repository Demo ---\n";

    LibraryRepository* repo =
        new SQLRepository();

    repo->saveBook(b1);
    repo->saveMember(m1);

    return 0;
}
#include "Database.h"
#include "User.h"

int main() {
    Database db("dbname=postgres user=postgres password=177013 hostaddr=127.0.0.1 port=5432");
    if (!db.isConnected()) {
        return 1;
    }

    // Create a new user
    User u("Alice", "alice@example.com");
    if (u.save(db)) {
        std::cout << "Created user with ID = " << u.getId() << "\n";
    }

    auto maybeBob = User::findById(db, 2);
    if (maybeBob) {
        maybeBob->setEmail("bob@newdomain.com");
        maybeBob->save(db);
        std::cout << "Updated Bob's email.\n";
    }

    auto all = User::findAll(db);
    for (auto& usrPtr : all) {
        std::cout << "User #" << usrPtr->getId()
                  << ": " << usrPtr->getName()
                  << " <" << usrPtr->getEmail() << ">\n";
    }

    if (maybeBob) {
        maybeBob->remove(db);
        std::cout << "Deleted Bob.\n";
    }

    return 0;
}

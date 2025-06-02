#include "Database.h"
#include "User.h"
#include "UserMapper.h"

int main() {
    Database db("host=localhost port=5432 dbname=myapp user=appuser password=secret");
    if (!db.isConnected()) return 1;

    UserMapper mapper(db);

    // Insert a new user
    User alice;
    alice.setName("Alice");
    alice.setEmail("alice@example.com");
    if (mapper.insert(alice)) {
        std::cout << "Inserted Alice with ID " << alice.getId() << "\n";
    }

    // Load by ID
    auto maybeBob = mapper.findById(2);
    if (maybeBob) {
        std::cout << "Found Bob: " << maybeBob->getName()
                  << " <" << maybeBob->getEmail() << ">\n";
        maybeBob->setEmail("bob@newdomain.com");
        mapper.update(*maybeBob);
    }

    // List all users
    auto allUsers = mapper.findAll();
    for (auto& uPtr : allUsers) {
        std::cout << "User #" << uPtr->getId()
                  << ": " << uPtr->getName()
                  << " <" << uPtr->getEmail() << ">\n";
    }

    // Delete Bob
    if (maybeBob) {
        mapper.remove(*maybeBob);
        std::cout << "Deleted Bob (ID now " << maybeBob->getId() << ")\n";
    }

    return 0;
}

#include <iostream>
#include <string>

class House {
public:
    void setFoundation(const std::string& foundation) {
        foundation_ = foundation;
    }
    void setWalls(const std::string& walls) {
        walls_ = walls;
    }
    void setRoof(const std::string& roof) {
        roof_ = roof;
    }

    void show() const {
        std::cout << "House built with:\n"
            << "  Foundation: " << foundation_ << "\n"
            << "  Walls:      " << walls_ << "\n"
            << "  Roof:       " << roof_ << "\n";
    }

private:
    std::string foundation_;
    std::string walls_;
    std::string roof_;
};

class HouseBuilder {
public:
    virtual ~HouseBuilder() {}
    virtual void buildFoundation() = 0;
    virtual void buildWalls() = 0;
    virtual void buildRoof() = 0;
    virtual House* getHouse() = 0;
};

class SimpleHouseBuilder : public HouseBuilder {
public:
    SimpleHouseBuilder() {
        house_ = new House();
    }
    ~SimpleHouseBuilder() override {
        delete house_;
    }

    void buildFoundation() override {
        house_->setFoundation("Simple concrete foundation");
    }
    void buildWalls() override {
        house_->setWalls("Wooden walls with drywall");
    }
    void buildRoof() override {
        house_->setRoof("Shingle roof");
    }
    House* getHouse() override {
        return house_;
    }

private:
    House* house_;
};

class Director {
public:
    void constructHouse(HouseBuilder* builder) {
        builder->buildFoundation();
        builder->buildWalls();
        builder->buildRoof();
    }
};

int main() {
    Director director;

    HouseBuilder* builder = new SimpleHouseBuilder();

    director.constructHouse(builder);

    House* house = builder->getHouse();
    house->show();

    delete builder;
    return 0;
}

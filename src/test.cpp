#include <custom_stl_vector.h>
#include <gtest/gtest.h>

class Product {
public:
    Product() : name_(nullptr), quantity_(0), price_(0.0) {}
    Product(const char* name, int quantity, double price)
        : quantity_(quantity), price_(price) {
        if (name) {
            name_ = new char[strlen(name) + 1];
            strcpy(name_, name);
        } else {
            name_ = nullptr;
        }
    }
    Product(const Product& other) {
        if (other.name_) {
            name_ = new char[strlen(other.name_) + 1];
            strcpy(name_, other.name_);
        } else {
            name_ = nullptr;
        }
        quantity_ = other.quantity_;
        price_ = other.price_;
    }
    Product& operator=(const Product& other) {
        if (this == &other) {
            return *this;
        }

        Product tmp(other);
        swap_(tmp);
        return *this;
    }
    ~Product() { delete[] name_; }
    bool operator==(const Product& other) const {
        return (strcmp(name_, other.name_) == 0) &&
               (quantity_ == other.quantity_) && (price_ == other.price_);
    }
private:
    void swap_(Product& other) {
        std::swap(name_, other.name_);
        std::swap(quantity_, other.quantity_);
        std::swap(price_, other.price_);
    }

    char* name_;
    int quantity_;
    double price_;
};

TEST(VectorTest, Constructors) {
    // vector();
    kirillidk_containers::vector<Product> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);

    // explicit vector(size_type __count);
    kirillidk_containers::vector<Product> v1(3);
    EXPECT_EQ(v1.size(), 3);

    // vector(const vector<T>& __other);
    kirillidk_containers::vector<Product> v2;
    v2.push_back(Product("Product1", 10, 20.5));
    v2.push_back(Product("Product2", 5, 15.0));

    kirillidk_containers::vector<Product> v3(v2);
    EXPECT_EQ(v3.size(), 2);
    EXPECT_EQ(v3[0], Product("Product1", 10, 20.5));
    EXPECT_EQ(v3[1], Product("Product2", 5, 15.0));
}

TEST(VectorTest, AssignmentOperator) {
    kirillidk_containers::vector<Product> v1;
    v1.push_back(Product("Product1", 10, 20.5));
    v1.push_back(Product("Product2", 5, 15.0));

    kirillidk_containers::vector<Product> v2;
    v2.push_back(Product("Product3", 7, 12.5));

    v2 = v1;

    EXPECT_EQ(v2.size(), 2);
    EXPECT_EQ(v2[0], Product("Product1", 10, 20.5));
    EXPECT_EQ(v2[1], Product("Product2", 5, 15.0));

    EXPECT_EQ(v1.size(), 2);
    EXPECT_EQ(v1[0], Product("Product1", 10, 20.5));
    EXPECT_EQ(v1[1], Product("Product2", 5, 15.0));
}

TEST(VectorTest, ElementAccess) {
    kirillidk_containers::vector<Product> v;
    v.push_back(Product("Product1", 10, 20.5));
    v.push_back(Product("Product2", 5, 15.0));

    EXPECT_EQ(v[0], Product("Product1", 10, 20.5));
    EXPECT_EQ(v[1], Product("Product2", 5, 15.0));
}

TEST(VectorTest, Capacity) {
    kirillidk_containers::vector<Product> v;
    EXPECT_EQ(v.empty(), true);
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);

    v.push_back(Product("Product1", 10, 20.5));
    EXPECT_EQ(v.empty(), false);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), 1);

    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);
}

TEST(VectorTest, Modifiers) {
    kirillidk_containers::vector<Product> v;
    v.push_back(Product("Product1", 10, 20.5));
    v.push_back(Product("Product2", 5, 15.0));

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], Product("Product1", 10, 20.5));
    EXPECT_EQ(v[1], Product("Product2", 5, 15.0));

    v.pop_back();
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], Product("Product1", 10, 20.5));

    v.push_back(Product("Product3", 7, 12.5));
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[1], Product("Product3", 7, 12.5));

    v.resize(3);
    EXPECT_EQ(v.size(), 3);
    v.resize(1);
    EXPECT_EQ(v.size(), 1);

    v.clear();
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.empty(), true);
}
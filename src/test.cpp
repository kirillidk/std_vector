#include <custom_stl_vector.h>
#include <gtest/gtest.h>

#include <string>

class Product {
public:
    Product() : name_("Untitled"), quantity_(0), price_(0) {}
    Product(const std::string& name, int quantity, double price)
        : name_(name), quantity_(quantity), price_(price) {}
    Product(const Product& other) = default;
    Product& operator=(const Product& other) = default;
    Product(Product&& other)
        : name_(std::move(other.name_)),
          quantity_(other.quantity_),
          price_(other.price_) {
        other.quantity_ = 0;
        other.price_ = 0;
    }
    Product& operator=(Product&& other) {
        if (this == &other) {
            return *this;
        }

        name_ = std::move(other.name_);
        quantity_ = other.quantity_;
        price_ = other.price_;

        other.quantity_ = 0;
        other.price_ = 0.0;

        return *this;
    }
    bool operator==(const Product& other) const {
        return (name_ == other.name_) && (quantity_ == other.quantity_) &&
               (price_ == other.price_);
    }
private:
    std::string name_;
    std::int32_t quantity_;
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

TEST(VectorTest, MoveConstructor) {
    kirillidk_containers::vector<Product> v;
    v.push_back(Product("Product1", 10, 20.5));
    v.push_back(Product("Product2", 5, 15.0));

    kirillidk_containers::vector<Product> moved_vector(std::move(v));

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);

    EXPECT_EQ(moved_vector.size(), 2);
    EXPECT_EQ(moved_vector[0], Product("Product1", 10, 20.5));
    EXPECT_EQ(moved_vector[1], Product("Product2", 5, 15.0));

    v.push_back(Product("Product1", 10, 20.5));
    v.push_back(Product("Product2", 5, 15.0));

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.capacity(), 2);
}

TEST(VectorTest, CopyAssignmentOperator) {
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

TEST(VectorTest, MoveAssignmentOperator) {
    kirillidk_containers::vector<Product> v1;
    v1.push_back(Product("Product3", 8, 12.5));
    v1.push_back(Product("Product4", 2, 5.0));

    kirillidk_containers::vector<Product> v2;
    v2.push_back(Product("OldProduct", 1, 1.0));

    v2 = std::move(v1);

    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.capacity(), 0);

    EXPECT_EQ(v2.size(), 2);
    EXPECT_EQ(v2[0], Product("Product3", 8, 12.5));
    EXPECT_EQ(v2[1], Product("Product4", 2, 5.0));
}

TEST(VectorTest, PushBackOverloads) {
    kirillidk_containers::vector<Product> v;

    Product product1("Product1", 10, 20.5);
    v.push_back(product1);

    EXPECT_EQ(product1, Product("Product1", 10, 20.5));
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], Product("Product1", 10, 20.5));

    Product product2("Product2", 5, 15.0);
    v.push_back(std::move(product2));

    EXPECT_EQ(product2, Product("", 0, 0.0));

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[1], Product("Product2", 5, 15.0));

    v.push_back(Product("TempProduct", 3, 7.5));
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[2], Product("TempProduct", 3, 7.5));

    v.reserve(1);
    v.push_back(Product("AnotherProduct", 1, 1.0));

    ASSERT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], Product("Product1", 10, 20.5));
    EXPECT_EQ(v[1], Product("Product2", 5, 15.0));
    EXPECT_EQ(v[2], Product("TempProduct", 3, 7.5));
    EXPECT_EQ(v[3], Product("AnotherProduct", 1, 1.0));
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
#include "gtest/gtest.h"
#include "tools/math/vec2.h"

using namespace tools::math;

TEST(Vec2Should, initializeCorrectly) {
    Vec2<float> x(1, 0);

    EXPECT_EQ(x.i, 1);
    EXPECT_EQ(x.j, 0);
}

TEST(Vec2Should, magCorrectly) {
    Vec2<float> x(1, 0);
    Vec2<float> y(0, 1);
    Vec2<float> z(1, 1);

    EXPECT_EQ(1, x.mag());
    EXPECT_EQ(1, y.mag());
    EXPECT_EQ(sqrt((float) 2), z.mag());
}

TEST(Vec2Should, dotProductCorrectly) {
    Vec2<int> x(1, 0);
    Vec2<int> y(0, 1);
    Vec2<int> z(1, 1);

    EXPECT_EQ(0, x.dot(y));
    EXPECT_EQ(0, y.dot(x));
    EXPECT_EQ(1, x.dot(z));
    EXPECT_EQ(1, y.dot(z));
    EXPECT_EQ(2, z.dot(z));
}

TEST(Vec2Should, crossProductCorrectly) {
    Vec2<int> x(1, 0);
    Vec2<int> x2(-1, 0);
    Vec2<int> y(0, 1);

    EXPECT_EQ(0, x.cross(x2));
    EXPECT_EQ(1, x.cross(y));
}

TEST(Vec2Should, scalarMultiply) {
    Vec2<float> x(42, 69);

    auto y = x.scalarMultiply(2);

    EXPECT_EQ(42 * 2, y.i);
    EXPECT_EQ(69 * 2, y.j);
}

TEST(Vec2Should, rotate) {
    Vec2<float> x(1, 0);

    auto y = x.rotate(M_PI / 2);

    EXPECT_EQ(Vec2<float>(cos((float)M_PI / 2), 1), y);
}

TEST(Vec2Should, provideEqualityOperator) {
    Vec2<float> x(0.234, 1.234);
    Vec2<float> y(0.234, 1.234);
    Vec2<float> z(0, 1.234);

    EXPECT_EQ(true, x == y);
    EXPECT_EQ(false, x == z);
}


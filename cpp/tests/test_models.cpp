#include "models.hpp"

#include <gtest/gtest.h>

namespace othello
{

TEST(step, initialization)
{
    constexpr Step step(1, 2);
    ASSERT_EQ(step.x, 1);
    ASSERT_EQ(step.y, 2);
}

TEST(step, addition)
{
    constexpr Step step1(1, 2);
    constexpr Step step2(2, 3);
    Step result = step1 + step2;
    ASSERT_EQ(result.x, 3);
    ASSERT_EQ(result.y, 5);

    result = Step {0, 0} + Step {1, 1};
    ASSERT_EQ(result.x, 1);
    ASSERT_EQ(result.y, 1);

    result = Step {-1, 0} + Step {1, 0};
    ASSERT_EQ(result.x, 0);
    ASSERT_EQ(result.y, 0);

    result += Step {-1, -1};
    ASSERT_EQ(result.x, -1);
    ASSERT_EQ(result.y, -1);

    result += Step {1, 1};
    ASSERT_EQ(result.x, 0);
    ASSERT_EQ(result.y, 0);
}

TEST(step, comparison)
{
    constexpr Step step1(1, 2);
    constexpr Step step2(1, 3);
    ASSERT_TRUE(step1 < step2);
    ASSERT_FALSE(step1 == step2);
    constexpr Step step3(2, 3);
    ASSERT_TRUE(step2 < step3);
    ASSERT_TRUE(step3 == Step(2, 3));
}

TEST(square, initialization)
{
    const Square square(3, 4);
    ASSERT_EQ(square.x, 3);
    ASSERT_EQ(square.y, 4);
}

TEST(square, comparison)
{
    const Square square1(3, 4);
    const Square square2(3, 5);
    ASSERT_TRUE(square1 < square2);
    ASSERT_FALSE(square1 == square2);
}

TEST(square, addition_with_square)
{
    const Square square2(1, 2);
    const Square square1(3, 4);
    Square result = square1 + square2;
    ASSERT_EQ(result.x, 4);
    ASSERT_EQ(result.y, 6);

    result = Square {4, 4} + Square {1, 1};
    ASSERT_EQ(result.x, 5);
    ASSERT_EQ(result.y, 5);

    result = Square {4, 4} + Square {0, 0};
    ASSERT_EQ(result.x, 4);
    ASSERT_EQ(result.y, 4);

    result = Square {4, 4} + Step {-1, 1};
    ASSERT_EQ(result.x, 3);
    ASSERT_EQ(result.y, 5);

    result += Square {0, 0};
    ASSERT_EQ(result.x, 3);
    ASSERT_EQ(result.y, 5);

    const Square another = Square {-3, -2} + Square {2, 3};
    ASSERT_EQ(another.x, -1);
    ASSERT_EQ(another.y, 1);
}

TEST(square, addition_with_step)
{
    const Square square(3, 4);
    constexpr Step step(1, 2);
    Square result = square + step;
    ASSERT_EQ(result.x, 4);
    ASSERT_EQ(result.y, 6);

    result += Step {-1, -1};
    ASSERT_EQ(result.x, 3);
    ASSERT_EQ(result.y, 5);
    result += Step {1, 0};
    ASSERT_EQ(result.x, 4);
    ASSERT_EQ(result.y, 5);
}

TEST(square, step_directions)
{
    const Square origin {1, 1};
    const Step step_directions[] = {
        {-1, -1},
        {-1, 0},
        {-1, 1},
        {0, -1},
        {0, 1},
        {1, -1},
        {1, 0},
        {1, 1},
    };
    const Square expected_results[] = {
        {0, 0},
        {0, 1},
        {0, 2},
        {1, 0},
        {1, 2},
        {2, 0},
        {2, 1},
        {2, 2},
    };

    for (int i = 0; i < std::size(step_directions); ++i) {
        Square result = origin + step_directions[i];
        ASSERT_EQ(result, expected_results[i]) << "Failed at index: " << i;
    }
}

}  // namespace othello

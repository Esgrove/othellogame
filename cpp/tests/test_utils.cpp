#include "utils.hpp"

#include <gtest/gtest.h>

#include <string>

namespace othello
{

TEST(step, initialization)
{
    const Step step(1, 2);
    ASSERT_EQ(step.x, 1);
    ASSERT_EQ(step.y, 2);
}

TEST(step, addition)
{
    const Step step1(1, 2);
    const Step step2(2, 3);
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
    const Step step1(1, 2);
    const Step step2(1, 3);
    ASSERT_TRUE(step1 < step2);
    ASSERT_FALSE(step1 == step2);
    const Step step3(2, 3);
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
    const Step step(1, 2);
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

TEST(disk, opponent)
{
    EXPECT_EQ(opponent(Disk::white), Disk::black);
    EXPECT_EQ(opponent(Disk::black), Disk::white);
    EXPECT_EQ(opponent(Disk::empty), Disk::empty);
}

TEST(calculate_sha256, simple)
{
    EXPECT_EQ(
        calculate_sha256("test"),
        "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08");
}

TEST(calculate_sha256, log_line)
{
    EXPECT_EQ(
        calculate_sha256("01: B:(0,1),1;____BBB__BW_____"),
        "d087cd890b81b7153cc870fba7372396e91cdcd4b6d13b82bbc88b5a4eb889d1");
}

TEST(calculate_sha256, full_log)
{
    std::string test_log;
    test_log += "01: B:(0,1),1;____BBB__BW_____\n";
    test_log += "02: W:(0,0),1;W___BWB__BW_____\n";
    test_log += "03: B:(1,0),1;WB__BBB__BW_____\n";
    test_log += "04: W:(0,2),2;WB__WBB_WWW_____\n";
    test_log += "05: B:(0,3),1;WB__WBB_WBW_B___\n";
    test_log += "06: W:(2,0),3;WWW_WWW_WBW_B___\n";
    test_log += "07: B:(3,0),1;WWWBWWB_WBW_B___\n";
    test_log += "08: W:(1,3),1;WWWBWWB_WWW_BW__\n";
    test_log += "09: B:(2,3),2;WWWBWWB_WWB_BBB_\n";
    test_log += "10: W:(3,2),2;WWWBWWW_WWWWBBB_\n";
    test_log += "11: B:(3,1),1;WWWBWWWBWWBWBBB_\n";
    test_log += "12: W:(3,3),1;WWWBWWWBWWWWBBBW";

    EXPECT_EQ(
        calculate_sha256(test_log),
        "ad4679949110ed7997aa1bf09441a7eb491b498189c03685ffdd6ddbb70e3c22");
}
}  // namespace othello

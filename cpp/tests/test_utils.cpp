#include "utils.hpp"

#include <gtest/gtest.h>

#include <string>

namespace othello
{
TEST(disk, opponent)
{
    EXPECT_EQ(opponent(Disk::white), Disk::black);
    EXPECT_EQ(opponent(Disk::black), Disk::white);
    EXPECT_EQ(opponent(Disk::empty), Disk::empty);
}

TEST(calculate_sha256, simple)
{
    EXPECT_EQ(
        calculate_sha256("test"), "9f86d081884c7d659a2feaa0c55ad015a3bf4f1b2b0b822cd15d6c15b0f00a08"
    );
}

TEST(calculate_sha256, log_line)
{
    EXPECT_EQ(
        calculate_sha256("01: B:(0,1),1;____BBB__BW_____"),
        "d087cd890b81b7153cc870fba7372396e91cdcd4b6d13b82bbc88b5a4eb889d1"
    );
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
        "ad4679949110ed7997aa1bf09441a7eb491b498189c03685ffdd6ddbb70e3c22"
    );
}
}  // namespace othello

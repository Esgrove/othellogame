#include "board.hpp"

#include <gtest/gtest.h>

namespace othello
{

TEST(board, can_play)
{
    Board board(8);
    EXPECT_TRUE(board.can_play());
}

TEST(board, game_result)
{
    Board board(8);
    Disk result = board.result();
    EXPECT_EQ(result, Disk::empty);

    auto moves = board.possible_moves(Disk::black);
    board.place_disk(moves[0]);

    result = board.result();
    EXPECT_EQ(result, Disk::black);
}

TEST(board, log_entry)
{
    Board board8(8);
    EXPECT_EQ(
        board8.log_entry(), "___________________________WB______BW___________________________"
    );

    Board board4(4);
    EXPECT_EQ(board4.log_entry(), "_____WB__BW_____");

    auto moves = board4.possible_moves(Disk::black);
    board4.place_disk(moves[0]);
    EXPECT_EQ(board4.log_entry(), "____BBB__BW_____");
}

}  // namespace othello

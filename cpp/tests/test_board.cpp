#include "board.hpp"

#include <gtest/gtest.h>

namespace othello
{

/// Test fixture that has access to the private members of `Board`.
class BoardTest : public ::testing::Test
{
protected:
    static std::optional<Disk> get_square(const Board& board, const Square& square)
    {
        return board.get_square(square);
    }

    static std::tuple<int, int> player_scores(const Board& board)
    {
        return board.player_scores();
    }

    static int score(const Board& board)
    {
        return board.score();
    }

    static size_t size(const Board& board)
    {
        return board.size;
    }
};

TEST_F(BoardTest, board_initialization)
{
    constexpr size_t board_size = 8;
    const Board board(board_size);
    EXPECT_EQ(size(board), board_size);
    // Check initial disk positions
    EXPECT_EQ(get_square(board, Square {3, 3}).value(), Disk::white);
    EXPECT_EQ(get_square(board, Square {4, 4}).value(), Disk::white);
    EXPECT_EQ(get_square(board, Square {3, 4}).value(), Disk::black);
    EXPECT_EQ(get_square(board, Square {4, 3}).value(), Disk::black);
    // Rest of the board should be empty
    EXPECT_EQ(get_square(board, Square {0, 0}).value(), Disk::empty);
    EXPECT_EQ(get_square(board, Square {7, 7}).value(), Disk::empty);
}

TEST_F(BoardTest, can_play)
{
    const Board board(8);
    EXPECT_TRUE(board.can_play());
}

TEST_F(BoardTest, scoring)
{
    const Board board(8);
    const auto [black_score, white_score] = player_scores(board);
    EXPECT_EQ(black_score, 2);
    EXPECT_EQ(white_score, 2);
    EXPECT_EQ(score(board), 0);
}

TEST_F(BoardTest, game_result)
{
    Board board(8);
    Disk result = board.result();
    EXPECT_EQ(result, Disk::empty);

    const auto moves = board.possible_moves(Disk::black);
    board.place_disk(moves[0]);

    result = board.result();
    EXPECT_EQ(result, Disk::black);
}

TEST_F(BoardTest, log_entry)
{
    const Board board8(8);
    EXPECT_EQ(
        board8.log_entry(), "___________________________WB______BW___________________________"
    );

    Board board4(4);
    EXPECT_EQ(board4.log_entry(), "_____WB__BW_____");

    const auto moves = board4.possible_moves(Disk::black);
    board4.place_disk(moves[0]);
    EXPECT_EQ(board4.log_entry(), "____BBB__BW_____");
}

}  // namespace othello

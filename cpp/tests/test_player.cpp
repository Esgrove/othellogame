#include "player.hpp"

#include <gtest/gtest.h>

namespace othello
{

TEST(player, new_player)
{
    Player player = Player::black(PlayerSettings {});
    EXPECT_EQ(player.can_play, true);
    EXPECT_TRUE(player.human());
    EXPECT_FALSE(player.computer());
}

TEST(player, reset)
{
    Player player = Player::white(PlayerSettings {});
    player.can_play = false;
    player.reset();
    EXPECT_TRUE(player.can_play);
}

TEST(player, set_human)
{
    Player player = Player::black(PlayerSettings {});
    EXPECT_TRUE(player.human());

    player.set_computer();
    EXPECT_TRUE(player.computer());
    EXPECT_FALSE(player.human());

    player.set_human();
    EXPECT_TRUE(player.human());
    EXPECT_FALSE(player.computer());
}

TEST(player, type_string)
{
    Player player = Player::black(PlayerSettings {});
    EXPECT_EQ(player.type_string(), "Human   ");

    player.set_computer();
    EXPECT_EQ(player.type_string(), "Computer");
}

}  // namespace othello

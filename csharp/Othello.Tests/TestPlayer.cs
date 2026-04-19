namespace Othello.Tests {
    public class TestPlayer {
        [Fact]
        public void NewPlayer() {
            Player player = Player.Black(new PlayerSettings(false, true, false));
            Assert.True(player.CanPlay);
            Assert.True(player.Human());
            Assert.False(player.Computer());
        }

        [Fact]
        public void ResetPlayer() {
            Player player = Player.White(new PlayerSettings(false, true, false));
            player.CanPlay = false;
            player.Reset();
            Assert.True(player.CanPlay);
        }

        [Fact]
        public void SetHumanAndComputer() {
            Player player = Player.Black(new PlayerSettings(false, true, false));
            Assert.True(player.Human());

            player.SetComputer();
            Assert.True(player.Computer());
            Assert.False(player.Human());

            player.SetHuman();
            Assert.True(player.Human());
            Assert.False(player.Computer());
        }
    }
}

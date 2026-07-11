namespace Othello.Tests;

public class TestPlayer {
    [Fact]
    public void NewPlayer() {
        Player player = new(Disk.Black, PlayerSettings.Default());
        Assert.Equal(Disk.Black, player.Disk);
        Assert.True(player.Human());
        Assert.True(player.CanPlay);
        Assert.Equal(0, player.RoundsPlayed);
        Assert.Equal(PlayerSettings.Default(), player.Settings);
    }

    [Fact]
    public void ResetPlayer() {
        Player player = new(Disk.White, PlayerSettings.Default());
        player.CanPlay = false;
        player.RoundsPlayed = 10;
        player.Reset();
        Assert.True(player.CanPlay);
        Assert.Equal(0, player.RoundsPlayed);
    }

    [Fact]
    public void SetHuman() {
        Player player = new(Disk.Black, PlayerSettings.Default());
        player.SetComputer();
        Assert.True(player.Computer());
        Assert.False(player.Human());

        player.SetHuman();
        Assert.True(player.Human());
        Assert.False(player.Computer());
    }

    [Fact]
    public void PlayerTypeString() {
        Player player = new(Disk.Black, PlayerSettings.Default());
        Assert.Equal("Human   ", player.TypeString());

        player.SetComputer();
        Assert.Equal("Computer", player.TypeString());
    }
}

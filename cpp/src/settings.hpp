//==========================================================
// Settings header
// Defines game and player settings.
// Akseli Lukkarila
// 2019-2025
//==========================================================

#pragma once

namespace othello
{
/// Player settings.
struct PlayerSettings {
    explicit PlayerSettings(const bool show_helpers, const bool check_mode, const bool test_mode) :
        show_helpers(show_helpers),
        check_mode(check_mode),
        test_mode(test_mode)
    {}

    PlayerSettings() : show_helpers(true), check_mode(false), test_mode(false) {}

    bool show_helpers;
    bool check_mode;
    bool test_mode;
};

/// Game settings.
struct Settings {
    Settings(
        const size_t board_size,
        const bool autoplay_mode,
        const bool check_mode,
        const bool show_helpers,
        const bool show_log,
        const bool test_mode,
        const bool use_defaults
    ) :
        board_size(board_size),
        autoplay_mode(autoplay_mode),
        check_mode(check_mode),
        show_helpers(show_helpers),
        show_log(show_log),
        test_mode(test_mode),
        use_defaults(use_defaults)
    {}

    Settings() :
        board_size(8),
        autoplay_mode(false),
        check_mode(false),
        show_helpers(true),
        show_log(false),
        test_mode(false),
        use_defaults(false)
    {}

    /// Get player setting values from overall game settings.
    [[nodiscard]] PlayerSettings to_player_settings() const
    {
        return PlayerSettings(show_helpers, check_mode, test_mode);
    }

    size_t board_size;
    bool autoplay_mode;
    bool check_mode;
    bool show_helpers;
    bool show_log;
    bool test_mode;
    bool use_defaults;
};
}  // namespace othello

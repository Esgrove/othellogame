# Unification plan

Scope: **cpp, csharp, go, java, kotlin, kotlin-native, python, rust, swift**.
TypeScript and Zig are explicitly out of scope.

Canonical reference: **Rust** — for names, member ordering, output strings, and doc comments —
with one exception (Phase 2): the player-default semantics, where Rust is changed to match the others.

Verification anchor: `./check.sh` (game-log SHA-256 parity for board sizes 4–10 across all
implementations) must stay green after every phase, plus each language's own unit tests.

## Global decisions (apply consistently everywhere)

1. **Player defaults — change Rust, keep others** (user decision):
   - `Player::white()` creates a **Human** player (like `black()`).
   - `Player::new` drops the `player_type` parameter → `new(disk, settings)`; type always starts Human
     (matches every other implementation).
   - `Othello::init_game` gains the `else if use_defaults { player_white.set_computer() }` branch.
   - Autoplay branch sets black first, then white (majority order).
2. **`Disk` API shape**: two separate methods `board_char()` (plain) and `board_char_with_color()`,
   plus `disk_color()`, `disk_string()`, `opponent()` — replace the merged `boardChar(color=true)`
   pattern everywhere. `disk_string` colors EMPTY white too (Swift currently doesn't).
3. **Module placement**: `PlayerType` lives in the player module; `version_info()` lives in the
   version module and returns the **full** string including the `"Othello <Lang> "` prefix;
   all `Disk` helpers live in models (C++ currently has them in utils).
4. **Output strings**: Rust's exact strings are canonical (see UNIFICATION_NOTES.md §F and the
   canonical-strings list). Highlights: `Error: `/`Warning: ` prefixes (not ALL CAPS),
   `4..10` (two dots), no `Warning:` prefix on the clamp message, trailing `!` on the two
   player error messages, colon in the occupied-square message, bold board indices,
   32-char separators printed after each player move, bold green `"The game is finished!\n"`,
   plain (non-bold) tie/winner lines, leading blank line in `print_score`,
   5-char indent before the first column index in `print_possible_moves`.
5. **Input handling**: Rust behavior is canonical:
   - `get_square`: read a line, split on `,`, parse arbitrary non-negative integers
     (multi-digit must work — board size 10), reject negatives, re-prompt on bad input.
   - `get_answer`: trim + case-insensitive compare.
   - `get_board_size`: clamp out-of-range with the plain yellow message; unparseable input →
     `print_warn("Invalid size, defaulting to 8...")` and return default (no infinite loop).
6. **`STEP_DIRECTIONS`**: element order matches Rust:
     `(-1,-1) (-1,1) (-1,0) (0,-1) (0,1) (1,-1) (1,1) (1,0)` (built from UP/DOWN/LEFT/RIGHT/STILL consts).
7. **`Move` semantics**: ordering = descending `value` then ascending square; equality and hash use
   `square`, `value`, `disk` only (never `directions`). Field/ctor order `square, disk, value, directions`.
8. **`PlayerSettings`** field order: `(show_helpers, check_mode, test_mode)`.
9. **`Settings` Display/toString**: match Rust's exact output (snake_case labels, order
   `board_size, autoplay, check_mode, use_defaults, show_helpers, show_log, test_mode`, no trailing newline).
10. **CLI**: `-a/--autoplay` conflicts with `-d/--default` in every implementation; version flag is
    lowercase `-v` everywhere (Java must override picocli's `-V`).
11. **Naming exceptions** (documented, intentional):
    - Go: `BlackPlayer`/`WhitePlayer`/`IsComputer` stay (name collisions with package-level constants).
    - Go: `CalculateSHA256` stays (Go initialism convention).
    - Swift: `defaultSettings` property for `--default` (keyword collision).
12. **Comments & docstrings**: Rust's doc comments are the canonical text. Port them verbatim
    (modulo language-appropriate doc syntax: `///`, `/** */`, KDoc, docstrings, Go doc comments)
    to every public type/method. Inline comments that explain logic (e.g. "Keep track of empty
    squares on board to avoid checking already filled positions", "Valid move only if a line of
    opposing disks ends with own disk") are copied too. File headers keep the existing
    `Akseli Lukkarila / 2019-2026` pattern (fix `python/othello/settings.py` which says 2019-2025).
13. **Extras policy**: keep platform-necessitated extras (ANSI color helpers, `clamp` where the
    stdlib lacks it, kotlin-native `sleep`) but give them one consistent shape and location
    (`colorprint` / `utils`). Delete dead code: Go `Board.get`, Python `print_blue`→fix,
    Java empty `MainTest.java`, Python broken `tests/test_othello.py` stub.

---

## Phase 0 — Baseline (no code changes)

- [ ] Run `./build.sh` and `./check.sh`; record green baseline.
- [ ] Run every language's unit test suite; record green baseline.

## Phase 1 — Bug fixes (small independent diffs, one commit per language)

- [ ] **C++**: clear `game_log` in `init_game` (othello.cpp:47); restore leading blank line in
      `print_score` (board.cpp:137, currently `fmt::print("")`); `place_disk` → distinct
      invalid-coordinates error + colon in occupied message; `get_square` multi-digit parsing;
      `print_error` to stdout (not stderr); remove double `\n` in the two player error messages;
      remove stray `false` arg at board.cpp:120.
- [ ] **Python**: clear `self.game_log` in `_init_game` (othello.py:56); fix `print_blue` color
      (colorprint.py:84); `Move.__eq__` must include `disk`; add `Move.__hash__`; fix broken
      `__le__`/`__ge__` on Step/Square/Direction/Move (derive from `__lt__`/`__eq__`);
      `_get_square` rejects negative coordinates.
- [ ] **Swift**: delete the 8 dead files in `Sources/Othello/` + the `exclude:` list in
      `Package.swift`; clear `gameLog` in `initGame`; add occupied-square guard to `placeDisk`;
      `setSquare` aborts on invalid coordinates; gate `printScore` on `!checkMode` (not testMode).
- [ ] **Java**: fix `Settings.toString()` label/argument order (Settings.java:26-44); fix the
      unformatted `%d` in `getBoardSize` (Main.java:104) and route it through `printWarn`;
      replace picocli standard `-V` with custom `-v` version flag; `getSquare` multi-digit parsing;
      `getAnswer` trims input.
- [ ] **C#**: `GetSquare` multi-digit parsing; `GetAnswer` trims input; `Move` equality/hash
      use square+value+disk only (no `Directions` reference comparison).
- [ ] **Kotlin + Kotlin-Native** (apply identically to both): `getSquare` multi-digit parsing;
      `getAnswer` trims input; `getBoardSize` trims input.
- [ ] **Go**: add missing `!checkMode` guard to helper printing in `PlayOneMove` (player.go:62);
      remove dead `Board.get` (board.go:218).
- [ ] Verify: `./check.sh` still green (none of these affect single-game logs); unit tests green.

## Phase 2 — Player-defaults alignment (Rust changes to match the majority)

- [ ] **Rust**: `Player::new(disk, settings)` (drop `player_type` param, starts Human);
      `Player::white()` → Human; `init_game` gains `else if self.settings.use_defaults
      { self.player_white.set_computer(); }`; autoplay branch sets black before white;
      update Rust unit tests (`new_player`, `reset_player`, `set_human`).
- [ ] **All others**: confirm constructor shape is `(disk, settings)` and the `use_defaults`
      branch matches Rust's new wording/structure exactly (C#'s currently lives in a different
      spot; Python's is `elif` — fine).
- [ ] Verify: `./check.sh` green (autoplay/check paths set both players computer explicitly,
      so hashes are unaffected). Manually sanity-check the interactive flow once in Rust.

## Phase 3 — Output-string unification (canonical = Rust)

Per-language fixes from UNIFICATION_NOTES.md §F:

- [ ] **Go**: `Error: `/`Warning: ` prefixes (drop ALL CAPS); insert prefix after leading
      whitespace (port `split_leading_whitespace`); yellow "No moves available..."; red
      `print_error` for invalid human move; bold board indices (both `String()` and
      `printPossibleMoves`); bold "The game is finished!" + trailing blank line; drop leading
      `\n` from "Result:"; clamp message without `WARNING:` prefix, `4..10`;
      `Settings.String()` → Rust format (fixes `useDefaults` casing typo).
- [ ] **Python**: `Error: `/`Warning: ` prefixes; clamp message plain yellow `4..10`;
      tie/winner lines not bold; `show_log` composition moves to main (`show_log=log or check`)
      instead of inside `play()`; add `Settings.__str__` (Rust format).
- [ ] **C++**: clamp message plain yellow (not `print_warn`) + `4..10`; `Settings` Display →
      Rust format/order; missing `!` in `'x,y'` message.
- [ ] **C#**: bold "\nPlayers:"/"Result:"/round header/footer (currently Silver/plain);
      footer trailing blank line; clamp message plain yellow + `4..10`; missing `!` in `'x,y'`
      message; occupied-square colon; possible-moves header indent; bold+green banner;
      `ColorPrint.Error/Warn` → `PrintError`/`PrintWarn`; add `Settings.ToString` (Rust format).
- [ ] **Java**: colored `diskString()` everywhere `disk.name()` is printed (Turn line,
      can't-place error, winner line); tie/winner lines not bold; missing `!`/`:` punctuation;
      `setSquare` message → `"Invalid coordinates: {square}"`.
- [ ] **Kotlin ×2**: occupied-square colon; missing `!` ×2; bold board indices; tie/winner not
      bold; clamp message plain yellow + `4..10`; `Settings.toString` → Rust format;
      possible-moves header indent.
- [ ] **Swift**: 32-char separators, printed after each player's move (inside the player loop);
      bold (not dim) indices; bold footer; colored EMPTY; missing `!` ×2; `getBoardSize` →
      Rust behavior (clamp warning + default-on-invalid, no infinite loop); `description` for
      Settings → Rust format; basic ANSI magenta/cyan (match other implementations' codes).
- [ ] Add a **full-output parity check**: extend `check.sh` (or add `compare_output.sh`) to run
      each binary with e.g. `8 -atl`, strip ANSI codes and the language-specific banner/version
      lines, and diff the remaining stdout across implementations. This is the regression net
      for everything in this phase.
- [ ] Verify: hash check green + new full-output diff green.

## Phase 4 — Structural alignment (placement, helpers, ordering, naming)

One commit per language; no behavior changes — `check.sh` and the Phase-3 output diff must
stay byte-identical.

- [ ] **4a. Module placement**:
  - C++: move `board_char`, `board_char_with_color`, `disk_color`, `disk_string`, `opponent`,
    Disk `operator<<` from utils → models; move `print_version`/version helpers → version;
    align models.hpp/cpp internal order.
  - C#, Go, Java, Kotlin ×2, Python, Swift: move `PlayerType` into the player module/file.
  - C#, Python: move `version_info` into a version module (C#: `Version.cs` partial over the
    generated class or a `VersionInfo` static; Python: `version.py`); include `"Othello <Lang> "`
    prefix in the returned string (Go, Swift too — currently prefix added at call site).
  - C#: un-nest `Othello` from `Program`; move `Main`/`ResolveBoardSize` to a `Program`/entry file.
  - Java: `getBoardSize` moves from Main.java → Othello.java (static), matching Rust.
  - Kotlin: `getBoardSize` top-level fn → companion of `Othello`.
- [ ] **4b. Missing API** (add everywhere absent; see notes §C matrix):
  - `Disk.board_char` / `board_char_with_color` split (cpp, cs, java, kt, py, swift) and
    `disk_string` as a named method (java, py→keep `__str__` delegating, swift→keep
    `description` delegating).
  - `Square.board_index(board_size)` (all languages).
  - `Board.check_square`, `Board.square_index`, `init_board`, `init_empty_squares` as named
    private helpers (per matrix; Python renames `_initialize_board` → `_init_board`).
  - `Othello.format_game_log`, `print_round_header`, `print_game_end_footer` (cpp, go, py, swift).
  - `PlayerType.human()`/`computer()` helpers + display string (`"Human   "`/`"Computer"`)
    (cpp, cs, go, java, kt; swift renames `isHuman`/`isComputer` → `human`/`computer`).
  - `split_leading_whitespace` in Go colorprint.
  - `Step` addition operators + `Move` hash where representable (cs `Step+Step`, py `Step.__add__`,
    swift `Step` operators & conformances).
  - Defaults: `Settings.default()` / `PlayerSettings.default()` equivalents where missing
    (cs, go, java, kt Settings, swift) — needed by ported tests.
- [ ] **4c. Ordering** — reorder to Rust order (no signature changes):
  - Type order in models: `Disk, Step, Square, Direction, Move` (fix swift Square/Step swap,
    java/kt Direction/Move placement).
  - Disk methods: `board_char, board_char_with_color, disk_color, disk_string, opponent`.
  - Board: public `new, can_play, place_disk, possible_moves, print_possible_moves, print_score,
    result, log_entry`; private `check_coordinates, check_square, get_square, square_index,
    player_scores, score, set_square, init_board, init_empty_squares`; to-string last.
    (Fix C#/Swift `result`/`log_entry`+`print_score` swaps; Go/Kotlin `initBoard` position;
    C++ header+source order.)
  - Player: `new, black, white, play_one_move, reset, human, computer, set_player_type,
    set_human, set_computer, get_computer_move, get_human_move, get_square, type_string`
    (fix the near-universal `reset` and `set_player_type` placement).
  - Othello: Rust method order; `print_divider` (cs, java, kt) becomes the canonical inlined
    println or is kept only if added to Rust — **decision: inline it like Rust** to keep
    method lists identical.
  - Field orders: Board `{board, empty_squares, indices, size}`, Player `{can_play, disk,
    player_type, rounds_played, settings}`, Othello `{board, settings, player_black,
    player_white, game_log, games_played, rounds_played}`, Move ctor `square, disk, value,
    directions`, PlayerSettings `(show_helpers, check_mode, test_mode)`.
  - `STEP_DIRECTIONS` element order + UP/DOWN/LEFT/RIGHT/STILL constants (cpp, cs, go, java, py).
  - `play_one_move` branch shape: empty-moves case first (Rust style) — cpp, cs, java, kt, py, swift.
- [ ] **4d. Remaining naming**: Java `stepDirections` → `STEP_DIRECTIONS`; C# `useDefaultOptions`
      ctor param → `useDefaults`; parameter names `chosen_move` for `place_disk` everywhere;
      Go exceptions documented in a comment.
- [ ] Verify: builds green, unit tests green, hash + output diff byte-identical to Phase 3.

## Phase 5 — Comments and docstrings

- [ ] Extract the canonical doc text from Rust (one pass over `rust/src/`).
- [ ] Port to each language with its native doc syntax; same wording, same placement.
      Include the inline algorithm comments in `possible_moves`, `place_disk`, `init_board`,
      `Board::new` (indices/empty-squares rationale), `play_one_move`.
- [ ] Language-specific notes stay where genuinely language-specific (e.g. Rust's lifetime
      explanation comments stay Rust-only; kotlin-native `sleep` docs stay native-only).
- [ ] Fix file-header years (`python/othello/settings.py` 2019-2025 → 2019-2026).
- [ ] Verify: builds green (comment-only).

## Phase 6 — Tests

- [ ] Port missing Rust tests to every language: `board_initialization`, `scoring`,
      `step_addition` (where Step ops now exist), full `square_addition` chains,
      `player_type_string`, `new_player`/`reset_player` missing assertions
      (`rounds_played`, settings equality — enabled by Phase 4b defaults/equality).
  - Visibility strategy: Kotlin-style `internal` where available (kt, swift `@testable` needs
    `internal` not `private` on `getSquare`/`playerScores`/`score`; C# `InternalsVisibleTo`;
    Java package-private; Go internal test package `othello` instead of `othello_test` for
    white-box tests; C++ friend test fixture or test accessors).
- [ ] Align test names to Rust's (`set_human` not `setHumanAndComputer`, `calculate_sha256_simple`
      not `CalculateSha256Basic`, etc.) and ordering within test files.
- [ ] Remove/replace dead stubs: Java `MainTest.java`, Python `tests/test_othello.py`
      (fix the fixture and add a real replay-regression test: two consecutive games via
      mocked input produce independent logs — this covers the Phase 1 game_log bug class
      in at least one language; add equivalents where mocking stdin is cheap).
- [ ] Keep justified extra tests (Step/Square comparison tests, clamp tests) and port the
      good ones *to Rust* so every suite has the same test list.
- [ ] Verify: all suites green; `./check.sh` + output diff green.

## Phase 7 — Kotlin drift sync + final sweep

- [ ] Diff `kotlin/` vs `kotlin-native/` file by file; eliminate all accidental drift
      (Models.kt member order, ColorPrint wording, `take` vs `substring`) so only the
      platform-forced differences remain (`sleep`, `String.format` → use `padStart` in both,
      clikt command name/help). Optional future work (out of scope): merge into one Kotlin
      Multiplatform project.
- [ ] Re-run the Phase-1..6 checklists' verify steps once more from clean builds
      (`./build.sh && ./check.sh` + all unit suites + full-output diff).
- [ ] Update README if it documents structure; delete or update UNIFICATION_NOTES.md sections
      that are resolved.

## Commit / review strategy

- One commit per language per phase (e.g. `cpp: fix game log not cleared on replay`),
  so each diff is reviewable against exactly one section of this plan.
- Phases are sequential; languages within a phase are independent and can be done in any
  order (or in parallel).
- After every commit: that language's unit tests; after every phase: `./check.sh`.

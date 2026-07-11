# Cross-language unification notes

Comparison of all implementations against the Rust version (the canonical reference),
covering naming, code organization/ordering, behavior, and test coverage.
Generated 2026-06-11.

## Canonical structure (from Rust)

Module layout: `board`, `colorprint`, `models`, `othello`, `player`, `settings`, `utils`, `version`, `main`.

Canonical member ordering:

- **models**: types `Disk` (Black=-1, Empty=0, White=1), `Step{x,y}`, `Square{x,y}`, `Direction{step,count}`, `Move{square,disk,value,directions}`.
  Disk methods: `board_char`, `board_char_with_color`, `disk_color`, `disk_string`, `opponent`.
  Move methods: `log_entry`, `affected_squares`. Square: `board_index`.
  Move ordering: descending `value`, then ascending square. Move equality/hash: `square`, `value`, `disk` (NOT `directions`).
- **board**: `STEP_DIRECTIONS`; fields `{board, empty_squares, indices, size}`;
  public: `new`, `can_play`, `place_disk`, `possible_moves`, `print_possible_moves`, `print_score`, `result`, `log_entry`;
  private: `check_coordinates`, `check_square`, `get_square`, `square_index`, `player_scores`, `score`, `set_square`, `init_board`, `init_empty_squares`; Display last.
- **player**: `PlayerType{Human,Computer}` lives here (not in models); fields `{can_play, disk, player_type, rounds_played, settings}`;
  methods: `new(disk, settings, player_type)`, `black`, `white`, `play_one_move`, `reset`, `human`, `computer`, `set_player_type`, `set_human`, `set_computer`, `get_computer_move`, `get_human_move`, `get_square`, `type_string`.
  **`white()` constructs a Computer player; `black()` a Human.**
- **othello**: fields `{board, settings, player_black, player_white, game_log, games_played, rounds_played}`;
  methods: `new`, `play`, `init_game`, `game_loop`, `format_game_log`, `print_round_header`, `print_game_end_footer`, `print_log`, `print_result`, `print_status`, `get_answer`, `get_board_size`.
  `init_game` clears `game_log` on replay.
- **settings**: `MIN_BOARD_SIZE=4`, `MAX_BOARD_SIZE=10`, `DEFAULT_BOARD_SIZE=8`;
  `Settings{board_size, autoplay_mode, check_mode, show_helpers, show_log, test_mode, use_defaults}`;
  `PlayerSettings{show_helpers, check_mode, test_mode}`; `to_player_settings`.
- **utils**: `calculate_sha256`.
- **colorprint**: `print_error`, `print_warn`, `split_leading_whitespace`. Prefixes: `Error: ` / `Warning: ` (capitalized word, not ALL CAPS).
- **version**: `version_info()` returning `"Othello <Lang> <version> <build_time> <git_branch> <git_commit>"`.
- **main**: args (positional `size`; `-a/--autoplay` conflicts with `-d/--default`; `-c/--check`, `-l/--log`, `-n/--no-helpers`, `-t/--test`, `-v/--version`); `main`; `resolve_board_size`.
  Settings composition: `autoplay_mode = autoplay || check`, `show_log = log || check`, `test_mode = test || check`, `show_helpers = !no_helpers`.

Canonical output strings (for `-c` cross-language hash parity):

- `"Trying to place disk to an occupied square: {square}!"` (with colon)
- `"Invalid coordinates: {square}"` (distinct from occupied-square error)
- `"  Can't place a {disk} disk in square {square}!"` (trailing `!`)
- `"  Give coordinates in the form 'x,y'!"` (trailing `!`)
- `"Limiting board size to valid range 4..10"` — plain yellow, **no** `Warning:` prefix, two dots
- `"Invalid size, defaulting to 8..."` — via `print_warn` (gets `Warning:` prefix)
- 32-char `--------------------------------` after each player move; 32-char `================================` bold at game end
- `"The game is finished!\n"` green **bold** (trailing blank line)
- Board indices bold; `print_score` has a leading blank line; possible-moves header indent is 4 spaces + joined (5 chars before first index)
- `get_answer` trims and lowercases input

---

## A. Bugs (behavior diverges from Rust in a way that matters)

| # | Language | Bug | Location |
|---|----------|-----|----------|
| 1 | C++ | `init_game` does not clear `game_log` → replay produces wrong log + SHA-256 hash | `cpp/src/othello.cpp:47-52` |
| 2 | Python | Same: `_init_game` never clears `self.game_log` | `python/othello/othello.py:56-60` |
| 3 | Swift | Same: `initGame` never clears `gameLog` | `swift/Sources/OthelloLib/othello.swift:44-50` |
| 4 | Swift | `placeDisk` has **no occupied-square guard** at all | `swift/Sources/OthelloLib/board.swift:60-67` |
| 5 | Swift | `playOneMove` gates `printScore` on `!testMode` instead of `!checkMode` | `swift/Sources/OthelloLib/player.swift:46-49` |
| 6 | Java | `Settings.toString()` label/argument order mismatch — `showHelpers:` prints `useDefaults` value etc. | `java/.../Settings.java:26-44` |
| 7 | Java | `getBoardSize()` prints literal `%d` (never formatted) on empty input | `java/.../Main.java:103-106` |
| 8 | Java | Version flag is picocli's `-V`, not `-v` like all other implementations | `java/.../Main.java:21` |
| 9 | Go | `PlayOneMove` helper-display condition missing the `!checkMode` guard | `go/othello/player.go:62` |
| 10 | Python | `print_blue` passes `Color.red` | `python/othello/colorprint.py:84` |
| 11 | TypeScript | ~~`--no-helpers` broken: `showHelpers` is always `true` (Commander negated-flag + extra `!`)~~ **fixed** | `typescript/src/main.ts` |
| 12 | TypeScript | ~~`--check` does not imply autoplay/log/test~~ **fixed** | `typescript/src/main.ts` |
| 13 | Swift | `Sources/Othello/` contains 8 **dead, drifted copies** of the lib files (excluded in Package.swift) — should be deleted | `swift/Package.swift`, `swift/Sources/Othello/*` |
| 14 | C++, C#, Java, Kotlin×2 | `get_square` input parsing requires exactly 3 chars (`x,y` single digits) — breaks on board size 10 | e.g. `cpp/src/player.cpp:138`, `csharp/Othello/Player.cs:132`, `java/.../Player.java:169`, `kotlin*/.../Player.kt:111` |

## B. The big semantic fork: default player types

Rust: `Player::black()` = Human, `Player::white()` = **Computer**. `init_game` only asks questions;
white is computer by construction.

C#, Go, Java, Kotlin, Kotlin-Native, Swift: both factories create **Human** players, and
`init_game` adds a compensating `else if (useDefaults) playerWhite.setComputer()` branch that Rust doesn't have.

Net divergence: when the user answers **"n"** to "Would you like to play against the computer",
Rust plays human (black) vs computer (white); the others play human vs human.
(And answering "yes" + "play as white": Rust ends computer-vs-computer; others black-computer vs white-human.)

**Decide the canonical intent, then align all implementations** (the Rust behavior of never
allowing human-vs-human looks accidental — but it is the reference; either fix Rust or the rest).

## C. Cross-cutting structural gaps (same fix in many languages)

Missing Rust members. ✗ = missing/inlined, ✓ = present. (kt = both Kotlin variants)

| Rust item | cpp | cs | go | java | kt | py | swift | ts |
|---|---|---|---|---|---|---|---|---|
| `Disk::board_char_with_color` (separate from `board_char`) | ✗ merged w/ bool param | ✗ | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `Disk::disk_string` | in utils | ✓ | ✓ | ✗ uses `.name()` | ✓ | in `__str__` | in `description` | ✗ |
| `Disk` helpers located in models (not utils) | ✗ in utils | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `Square::board_index` | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `Board::check_square` | ✗ | ✗ | ✗ | ✗ | ✗ | ✓ | ✗ | ✗ |
| `Board::square_index` | ✓ | ✗ | ✓ | ✗ | ✗ | ✓ | ✗ | ✗ |
| `Board::init_board` helper | ✗ | ✗ | ✓ | ✗ | ✓ | ✓ (`_initialize_board` name) | ✗ | ✗ |
| `Board::init_empty_squares` helper | ✗ | ✗ | ✗ | ✗ | ✓ | ✗ | ✗ | ✗ |
| `Othello::format_game_log` | ✗ | ✓ | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ |
| `Othello::print_round_header` | ✗ | ✓ | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ |
| `Othello::print_game_end_footer` | ✗ | ✓ | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ |
| `PlayerType` defined in player module | ✓ | ✗ models | ✗ models | ✗ models | ✗ models | ✗ models | ✗ models | ✓ |
| `PlayerType::human/computer` helpers | ✗ | ✗ | ✗ | ✗ | ✗ | ✓ | ✓ (`isHuman/isComputer` names) | ✗ |
| `Player::new` takes `player_type` param | ✓ | ✗ | ✗ | ✗ | ✗ | ✓ | ✗ | ✗ |
| `version_info()` in version module | ✗ in utils | ✗ in utils | ✓ | ✗ constant | ✗ constant | ✗ in utils | ✓ | ✗ |
| `-a` conflicts with `-d` | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `split_leading_whitespace` | ✓ | ✓ | ✗ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `Settings` Display/toString | ~ differs | ✗ | ~ differs | bug | ~ differs | ✗ | ~ differs | ✗ |

**Note: the `-a`/`-d` CLI conflict exists in NO other language** — only Rust enforces it.

Common extras not in Rust (decide: port to Rust or drop):
- `clamp` helper (go, java, py, swift, ts) — Rust uses built-in.
- `print_bold`/`print_color`/`get_color` + ANSI color enum (cs, go, java, kt, py, swift) — replaces the `colored` crate; unavoidable, but should have ONE agreed shape and file (`colorprint`).
- `Step.toString` `"[x,y]"` (cs, go, java, kt, py) — Rust `Step` has no Display.
- `print_divider` method (cs, java, kt) — Rust inlines it.
- Go: dead `Board.get(x,y)` method; Python: broken `tests/test_othello.py` stub; Java: empty `MainTest.java`.

## D. Naming mismatches (after casing normalization)

- Go: `BlackPlayer`/`WhitePlayer`/`IsComputer` vs expected `Black`/`White`/`Computer` (name collisions with package-level constants — may need a deliberate convention).
- C#: `ColorPrint.Error`/`Warn` vs expected `PrintError`/`PrintWarn`.
- Python: `_initialize_board` vs `init_board`; `_type_string` private vs Rust public.
- Swift: `isHuman`/`isComputer` on PlayerType vs Rust `human`/`computer`; dead copy uses `calculateSHA256` vs lib `calculateSha256`.
- Go: `CalculateSHA256` (initialism style) vs others `CalculateSha256`-shaped — pick one rule for initialisms.
- Java: `stepDirections` constant not `STEP_DIRECTIONS` (static final convention).
- Test names: Rust `set_human` vs `setHumanAndComputer`/`SetHumanAndComputer` (cs, go, java, kt, swift) — align one way or the other.

## E. Ordering differences (most common patterns)

1. **`reset` placement**: Rust has it immediately after `play_one_move`; cpp, cs, py have it after the setters.
2. **`set_player_type` placement**: Rust has it *before* `set_human`/`set_computer`; cs, go, java, kt, py, swift have it after.
3. **`init_board`/`init_empty_squares`**: Rust last among private methods; go and kt have them first.
4. **Field order drift** in `Board`, `Player`, `Othello`, and `Move` ctor (`value`/`disk` swapped in cs, java, kt) in nearly every language — align to Rust field order.
5. **`result` vs `log_entry`**: C# and Swift swap them.
6. **`PlayerSettings` field order**: cs, java, kt use `(checkMode, showHelpers, testMode)`; Rust is `(show_helpers, check_mode, test_mode)`.
7. **`STEP_DIRECTIONS` element order** differs in cpp, cs, go (gameplay-neutral but trivially unifiable).
8. **Disk method order** differs in every language that has them.

## F. Output-string drift (matters for cross-language comparison)

| String | Languages diverging |
|---|---|
| Occupied-square message missing `:` | cpp, cs, java, kt |
| `"...in square {square}"` missing `!` | java, kt, swift, (go: whole message unstyled) |
| `"'x,y'"` missing `!` | cpp, cs, go, java, kt, swift |
| `ERROR:`/`WARNING:` ALL-CAPS prefixes | go, py |
| `Warning:` prefix wrongly added to board-size-limit message | cpp, cs, go (as `WARNING:`), java(kt), py, kt |
| `4...10` (three dots) vs `4..10` | cpp, cs, go, kt, py, ts |
| Board indices not bold | go, kt, swift (dim instead) |
| `"The game is finished!"` not bold / missing trailing blank line | cs, go, swift |
| Tie/winner lines bold (Rust: plain) | java, kt, py |
| `print_score` missing leading blank line | cpp |
| 31-char separators; separator per-round not per-move | swift |
| `print_error` to stderr (Rust: stdout) | cpp |
| Possible-moves header indent off by one space | cpp, cs, kt |
| `get_answer` doesn't trim input | cs, java, kt, py, swift |

## G. Per-language status summary

| Language | Status | Biggest work items |
|---|---|---|
| **Rust** | reference | Decide if white-defaults-to-Computer is really intended |
| **C++** | good | game_log clear bug; Disk helpers out of utils→models; header/source ordering; get_square parsing |
| **C#** | good | Move equality includes Directions (by reference!); white()=Human; nested Program class → separate Main; get_square parsing |
| **Go** | good | missing `!checkMode` guard; uncolored error messages; Settings.String drift; naming collisions (BlackPlayer/WhitePlayer) |
| **Java** | good | Settings.toString bug; `%d` bug; `-V` flag; uncolored `disk.name()` usage; PlayerSettings extra file ok-ish |
| **Kotlin** | good | white()=Human; unbolded indices; getAnswer trim; ~98% identical to kotlin-native — consider Kotlin Multiplatform merge |
| **Kotlin-Native** | good | same as Kotlin + identifies itself as plain "Othello Kotlin" (no NATIVE distinction); Move member order differs from JVM file |
| **Python** | good | game_log clear bug; Move.__eq__ ignores disk; broken `__le__`/`__ge__`; ERROR:/WARNING: prefixes; missing Step addition |
| **Swift** | fair | delete 8 dead files in Sources/Othello; gameLog clear bug; placeDisk guard; testMode/checkMode swap; dim vs bold |
| **TypeScript** | good | Full implementation unified with the Rust reference; included in check.sh and compare_output.sh |
| **Zig** | **empty** | Directory exists, no source at all |

## H. Suggested unification order

1. Fix the outright bugs (section A) — small, independent diffs per language.
2. Decide the canonical answer to the player-defaults fork (section B) and align `Player.white`/`init_game` everywhere.
3. Unify output strings (section F) — verify with `-c` check-mode hash across languages (`utils.calculate_sha256` of the game log makes this scriptable: run every binary with `-c -t <size>` and diff hashes).
4. Structural alignment (sections C–E): add missing helpers, move `PlayerType`/`version_info`/Disk helpers to their canonical modules, reorder members to Rust order.
5. Port missing tests (board_initialization, scoring, step/square addition, player_type_string are the common gaps; C++/C#/Go/Swift need test access to private members — follow Kotlin's `internal` approach or add test accessors).
6. ~~Bring TypeScript up to parity~~ Done: full implementation with models.ts, settings.ts, colorprint.ts, version.ts, board/player/othello, sha256, and a separate main entry.
7. Swift cleanup: remove dead Sources/Othello copies; consider Kotlin Multiplatform merge for kotlin/kotlin-native.
8. Decide zig/: implement or remove the empty directory.

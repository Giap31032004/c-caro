# Caro Game High-Level C++

Console Caro game written in C++17.

## Build

```bash
make
./build/CaroGame
```

On Windows, you can build with WSL:

```powershell
wsl bash -lc "cd '/mnt/c/dtvt/cc++/Projects/CaroGame_HighLevel_CPP' && make"
```

## Gameplay Features

- Player vs Player
- Player vs Bot
- Bot vs Bot
- Board size settings: 10x10, 15x15, 20x20
- Win rule settings:
  - Five or more
  - Exactly five
  - Not blocked at both ends
- Optional random first player
- Optional X/O symbol selection
- Optional turn time limit
- In-game commands:
  - `row col`
  - `undo`
  - `redo`
  - `history`
  - `hint`
  - `pause`
  - `quit`
- Last move highlight
- Replay save/watch/delete
- Player statistics

## Bot Features

- Easy: random empty move
- Medium: win immediately or block immediate loss
- Hard: attack/defense evaluation scoring
- Expert: minimax with alpha-beta pruning over strongest candidate moves
- Bot styles:
  - Balanced
  - Aggressive
  - Defensive

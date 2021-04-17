# minesweeper

Clone of Windows Minesweeper

## Dependencies
 - [SDL2](https://www.libsdl.org/download-2.0.php)

## Installation
```bash
git clone https://github.com/bitt4/minesweeper.git
cd minesweeper
make
./minesweeper
```

## Options
| Option              | Effect                                                        | Default value |
|---------------------|---------------------------------------------------------------|---------------|
|`-w`, `--width=NUM`  | Set width of the grid in cells                                | 16            |
|`-h`, `--height=NUM` | Set height of the grid in cells                               | 16            |
|`-d`, `--diff=NUM`   | Set difficulty, chance 1 in NUM that a mine will be generated | 7             |
|`-H`, `--help`       | Display help                                                  |               |

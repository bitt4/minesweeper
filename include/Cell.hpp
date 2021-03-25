#ifndef CELL_HPP
#define CELL_HPP

enum class CellType {
    Hidden,
    Flag,
    Question,
    TriggeredMine,
    FalseMine,
    Mine,
    Question2,
    Clear = 15
};

class Cell {
private:
    CellType m_type;
    bool m_flagged;
public:
    Cell(CellType type);
    void flag();
    void unflag();
    CellType type();
};

#endif //CELL_HPP

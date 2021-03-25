#ifndef CELL_HPP
#define CELL_HPP

#include <cstddef>

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
    CellType m_type { CellType::Clear };
    bool m_flagged;
public:
    static const size_t cell_width { 16 };
    void flag();
    void unflag();
    CellType type() const;
    void set_type(CellType type);
};

#endif //CELL_HPP

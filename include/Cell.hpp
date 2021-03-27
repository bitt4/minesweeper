#ifndef CELL_HPP
#define CELL_HPP

#include <cstddef>
#include <cstdint>

class Cell {
public:
    enum class Type : uint8_t {
        Hidden,
        Flag,
        Question,
        TriggeredMine,
        FalseMine,
        Mine,
        Question2,
        Nearby8,
        Nearby7,
        Nearby6,
        Nearby5,
        Nearby4,
        Nearby3,
        Nearby2,
        Nearby1,
        Nearby0,
    };
    static const size_t cell_width { 16 };
    void toggle_flag();
    Type type() const;
    bool revealed() const;
    void reveal();
    void set_type(Type type);
private:
    Type m_type { Type::Nearby0 };
    bool m_flagged;
    bool m_revealed;
};

#endif //CELL_HPP

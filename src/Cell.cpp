#include "../include/Cell.hpp"

void Cell::toggle_flag(){
    m_flagged = !m_flagged;
}

CellType Cell::type() const {
    return m_type;
}

void Cell::set_type(CellType type){
    m_type = type;
}

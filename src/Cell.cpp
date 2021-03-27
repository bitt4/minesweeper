#include "../include/Cell.hpp"

void Cell::toggle_flag(){
    m_flagged = !m_flagged;
}

Cell::Type Cell::type() const {
    return m_type;
}

void Cell::set_type(Cell::Type type){
    m_type = type;
}

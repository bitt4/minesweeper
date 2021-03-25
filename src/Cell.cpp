#include "../include/Cell.hpp"

void Cell::flag(){
    m_flagged = true;
}

void Cell::unflag(){
    m_flagged = false;
}

CellType Cell::type() const {
    return m_type;
}

void Cell::set_type(CellType type){
    m_type = type;
}

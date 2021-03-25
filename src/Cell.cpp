#include "../include/Cell.hpp"

Cell::Cell(CellType type)
    :m_type(type)
{

}

void Cell::flag(){
    m_flagged = true;
}

void Cell::unflag(){
    m_flagged = false;
}

CellType Cell::type(){
    return m_type;
}

#include "Cell.hpp"

#include "Field.hpp"

Cell::Cell(Field *field, int x, int y):
    QObject()
{
    m_field = field;

    m_x = x;
    m_y = y;

    m_haveMine = false;
    m_open = false;
    m_mark = 0;
}

int Cell::minesAround() const
{
    int mines = 0;
    for (Cell *cell : getNeighbors()) {
        if (cell->haveMine()) {
            ++mines;
        }
    }

    return mines;
}

void Cell::setHaveMine(bool haveMine)
{
    m_haveMine = haveMine;
}

void Cell::open()
{
    if (m_field->isGenerated() == false) {
        m_field->generate(m_x, m_y);
    }

    if (m_open) {
        return;
    }

    m_open = true;

    if (minesAround() == 0) {
        for (Cell *cell : getNeighbors()) {
            cell->open();
        }
    }

    if (haveMine()) {
        m_field->lose();
    }
}

void Cell::reset()
{
    m_haveMine = false;
    m_open = false;
    m_mark = 0;
}

void Cell::toggleMark()
{
    if (m_mark == 2) {
        m_mark = 0;
    } else {
        ++m_mark;
    }
}

void Cell::tryToOpenAround()
{
    int mark;

    for (Cell *cell : getNeighbors()) {
        if (cell->mark() == 1) {
            ++mark;
        }
    }

    if (mark == minesAround()){
        for (Cell *cell : getNeighbors()) {
            if (cell->mark() == 0){
                cell->open();
            }
        }
    }
}

void Cell::setNeighbors(const QVector<Cell *> &neighbors)
{
    m_neighbors = neighbors;
}

QVector<Cell *> Cell::getNeighbors() const
{
    return m_neighbors;
}

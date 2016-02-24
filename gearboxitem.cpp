#include "gearboxitem.h"

//-------------
// Constructor
//-------------
GearBoxItem::GearBoxItem()
{
    // For now specify 0 as lowest gear and 7 as highest
    m_lowestGear = 0;
    m_highestGear = 7;
}

//----------------------------------------------
// Setter - setting specific gear
//
//  inputs: gear - target gear index (0-based)
//----------------------------------------------
void GearBoxItem::setSelectedGear(int gear)
{
    setGear(gear);
}

//----------------------------------------------------------------------
// Actual setting function for setting gear. Contains validity checks
//
// inputs: gear - target gear index (0-based)
//----------------------------------------------------------------------
void GearBoxItem::setGear(int gear)
{
    if (gear < m_lowestGear)
        m_SelectedGear = m_lowestGear;
    else if (gear > m_highestGear)
        m_SelectedGear = m_highestGear;
    else
        m_SelectedGear = gear;
}


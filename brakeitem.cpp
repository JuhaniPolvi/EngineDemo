#include "brakeitem.h"

//=============
// Constructor
//=============
BrakeItem::BrakeItem()
{
    setBrakingLevel(0);
}

//========================================================
// Setter: setting current braking level to desired value
//========================================================
void BrakeItem::setBrakingLevel(float value)
{
    // Checks are used to prevent setting invalid value. Given value is coerced to valid range
    if (value < 0)
        m_BrakingLevel = 0;
    else if (value > 100)
        m_BrakingLevel = 100;
    else
        m_BrakingLevel = value;
}


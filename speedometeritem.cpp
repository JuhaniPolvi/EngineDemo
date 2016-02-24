#include "speedometeritem.h"

//--------------
// Constructor
//--------------
SpeedoMeterItem::SpeedoMeterItem()
{
    // Initialise maximum speed and current speed
    setMaximumSpeed(270);
    setCurrentSpeed(0);
}

//----------------------------------------------------------
// Setter: setting current speed. Includes safety checks
//
// inputs: value - desired speed (km/h)
//----------------------------------------------------------
void SpeedoMeterItem::setCurrentSpeed(float value)
{
    // Ensure valid value
    if (value < 0)
        m_CurrentSpeed = 0;
    else if (value > m_MaxSpeed)
        m_CurrentSpeed = m_MaxSpeed;
    else
        m_CurrentSpeed = value;
}

//---------------------------------------------------------
// Setter: setting maximum speed
//
// inputs: value - desired maximum speed (km/h)
//---------------------------------------------------------
void SpeedoMeterItem::setMaximumSpeed(float value)
{
    // At the moment limit maximum speed to 270
    m_MaxSpeed = 270;
}

//--------------------------------------------------
// Setter: setting current speed
//
// inputs: speed - desired speed (km/h)
//--------------------------------------------------
void SpeedoMeterItem::setSpeed(float speed)
{
    setCurrentSpeed(speed);
}


#include "gear.h"

//-----------------------------------------------------------------------------
// Check function used to determine if referred gear is the same as this one.
// Checks sameness both by pointer and by values
//
// inputs: other - pointer to gear to check against
// returns: false - gears are not same
//          true  - gears are same (either by pointer or by values)
//-----------------------------------------------------------------------------
bool Gear::IsSame(Gear *other)
{
    // Null pointer is not the same
    if (other == 0)
        return false;

    // If pointers are the same it is same
    if (this == other)
        return true;

    // Check all members. If even one is different gears are not same
    if (m_GearMarking != other->Marking())
        return false;
    if (m_MaximumSpeed != other->MaxSpeed())
        return false;
    if (m_Acceleration != other->Acceleration())
        return false;
    if (m_MinimumSafeSwitchSpeed != other->MinSafeSwitchSpeed())
        return false;
    if (m_MaximumSafeSwitchSpeed != other->MaxSafeSwitchSpeed())
        return false;
    if (m_Type != other->Type())
        return false;

    // All members same. Consider these two the same
    return true;
}

//-------------
// Constructor
//-------------
Gear::Gear(QString marking, float maxspeed, float acceleration, float minsafeswitch, float maxsafeswitch, GearType gtype)
{
    // Enforce sensible values (no direct assignment)

    // Marking must not be empty (default to "1")
    if (marking.isEmpty())
        m_GearMarking = "1";
    else
        m_GearMarking = marking;

    // Maximum speed must be > 0 (default to 270). For neutral gear force value to 0. For reverse gear positive value is still used.
    if (gtype == GearType_Neutral)
        m_MaximumSpeed = 0;
    else if (maxspeed <= 0)
        m_MaximumSpeed = 270;
    else
        m_MaximumSpeed = maxspeed;

    // Acceleration must be > 0 (note: even in reverse positive values are used). For neutral acceleration is forced to 0
    if (gtype == GearType_Neutral)
        m_Acceleration = 0;
    else if (acceleration <= 0)
        m_Acceleration = 30;
    else
        m_Acceleration = acceleration;

    // Minimum safe switch speed can be 0 but can't be negative. Also, it can't be greater than maximum speed.
    if (minsafeswitch < 0)
        m_MinimumSafeSwitchSpeed = 0;
    else if (minsafeswitch > m_MaximumSpeed)
        m_MinimumSafeSwitchSpeed = m_MaximumSpeed;
    else
        m_MinimumSafeSwitchSpeed = minsafeswitch;

    // Maximum safe switch speed must be greater than or equal to minimum safe switch speed but can't exceed maximum speed
    if (maxsafeswitch < m_MinimumSafeSwitchSpeed)
        m_MaximumSafeSwitchSpeed = maxsafeswitch;
    else if (maxsafeswitch > m_MaximumSpeed)
        m_MaximumSafeSwitchSpeed = m_MaximumSpeed;
    else
        m_MaximumSafeSwitchSpeed = maxsafeswitch;

    m_Type = gtype;
}

//================================================================================
// Calculate acceleration for this gear according to relative acceleration amount
//
// inputs:   relativegas - relative acceleration in percent. Limited to valid
//                         range
// returns:  calculated acceleration (in km/h per second)
//================================================================================

float Gear::CalculateAcceleration(float relativegas)
{
    if (relativegas < 0)
        return 0;
    else if (relativegas > 100)
        return m_Acceleration;
    else
    {
        return ((relativegas*m_Acceleration)/100.0f);
    }
}


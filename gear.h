#ifndef GEAR_H
#define GEAR_H

#include <QString>

typedef enum
{
    GearType_Reverse, // Reverse gear
    GearType_Neutral, // Neutral gear
    GearType_Normal // Normal (forward) gear
} GearType;

//----------------------------------------
// Class used to model one gear in engine
//----------------------------------------

class Gear
{
public:

    //--------------------------
    // Public member variables
    //--------------------------

    //------------------
    // Public functions
    //------------------

    // Getters
    GearType Type() const {return m_Type;}
    QString Marking() const {return m_GearMarking;}
    float MaxSpeed() const {return m_MaximumSpeed;}
    float Acceleration() const {return m_Acceleration;}
    float MinSafeSwitchSpeed() const {return m_MinimumSafeSwitchSpeed;}
    float MaxSafeSwitchSpeed() const {return m_MaximumSafeSwitchSpeed;}

    // Is it safe to switch down from this gear (false=no, true=yes)
    bool SafeToSwitchDown(float speed)
    {
        if (speed < m_MinimumSafeSwitchSpeed)
            return false;
        else
            return true;
    }

    // Is it safe to switch up from this gear (false=no, true=yes)
    bool SafeToSwitchUp(float speed)
    {
        if (speed > m_MaximumSafeSwitchSpeed)
            return false;
        else
            return true;
    }

    // Check if given gear is same as this one
    bool IsSame(Gear* other);

    // Constructor. Use default variables to allow simplified construction. Marking is required
    Gear(QString marking, float maxspeed = 270, float acceleration = 30, float minsafeswitch = 0, float maxsafeswitch = 270, GearType gtype = GearType_Normal);

    // Calculate acceleration for this gear based on relative acceleration level (0=no acceleration, 100=maximum acceleration)
    float CalculateAcceleration(float relativegas);

protected:

    //----------------------------
    // Protected member variables
    //----------------------------

    // Note: all speeds are in km/h, all accelerations as change in km/h during one second

    QString m_GearMarking; // Text displayed on gearbox for this gear
    float m_MaximumSpeed; // Maximum reachable speed for this gear
    float m_Acceleration; // Maximum acceleration for this gear (at maximum gas)
    float m_MinimumSafeSwitchSpeed; // Minimum speed at which it is safe to switch from this gear to another
    float m_MaximumSafeSwitchSpeed; // Maximum speed at which it is safe to switch from this gear to another
    GearType m_Type; // Type of this gear

};

#endif // GEAR_H

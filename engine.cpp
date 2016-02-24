#include "engine.h"
#include "QtGlobal"
#include "QDebug"

//--------------------------------------------------------------------------
// Constructor
//
// inputs: parent - pointer to UI root object containing the UI components
//--------------------------------------------------------------------------
Engine::Engine(QObject *parent) : QObject(parent)
{
    // Set default rews and speed limits and initialise current values to 0
    m_MaxRews = 9000;
    m_CurrentRews = 0;
    m_TargetRews = 0;

    m_MaxSpeed = 270;
    m_TargetSpeed = 0;
    m_CurrentSpeed = 0;

    // Initialise acceleration and braking to 0 (scaled values as well)
    m_CurrentGas = 0;
    m_ScaledGas = 0.0f;
    m_CurrentBraking = 0;
    m_ScaledBraking = 0.0f;

    // Calculate timer period in seconds
    m_TimerPeriod = ((float)EngineTimerPeriod)/1000.0f;

    // At the beginning it is safe to switch gear
    m_TimeFromGearSwitch = GearSwitchPeriod;

    // Create gears
    m_NeutralCreated = false;
    m_ReverseCreated = false;
    AddGear(new Gear("R", 80, 80, 0, 0, GearType_Reverse));
    AddGear(new Gear("N", 0, 0, 0, 30, GearType_Neutral));
    AddGear(new Gear("1", 80, 100, 0, 60, GearType_Normal));
    AddGear(new Gear("2", 120, 80, 0, 100, GearType_Normal));
    AddGear(new Gear("3", 160, 60, 0, 140, GearType_Normal));
    AddGear(new Gear("4", 220, 40, 0, 200, GearType_Normal));
    AddGear(new Gear("5", 250, 20, 0, 230, GearType_Normal));
    AddGear(new Gear("6", 270, 10, 0, 270, GearType_Normal));

    // Debugging: list gears in order
    foreach (Gear *gr, m_Gears)
    {
        qDebug() << "Gear" << gr->Marking();
    }

    // By default use the neutral gear as the default one
    m_ActiveGearIndex = 1;
    m_TargetGearIndex = 1;

    qDebug() << "Starting timer...";

    // Set up timer for synchronization
    m_Timer = new QTimer(this);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(Timeout()));
    m_Timer->start(EngineTimerPeriod);
}

//----------------------------------------------------------------------
// Slot for handling acceleration change from the accelerator component
//
// inputs - relativelevel - percentage acceleration level (0 to 100)
//----------------------------------------------------------------------
void Engine::accelerate(float relativelevel)
{
    // Ensure valid value
    float actuallevel = relativelevel;
    if (actuallevel < 0)
        actuallevel = 0;
    else if (actuallevel > 100)
        actuallevel = 100;

    // Store current acceleration
    m_CurrentGas = relativelevel;

    // Calculate scaled value
    m_ScaledGas = relativelevel/100.0f;

//    qDebug() << "Acceleration level set to " << actuallevel;

//    // Rews are (for simplicity) linearly dependent on acceleration. Set target rews.
//    m_TargetRews = (actuallevel * m_MaxRews)/100.0f;

//    qDebug() << "Target rews set to " << m_TargetRews;

//    // Target speed is linearly dependent on rews
//    m_TargetSpeed = (actuallevel * m_Gears[m_ActiveGearIndex]->MaxSpeed())/m_TargetRews;
}

//----------------------------------------------------------------
// Slot for handling braking info from the brake component
//
// inputs - relativelevel - percentage braking level (0 to 100)
//----------------------------------------------------------------
void Engine::brake(float relativelevel)
{
    // Ensure valid value
    float actuallevel = relativelevel;
    if (actuallevel < 0)
        actuallevel = 0;
    else if (actuallevel > 100)
        actuallevel = 100;

    // Store value
    m_CurrentBraking = relativelevel;

    // Calculate scaled value
    m_ScaledBraking = relativelevel/100.0f;
}

//-------------------------------------------------------------------
// Slot for handling switching down from the gearbox component
//-------------------------------------------------------------------
void Engine::switchDown()
{
    // Ignore switch if already at lowest gear
    if (m_ActiveGearIndex == 0)
        return;

    // Check if it is safe to switch down. If not do nothing. Switching from reverse can only be done if stationary
    if (m_Gears[m_ActiveGearIndex]->Type() == GearType_Reverse)
    {
        if (m_CurrentSpeed > 0)
        {
            emit unsafeGearSwitch();
            return;
        }
    }
    else
    {
        if (!m_Gears[m_ActiveGearIndex]->SafeToSwitchDown(m_CurrentSpeed))
        {
            emit unsafeGearSwitch();
            return;
        }
    }

    // It is good to switch. Set target gear for delayed switching
    m_TargetGearIndex = m_ActiveGearIndex - 1;
}

//--------------------------------------------------------------------
// Slot for handling switching up from the gearbox component
//--------------------------------------------------------------------
void Engine::switchUp()
{
    // Ignore switch if already at highest gear
    if (m_ActiveGearIndex == m_Gears.count()-1)
        return;

    // Check if it is safe to switch up. If not do nothing. Switching from reverse can only be done if stationary
    if (m_Gears[m_ActiveGearIndex]->Type() == GearType_Reverse)
    {
        if (m_CurrentSpeed > 0)
        {
            emit unsafeGearSwitch();
            return;
        }
    }
    else
    {
        if (!m_Gears[m_ActiveGearIndex]->SafeToSwitchUp(m_CurrentSpeed))
        {
            emit unsafeGearSwitch();
            return;
        }
    }

    // It is good to switch. Set target gear for delayed switching
    m_TargetGearIndex = m_ActiveGearIndex + 1;
}

//---------------------------------------------------------------------------
// Slot for handling switching to specific gear from the gearbox component
//
// inputs - gearindex - index of gear to switch to (0 to number of gears-1)
//---------------------------------------------------------------------------
void Engine::switchToGear(int gearindex)
{
    // Ignore invalid index
    if ((gearindex < 0) || (gearindex >= m_Gears.count()))
        return;

    qDebug() << "Set target gear to" << gearindex;

    // Set target gear for delayed switching
    m_TargetGearIndex = gearindex;
}

//------------------------------------------------------------------
// Event handler function for timeout. Used to synchronize events
//------------------------------------------------------------------
void Engine::Timeout()
{
    // Handle change in state (speed/rews)
    HandleStateChange();

    // Handle gear switching
    SwitchGear();

    // Increment time from last gear switching
    m_TimeFromGearSwitch += EngineTimerPeriod;
}


//------------------------------------------------------------------------
// Add one gear to list of gears. Some logic is used to prevent having
// multiples of same gear. Valid gears are added to the list of gears
// in the order they are passed to this function
//
// inputs - gear - pointer to previously created gear object
//------------------------------------------------------------------------
void Engine::AddGear(Gear *gear)
{
    // Ignore null pointers (some safety needed)
    if (gear == 0)
        return;

    // Ignore duplicates (existing pointer)
    if (m_Gears.contains(gear))
    {
        return;
    }

    // Ignore duplicates (same values)
    foreach (Gear* check, m_Gears)
    {
        if (gear->IsSame(check))
        {
            return;
        }
    }

    // Ignore duplicate reverse gears
    if ((m_ReverseCreated) && (gear->Type() == GearType_Reverse))
    {
        return;
    }

    // Ignore duplicate neutral gears
    if ((m_NeutralCreated) && (gear->Type() == GearType_Neutral))
    {
        return;
    }

    // If gear is reverse or neutral mark it as created
    if (gear->Type() == GearType_Neutral)
        m_NeutralCreated = true;
    if (gear->Type() == GearType_Reverse)
        m_ReverseCreated = true;

    // Insert gear to list
    m_Gears.append(gear);

    // If gear is neutral mark it as active gear (otherwise use the first gear)
    if (gear->Type() == GearType_Neutral)
    {
        m_ActiveGearIndex = m_Gears.count()-1;
        m_TargetGearIndex = m_ActiveGearIndex;
    }
}

//--------------------------------------------------------------
// Try to switch gears if needed
//--------------------------------------------------------------
void Engine::SwitchGear()
{
    // If target gear is same as active gear do nothing
    if (m_ActiveGearIndex == m_TargetGearIndex)
        return;

    // If not enough time has been passed since last switch do nothing
    if (m_TimeFromGearSwitch < GearSwitchPeriod)
        return;

//    qDebug() << "Enough time passed since last gear switch";

    // If not safe to switch do nothing
    if (!SafeToSwitchGear())
        return;

//    qDebug() << "Safe for gear switch";

    // Switch down or up depending on target. Also reset time since gear switch
    if (m_TargetGearIndex < m_ActiveGearIndex)
    {
        m_ActiveGearIndex--;
        emit gearSwitched(m_ActiveGearIndex);
    }
    else
    {
        m_ActiveGearIndex++;
        emit gearSwitched(m_ActiveGearIndex);
    }

    m_TimeFromGearSwitch = 0;
}

//----------------------------------------------------------------------------
// Check if it is safe to switch gears
//
// returns - false if it is not safe to switch gears
//           true if it is safe to switch gears
//----------------------------------------------------------------------------
bool Engine::SafeToSwitchGear()
{
    // At the moment it is not permissible to switch from/to reverse gear if speed is not 0,
    // otherwise it is. The logic can be revised later.

    // Determine next gear (default to currently selected one for safety)
    int nextgearindex = m_ActiveGearIndex;
    if (m_TargetGearIndex < m_ActiveGearIndex)
        nextgearindex--;
    else if (m_TargetGearIndex > m_ActiveGearIndex)
        nextgearindex++;

    // Switching from/to reverse can only be done if stationary
    if ((m_Gears[m_ActiveGearIndex]->Type() == GearType_Reverse) && (m_CurrentSpeed > 0))
        return false;
    if ((m_Gears[nextgearindex]->Type() == GearType_Reverse) && (m_CurrentSpeed > 0))
        return false;

    return true;
}

//--------------------------------------------------------------------------------
// Event function used to handle rews/speed changes. Called from timer function
//--------------------------------------------------------------------------------
void Engine::HandleStateChange()
{
    if (m_Gears[m_ActiveGearIndex]->Type() != GearType_Neutral)
        HandleNonNeutralGear();
    else
        HandleNeutralGear();
}

//--------------------------------------------------------------------------------------------------------
// Event handler for handling speed/rews change on neutral gear. Called from HandleStateChange on timeout
//--------------------------------------------------------------------------------------------------------
void Engine::HandleNeutralGear()
{
    // On neutral gear rews are linearly dependent on current gas and will rise/fall on constant speed.
    // Speed will drop at a speed dependent on amount of braking but will drop even if not braking.
    // Rews and speed are not connected.

    //qDebug() << "Neutral gear, scaled gas is" << m_ScaledGas;

    // Calculate and set target rews
    float upperLimit = m_MaxRews * m_ScaledGas;
    if (m_CurrentRews < upperLimit)
        SetRews(m_CurrentRews + m_TimerPeriod * RewsIncreaseRate);
    else
        SetRews(m_CurrentRews - m_TimerPeriod * RewsDecreaseRate);

    // Calculate deceleration for speed drop
    float deceleration = (DecelerationRate + m_ScaledBraking * BrakingPower) * m_TimerPeriod;
    float newspeed = m_CurrentSpeed - deceleration;
    SetSpeed(newspeed);
}

//-------------------------------------------------------------------------------------------------------------
// Event handler for handling speed/rews change on non-neutral gear. Called from HandleStateChange on timeout
//-------------------------------------------------------------------------------------------------------------
void Engine::HandleNonNeutralGear()
{
    // On non-neutral gear rews are connected to speed and selected gear. At maximum speed for selected gear the rews
    // are also at maximum, at half speed at 50% and so on. Change in speed is calculated first and relevant rews
    // Calculated from result

    //qDebug() << "Non-neutral gear, scaled gas is" << m_ScaledGas << ", maximum speed is" << m_Gears[m_ActiveGearIndex]->MaxSpeed() << ", acceleration is" << m_Gears[m_ActiveGearIndex]->Acceleration();

    // Safety check: if maximum speed for the selected gear is not greater than 0 handle it as a neutral
    if (m_Gears[m_ActiveGearIndex]->MaxSpeed() <= 0)
        HandleNeutralGear();
    else
    {
        // Speed change is dependent on gas (increase) and braking (decrease). It is also affected by the natural deceleration (decrease)
        float speedchange = (-DecelerationRate + m_Gears[m_ActiveGearIndex]->Acceleration() - m_ScaledBraking * BrakingPower) * m_TimerPeriod;
        float deceleration = (DecelerationRate + m_ScaledBraking * BrakingPower) * m_TimerPeriod;
        float newspeed = m_CurrentSpeed;
        float targetspeed = m_Gears[m_ActiveGearIndex]->MaxSpeed() * m_ScaledGas;

        // targetspeed is the speed that current gas and current gear should have. Actual speed is gradually changed from current speed

        // Limit targetspeed to valid range
        if (targetspeed < 0)
            targetspeed = 0;
        else if (targetspeed > m_MaxSpeed)
            targetspeed = m_MaxSpeed;

        //qDebug() << "Target speed" << targetspeed << ", speedchange" << speedchange << ", deceleration"  << deceleration << ", newspeed" << newspeed;

        // Determine required speed change
        if (m_CurrentSpeed < targetspeed)
        {
            newspeed = m_CurrentSpeed + speedchange;
        }
        else
        {
            newspeed = m_CurrentSpeed - deceleration;
        }

        // Calculate rews based on speed
        float newrews = (newspeed * m_MaxRews)/m_Gears[m_ActiveGearIndex]->MaxSpeed();

        // Set values
        SetSpeed(newspeed);
        SetRews(newrews);
    }
}

//-----------------------------------------------------------------------------------------
// Setter function for setting speed. Contains validity checks to ensure sensible values
// and some logic to deal with decreasing speed if switched to lower gear
//
// inputs: value - desired speed (km/h)
//-----------------------------------------------------------------------------------------
void Engine::SetSpeed(float value)
{
    // Some logic used to decelerate on lower gear
    float deceleration = (DecelerationRate + m_ScaledBraking * BrakingPower) * m_TimerPeriod;
    float actualspeed = value;

    // Check if current speed is greater than the selected gear allows. If it is decelerate
    if (actualspeed > m_Gears[m_ActiveGearIndex]->MaxSpeed())
    {
        actualspeed -= deceleration;
    }

    // Limit value to valid range
    if (actualspeed < 0)
        actualspeed = 0;
    else if (actualspeed > m_MaxSpeed)
        actualspeed = m_MaxSpeed;

    // Set value and signal it
    m_CurrentSpeed = actualspeed;
    emit newSpeed(m_CurrentSpeed);
}

//---------------------------------------------------------------------------------------
// Setter function for setting rews. Contains validity checks to ensure sensible values.
// Uses member variable m_TargetRews as limit.
//
// inputs: value - amount of desired rews
//         upperLimit - maximum allowed rews
//---------------------------------------------------------------------------------------
void Engine::SetRews(float value)
{
    // Limit given value to valid range
    float actualvalue = value;
    if (actualvalue < 0)
        actualvalue = 0;
    else if (actualvalue > m_MaxRews)
        actualvalue = m_MaxRews;

    // Set value and signal it
    m_CurrentRews = actualvalue;
    emit newRews(m_CurrentRews);
}


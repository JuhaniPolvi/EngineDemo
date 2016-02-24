#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include "gear.h"
#include <QString>
#include <QList>
#include <QTimer>

//----------------------
// Constant definitions
//----------------------

// Engine timer period (ms)
const static int EngineTimerPeriod = 10;

// Minimum time required between gear switches (ms)
const static int GearSwitchPeriod = 1000;

// How fast rews go up in neutral when giving more gas (+rews/second)
const static int RewsIncreaseRate = 15000;

// How fast rews go down in neutral when letting go of gas (-rews/second)
const static int RewsDecreaseRate = 15000;

// How fast braking will decrease speed (-km/h per second at full braking)
const static float BrakingPower = 50.0f;

// How fast engine loses speed when switching down (km/h lost per second)
const static float DecelerationRate = 40.0f;

//==================================
// Class to model simple car engine
//==================================

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = 0);

protected:
    //-------------------------
    // Hidden member variables
    //-------------------------

    // List of available gears
    QList<Gear*> m_Gears;

    // Flags to determine if reverse/neutral gear is already created. Used to avoid duplicates
    bool m_ReverseCreated;
    bool m_NeutralCreated;

    // Index of currently selected gear (0-based)
    int m_ActiveGearIndex;

    // Target gear index. Used in delayed switching of gears
    int m_TargetGearIndex;

    // Time elapsed since last gear switch (ms)
    int m_TimeFromGearSwitch;

    // Speed info (current speed, target speed and maximum speed)
    float m_CurrentSpeed;
    float m_TargetSpeed;
    float m_MaxSpeed;

    // Rews info (current value, target value and maximum value)
    float m_CurrentRews;
    float m_TargetRews;
    float m_MaxRews;

    // Gas info (acceleration)
    float m_CurrentGas;
    float m_ScaledGas; // Current gas scaled to range 0.0 to 1.0 (1.0 at full gas)

    // Braking info
    float m_CurrentBraking;
    float m_ScaledBraking; // Current braking scaled to range 0.0 to 1.0 (1.0 at full braking)

    // Is the car reversing
    bool m_IsReversing;

    // Synchronization variable used in timer event (seconds in one tick)
    float m_TimerPeriod;

    // Timer used in synchronized control
    QTimer *m_Timer;

signals:
    void newRews(float value); // Number of rews has changed (new value)
    void newSpeed(float speedvalue); // Speed has changed (new value in km/h)
    void unsafeGearSwitch(); // Signal unsafe gear switch (switched gear at speed not suitable for previous gear)
    void gearSwitched(int gearindex); // Active gear has switched (selected gear index)

public slots:
    void accelerate(float relativelevel); // Accelerate at certain power (0=no acceleration, 100=maximum acceleration)
    void brake(float relativelevel); // Brake at certain power (0=no braking, 100=maximum braking)
    void switchDown(); // Switch one gear down (if possible)
    void switchUp(); // Switch one gear up (if possible)
    void switchToGear(int gearindex); // Try to switch straight to specified gear (identified by marking)

protected slots:
    void Timeout();

protected:
    void AddGear(Gear* gear); // Accessory function used to add gears to list
    void SwitchGear(); // Perform actual gear switch
    bool SafeToSwitchGear(); // Is it safe to switch at the moment

    // Function for handling state change. Called from timer event function
    void HandleStateChange();

    // Handler function for calculating speed/rews change on neutral gear.
    // Called from HandleStateChange if neutral gear is selected
    void HandleNeutralGear();

    // Handler function for calculating speed/rews change on non-neutral gear.
    // Called from HandleStateChange if non-neutral gear is selected
    void HandleNonNeutralGear();

    // Setter function for setting new speed. Contains validity checks to avoid erroneous setting.
    // Also contains some logic (such as decelerating if switched to lower gear)
    void SetSpeed(float value);

    // Setter function for setting new rews. Contains validity checks to avoid erroneous setting.
    void SetRews(float value);
};

#endif // ENGINE_H

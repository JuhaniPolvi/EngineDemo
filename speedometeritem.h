#ifndef SPEEDOMETERITEM_H
#define SPEEDOMETERITEM_H

#include <QQuickItem>

//=========================================
// UI class used to implement speedometer.
// Gets speed information from engine
//=========================================

class SpeedoMeterItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(float currentSpeed MEMBER m_CurrentSpeed)
    Q_PROPERTY(float maximumSpeed MEMBER m_MaxSpeed)
public:
    SpeedoMeterItem();

    // Version number
    static int majorversion() {return 1;}
    static int minorversion() {return 0;}

    // Setters
    void setCurrentSpeed(float value);
    void setMaximumSpeed(float value);

signals:
    // No signals, input only

public slots:
    void setSpeed(float speed);

protected:

    //==================
    // Member variables
    //==================

    float m_CurrentSpeed;
    float m_MaxSpeed;
};

#endif // SPEEDOMETERITEM_H

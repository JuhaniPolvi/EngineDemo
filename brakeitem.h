#ifndef BRAKEITEM_H
#define BRAKEITEM_H

#include <QQuickItem>

//=====================================================
// UI class used to implement brake input for engine.
// Passes braking information to engine.
//=====================================================

class BrakeItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(float brakinglevel MEMBER m_BrakingLevel)
public:
    BrakeItem();

    // Version number
    static int majorversion() {return 1;}
    static int minorversion() {return 0;}

    // Setters
    void setBrakingLevel(float value);

signals:
    // Event for passing braking level to engine. level is percentile value from 0 to 100.
    void brakingLevelChanged(float level);

public slots:
    // No slots, output only.

protected:
    float m_BrakingLevel;
};

#endif // BRAKEITEM_H

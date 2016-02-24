#ifndef ACCELERATORITEM_H
#define ACCELERATORITEM_H

#include <QQuickItem>

//===============================================
// UI class for accelerator input (gas) in QML.
// Passes gas info to engine
//===============================================

class AcceleratorItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(float acceleration MEMBER m_CurrentAcceleration)
public:
    AcceleratorItem();

    // Version number
    static int majorversion() {return 1;}
    static int minorversion() {return 0;}

signals:
    // Event used to pass acceleration level to engine. level is a percentile value from 0 to 100
    void accelerationChanged(float level);

public slots:
    // No slots, output only item

protected:
    float m_CurrentAcceleration;
};

#endif // ACCELERATORITEM_H

#ifndef REWSMETERITEM_H
#define REWSMETERITEM_H

#include <QQuickItem>
#include "engine.h"

//========================================
// UI class used to implement rews meter.
// Gets rews from engine.
//========================================

class RewsMeterItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(float currentRews MEMBER m_CurrentRews)
    Q_PROPERTY(float maximumRews MEMBER m_MaxRews)
public:
    RewsMeterItem();

    // Version number
    static int majorversion() {return 1;}
    static int minorversion() {return 0;}

    void setCurrentRews(float value);
    void setMaximumRews(float value);

signals:
    // No signals, input only

public slots:
    void setRews(float rews);

protected:

    //==================
    // Member variables
    //==================

    float m_MaxRews;
    float m_CurrentRews;

};

#endif // REWSMETERITEM_H

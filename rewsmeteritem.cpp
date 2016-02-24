#include "rewsmeteritem.h"
#include "QDebug"

//--------------
// Constructor
//--------------
RewsMeterItem::RewsMeterItem()
{
    // Initialise maximum and current value
    setMaximumRews(9000);
    setCurrentRews(0);
}

//------------------------------------------------------
// Setter: setting current rews. Includes safety checks
//------------------------------------------------------
void RewsMeterItem::setCurrentRews(float value)
{
    // Ensure valid value
    if (value < 0)
        m_CurrentRews = 0;
    else if (value > m_MaxRews)
        m_CurrentRews = m_MaxRews;
    else
        m_CurrentRews = value;

    qDebug() << "Set rews meter reading to" << m_CurrentRews;
}

//--------------------------------
// Setter: setting maximum rews.
//--------------------------------
void RewsMeterItem::setMaximumRews(float value)
{
    // At the moment limit maximum to 9000
    m_MaxRews = 9000;
}

//------------------------------
// Setter: setting current rews
//------------------------------
void RewsMeterItem::setRews(float rews)
{
    qDebug() << "Set rews meter reading to" << rews;

    setCurrentRews(rews);
}



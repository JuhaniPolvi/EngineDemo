#ifndef GEARBOXITEM_H
#define GEARBOXITEM_H

#include <QQuickItem>

//==============================================
// UI class for implementing a gearbox item.
// Passes gear selection (by click) to engine
// and actual gear switch from engine to item
//==============================================

class GearBoxItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int selectedGear MEMBER m_SelectedGear)
public:
    GearBoxItem();

    // Version number
    static int majorversion() {return 1;}
    static int minorversion() {return 0;}

    // Setters
    void setSelectedGear(int gear);

signals:
    // User has clicked on gear. Gear index (0+) is passed to client
    void gearClicked(int clickedgear);

public slots:
    void setGear(int gear);

protected:
    int m_lowestGear;
    int m_highestGear;
    int m_SelectedGear;
};

#endif // GEARBOXITEM_H

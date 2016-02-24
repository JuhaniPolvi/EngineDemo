#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QList>
#include <QDebug>
#include <QQuickItem>
#include "rewsmeteritem.h"
#include "speedometeritem.h"
#include "acceleratoritem.h"
#include "brakeitem.h"
#include "gearboxitem.h"
#include "engine.h"
#include <QQmlContext>

//===============
// Main function
//===============

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Register custom types used in UI
    qmlRegisterType<RewsMeterItem>("Dashboard", RewsMeterItem::majorversion(), RewsMeterItem::minorversion(), "RewsMeter");
    qmlRegisterType<SpeedoMeterItem>("Dashboard", SpeedoMeterItem::majorversion(), SpeedoMeterItem::minorversion(), "SpeedMeter");
    qmlRegisterType<AcceleratorItem>("Carcontrols", AcceleratorItem::majorversion(), AcceleratorItem::minorversion(), "Accelerator");
    qmlRegisterType<BrakeItem>("Carcontrols", BrakeItem::majorversion(), BrakeItem::minorversion(), "Brake");
    qmlRegisterType<GearBoxItem>("Carcontrols", GearBoxItem::majorversion(), GearBoxItem::minorversion(), "Gearbox");

    // Get root context for engine embedding and attach engine
    QQmlContext *ctx = engine.rootContext();
    ctx->setContextProperty("engine", new Engine(0));

    // Load the ui
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    int returncode;

    returncode = app.exec();

    return returncode;
}


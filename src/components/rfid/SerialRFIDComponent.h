#ifndef SERIALRFIDCOMPONENT_H
#define SERIALRFIDCOMPONENT_H

#include <QObject>
#include "src/components/rfid/BaseRFIDComponent.h"

class SerialRFIDComponent : public BaseRFIDComponent
{
    Q_OBJECT
public:
    explicit SerialRFIDComponent(QObject *parent = nullptr);

};

#endif // SERIALRFIDCOMPONENT_H

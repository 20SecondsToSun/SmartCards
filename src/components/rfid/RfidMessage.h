#ifndef RFIDMESSAGE_H
#define RFIDMESSAGE_H

#include <QObject>

class QNdefMessage;

class RfidMessage : public QObject
{
    Q_OBJECT
public:
    RfidMessage();

    enum class RfidMessageType
    {
        RawData,
        NDEFText,
        NDEFUri
    };
    Q_ENUMS(RfidMessageType)

    QByteArray prepareBytesForWrite(const QString& data, RfidMessageType type) const;
    QString parseBytes(const QByteArray& data, RfidMessageType type);

private:
    QByteArray messageData;

    QString parseNdefMessage(const QNdefMessage &message);

};

typedef RfidMessage::RfidMessageType RfidMessageType;

#endif // RFIDMESSAGE_H

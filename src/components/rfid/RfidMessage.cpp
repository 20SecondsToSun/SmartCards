#include "RfidMessage.h"

#include <QNdefNfcTextRecord>
#include <QNdefNfcUriRecord>
#include <QNdefMessage>
#include <QDebug>
#include <QUrl>

RfidMessage::RfidMessage()
{

}

QByteArray RfidMessage::prepareBytesForWrite(const QString& data, RfidMessageType type) const
{
    if(type == RfidMessageType::RawData)
    {
        return data.toUtf8();
    }
    else if(type == RfidMessageType::NDEFText)
    {
        QNdefNfcTextRecord record;
        QNdefMessage message;

        record.setEncoding(QNdefNfcTextRecord::Utf8);
        record.setLocale("en-us");
        record.setText(data);
        message.append(record);
        return message.toByteArray();
    }
    else if(type == RfidMessageType::NDEFUri)
    {
        QNdefNfcUriRecord record;
        QNdefMessage message;
        record.setUri(data);
        message.append(record);
        return message.toByteArray();
    }
}

QString RfidMessage::parseBytes(const QByteArray& data, RfidMessageType type)
{
    if(type == RfidMessageType::RawData)
    {
        return QString(data);
    }

    QNdefMessage msg =  QNdefMessage::fromByteArray(data);

    if(!msg.isEmpty())
    {
        return parseNdefMessage(msg);
    }
    else
    {
        qDebug()<<"cannot find";
        return QString();
    }
}

QString RfidMessage::parseNdefMessage(const QNdefMessage &message)
{
    qDebug()<<"ndefMessageRead ........";

    QString data;

    foreach (const QNdefRecord &record, message)
    {
        if (record.isRecordType<QNdefNfcTextRecord>())
        {
            data += QNdefNfcTextRecord(record).text();
        }
        else if (record.isRecordType<QNdefNfcUriRecord>())
        {
            QUrl url = QNdefNfcUriRecord(record).uri();
            data +=  url.toString();
        }
        else if (record.typeNameFormat() == QNdefRecord::Mime &&
                 record.type().startsWith("image/"))
        {

        }
        else if (record.isEmpty())
        {

        }
        else
        {

        }
    }

    return data;
}


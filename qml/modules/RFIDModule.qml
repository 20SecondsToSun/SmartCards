import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item
{
    ColumnLayout
    {
        x: 50;
        y: 50;
        spacing: 10;

        Text
        {
            text: "RFID component";
            font.family: "Helvetica";
            font.pixelSize: 17;
            color: "#008800";
        }

        Button
        {
            id:startRead;
            text: "Start read";
            onClicked:
            {
                smartCard.read();
            }
        }

        Button
        {
            id:startWrite;
            text: "Start write";
            onClicked:
            {
                smartCard.write("user data user- user data 000000000000000");
            }
        }
    }
}

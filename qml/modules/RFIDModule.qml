import QtQuick 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

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

        Item
        {
            implicitWidth: 400;
            implicitHeight: 20;
        }

        TextArea
        {
            implicitWidth: 400;
            implicitHeight: 60;
            id:inputText;
            width: 310;
            height: 200;
            wrapMode: TextEdit.Wrap;
            selectByMouse: true;
            font.family: "Helvetica";
            font.pixelSize: 14;
            text: "some data"

            background: Rectangle
            {
                implicitWidth: 400
                implicitHeight: 60
                border.color: "#21be2b";
            }
        }

        Button
        {
            id:startWrite;
            text: "Start write";
            enabled: !smartCard.isBusy;
            onClicked:
            {
                smartCard.write(inputText.text);
            }
        }

        Item
        {
            implicitWidth: 400;
            implicitHeight: 20;
        }

        TextArea
        {
            implicitWidth: 400;
            implicitHeight: 60;
            id: outputText;
            width: 310;
            height: 200;
            wrapMode: TextEdit.Wrap;
            selectByMouse: true;
            font.family: "Helvetica";
            font.pixelSize: 14;

            background: Rectangle
            {
                implicitWidth: 400
                implicitHeight: 60
                border.color: "#21be2b";
            }
        }

        Button
        {
            id:startRead;
            text: "Start read";
            enabled: !smartCard.isBusy;
            onClicked:
            {
                smartCard.read();
            }
        }
    }

    Connections
    {
        target: smartCard;
        onDataReaded:
        {
            outputText.text = data;
        }
    }
}

#include "FileReader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>

FileReader::FileReader(QObject *parent) : QObject(parent)
{

}

void FileReader::setQmlContext(QQmlContext* qmlContext)
{
    qmlContext->setContextProperty("fileReader", this);
}

QString FileReader::read(const QString& filePath)
{
    if (filePath.isEmpty())
    {
        emit readError("File path is empty");
        return QString();
    }

    QString fullPath  = qApp->applicationDirPath() + "/" + filePath;
    QFile file(fullPath);
    QString fileContent;

    if (file.open(QIODevice::ReadOnly))
    {
        QString line;
        QTextStream t(&file);
        do
        {
            line = t.readLine();
            fileContent += line;
        }
        while (!line.isNull());

        file.close();
    }
    else
    {
        emit readError("Unable to open the file");
        return QString();
    }

    return fileContent;
}

bool FileReader::write(const QString& filePath, const QString& data)
{
    if (filePath.isEmpty())
    {
        return false;
    }

    QFile file(filePath);

    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        return false;
    }

    QTextStream out(&file);
    out << data;
    file.close();

    return true;
}

#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QQmlContext>

class FileReader : public QObject
{
    Q_OBJECT

public:
    explicit FileReader(QObject *parent = nullptr);

    Q_INVOKABLE QString read(const QString& filePath);
    Q_INVOKABLE bool write(const QString& filePath, const QString& data);

    void setQmlContext(QQmlContext* value);

signals:
    void readError(const QString& error);

};

#endif // FILEREADER_H

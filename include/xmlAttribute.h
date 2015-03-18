#pragma once
#ifndef __XMLITEMATTRIBUTE__
#define __XMLITEMATTRIBUTE__

#include <QObject>
#include <QString>
#include <QList>
#include <QString>

//namespace Decimal {
//namespace DICOM_Parser {

class xmlItemAttribute //: QObject
{
//    Q_OBJECT


public:

//    xmlItemAttribute() {}

//    xmlItem(const QString &name, const QString &value)
//        :myTagName(name), myValue(value) {}


    QString tag() const;

    QString value() const;

    void setAttribute(const QString &name, const QString &value);



private:

    QString myName;
    QString myValue;

};
//}
//}


#endif // __XMLITEMATTRIBUTE__

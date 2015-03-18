#pragma once
#ifndef __XMLITEM__
#define __XMLITEM__

#include <QObject>
#include <QString>
#include <QList>
#include <QString>

#include "xmlAttribute.h"


//namespace Decimal {
//namespace DICOM_Parser {

//class xmlItemAttribute;

class xmlItem //: public QObject
{
//    Q_OBJECT

public:


//    xmlItem() {}

//    xmlItem(const QString &name, const QString &value)
//        :myTagName(name), myValue(value) {}


    void addAttribute(const QString &name, const QString &value);

    inline void addAttribute(const xmlItemAttribute &ia)
    {
        myAttributes.push_back(ia);
    }

    inline void addItem(const xmlItem &item)
    {
        myItems.push_back(item);
    }

    inline xmlItemAttribute attribute(int index) const
    {
        return myAttributes.at(index);
    }

    inline int attributeCount() const
    {
        return myAttributes.count();
    }

    inline int itemCount() const
    {
        return myItems.count();
    }

    inline QList<xmlItemAttribute> items() const
    {
        return myAttributes;
    }

    QString tag() const;

    QString value() const;

    xmlItem item(int index) const;

    QList<xmlItem> items();

    void setTag(const QString &name, const QString &value);

    inline void setValue(const QString &value)
    {
        myValue = value;
    }



private:

    QString myTagName;
    QString myValue;
    QList<xmlItemAttribute> myAttributes;
    QList<xmlItem> myItems;

};
//}
//}


#endif // __XMLITEM__

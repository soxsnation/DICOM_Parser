#include "xmlItem.h"
#include "xmlAttribute.h"

//using namespace Decimal::DICOM_Parser;

xmlItem xmlItem::item(int index) const
{
    if (myItems.count() > index) {
        return myItems.at(index);
    }
    else
    {
        return xmlItem();
    }
}

QList<xmlItem> xmlItem::items()
{
    return myItems;
}

void xmlItem::addAttribute(const QString &name, const QString &value)
{
//    xmlAttribute ia;
//    ia.setAttribute(name,value);
//        myAttributes.push_back(ia);
}

void xmlItem::setTag(const QString &name, const QString &value)
{
    myTagName = name;
    myValue = value;
}

QString xmlItem::tag() const
{
    return myTagName;
}

QString xmlItem::value() const
{
    return myValue;
}

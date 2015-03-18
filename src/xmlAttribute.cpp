#include "xmlAttribute.h"

//using namespace Decimal::DICOM_Parser;


void xmlItemAttribute::setAttribute(const QString &name, const QString &value)
{
    myName = name;
    myValue = value;
}

QString xmlItemAttribute::tag() const
{
    return myName;
}

QString xmlItemAttribute::value() const
{
    return myValue;
}

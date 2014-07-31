/*
 This file is part of vedgTools/QtXmlUtilities.
 Copyright (C) 2014 Igor Kushnir <igorkuo AT Google mail>

 vedgTools/QtXmlUtilities is free software: you can redistribute it and/or
 modify it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 vedgTools/QtXmlUtilities is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 vedgTools/QtXmlUtilities.  If not, see <http://www.gnu.org/licenses/>.
*/

# ifndef QT_XML_UTILITIES_READING_SHORTCUTS_INL_HPP
# define QT_XML_UTILITIES_READING_SHORTCUTS_INL_HPP

# include <QtXmlUtilities/ReadingShortcuts.hpp>

# include <QtCoreUtilities/Error.hpp>
# include <QtCoreUtilities/String.hpp>
# include <QtCoreUtilities/Validation.hpp>

# include <TemplateUtilities/Reserve.hpp>

# include <QString>
# include <QObject>
# include <QDomElement>

# include <utility>
# include <functional>


namespace QtUtilities
{
namespace XmlReading
{
template <typename T>
bool copyUniqueChildsTextTo(const QDomElement & e, const QString & tagName,
                            T & destination)
{
    QString text;
    if (copyUniqueChildsTextTo(e, tagName, text)) {
        try {
            destination = ConvertQString::to<T>(text);
        }
        catch (const StringError & error) {
            throw ReadError(QObject::tr("parsing %1 failed - ").arg(tagName) +
                            QString::fromUtf8(error.what()));
        }
        return true;
    }
    return false;
}


template <class QDomElementCollection>
QDomElementCollection getChildren(const QDomElement & e,
                                  const QString & tagName)
{
    QDomElementCollection children;
    for (QDomElement child = e.firstChildElement(tagName); ! child.isNull();
            child = children.back().nextSiblingElement(tagName)) {
        children.push_back(std::move(child));
    }
    return children;
}

template <class TCollection, typename ElementToT>
TCollection getChildren(const QDomElement & e, const QString & tagName,
                        ElementToT childToResultValue)
{
    auto children = getChildren(e, tagName);
    TCollection converted;
    TemplateUtilities::reserve(converted, children.size());
    for (auto & child : children)
        converted.push_back(childToResultValue(std::move(child)));
    return converted;
}


template <typename T, typename Validator>
bool copyUniqueChildsTextTo(const QDomElement & e, const QString & tagName,
                            T & destination, Validator validator)
{
    if (copyUniqueChildsTextTo(e, tagName, destination)) {
        try {
            validator(destination);
        }
        catch (const Error & error) {
            throw ReadError(QObject::tr("validating %1 failed - ").arg(tagName)
                            + QString::fromUtf8(error.what()));
        }
        return true;
    }
    return false;
}

template <typename T>
bool copyUniqueChildsTextToMin(const QDomElement & e, const QString & tagName,
                               T & destination, const T & minValue)
{
    return copyUniqueChildsTextTo(e, tagName, destination,
                                  std::bind(checkMinValue<T>,
                                            std::placeholders::_1,
                                            std::cref(minValue)));
}

template <typename T>
bool copyUniqueChildsTextToMax(const QDomElement & e, const QString & tagName,
                               T & destination, const T & maxValue)
{
    return copyUniqueChildsTextTo(e, tagName, destination,
                                  std::bind(checkMaxValue<T>,
                                            std::placeholders::_1,
                                            std::cref(maxValue)));
}

template <typename T>
bool copyUniqueChildsTextToRange(
    const QDomElement & e, const QString & tagName,
    T & destination, const T & minValue, const T & maxValue)
{
    return copyUniqueChildsTextTo(
               e, tagName, destination,
               std::bind(checkRange<T>, std::placeholders::_1,
                         std::cref(minValue), std::cref(maxValue)));
}

template <typename T>
bool copyUniqueChildsTextToRange0Allowed(
    const QDomElement & e, const QString & tagName,
    T & destination, const T & minValue, const T & maxValue)
{
    return copyUniqueChildsTextTo(
               e, tagName, destination,
               std::bind(checkRange0Allowed<T>, std::placeholders::_1,
                         std::cref(minValue), std::cref(maxValue)));
}

}

}

# endif // QT_XML_UTILITIES_READING_SHORTCUTS_INL_HPP

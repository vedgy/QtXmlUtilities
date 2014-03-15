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

# ifndef QT_XML_UTILITIES_SHORTCUTS_INL_HPP
# define QT_XML_UTILITIES_SHORTCUTS_INL_HPP

# include <QtXmlUtilities/Shortcuts.hpp>

# include <QtCoreUtilities/String.hpp>

# include <TemplateUtilities/Reserve.hpp>

# include <QString>
# include <QDomElement>

# include <utility>


namespace QtUtilities
{
namespace Xml
{
template <typename T>
bool copyUniqueChildsTextTo(const QDomElement & e, const QString & tagName,
                            T & destination)
{
    QString text;
    if (copyUniqueChildsTextTo(e, tagName, text)) {
        destination = ConvertQString::to<T>(text);
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
            child = child.nextSiblingElement(tagName)) {
        children.push_back(child);
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

}

}

# endif // QT_XML_UTILITIES_SHORTCUTS_INL_HPP

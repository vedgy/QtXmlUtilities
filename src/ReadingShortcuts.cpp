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

# include "ReadingShortcuts.hpp"

# include <QtCoreUtilities/String.hpp>

# include <QString>
# include <QStringList>
# include <QObject>
# include <QFile>
# include <QDomElement>
# include <QDomDocument>


namespace QtUtilities
{
namespace XmlReading
{
void assertTagName(const QDomElement & e, const QString & tagName)
{
    if (e.tagName() != tagName) {
        throw ReadError(
            QObject::tr(
                "tag name assertion failed. \"%1\" expected "
                "but \"%2\" found.").arg(tagName, e.tagName()));
    }
}

QDomElement loadRoot(const QString & filename)
{
    QDomDocument doc;
    {
        QFile file(filename);
        QString errorMsg;
        int line, column;
        if (! doc.setContent(& file, & errorMsg, & line, & column)) {
            throw ReadError(
                QObject::tr("could not load XML document from file %1."
                            " On line %2 at column %3: %4.").arg(filename).arg(
                    line).arg(column).arg(errorMsg));
        }
    }
    return doc.documentElement();
}

QDomElement loadRoot(const QString & filename, const QString & tagName)
{
    QDomElement root = loadRoot(filename);
    if (! root.isNull())
        assertTagName(root, tagName);
    return root;
}


QDomElement getUniqueChild(const QDomElement & e, const QString & tagName)
{
    QDomElement child = e.firstChildElement(tagName);
    if (! child.nextSiblingElement(tagName).isNull())
        throw ReadError(QObject::tr("element %1 is not unique.").arg(tagName));
    return child;
}

bool copyUniqueChildsTextTo(const QDomElement & e, const QString & tagName,
                            QString & destination)
{
    const QDomElement child = getUniqueChild(e, tagName);
    if (child.isNull())
        return false;
    destination = child.text();
    return true;
}

bool copyUniqueChildsTextToByteArray(
    const QDomElement & e, const QString & tagName, QByteArray & destination)
{
    QString text;
    if (copyUniqueChildsTextTo(e, tagName, text)) {
        destination = qStringToByteArray(text);
        return true;
    }
    return false;
}

bool copyQStringListTo(const QDomElement & e, const QString & listTagName,
                       const QString & stringTagName, QStringList & destination)
{
    const QDomElement listElement = getUniqueChild(e, listTagName);
    if (listElement.isNull())
        return false;
    destination = getChildren<QStringList>(listElement, stringTagName,
    [](const QDomElement & de) {
        return de.text();
    });
    return true;
}

}

}

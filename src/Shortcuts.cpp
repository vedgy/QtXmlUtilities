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

# include "Shortcuts.hpp"

# include <QString>
# include <QObject>
# include <QDir>
# include <QFile>
# include <QFileInfo>
# include <QDomElement>
# include <QDomDocument>


namespace QtUtilities
{
namespace Xml
{
void assertTagName(const QDomElement & e, const QString & tagName)
{
    if (e.tagName() != tagName) {
        throw XmlError(
            QObject::tr(
                "tag name assertion failed. \"%1\" expected "
                "but \"%2\" found.").arg(tagName, e.tagName()));
    }
}


QDomDocument createDocument()
{
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction(
                        "xml", "version=\"1.0\" encoding=\"UTF-8\""));
    return doc;
}

QDomElement createRoot(QDomDocument & doc, const QString & tagName)
{
    const QDomElement root = doc.createElement(tagName);
    doc.appendChild(root);
    return root;
}

QDomElement createElement(QDomDocument & doc, const QString & tagName,
                          const QString & text)
{
    QDomElement e = doc.createElement(tagName);
    e.appendChild(doc.createTextNode(text));
    return e;
}


QDomElement getUniqueChild(const QDomElement & e, const QString & tagName)
{
    const QDomElement child = e.firstChildElement(tagName);
    if (! child.nextSiblingElement(tagName).isNull()) {
        throw XmlError(QObject::tr("element with name %1 is not unique.").arg(
                           tagName));
    }
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


QDomElement loadRoot(const QString & filename)
{
    QDomDocument doc;
    {
        QFile file(filename);
        QString errorMsg;
        int line, column;
        if (! doc.setContent(& file, & errorMsg, & line, & column)) {
            throw XmlError(
                QObject::tr("could not load XML document from file %1."
                            " On line %2 at column %3: %4.").arg(filename).arg(
                    line).arg(column).arg(errorMsg));
        }
    }
    return doc.documentElement();
}

QDomElement loadRoot(const QString & filename, const QString & tagName)
{
    const QDomElement root = loadRoot(filename);
    if (! root.isNull())
        assertTagName(root, tagName);
    return root;
}

void save(const QDomDocument & doc, const QString & filename, const int indent)
{
    {
        QDir dir;
        dir.mkpath(QFileInfo(filename).absolutePath());
    }
    QFile file(filename);

    if (! file.open(QIODevice::WriteOnly)) {
        throw XmlError(
            QObject::tr("could not open file %1 for writing.").arg(filename));
    }
    if (file.write(doc.toByteArray(indent)) == -1) {
        throw XmlError(
            QObject::tr(
                "error occurred while writing to file %1.").arg(filename));
    }
}

}

}

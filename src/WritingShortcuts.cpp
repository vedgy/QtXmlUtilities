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

# include "WritingShortcuts.hpp"

# include <QString>
# include <QStringList>
# include <QObject>
# include <QDir>
# include <QFile>
# include <QFileInfo>
# include <QDomElement>
# include <QDomDocument>


namespace QtUtilities
{
namespace XmlWriting
{
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

QDomElement createQStringListElement(
    QDomDocument & doc, const QString & listTagName,
    const QString & stringTagName, const QStringList & list)
{
    QDomElement listElement = doc.createElement(listTagName);
    for (const QString & str : list)
        listElement.appendChild(createElement(doc, stringTagName, str));
    return listElement;
}


void save(const QDomDocument & doc, const QString & filename, const int indent)
{
    {
        QDir dir;
        dir.mkpath(QFileInfo(filename).absolutePath());
    }
    QFile file(filename);

    if (! file.open(QIODevice::WriteOnly)) {
        throw WriteError(
            QObject::tr("could not open file %1 for writing.").arg(filename));
    }
    if (file.write(doc.toByteArray(indent)) == -1) {
        throw WriteError(
            QObject::tr(
                "error occurred while writing to file %1.").arg(filename));
    }
}

}

}

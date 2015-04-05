/*
 This file is part of vedgTools/QtXmlUtilities.
 Copyright (C) 2014, 2015 Igor Kushnir <igorkuo AT Google mail>

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

# ifndef QT_XML_UTILITIES_WRITING_SHORTCUTS_HPP
# define QT_XML_UTILITIES_WRITING_SHORTCUTS_HPP

# include <QtCoreUtilities/Error.hpp>
# include <QtCoreUtilities/String.hpp>

# include <CommonUtilities/CopyAndMoveSemantics.hpp>

# include <QtGlobal>
# include <QDomNode>
# include <QDomElement>
# include <QDomDocument>


QT_FORWARD_DECLARE_CLASS(QByteArray)
QT_FORWARD_DECLARE_CLASS(QString)
QT_FORWARD_DECLARE_CLASS(QStringList)

namespace QtUtilities
{
/// @brief Provides shortcuts for constructing QDomDocument.
namespace XmlWriting
{
class WriteError : public Error
{
public:
    explicit WriteError(const QString & sWhat) : Error(sWhat) {}
    COPYABLE_AND_MOVABLE(WriteError)
    ~WriteError() noexcept override;
};

/// @return Document with UTF-8 processing instruction.
QDomDocument createDocument();
/// @brief Creates root element for doc with name=tagName and returns it.
QDomElement createRoot(QDomDocument & doc, const QString & tagName);

/// @brief Creates element (name=tagName, text=text) and returns it.
QDomElement createElement(QDomDocument & doc, const QString & tagName,
                          const QString & text);
template <typename T>
inline QDomElement createElement(QDomDocument & doc, const QString & tagName,
                                 const T & value)
{
    return createElement(doc, tagName, toQString(value));
}
inline QDomElement createElementFromByteArray(
    QDomDocument & doc, const QString & tagName, const QByteArray & byteArray)
{
    return createElement(doc, tagName, byteArrayToQString(byteArray));
}

/// @brief Sets the specified attribute's text to attributeText.
void setAttribute(QDomElement & element, const QString & attributeName,
                  const QString & attributeText);
template <typename T>
inline void setAttribute(QDomElement & element, const QString & attributeName,
                         const T & attributeValue)
{
    setAttribute(element, attributeName, toQString(attributeValue));
}

/// @brief Creates element L with name=listTagName. For each string "str" in
/// list creates element (name=stringTagName, text=str) and appends it to L.
/// @return L.
QDomElement createStringListElement(
    QDomDocument & doc, const QString & listTagName,
    const QString & stringTagName, const QStringList & list);


/// @brief Writes doc to file, specified by filename.
/// @param indent Amount of space to indent subelements.
/// @throw WriteError In case of saving error.
void save(const QDomDocument & doc, const QString & filename, int indent = 4);



struct Element {
    Element appendElement(const QString & tagName) {
        Element child { domDocument, domDocument.createElement(tagName) };
        domElement.appendChild(child.domElement);
        return child;
    }

    void appendChildNode(const QDomNode & node) {
        domElement.appendChild(node);
    }

    template <typename T>
    void appendChild(const QString & tagName, const T & value) {
        appendChildNode(createElement(domDocument, tagName, value));
    }

    void appendChildByteArray(const QString & tagName,
                              const QByteArray & byteArray) {
        appendChildNode(createElementFromByteArray(
                            domDocument, tagName, byteArray));
    }

    template <typename ValueType, typename AttributeType>
    void appendChildWithAttribute(const QString & tagName,
                                  const ValueType & value,
                                  const QString & attributeName,
                                  const AttributeType & attributeValue) {
        QDomElement element = createElement(domDocument, tagName, value);
        setAttribute(element, attributeName, attributeValue);
        appendChildNode(element);
    }

    void appendChildStringList(const QString & listTagName,
                               const QString & stringTagName,
                               const QStringList & list) {
        appendChildNode(createStringListElement(
                            domDocument, listTagName, stringTagName, list));
    }

    QDomDocument & domDocument;
    QDomElement domElement;
};

struct Document {
    explicit Document(const QString & rootTagName)
        : domDocument(createDocument()),
          root { domDocument, createRoot(domDocument, rootTagName) } {
    }

    void save(const QString & filename, int indent = 4) {
        XmlWriting::save(domDocument, filename, indent);
    }

    QDomDocument domDocument;
    Element root;
};

} // END namespace XmlWriting
} // END namespace QtUtilities

# endif // QT_XML_UTILITIES_WRITING_SHORTCUTS_HPP

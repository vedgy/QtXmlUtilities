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

# ifndef QT_XML_UTILITIES_SHORTCUTS_HPP
# define QT_XML_UTILITIES_SHORTCUTS_HPP

# include <QtCoreUtilities/Error.hpp>
# include <QtCoreUtilities/String.hpp>

# include <QtGlobal>
# include <QDomElement>

# include <vector>


QT_FORWARD_DECLARE_CLASS(QByteArray)
QT_FORWARD_DECLARE_CLASS(QString)
QT_FORWARD_DECLARE_CLASS(QDomDocument)

namespace QtUtilities
{
/// @brief Provides shortcuts for parsing and constructing QDomDocument.
/// @throw Error's descendant in case of error.
namespace Xml
{
class XmlError : public Error
{
public:
    explicit XmlError(const QString & sWhat) : Error(sWhat) {}
};


/// @throw XmlError If e's name does not match tagName.
void assertTagName(const QDomElement & e, const QString & tagName);


/// @brief Returns document with UTF-8 processing instruction.
QDomDocument createDocument();
/// @brief Creates root element for doc with name=tagName and returns it.
QDomElement createRoot(QDomDocument & doc, const QString & tagName);
/// @brief Creates element with name=tagName and returns it.
/// @param text Specifies text to be added to element.
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

/// @return Child of e with name=tagName.
/// If such a child doesn't exist, returns null element.
/// @throw XmlError If e has more than one child with name=tagName.
QDomElement getUniqueChild(const QDomElement & e, const QString & tagName);

/// @brief Calls getUniqueChild(e, tagName). If result is not a null element,
/// copies its text to destination. Otherwise, destination is not changed.
/// @return true if text was copied to destination.
bool copyUniqueChildsTextTo(const QDomElement & e, const QString & tagName,
                            QString & destination);
/// @brief Calls copyUniqueChildsTextTo(e, tagName, ...).
/// If text was received, converts text to type T and stores in destination.
/// Otherwise, destination is not changed.
/// @tparam T There must be a ConvertQString::to<T> specialization.
/// @return true if converted text was copied to destination.
template <typename T>
bool copyUniqueChildsTextTo(const QDomElement & e, const QString & tagName,
                            T & destination);
/// @brief Identical to template copyUniqueChildsTextTo<T>, but uses
/// qStringtoByteArray itstead of ConvertQString::to<T>.
bool copyUniqueChildsTextToByteArray(
    const QDomElement & e, const QString & tagName, QByteArray & destination);

/// @tparam QDomElementCollection push_back([const] QDomElement [&[&]] [or
/// implicitly convertible from]) method is required.
/// @return All e's children with name=tagName in the order of appearance in e.
template <class QDomElementCollection = std::vector<QDomElement>>
QDomElementCollection getChildren(const QDomElement & e,
                                  const QString & tagName);
/// @brief Calls getChildren(e, tagName), converts children to type T using
/// function childToResultValue and returns result of the conversion.
/// @tparam TCollection Must have push_back([const] T [&[&]]  [or implicitly
/// convertible from]) method; iterators.
/// @tparam ElementToT Must be a callable object that takes a single parameter
/// of type ([const] QDomElement [&[&]] [or implicitly convertible from]) and
/// returns T [or value of a type that is implicitly convertible to T].
template <class TCollection, typename ElementToT>
TCollection getChildren(const QDomElement & e, const QString & tagName,
                        ElementToT childToResultValue);


/// @brief Loads document from file, specified by filename, and returns its
/// root element.
QDomElement loadRoot(const QString & filename);
/// @brief This is an overloaded function. After extracting root element this
/// function calls assertTagName(root, tagName) if (! root.isNull()).
QDomElement loadRoot(const QString & filename, const QString & tagName);

/// @brief Writes doc to file, specified by filename.
/// @param indent Amount of space to indent subelements.
void save(const QDomDocument & doc, const QString & filename, int indent = 4);

}

}

# include "../../src/Shortcuts-inl.hpp"

# endif // QT_XML_UTILITIES_SHORTCUTS_HPP

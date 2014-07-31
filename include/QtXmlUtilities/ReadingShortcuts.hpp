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

# ifndef QT_XML_UTILITIES_READING_SHORTCUTS_HPP
# define QT_XML_UTILITIES_READING_SHORTCUTS_HPP

# include <QtCoreUtilities/Error.hpp>

# include <QtGlobal>
# include <QDomElement>

# include <vector>


QT_FORWARD_DECLARE_CLASS(QByteArray)
QT_FORWARD_DECLARE_CLASS(QString)
QT_FORWARD_DECLARE_CLASS(QStringList)

namespace QtUtilities
{
/// @brief Provides shortcuts for parsing QDomDocument.
/// @throw XmlReading::ReadError In case of parsing error.
namespace XmlReading
{
class ReadError : public Error
{
public:
    explicit ReadError(const QString & sWhat) : Error(sWhat) {}
};

/// @throw ReadError If e's name does not match tagName.
void assertTagName(const QDomElement & e, const QString & tagName);

/// @brief Loads document from file specified by filename and returns its
/// root element.
QDomElement loadRoot(const QString & filename);
/// @brief This is an overloaded function. After extracting root element this
/// function calls assertTagName(root, tagName) if (! root.isNull()).
QDomElement loadRoot(const QString & filename, const QString & tagName);


/// @return Child of e with name=tagName.
/// If such a child does not exist, returns null element.
/// @throw ReadError If e has more than one child with name=tagName.
QDomElement getUniqueChild(const QDomElement & e, const QString & tagName);

/// @brief Calls getUniqueChild(e, tagName). If result is not a null element,
/// copies its text to destination; otherwise destination is not changed.
/// @return true if text was copied to destination.
bool copyUniqueChildsTextTo(const QDomElement & e, const QString & tagName,
                            QString & destination);
/// @brief Calls copyUniqueChildsTextTo(e, tagName, <temporary_string>).
/// If text was received, converts text to type T and stores in destination;
/// otherwise destination is not changed.
/// @tparam T There must be a ConvertQString::to<T> specialization.
/// @return true if converted text was copied to destination.
template <typename T>
bool copyUniqueChildsTextTo(const QDomElement & e, const QString & tagName,
                            T & destination);
/// @brief Identical to template copyUniqueChildsTextTo<T> but uses
/// qStringtoByteArray itstead of ConvertQString::to<T>.
bool copyUniqueChildsTextToByteArray(
    const QDomElement & e, const QString & tagName, QByteArray & destination);

/// @tparam QDomElementCollection back() const and
/// push_back([const] QDomElement [&[&]] [or implicitly convertible from])
/// methods are required.
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


/// @brief Calls copyUniqueChildsTextTo(e, tagName, destination).
/// If destination was set, calls validator(destination) and returns true;
/// otherwise returns false.
/// @throw ReadError If calling validator throws Error.
template <typename T, typename Validator>
bool copyUniqueChildsTextTo(const QDomElement & e, const QString & tagName,
                            T & destination, Validator validator);

/// @brief Calls copyUniqueChildsTextTo with validator=checkMinValue.
template <typename T>
bool copyUniqueChildsTextToMin(const QDomElement & e, const QString & tagName,
                               T & destination, const T & minValue);
/// @brief Calls copyUniqueChildsTextTo with validator=checkMaxValue.
template <typename T>
bool copyUniqueChildsTextToMax(const QDomElement & e, const QString & tagName,
                               T & destination, const T & maxValue);
/// @brief Calls copyUniqueChildsTextTo with validator=checkRange.
template <typename T>
bool copyUniqueChildsTextToRange(
    const QDomElement & e, const QString & tagName,
    T & destination, const T & minValue, const T & maxValue);
/// @brief Calls copyUniqueChildsTextTo with validator=checkRange0Allowed.
template <typename T>
bool copyUniqueChildsTextToRange0Allowed(
    const QDomElement & e, const QString & tagName,
    T & destination, const T & minValue, const T & maxValue);


/// @brief Gets unique child of e with name=listTagName.
/// Let us name this child "L". If there is no such child, returns false.
/// Otherwise, gets all children of L, assigns list of these children's texts
/// to destination and returns true.
/// @throw ReadError If e has more than one child with name=listTagName.
bool copyQStringListTo(
    const QDomElement & e, const QString & listTagName,
    const QString & stringTagName, QStringList & destination);

}

}

# include "../../src/ReadingShortcuts-inl.hpp"

# endif // QT_XML_UTILITIES_READING_SHORTCUTS_HPP

/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/qassociativeiterable.h>
#include <QtCore/qvariant.h>

#include <QtCore/private/qiterable_p.h>

QT_BEGIN_NAMESPACE

/*!
    Returns the key this iterator points to.
*/
QVariant QAssociativeIterator::key() const
{
    return QIterablePrivate::retrieveElement(
                metaContainer().keyMetaType(), [this](void *dataPtr) {
        metaContainer().keyAtIterator(constIterator(), dataPtr);
    });
}

/*!
    Returns the mapped value this iterator points to. If the container does not
    provide a mapped value (for example a set), returns an invalid QVariantRef.
*/
QVariantRef<QAssociativeIterator> QAssociativeIterator::value() const
{
    const QMetaType mappedMetaType(metaContainer().mappedMetaType());
    return QVariantRef<QAssociativeIterator>(mappedMetaType.isValid() ? this : nullptr);
}

/*!
    Returns the current item, converted to a QVariantRef. The resulting
    QVariantRef resolves to the mapped value if there is one, or to the key
    value if not.
*/
QVariantRef<QAssociativeIterator> QAssociativeIterator::operator*() const
{
    return QVariantRef<QAssociativeIterator>(this);
}

/*!
    Returns the current item, converted to a QVariantPointer. The resulting
    QVariantPointer resolves to the mapped value if there is one, or to the key
    value if not.
*/
QVariantPointer<QAssociativeIterator> QAssociativeIterator::operator->() const
{
    return QVariantPointer<QAssociativeIterator>(this);
}

/*!
    Returns the key this iterator points to.
*/
QVariant QAssociativeConstIterator::key() const
{
    return QIterablePrivate::retrieveElement(
                metaContainer().keyMetaType(), [this](void *dataPtr) {
        metaContainer().keyAtConstIterator(constIterator(), dataPtr);
    });
}

/*!
    Returns the mapped value this iterator points to, or an invalid QVariant if
    there is no mapped value.
*/
QVariant QAssociativeConstIterator::value() const
{
    return QIterablePrivate::retrieveElement(
                metaContainer().mappedMetaType(), [this](void *dataPtr) {
        metaContainer().mappedAtConstIterator(constIterator(), dataPtr);
    });
}

/*!
    Returns the current item, converted to a QVariant. The returned value is the
    mapped value at the current iterator if there is one, or otherwise the key.
*/
QVariant QAssociativeConstIterator::operator*() const
{
    const QMetaType mappedMetaType(metaContainer().mappedMetaType());
    return mappedMetaType.isValid() ? value() : key();
}

/*!
    Returns the current item, converted to a QVariantConstPointer. The
    QVariantConstPointer will resolve to the mapped value at the current
    iterator if there is one, or otherwise the key.
*/
QVariantConstPointer QAssociativeConstIterator::operator->() const
{
    return QVariantConstPointer(operator*());
}

/*!
    \class QAssociativeIterable
    \since 5.2
    \inmodule QtCore
    \brief The QAssociativeIterable class is an iterable interface for an associative container in a QVariant.

    This class allows several methods of accessing the elements of an associative container held within
    a QVariant. An instance of QAssociativeIterable can be extracted from a QVariant if it can
    be converted to a QVariantHash or QVariantMap or if a custom mutable view has been registered.

    \snippet code/src_corelib_kernel_qvariant.cpp 10

    The container itself is not copied before iterating over it.

    \sa QVariant
*/

/*!
    \typedef QAssociativeIterable::RandomAccessIterator
    Exposes an iterator using std::random_access_iterator_tag.
*/

/*!
    \typedef QAssociativeIterable::BidirectionalIterator
    Exposes an iterator using std::bidirectional_iterator_tag.
*/

/*!
    \typedef QAssociativeIterable::ForwardIterator
    Exposes an iterator using std::forward_iterator_tag.
*/

/*!
    \typedef QAssociativeIterable::InputIterator
    Exposes an iterator using std::input_iterator_tag.
*/

/*!
    \typedef QAssociativeIterable::RandomAccessConstIterator
    Exposes a const_iterator using std::random_access_iterator_tag.
*/

/*!
    \typedef QAssociativeIterable::BidirectionalConstIterator
    Exposes a const_iterator using std::bidirectional_iterator_tag.
*/

/*!
    \typedef QAssociativeIterable::ForwardConstIterator
    Exposes a const_iterator using std::forward_iterator_tag.
*/

/*!
    \typedef QAssociativeIterable::InputConstIterator
    Exposes a const_iterator using std::input_iterator_tag.
*/

/*!
    Retrieves a const_iterator pointing to the element at the given \a key, or
    the end of the container if that key does not exist.
 */
QAssociativeIterable::const_iterator QAssociativeIterable::find(const QVariant &key) const
{
    const QMetaAssociation meta = metaContainer();
    QVariant converted = key;
    const void *keyData = QIterablePrivate::coerceType(converted, meta.keyMetaType());
    return const_iterator(QConstIterator(
                this, meta.createConstIteratorAtKey(constIterable(), keyData)));
}

/*!
    Retrieves an iterator pointing to the element at the given \a key, or
    the end of the container if that key does not exist.
 */
QAssociativeIterable::iterator QAssociativeIterable::mutableFind(const QVariant &key)
{
    const QMetaAssociation meta = metaContainer();
    QVariant converted = key;
    const void *keyData = QIterablePrivate::coerceType(converted, meta.keyMetaType());
    return iterator(QIterator(this, meta.createIteratorAtKey(mutableIterable(), keyData)));
}

/*!
    Retrieves the mapped value at the given \a key, or an invalid QVariant
    if the key does not exist.
 */
QVariant QAssociativeIterable::value(const QVariant &key) const
{
    const QMetaAssociation meta = metaContainer();
    QVariant converted = key;
    const void *keyData = QIterablePrivate::coerceType(converted, meta.keyMetaType());
    QVariant result(QMetaType(meta.mappedMetaType()));
    meta.mappedAtKey(constIterable(), keyData, result.data());
    return result;
}

/*!
    \class QAssociativeIterable::const_iterator
    \since 5.2
    \inmodule QtCore
    \brief The QAssociativeIterable::const_iterator allows iteration over a container in a QVariant.

    A QAssociativeIterable::const_iterator can only be created by a QAssociativeIterable instance,
    and can be used in a way similar to other stl-style iterators.

    \snippet code/src_corelib_kernel_qvariant.cpp 10

    \sa QAssociativeIterable
*/

/*!
    \class QAssociativeIterable::iterator
    \since 6.0
    \inmodule QtCore
    \brief The QAssociativeIterable::iterator allows iteration over a container in a QVariant.

    A QAssociativeIterable::iterator can only be created by a QAssociativeIterable instance,
    and can be used in a way similar to other stl-style iterators.

    \sa QAssociativeIterable
*/

QT_END_NAMESPACE

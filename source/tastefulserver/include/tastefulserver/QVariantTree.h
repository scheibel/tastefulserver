/**
 * (C) LGPL-3
 *
 * Tasteful Server <https://github.com/scheibel/tasteful-server>
 *
 * Copyright: 2012 Lux, Scheibel
 * Authors:
 *     Roland Lux <rollux2000@googlemail.com>
 *     Willy Scheibel <willyscheibel@gmx.de>
 *
 * This file is part of Tasteful Server.
 *
 * Tasteful Server is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tasteful Server is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Tasteful Server.  If not, see <http://www.gnu.org/licenses/>.
 **/

#pragma once

#include <QString>
#include <QHash>
#include <QVariant>

#include <tastefulserver/tastefulserver_api.h>

namespace tastefulserver {

class QVariantTree;
class QVariantLeaf;
class QVariantNullTree;

class TASTEFULSERVER_API QVariantAbstractTree
{
public:
    virtual ~QVariantAbstractTree();

    virtual bool isTree();
    virtual bool isLeaf();
    virtual bool isNull();
    QVariantTree* asTree();
    QVariantLeaf* asLeaf();
    QVariantAbstractTree &operator[](const QString & key);
    QVariantAbstractTree&get(const QString & key);
    QVariantAbstractTree&getByPath(const QString & path);
    template <class T>
    T value();

    bool contains(const QString & key);
    bool containsPath(const QString & path);

    virtual QVariant asQVariant() = 0;
    virtual QString printString(unsigned indent = 0) = 0;

protected:
    virtual QVariantAbstractTree* basicGet(const QString & key) = 0;
    static QVariantNullTree s_nullValue;
};

class TASTEFULSERVER_API QVariantTree : public QVariantAbstractTree
{
public:
    ~QVariantTree();

    bool isTree();

    int size();
    void insert(const QString & key, QVariantAbstractTree * value);
    void insert(const QString & key, QVariant value);
    QVariantTree&createSubtree(const QString & key);
    QVariantTree&obtainSubtree(const QString & key);

    QVariant asQVariant();
    QString printString(unsigned indent = 0);

protected:
    QVariantAbstractTree* basicGet(const QString & key);

    QHash<QString, QVariantAbstractTree *> m_children;
};

class TASTEFULSERVER_API QVariantLeaf : public QVariantAbstractTree
{
public:
    QVariantLeaf(QVariant value);

    bool isLeaf();

    QVariant asQVariant();
    QString printString(unsigned indent = 0);
    void setElement(const QVariant & newElement);

protected:
    QVariantAbstractTree* basicGet(const QString & key);
    QVariant m_element;
};

class TASTEFULSERVER_API QVariantNullTree : public QVariantAbstractTree
{
public:
    bool isNull();

    QVariant asQVariant();
    QString printString(unsigned indent = 0);

protected:
    QVariantAbstractTree* basicGet(const QString & key);
};

} // namespace tastefulserver

#include "QVariantTree.hpp"

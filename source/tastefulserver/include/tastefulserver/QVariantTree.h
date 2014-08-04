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

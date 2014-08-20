#pragma once

#include <QString>
#include <QHash>
#include <QVariant>

#include <tasteful-server/tasteful-server_api.h>

namespace tastefulserver {

class QVariantTree;
class QVariantLeaf;
class QVariantNullTree;

class TASTEFUL_SERVER_API QVariantAbstractTree
{
public:
    virtual ~QVariantAbstractTree();

    virtual bool isTree() const;
    virtual bool isLeaf() const;
    virtual bool isNull() const;
    const QVariantTree* asTree() const;
    QVariantTree* asTree();
    const QVariantLeaf* asLeaf() const;
    QVariantLeaf* asLeaf();
    const QVariantAbstractTree & operator[](const QString & key) const;
    QVariantAbstractTree & operator[](const QString & key);
    const QVariantAbstractTree & get(const QString & key) const;
    QVariantAbstractTree & get(const QString & key);
    const QVariantAbstractTree & getByPath(const QString & path) const;
    QVariantAbstractTree & getByPath(const QString & path);
    template <class T>
    T value() const;

    bool contains(const QString & key) const;
    bool containsPath(const QString & path) const;

    virtual QVariant asQVariant() const = 0;
    virtual QString printString(unsigned indent = 0) const = 0;

protected:
    virtual QVariantAbstractTree* basicGet(const QString & key) const = 0;
    static QVariantNullTree s_nullValue;
};

class TASTEFUL_SERVER_API QVariantTree : public QVariantAbstractTree
{
public:
    ~QVariantTree();

    virtual bool isTree() const override;

    QList<QString> keys() const;

    int size() const;
    void insert(const QString & key, QVariantAbstractTree * value);
    void insert(const QString & key, const QVariant & value);
    QVariantTree & createSubtree(const QString & key);
    QVariantTree & obtainSubtree(const QString & key);

    virtual QVariant asQVariant() const override;
    virtual QString printString(unsigned indent = 0) const override;

protected:
    virtual QVariantAbstractTree* basicGet(const QString & key) const override;

    QHash<QString, QVariantAbstractTree *> m_children;
};

class TASTEFUL_SERVER_API QVariantLeaf : public QVariantAbstractTree
{
public:
    QVariantLeaf(const QVariant & value);

    virtual bool isLeaf() const override;

    virtual QVariant asQVariant() const override;
    virtual QString printString(unsigned indent = 0) const override;
    void setElement(const QVariant & newElement);

protected:
    virtual QVariantAbstractTree* basicGet(const QString & key) const override;
    QVariant m_element;
};

class TASTEFUL_SERVER_API QVariantNullTree : public QVariantAbstractTree
{
public:
    virtual bool isNull() const override;

    virtual QVariant asQVariant() const override;
    virtual QString printString(unsigned indent = 0) const override;

protected:
    virtual QVariantAbstractTree* basicGet(const QString & key) const override;
};

} // namespace tastefulserver

#include "QVariantTree.hpp"

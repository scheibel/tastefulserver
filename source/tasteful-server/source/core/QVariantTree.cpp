#include <tasteful-server/QVariantTree.h>

#include <QStringList>
#include <QTextStream>

namespace tastefulserver {

QVariantNullTree QVariantAbstractTree::s_nullValue;
QVariantAbstractTree::~QVariantAbstractTree()
{
}

bool QVariantAbstractTree::isTree() const
{
    return false;
}

bool QVariantAbstractTree::isLeaf() const
{
    return false;
}

bool QVariantAbstractTree::isNull() const
{
    return false;
}

const QVariantTree * QVariantAbstractTree::asTree() const
{
    return isTree() ? static_cast<const QVariantTree *>(this) : nullptr;
}

QVariantTree * QVariantAbstractTree::asTree()
{
    return isTree() ? static_cast<QVariantTree *>(this) : nullptr;
}

const QVariantLeaf * QVariantAbstractTree::asLeaf() const
{
    return isLeaf() ? static_cast<const QVariantLeaf *>(this) : nullptr;
}

QVariantLeaf * QVariantAbstractTree::asLeaf()
{
    return isLeaf() ? static_cast<QVariantLeaf *>(this) : nullptr;
}

const QVariantAbstractTree & QVariantAbstractTree::operator[](const QString & key) const
{
    return get(key);
}

QVariantAbstractTree & QVariantAbstractTree::operator[](const QString & key)
{
    return get(key);
}

const QVariantAbstractTree & QVariantAbstractTree::get(const QString & key) const
{
    const QVariantAbstractTree * tree = basicGet(key);

    return tree ? *tree : s_nullValue;
}

QVariantAbstractTree &QVariantAbstractTree::get(const QString & key)
{
    QVariantAbstractTree * tree = basicGet(key);

    return tree ? *tree : s_nullValue;
}

const QVariantAbstractTree & QVariantAbstractTree::getByPath(const QString & path) const
{
    QStringList parts = path.split(QRegExp("(\\/|\\.)."));
    const QVariantAbstractTree * tree = this;

    for (QString part : parts)
    {
        tree = tree->basicGet(part);

        if (!tree)
        {
            return s_nullValue;
        }
    }

    return *tree;
}

QVariantAbstractTree & QVariantAbstractTree::getByPath(const QString & path)
{
    QStringList parts = path.split(QRegExp("(\\/|\\.)."));
    QVariantAbstractTree * tree = this;

    for (QString part : parts)
    {
        tree = tree->basicGet(part);

        if (!tree)
        {
            return s_nullValue;
        }
    }

    return *tree;
}

bool QVariantAbstractTree::contains(const QString & key) const
{
    return !get(key).isNull();
}

bool QVariantAbstractTree::containsPath(const QString & path) const
{
    return !getByPath(path).isNull();
}

QVariantTree::~QVariantTree()
{
    for (QVariantAbstractTree * child : m_children)
    {
        delete child;
    }
}

bool QVariantTree::isTree() const
{
    return true;
}

QList<QString> QVariantTree::keys() const
{
    return m_children.keys();
}

int QVariantTree::size() const
{
    return m_children.size();
}

void QVariantTree::insert(const QString & key, QVariantAbstractTree * value)
{
    if (m_children.contains(key))
    {
        delete m_children.take(key);
    }
    m_children.insert(key, value);
}

void QVariantTree::insert(const QString & key, const QVariant & value)
{
    insert(key, new QVariantLeaf(value));
}

QVariantTree &QVariantTree::createSubtree(const QString & key)
{
    QVariantTree * tree = new QVariantTree();

    insert(key, tree);

    return *tree;
}

QVariantTree &QVariantTree::obtainSubtree(const QString & key)
{
    return get(key).isTree() ? *get(key).asTree() : createSubtree(key);
}

QVariantAbstractTree * QVariantTree::basicGet(const QString & key) const
{
    return m_children.value(key, nullptr);
}

QVariant QVariantTree::asQVariant() const
{
    QVariantMap map;

    for (QString & key : m_children.keys())
    {
        map.insert(key, m_children[key]->asQVariant());
    }

    return map;
}

QString QVariantTree::printString(unsigned indent) const
{
    QString string;
    QTextStream stream(&string);
    QString indentString(indent, ' ');

    stream << "{" << endl;
    QStringList keys = m_children.keys();
    for (int i = 0;i<keys.size();++i)
    {
        QString key = keys[i];
        stream << indentString << "  " << key << " = " << m_children[key]->printString(indent + 2);
        if (i<keys.size() - 1)
        {
            stream << ",";
        }
        stream << endl;
    }
    stream << indentString << "}";

    return string;
}

QVariantLeaf::QVariantLeaf(const QVariant & value)
    : m_element(value)
{
}

bool QVariantLeaf::isLeaf() const
{
    return true;
}

QVariantAbstractTree * QVariantLeaf::basicGet(const QString & /*key*/) const
{
    return nullptr;
}

QVariant QVariantLeaf::asQVariant() const
{
    return m_element;
}

QString QVariantLeaf::printString(unsigned /*indent*/) const
{
    if (m_element.type()==QVariant::UserType)
    {
        return QString("%1()").arg(m_element.typeName());
    }

    return m_element.toString();
}

void QVariantLeaf::setElement(const QVariant & newElement)
{
    m_element = newElement;
}

bool QVariantNullTree::isNull() const
{
    return true;
}

QVariantAbstractTree * QVariantNullTree::basicGet(const QString & /*key*/) const
{
    return nullptr;
}

QVariant QVariantNullTree::asQVariant() const
{
    return QVariant();
}

QString QVariantNullTree::printString(unsigned /*indent*/) const
{
    return "NULL";
}

} // namespace tastefulserver

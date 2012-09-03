#include <internal/QVariantTree>
#include <QStringList>
#include <QTextStream>

using namespace internal;

QVariantNullTree QVariantAbstractTree::nullValue;

bool QVariantAbstractTree::isTree() {
	return false;
}

bool QVariantAbstractTree::isLeaf() {
	return false;
}

bool QVariantAbstractTree::isNull() {
	return false;
}

QVariantTree* QVariantAbstractTree::asTree() {
	return isTree() ? (QVariantTree*)this : nullptr;
}

QVariantLeaf* QVariantAbstractTree::asLeaf() {
	return isLeaf() ? (QVariantLeaf*)this : nullptr;
}

QVariantAbstractTree& QVariantAbstractTree::operator[](const QString& key) {
	return get(key);
}

QVariantAbstractTree& QVariantAbstractTree::get(const QString& key) {
	QVariantAbstractTree* tree = basicGet(key);
	
	return tree ? *tree : nullValue;
}

QVariantAbstractTree& QVariantAbstractTree::getByPath(const QString& path) {
	QStringList parts = path.split(QRegExp("(\\/|\\.)."));
	
	QVariantAbstractTree* tree = this;
	for(QString part : parts) {
		tree = tree->basicGet(part);
		
		if (!tree) {
			return nullValue;
		}
	}
	
	return *tree;
}

bool QVariantAbstractTree::contains(const QString& key) {
	return !get(key).isNull();
}

bool QVariantAbstractTree::containsPath(const QString& path) {
	return !getByPath(path).isNull();
}

QVariantTree::~QVariantTree() {
	for (QVariantAbstractTree* child : children) {
		delete child;
	}
}

bool QVariantTree::isTree() {
	return true;
}

int QVariantTree::size() {
	return children.size();
}

void QVariantTree::insert(const QString& key, QVariantAbstractTree* value) {
	if (children.contains(key)) {
		delete children.take(key);
	}
	children.insert(key, value);
}

void QVariantTree::insert(const QString& key, QVariant value) {
	insert(key, new QVariantLeaf(value));
}

QVariantTree& QVariantTree::createSubtree(const QString& key) {
	QVariantTree* tree = new QVariantTree();
	
	insert(key, tree);
	
	return *tree;
}

QVariantTree& QVariantTree::obtainSubtree(const QString& key) {
	return get(key).isTree() ? *get(key).asTree() : createSubtree(key);
}

QVariantAbstractTree* QVariantTree::basicGet(const QString& key) {
	return children.value(key, nullptr);
}

QVariant QVariantTree::asQVariant() {
	QVariantMap map;
	
	for (QString& key : children.keys()) {
		map.insert(key, children[key]->asQVariant());
	}
	
	return map;
}

QString QVariantTree::printString(unsigned indent) {
	QString string;
	QTextStream stream(&string);
	QString indentString(indent, ' ');
	
	stream << "{" << endl;
	QStringList keys = children.keys();
	for (int i=0; i<keys.size(); ++i) {
		QString key = keys[i];
		stream << indentString << "  " << key << " = " << children[key]->printString(indent+2);
		if (i<keys.size()-1) stream << ",";
		stream << endl;
	}
	stream << indentString << "}";
	
	return string;
}

QVariantLeaf::QVariantLeaf(QVariant value) : element(value) {
}

bool QVariantLeaf::isLeaf() {
	return true;
}

QVariantAbstractTree* QVariantLeaf::basicGet(const QString& key) {
	return nullptr;
}

QVariant QVariantLeaf::asQVariant() {
	return element;
}

QString QVariantLeaf::printString(unsigned indent) {
	if (element.type()==QVariant::UserType) return QString("%1()").arg(element.typeName());
	return element.toString();
}

bool QVariantNullTree::isNull() {
	return true;
}

QVariantAbstractTree* QVariantNullTree::basicGet(const QString& key) {
	return nullptr;
}

QVariant QVariantNullTree::asQVariant() {
	return QVariant();
}

QString QVariantNullTree::printString(unsigned indent) {
	return "NULL";
}


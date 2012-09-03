#include <UploadedFile>


UploadedFile::UploadedFile() {
}

UploadedFile::UploadedFile(ContentType contentType) : contentType(contentType) {
}

QVariant UploadedFile::toQVariant() {
	return QVariant::fromValue(*this);
}

void UploadedFile::setFilename(const QString& filename) {
	this->filename = filename;
}

void UploadedFile::setContent(QByteArray content) {
	this->content = content;
}

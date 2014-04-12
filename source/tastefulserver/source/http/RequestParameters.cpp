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

#include <tastefulserver/RequestParameters.h>
#include <tastefulserver/UploadedFile.h>
#include <tastefulserver/ByteArrayStream.h>

#include "../core/QVariantTree.h"

#include <QUrl>
#include <QRegExp>

using namespace internal;

RequestParameters::RequestParameters() : params(new QVariantTree()) {
}

RequestParameters RequestParameters::fromUrl(const QUrl& url) {
	RequestParameters params;
	params.parseUrl(url);
	return params;
}

RequestParameters RequestParameters::fromUrlEncoded(const QByteArray& urlEncodedPost) {
	RequestParameters params;
	params.parseUrlEncoded(urlEncodedPost);
	return params;
}

RequestParameters RequestParameters::fromMultiPart(const MultiPart& multiPart) {
	RequestParameters params;
	params.parseMultiPart(multiPart);
	return params;
}

QString RequestParameters::toString() const {
	return params->printString();
}

void RequestParameters::parseUrl(const QUrl& url) {
	if (!url.hasQuery()) return;

	QList<QPair<QString, QVariant>> parameters;

	for (const QString& parameter: url.query().split('&')) {
		int splitIndex = parameter.indexOf('=');
		if (splitIndex<=0) continue;

		QString key = parameter.left(splitIndex);
		QString value = parameter.mid(splitIndex+1);
		
		parameters << QPair<QString, QVariant>(key, value);
	}

	parseList(parameters);
}

void RequestParameters::parseUrlEncoded(const QByteArray& urlEncoded) {
	QByteArray copy(urlEncoded);
	
	parseUrl(QUrl::fromPercentEncoding("/?"+copy.replace('+',' ')));
}

void RequestParameters::parseMultiPart(const MultiPart& multiPart) {
	QList<QPair<QString, QVariant>> parameters;
	
	for (const Part& part: multiPart.getParts()) {
		HttpHeader contentDisposition = part.getHeader(http::ContentDisposition);
		HttpHeaderElement element = contentDisposition.getElement();
		if (element.getName()=="form-data") {
			QString name = element.getParameter("name");
			if (part.hasHeader(http::ContentType)) {
				UploadedFile uploadedFile;
				uploadedFile.setContent(part.getContent());
				if (element.isSet("filename")) {
					uploadedFile.setFilename(element.getParameter("filename"));
				}
				parameters << QPair<QString, QVariant>(name, uploadedFile.toQVariant());
			} else {
				parameters << QPair<QString, QVariant>(name, QString(part.getContent()));
			}
		}
	}

	parseList(parameters);
}

void RequestParameters::parseList(const QList<QPair<QString, QVariant>>& parameters) {
	for (const QPair<QString, QVariant>& pair : parameters) {
		if (pair.first.isEmpty()) {
			continue;
		}

		QList<QString> indices = extractIndices(pair.first);

		QVariantTree* currentParams = params.data();
		QString index;
		for (unsigned i = 0; i < indices.size() - 1; ++i) {
			index = indices[i].isEmpty() ? QString::number(currentParams->size()) : indices[i];
			currentParams = &currentParams->obtainSubtree(index);
		}

		index = indices.last().isEmpty() ? QString::number(currentParams->size()) : indices.last();
		currentParams->insert(index, pair.second);
	}
}

QVariantAbstractTree& RequestParameters::operator[](const QString& key) const {
	return get(key);
}

QVariantAbstractTree& RequestParameters::getByPath(const QString& path) const {
	return params->getByPath(path);
}

QVariantAbstractTree& RequestParameters::get(const QString& key) const {
	return params->get(key);
}

void RequestParameters::insert(const QString& key, const QVariant& value) {
	params->insert(key, value);
}

bool RequestParameters::contains(const QString& key) const {
	return params->contains(key);
}

bool RequestParameters::containsPath(const QString& path) const {
	return params->containsPath(path);
}

QList<QString> RequestParameters::extractIndices(const QString& key) const {
	ByteArrayStream stream(key.toUtf8());
	QString name = stream.readUpTo('[');

	if (name.isEmpty()) {
		return QList<QString>() << key;
	}

	QList<QString> indices = QList<QString>() << name;

	bool valid = true;
	while (stream.canReadUpTo('[')) {
		stream.skipBehind('[');

		if (stream.canReadUpTo(']')) {
			indices << stream.readUpTo(']', true);
		} else {
			return QList<QString>() << key;
		}
	}

	if (!stream.atEnd()) {
		return QList<QString>() << key;
	}

	return indices;
}

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

#include <MultiPart>
#include <internal/QVariantTree>

#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <QByteArray>
#include <QSharedPointer>

using namespace internal;

class RequestParameters {
	public:
		RequestParameters();
		
		bool contains(const QString& key) const;
		bool containsPath(const QString& path) const;
		
		QVariantAbstractTree& operator[](const QString& key) const;
		QVariantAbstractTree& get(const QString& key) const;
		QVariantAbstractTree& getByPath(const QString& path) const;
		
		void insert(const QString& key, const QVariant& value);
		
		void parseUrl(const QUrl& url);
		void parseUrlEncoded(QByteArray urlEncodedPost);
		void parseMultiPart(const MultiPart& multiPart);
		
		QString toString() const;
		
		static RequestParameters fromUrl(const QUrl& url);
		static RequestParameters fromUrlEncoded(QByteArray urlEncoded);
		static RequestParameters fromMultiPart(const MultiPart& multiPart);
	protected:
		void parseList(QList<QPair<QString, QVariant>> parameters);
		QList<QString> extractIndices(const QString& key) const;
		
		QSharedPointer<QVariantTree> params;
};

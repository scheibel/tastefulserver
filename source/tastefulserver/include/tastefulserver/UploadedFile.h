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

#include <tastefulserver/tastefulserver_api.h>

#include <QString>
#include <QByteArray>
#include <QMetaType>
#include <QVariant>

#include <tastefulserver/ContentType.h>

class TASTEFULSERVER_API UploadedFile {
	public:
		UploadedFile();
		UploadedFile(const ContentType& contentType);
	
		void setFilename(const QString& filename);
		void setContent(const QByteArray& content);
	
        const QString& getFilename() const;
        const QByteArray& getContent() const;
        const ContentType& getContentType() const;
	
        QVariant toQVariant() const;
	private:
		QString filename;
		QByteArray content;
		ContentType contentType;
};

Q_DECLARE_METATYPE(UploadedFile)

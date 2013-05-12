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

#include <QCoreApplication>
#include <HttpServer>

int main(int argc, char** argv) {
	QCoreApplication app(argc, argv);

	HttpServer server([](const HttpRequest& request) {
		HttpResponse response(request);

		response.setStatusCode(http::OK);
		
		QByteArray content = request.toByteArray();
		content.append("Client IP:" + request.address().toString() + " (Port " + QString::number(request.port()) + ")\r\n");
		
		response.setContent(content);

		return response;
	});

	server.listen(QHostAddress::Any, 8080);

	return app.exec();
}

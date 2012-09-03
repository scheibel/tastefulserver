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
#include <HttpsServer>

QSslCertificate getCertificate() {
	return QSslCertificate(R"DATA(-----BEGIN CERTIFICATE-----
MIICrjCCAhegAwIBAgIJAK+QVncXmApiMA0GCSqGSIb3DQEBBQUAMHAxCzAJBgNV
BAYTAkRFMRAwDgYDVQQIDAdQb3RzZGFtMRAwDgYDVQQHDAdQb3RzZGFtMQswCQYD
VQQKDAJ0czELMAkGA1UECwwCdHMxCzAJBgNVBAMMAm1lMRYwFAYJKoZIhvcNAQkB
FgdhQGIuY29tMB4XDTEyMDgxNjIxMTUwMloXDTEzMDgxNjIxMTUwMlowcDELMAkG
A1UEBhMCREUxEDAOBgNVBAgMB1BvdHNkYW0xEDAOBgNVBAcMB1BvdHNkYW0xCzAJ
BgNVBAoMAnRzMQswCQYDVQQLDAJ0czELMAkGA1UEAwwCbWUxFjAUBgkqhkiG9w0B
CQEWB2FAYi5jb20wgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAJ6Hh7/VkrdW
9hecUrkNS/CNDVX3T51eEvAeE3+69kewHZxs+uVlJvKvpV51mk/AqdYBQV8t1PLZ
v3QMAOWGqGzallEKyDSUKO4xQIzMYJKwmvvAFxUfDJa4IwFSh/pl5sFBCXJP8bHP
dKlKzJ8r62lQAZjVbhvSyrpudEbJWi6HAgMBAAGjUDBOMB0GA1UdDgQWBBSM7FU1
GKidm1TsiIf77Gc6sldEkTAfBgNVHSMEGDAWgBSM7FU1GKidm1TsiIf77Gc6sldE
kTAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBBQUAA4GBAJPH4CnbVwuJ9eG6Xuiy
sfekT7x1hu/NnFMg61tEE3CwKOqFmg0caBKw4xHNLG03sOx0DvvJzB9UdXjydPy7
jOVtNWLSIeRQb2HalheoaJNhpEOgpyMELKyuQcpY9IJ9TuMpanu0bQdh6gf5Bxud
pdUcCqzwnS7ZpXOnf19PqFX+
-----END CERTIFICATE-----
)DATA", QSsl::Pem);
}

QSslKey getPrivateKey() {
	return QSslKey(R"DATA(-----BEGIN PRIVATE KEY-----
MIICdQIBADANBgkqhkiG9w0BAQEFAASCAl8wggJbAgEAAoGBAJ6Hh7/VkrdW9hec
UrkNS/CNDVX3T51eEvAeE3+69kewHZxs+uVlJvKvpV51mk/AqdYBQV8t1PLZv3QM
AOWGqGzallEKyDSUKO4xQIzMYJKwmvvAFxUfDJa4IwFSh/pl5sFBCXJP8bHPdKlK
zJ8r62lQAZjVbhvSyrpudEbJWi6HAgMBAAECgYBm6dMNjSd20ARoZXoqWeBvpBIB
OdC8d2LR8KR16jiRZ1qBvxnNq7HyIGa5gJ0RoyBeJmh60nUIxqUMP/KhDp7tLeQA
8USO1p+V1lMUqh3Pr86TAK9Vbe1t//jjZAi7cfUZgn+GHTwUaBW5uNtwF5dRgB14
Oo1uilLDuvRWPo/YmQJBANFs6SOYzrbAEWyHZOD+sXrvB1dAxbmW9Zga9rXXXCtT
vkuRjkSQyKPR6WgLlLymLztGLQrDDHABJC2pqbtgQ00CQQDByPzpDCYU+DG/02cg
5vA2dVgZqU6f4w7QxJ9+veUjCbJ7Vn3sNWD7rZ4hy3yiVefodsygK8pmkHvYcQXY
EWcjAkAG59F0NAi06/ZnHrvL03oktJcIYIFkkj686j6yVkyavSMsOLt5NVD4XeaV
YLRh2BT8nl7D2RSi6Na5jrujCxSNAkBNJqPqdLfEqXvFD3mDk+4XAGju7oUrCs8f
Nuj5IMlVRcsVZ74CiM3v5ZOLiOQ6DVGjL4kQS97tLqDf7z+QYn1ZAkBlnCMSe9jw
OcIZi8u7A/JR3Uw9JthNMp03u00XQ+xAlsQDTWDFvZQj/1rP62MDo/lv8woCQ/eg
hBGw7TiSG6vp
-----END PRIVATE KEY-----
)DATA", QSsl::Rsa);
}

int main(int argc, char** argv) {
	QCoreApplication app(argc, argv);
	
	HttpsServer server(getCertificate(), getPrivateKey(), [](HttpRequest& request) {
		HttpResponse response;
		
		QByteArray content = request.toByteArray();
		if (!request.getCookies().isEmpty()) content.append("\r\n\r\nCookies\r\n");
		for (Cookie& cookie: request.getCookies()) {
			content.append(cookie.toString() + "\r\n");
		}
		
		response.setContent(content);
		response.setStatusCode(http::OK);
		
		Cookie cookie("testcookie", "testvalue");
		cookie.setMaxAge(30);
		cookie.setPath("/test");
		response.getCookies().set(cookie);
		
		return response;
	});
	server.listen(QHostAddress::Any, 8080);
	
	return app.exec();
}
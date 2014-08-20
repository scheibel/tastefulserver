#include <QCoreApplication>

#include <QBuffer>

#include <tasteful-server/HttpsCallbackServer.h>

QSslCertificate getCertificate()
{
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

QSslKey getPrivateKey()
{
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

using namespace tastefulserver;

int main(int argc, char ** argv)
{
    QCoreApplication app(argc, argv);
    HttpsCallbackServer server(getCertificate(), getPrivateKey(), [](const HttpRequest & request) {
            HttpResponse response(http::OK, request);

            QByteArray content;
            QBuffer buffer(&content);
            buffer.open(QIODevice::ReadWrite);

            request.writeTo(buffer);

            content.append("Client IP:" + request.address().toString() + " (Port " + QString::number(request.port()) + ")\r\n");
            if (request.hasCookies())
            {
                content.append("\r\n\r\nCookies\r\n");
            }
            for (const Cookie & cookie : request.getCookies())
            {
                content.append(cookie.toString() + "\r\n");
            }

            response.setContent(content);
            response.setStatusCode(http::OK);
            response.setCookie("testcookie", "testvalue").setPath("/test").setMaxAge(30);

            return response;
        });

    server.listen(QHostAddress::Any, 8080);

    return app.exec();
}

Tasteful Server
=========

Tasteful Server provides a mulithreaded server architecture for high performance network applications.
It is written in C++ and uses the Qt library.

You can specify a number of servers with their own handlers.
Those servers share a thread pool of user defined size to handle incoming data.

This library comes with a HTTP suite for handling incoming HTTP requests.
You can pass your own callback to handle those completely preparsed requests with an object oriented interface.
SSL for HTTPS is also supported.

Platform independency is provided through [cmake-init](https://github.com/hpicgs/cmake-init).
Currently supported platforms are:
 * Windows
 * Linux
 * OS X

Features
-------------

 * Thread pool with user defined size
 * Multiple servers share one thread pool
 * HTTP server and HTTPS server
 * Handling of requests through callbacks
 * HTTP parser which translates requests to objects
 * Convenience support for several HTTP features
     * GET and POST parameters
     * Multipart and uploaded files
     * Cookies
     * Websockets

Requirements
---------------------

 * [Qt 5.0](http://qt.nokia.com/)
 * [CMake 2.8.9](http://www.cmake.org/)
 * C++11 compatible Compiler (gcc >= 4.7, clang >= 3.4, MSVC 2013)

Known Projects Using Tasteful Server
--------------------------------------------------------

 * [Tasteful Framework](https://github.com/scheibel/tasteful-framework)

//============================================================================
// Name        : Ravioli Racing
// Author      : Branden Lee
// Version     : 0.01
// Copyright   : GPL
// Description : C++ Racing Game
//
// IDE: Eclipse Version: Neon.2 Release (4.6.2)
// Requires Cygwin in windows and GCC in linux
//============================================================================
#include "../src/POSIX_HTTP.h"
#ifdef __linux__

POSIX_HTTP::POSIX_HTTP()
{

}

int POSIX_HTTP::socket_connect(char *host, int port_)
{
	int on = 1, sock;
	in_port_t port = (in_port_t) port_;
	struct hostent *hp;
	struct sockaddr_in addr;

	if ((hp = gethostbyname(host)) == NULL)
	{
		throw std::runtime_error("gethostbyname");
	}
	bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *) &on, sizeof(int));

	if (sock == -1)
	{
		throw std::runtime_error("setsockopt");
	}

	if (connect(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr_in))
			== -1)
	{
		throw std::runtime_error("connect");

	}
	return sock;
}

std::string POSIX_HTTP::getWebsite(std::string url, std::string path)
{
	int fd;
	char buffer[BUFFER_SIZE];
	std::string response, get_http;
	response = "";
	get_http =
	"GET " + path + " HTTP/1.1" + "\r\nHost: " + url
	+ "\r\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36"
	+ "\r\nAccept-Language: en-us"
	+ "\r\nConnection: close\r\n\r\n";
	char *cstr = new char[url.length() + 1];
	char *getHttpC = new char[get_http.length() + 1];
	strcpy(cstr, url.c_str());
	strcpy(getHttpC, get_http.c_str());
	try
	{
		fd = socket_connect(cstr, 80);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
	write(fd, getHttpC, strlen(getHttpC)); // write(fd, char[]*, len);
	bzero(buffer, BUFFER_SIZE);

	while (read(fd, buffer, BUFFER_SIZE - 1) != 0)
	{
		response += buffer;
		bzero(buffer, BUFFER_SIZE);
	}

	shutdown(fd, SHUT_RDWR);
	close(fd);
	return response;
}

std::string POSIX_HTTP::html(std::string response)
{
	return response.substr(response.find("\r\n\r\n") + 4);
}
#endif

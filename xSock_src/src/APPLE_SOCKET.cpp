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
#include "../src/APPLE_SOCKET.h"
#ifdef __APPLE__

APPLE_SOCKET::APPLE_SOCKET(int port_)
{
	port = (u_short) port_;
	//setup a socket and connection tools
	memset((char*) &servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);
	listenFlag = false;
}

APPLE_SOCKET::APPLE_SOCKET(std::string host_, int port_)
{
	port = (u_short) port_;
	host = host_;
	char* hostC = new char[host.length() + 1];
	strcpy(hostC, host.c_str());

	/* get IP by host */
	char hostname[NI_MAXHOST];
	char servInfo[NI_MAXSERV];
	std::string errMsg;
	struct addrinfo hints, *res;
	struct in_addr addr;
	int err;

	//WSAData data;
	//WSAStartup(MAKEWORD(2, 0), &data);

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;

	if ((err = getaddrinfo(host_.c_str(), NULL, &hints, &res)) != 0)
	{
		errMsg = "";
		if (err == 11001)
		{
			errMsg = "Host not found";
		}
		/*throw std::runtime_error(
		 "Error Code " + std::to_string(err)
		 + (errMsg.length() > 0 ? ": " + errMsg : ""));*/
	}

	try
	{
		addr.s_addr = ((struct sockaddr_in *) (res->ai_addr))->sin_addr.s_addr;
	} catch (const std::exception& e)
	{
		throw std::runtime_error(
				"Could not connect to the internet." + std::string(e.what()));
	}
	freeaddrinfo(res);
	//WSACleanup();

	//setup a socket and connection tools
	memset((char*) &servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = *((unsigned long*) &(addr.s_addr));
	servAddr.sin_port = htons(port);
	listenFlag = false;

	/* get ip address string */
	int dwRetval;
	dwRetval = getnameinfo((struct sockaddr *) &servAddr,
			sizeof(struct sockaddr), hostname,
			NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);

	if (dwRetval == 0)
	{
		std::cout << "ip address: " << hostname << std::endl;
	}
}

void APPLE_SOCKET::sockSetup()
{
	/*WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		throw std::runtime_error("WSAStartup failed.");
		//return 1;
	}*/

	//open stream oriented socket with internet address
	//also keep track of the socket descriptor
	serverSd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSd < 0)
	{
		throw std::runtime_error("Error establishing the server socket");
	}
}

void APPLE_SOCKET::sockConnect()
{
	//try to connect...
	int status = connect(serverSd, (sockaddr*) &servAddr, sizeof(servAddr));
	if (status != 0)
	{
		throw std::runtime_error("Error connecting to socket!");
	}
}

void APPLE_SOCKET::sockBind()
{
	//bind the socket to its local address
	int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr,
			sizeof(servAddr));
	if (bindStatus < 0)
	{
		throw std::runtime_error("Error binding socket to local address");
	}
}

void APPLE_SOCKET::sockListen(std::function<void(APPLE_SOCKET*)>* conCB,
		std::function<void(APPLE_SOCKET*)>* dconCB,
		std::function<void(APPLE_SOCKET*)>* rcvCB)
{
	//listen for up to 5 requests at a time
	listen(serverSd, 5);
	//receive a request from client using accept
	//we need a new address to connect with the client
	//sockaddr_in newSockAddr;
	//socklen_t newSockAddrSize = sizeof(newSockAddr);
	//accept, create a new socket descriptor to
	//handle the new connection with client
	/*acceptSd = accept(serverSd, (sockaddr *) &newSockAddr, &newSockAddrSize);
	 if (acceptSd < 0)
	 {
	 throw std::runtime_error("Error accepting request from client!");
	 }*/
	listenFlag = true;
	//lets keep track of the session time
	//std::chrono::system_clock::time_point t1, t2;
	unsigned int elapsedTime = 0;
	//t1 = std::chrono::system_clock::now();
	//also keep track of the amount of data sent as well
	int bytesRead, bytesWritten = 0;
	char msg[1500];

	/* Set nonblock for stdin. */
	/*u_long iMode = 1;
	 int iResult = ioctlsocket(STDIN_FILENO, FIONBIO, &iMode);
	 if (iResult != NO_ERROR)
	 {
	 printf("ioctlsocket failed with error: %d\n", iResult);
	 }*/
	/*int iResult = fcntl(STDIN_FILENO, F_GETFL, 0);
	 flag |= O_NONBLOCK;
	 fcntl(STDIN_FILENO, F_SETFL, flag);*/

	int i;
	int n = connection_list.size();
	for (i = 0; i < n; ++i)
	{
		connection_list[i]->socket = NO_SOCKET;
		create_peer(connection_list[i]);
	}

	int high_sock = serverSd;
	while (listenFlag)
	{
		std::cout << "loop\n";
		build_fd_sets(&read_fds, &write_fds, &except_fds);

		high_sock = serverSd;
		n = connection_list.size();
		for (i = 0; i < n; ++i)
		{
			if (connection_list[i]->socket > high_sock)
				high_sock = connection_list[i]->socket;
		}

		int activity = select(high_sock + 1, &read_fds, &write_fds, &except_fds,
		NULL);

		switch (activity)
		{
		case -1:
			//perror("select()");
			//shutdown_properly(EXIT_FAILURE);

		case 0:
			// you should never get here
			//printf("select() returns 0.\n");
			//shutdown_properly(EXIT_FAILURE);

		default:
			/* All set fds should be checked. */
			/*if (FD_ISSET(STDIN_FILENO, &read_fds))
			 {
			 std::cout << "STDIN_FILENO\n";
			 if (handle_read_from_stdin() != 0)
			 {
			 //shutdown_properly(EXIT_FAILURE);
			 }
			 }*/

			if (FD_ISSET(serverSd, &read_fds))
			{
				std::cout << "handle\n";
				handle_new_connection();
				(*conCB)(this);
			}

			if (FD_ISSET(STDIN_FILENO, &except_fds))
			{
				//printf("except_fds for stdin.\n");
				//shutdown_properly(EXIT_FAILURE);
			}

			if (FD_ISSET(serverSd, &except_fds))
			{
				//printf("Exception listen socket fd.\n");
				//shutdown_properly(EXIT_FAILURE);
			}

			//std::cout << "connections...\n";
			n = connection_list.size();
			for (i = 0; i < n; ++i)
			{
				//std::cout << "socket: " << i << std::endl;
				if (connection_list[i]->socket != NO_SOCKET
						&& FD_ISSET(connection_list[i]->socket, &read_fds))
				{

					u_long iMode = 1;
					int iResult = ioctlsocket(connection_list[i]->socket,
							UIO_READ, &iMode);
					if (iResult != NO_ERROR)
					{
						printf("ioctlsocket failed with error: %d\n", iResult);
					}
					if (iMode == 0)
					{
						std::cout << "Client Disconnected: #"
								<< connection_list[i]->socket << std::endl;
						close_client_connection(connection_list[i]);
						(*dconCB)(this);
						continue;
					}

					std::function<int(message_t*)> CB = std::bind(
							&APPLE_SOCKET::handle_received_message, this,
							std::placeholders::_1);
					if (receive_from_peer(connection_list[i], &CB) == 0)
					{
						(*rcvCB)(this);
					}
					else
					{
						close_client_connection(connection_list[i]);
						continue;
					}
				}

				if (connection_list[i]->socket != NO_SOCKET
						&& FD_ISSET(connection_list[i]->socket, &write_fds))
				{
					if (send_to_peer(connection_list[i]) != 0)
					{
						close_client_connection(connection_list[i]);
						continue;
					}
				}

				if (connection_list[i]->socket != NO_SOCKET
						&& FD_ISSET(connection_list[i]->socket, &except_fds))
				{
					printf("Exception client fd.\n");
					close_client_connection(connection_list[i]);
					continue;
				}
			}
		}

		/*std::cout
		 << "And we are still waiting for clients' or stdin activity. You can type something to send:\n";*/

		//receive a message from the client (listen)
		/*std::cout << "Awaiting client response..." << std::endl;
		 memset(&msg, 0, sizeof(msg));	//clear the buffer
		 bytesRead += recv(acceptSd, (char*) &msg, sizeof(msg), 0);
		 if (!strcmp(msg, "exit"))
		 {
		 std::cout << "Client has quit the session" << std::endl;
		 break;
		 }
		 std::cout << "Client: " << msg << std::endl;
		 std::cout << ">";
		 std::string data;
		 std::getline(std::cin, data);
		 if (data == "exit")
		 {
		 //send to the client that server has closed the connection
		 sendFromServer(data);
		 break;
		 }
		 //send the message to client
		 sendFromServer(data);*/
		//(*listenCB)(this);
	}
	//we need to close the socket descriptors after we're all done
	/*t2 = std::chrono::system_clock::now();
	 elapsedTime = (int) std::chrono::duration_cast < std::chrono::seconds
	 > (t2 - t1).count();*/
	//closesocket(acceptSd);
	closesocket(serverSd);
	std::cout << "********Session********" << std::endl;
	std::cout << "Bytes written: " << bytesWritten << " Bytes read: "
			<< bytesRead << std::endl;
	std::cout << "Elapsed time: " << elapsedTime << " secs" << std::endl;
	std::cout << "Connection closed..." << std::endl;
}

void APPLE_SOCKET::sockLoop(std::function<void(APPLE_SOCKET*)>* listenCB)
{
	int bytesRead, bytesWritten = 0;
	//std::chrono::system_clock::time_point t1, t2;
	unsigned int elapsedTime = 0;
	//t1 = std::chrono::system_clock::now();
	char msg[1500];
	while (1)
	{
		std::cout << ">";
		std::string data;
		std::getline(std::cin, data);
		memset(&msg, 0, sizeof(msg)); //clear the buffer
		strcpy(msg, data.c_str());
		if (data == "exit")
		{
			send(serverSd, (char*) &msg, strlen(msg), 0);
			break;
		}
		bytesWritten += send(serverSd, (char*) &msg, strlen(msg), 0);
		std::cout << "Awaiting server response..." << std::endl;
		memset(&msg, 0, sizeof(msg)); //clear the buffer
		bytesRead += recv(serverSd, (char*) &msg, sizeof(msg), 0);
		if (!strcmp(msg, "exit"))
		{
			std::cout << "Server has quit the session" << std::endl;
			break;
		}
		std::cout << "Server: " << msg << std::endl;
		(*listenCB)(this);
	}
	/*t2 = std::chrono::system_clock::now();
	 elapsedTime = (int) std::chrono::duration_cast < std::chrono::seconds
	 > (t2 - t1).count();*/
	closesocket(serverSd);
	std::cout << "********Session********" << std::endl;
	std::cout << "Bytes written: " << bytesWritten << " Bytes read: "
			<< bytesRead << std::endl;
	std::cout << "Elapsed time: " << elapsedTime << " secs" << std::endl;
	std::cout << "Connection closed" << std::endl;
}

int APPLE_SOCKET::prepare_message(char *sender, char *data, message_t *message)
{
	sprintf(message->sender, "%s", sender);
	sprintf(message->data, "%s", data);
	return 0;
}

int APPLE_SOCKET::print_message(message_t *message)
{
	printf("Message: \"%s: %s\"\n", message->sender, message->data);
	return 0;
}

int APPLE_SOCKET::create_message_queue(int queue_size, message_queue_t *queue)
{
	queue->data = (message_t*) calloc(queue_size, sizeof(message_t));
	queue->size = queue_size;
	queue->current = 0;

	return 0;
}

void APPLE_SOCKET::delete_message_queue(message_queue_t *queue)
{
	free(queue->data);
	queue->data = NULL;
}

int APPLE_SOCKET::enqueue(message_queue_t *queue, message_t *message)
{
	if (queue->current == queue->size)
		return -1;

	memcpy(&queue->data[queue->current], message, sizeof(message_t));
	queue->current++;

	return 0;
}

int APPLE_SOCKET::dequeue(message_queue_t *queue, message_t *message)
{
	if (queue->current == 0)
		return -1;

	memcpy(message, &queue->data[queue->current - 1], sizeof(message_t));
	queue->current--;

	return 0;
}

int APPLE_SOCKET::dequeue_all(message_queue_t *queue)
{
	queue->current = 0;

	return 0;
}

int APPLE_SOCKET::delete_peer(peer_t *peer)
{
	closesocket(peer->socket);
	delete_message_queue(&peer->send_buffer);
}

int APPLE_SOCKET::create_peer(peer_t *peer)
{
	create_message_queue(MAX_MESSAGES_BUFFER_SIZE, &peer->send_buffer);

	peer->current_sending_byte = -1;
	peer->current_receiving_byte = 0;

	return 0;
}

char* APPLE_SOCKET::peer_get_addres_str(peer_t *peer)
{
	/*static char ret[INET_ADDRSTRLEN + 10];
	 char peer_ipv4_str[INET_ADDRSTRLEN];
	 inet_ntop(AF_INET, &peer->addres.sin_addr, peer_ipv4_str, INET_ADDRSTRLEN);
	 sprintf(ret, "%s:%d", peer_ipv4_str, peer->addres.sin_port);
	 return ret;*/
	return "test";
}

int APPLE_SOCKET::peer_add_to_send(peer_t *peer, message_t *message)
{
	return enqueue(&peer->send_buffer, message);
}

/* Receive message from peer and handle it with message_handler(). */
int APPLE_SOCKET::receive_from_peer(peer_t *peer,
		std::function<int(message_t*)>* CB)
{
	printf("Ready for recv() from %s.\n", peer_get_addres_str(peer));

	size_t len_to_receive;
	ssize_t received_count;
	size_t received_total = 0;
	do
	{
		// Is completely received?
		if (peer->current_receiving_byte >= sizeof(peer->receiving_buffer))
		{
			(*CB)(&peer->receiving_buffer);
			peer->current_receiving_byte = 0;
		}

		// Count bytes to send.
		len_to_receive = sizeof(peer->receiving_buffer)
				- peer->current_receiving_byte;
		if (len_to_receive > MAX_SEND_SIZE)
			len_to_receive = MAX_SEND_SIZE;

		std::cout << "try to recv() bytes: " << len_to_receive << std::endl;
		received_count = recv(peer->socket,
				(char *) &peer->receiving_buffer + peer->current_receiving_byte,
				len_to_receive, MSG_DONTWAIT);
		if (received_count < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				printf("peer is not ready right now, try again later.\n");
			}
			else
			{
				perror("recv() from peer error");
				//return -1;
				break;
			}
		}
		else if (received_count < 0
				&& (errno == EAGAIN || errno == EWOULDBLOCK))
		{
			break;
		}
		// If recv() returns 0, it means that peer gracefully shutdown. Shutdown client.
		else if (received_count == 0)
		{
			printf("recv() 0 bytes. Peer gracefully shutdown.\n");
			return -1;
		}
		else if (received_count > 0)
		{
			peer->current_receiving_byte += received_count;
			received_total += received_count;
			std::cout << "recv() bytes: " << received_count << std::endl;
		}
	} while (received_count > 0);

	printf("Total recv()'ed %zu bytes.\n", received_total);
	return 0;
}

int APPLE_SOCKET::send_to_peer(peer_t *peer)
{
	printf("Ready for send() to %s.\n", peer_get_addres_str(peer));

	size_t len_to_send;
	ssize_t send_count;
	size_t send_total = 0;
	do
	{
		// If sending message has completely sent and there are messages in queue, why not send them?
		if (peer->current_sending_byte < 0
				|| peer->current_sending_byte >= sizeof(peer->sending_buffer))
		{
			printf(
					"There is no pending to send() message, maybe we can find one in queue... ");
			if (dequeue(&peer->send_buffer, &peer->sending_buffer) != 0)
			{
				peer->current_sending_byte = -1;
				printf("No, there is nothing to send() anymore.\n");
				break;
			}
			printf("Yes, pop and send() one of them.\n");
			peer->current_sending_byte = 0;
		}

		// Count bytes to send.
		len_to_send = sizeof(peer->sending_buffer) - peer->current_sending_byte;
		if (len_to_send > MAX_SEND_SIZE)
			len_to_send = MAX_SEND_SIZE;

		printf("Let's try to send() %zd bytes... ", len_to_send);
		send_count = send(peer->socket,
				(char *) &peer->sending_buffer + peer->current_sending_byte,
				len_to_send, 0);
		if (send_count < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				printf("peer is not ready right now, try again later.\n");
			}
			else
			{
				perror("send() from peer error");
				return -1;
			}
		}
		// we have read as many as possible
		else if (send_count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
		{
			break;
		}
		else if (send_count == 0)
		{
			printf(
					"send()'ed 0 bytes. It seems that peer can't accept data right now. Try again later.\n");
			break;
		}
		else if (send_count > 0)
		{
			peer->current_sending_byte += send_count;
			send_total += send_count;
			printf("send()'ed %zd bytes.\n", send_count);
		}
	} while (send_count > 0);

	printf("Total send()'ed %zu bytes.\n", send_total);
	return 0;
}

int APPLE_SOCKET::read_from_stdin(char *read_buffer, size_t max_len)
{
	memset(read_buffer, 0, max_len);

	ssize_t read_count = 0;
	ssize_t total_read = 0;

	do
	{
		std::cout << "reading\n";
		read_count = read(STDIN_FILENO, read_buffer, max_len);
		//std::cout << "--------------------------\n" << read_buffer << "\n";
		if (read_count < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
		{
			perror("read()");
			return -1;
		}
		else if (read_count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
		{
			break;
		}
		else if (read_count > 0)
		{
			total_read += read_count;
			if (total_read > max_len)
			{
				printf(
						"Message too large and will be chopped. Please try to be shorter next time.\n");
				fflush(STDIN_FILENO);
				break;
			}
		}
	} while (read_count > 0);

	size_t len = strlen(read_buffer);
	if (len > 0 && read_buffer[len - 1] == '\n')
		read_buffer[len - 1] = '\0';

	printf("Read from stdin %zu bytes. Let's prepare message to send.\n",
			strlen(read_buffer));

	return 0;
}

void APPLE_SOCKET::shutdown_properly(int code)
{
	int i;
	int n = connection_list.size();

	closesocket(serverSd);

	for (i = 0; i < n; ++i)
		if (connection_list[i]->socket != NO_SOCKET)
			closesocket(connection_list[i]->socket);

	printf("Shutdown server properly.\n");
	exit(code);
}

int APPLE_SOCKET::build_fd_sets(fd_set *read_fds, fd_set *write_fds,
		fd_set *except_fds)
{
	int i;
	int n = connection_list.size();

	FD_ZERO(read_fds);
	//FD_SET(STDIN_FILENO, read_fds);
	FD_SET(serverSd, read_fds);
	for (i = 0; i < n; ++i)
		if (connection_list[i]->socket != NO_SOCKET)
			FD_SET(connection_list[i]->socket, read_fds);

	FD_ZERO(write_fds);
	for (i = 0; i < n; ++i)
		if (connection_list[i]->socket != NO_SOCKET
				&& connection_list[i]->send_buffer.current > 0)
			FD_SET(connection_list[i]->socket, write_fds);

	FD_ZERO(except_fds);
	//FD_SET(STDIN_FILENO, except_fds);
	FD_SET(serverSd, except_fds);
	for (i = 0; i < n; ++i)
		if (connection_list[i]->socket != NO_SOCKET)
			FD_SET(connection_list[i]->socket, except_fds);

	return 0;
}

int APPLE_SOCKET::handle_new_connection()
{
	sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(client_addr));
	socklen_t client_len = sizeof(client_addr);
	std::cout << "accept..." << std::endl;
	int new_client_sock = accept(serverSd, (struct sockaddr *) &client_addr,
			&client_len);
	std::cout << "accept...DONE" << std::endl;
	if (new_client_sock < 0)
	{
		perror("accept()");
		return -1;
	}

	char client_ipv4_str[INET_ADDRSTRLEN];
	int err = getnameinfo((struct sockaddr*) &client_addr.sin_addr,
			sizeof(client_addr.sin_addr), client_ipv4_str,
			sizeof(client_ipv4_str), 0, 0, NI_NUMERICHOST);
	if (err != 0)
	{
		strcpy(client_ipv4_str, "error");
	}
	//inet_ntop(AF_INET, &client_addr.sin_addr, client_ipv4_str, INET_ADDRSTRLEN);

	//std::cout << "client_addr.sin_addr = " << client_addr.sin_addr << std::endl;
	printf("Incoming connection from %s:%d.\n", client_ipv4_str,
			client_addr.sin_port);

	int i;
	int n = connection_list.size();
	peer_t* newPeer = new peer_t();
	newPeer->socket = new_client_sock;
	newPeer->addres = client_addr;
	newPeer->current_sending_byte = -1;
	newPeer->current_receiving_byte = 0;
	connection_list.push_back(newPeer);

	int flags = ioctlsocket(new_client_sock, F_GETFL, 0);
	fcntl(new_client_sock, F_SETFL, flags | O_NONBLOCK);

	return 0;

	printf("There are too many connections. Close new connection %s:%d.\n",
			client_ipv4_str, client_addr.sin_port);
	closesocket(new_client_sock);
	return -1;
}

int APPLE_SOCKET::close_client_connection(peer_t *client)
{
	printf("Close client socket for %s.\n", peer_get_addres_str(client));

	closesocket(client->socket);
	client->socket = NO_SOCKET;
	dequeue_all(&client->send_buffer);
	client->current_sending_byte = -1;
	client->current_receiving_byte = 0;
}

/* Reads from stdin and create new message. This message enqueues to send queueu. */
int APPLE_SOCKET::handle_read_from_stdin()
{
	char read_buffer[DATA_MAXSIZE]; // buffer for stdin
	if (read_from_stdin(read_buffer, DATA_MAXSIZE) != 0)
		return -1;

	// Create new message and enqueue it.
	message_t new_message;
	prepare_message("Server", read_buffer, &new_message);
	print_message(&new_message);

	/* enqueue message for all clients */
	int i;
	int n = connection_list.size();
	for (i = 0; i < n; ++i)
	{
		if (connection_list[i]->socket != NO_SOCKET)
		{
			if (peer_add_to_send(connection_list[i], &new_message) != 0)
			{
				printf("Send buffer was overflowed, we lost this message!\n");
				continue;
			}
			printf("New message to send was enqueued right now.\n");
		}
	}

	return 0;
}

int APPLE_SOCKET::handle_received_message(message_t *message)
{
	printf("Received message from client.\n");
	print_message(message);
	return 0;
}

void APPLE_SOCKET::stdinListen(std::string str)
{
	std::cout << "got: " << str << std::endl;
}

void APPLE_SOCKET::sendFromServer(std::string data)
{
	char msg[1500];
	memset(&msg, 0, sizeof(msg)); //clear the buffer
	strcpy(msg, data.c_str());
	send(acceptSd, (char*) &msg, strlen(msg), 0);
}

void signalHandle::cb(int sig_number)
{
	if (sig_number == SIGINT)
	{
		printf("SIGINT was caught!\n");
		//((o).(*fn))(EXIT_SUCCESS);
	}
	/* else if (sig_number == SIGPIPE) {
	 printf("SIGPIPE was catched!\n");
	 shutdown_properly(EXIT_SUCCESS);
	 }*/
}

int signalHandle::init()
{
	signal(SIGABRT, signalHandle::cb);
	/*
	 if (signal(SIGINT, &sa, 0) != 0) {
	 perror("sigaction()");
	 return -1;
	 }
	 if (signal(SIGPIPE, &sa, 0) != 0) {
	 perror("sigaction()");
	 return -1;
	 }*/
	return 0;
}

#endif

#include "../includes/Server.hpp"

Client* Server::findClientByNick(const std::string &nick)
{
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second.nickName == nick)
			return &it->second;
	}
    return NULL;
}

Channel* Server::findChannel(const std::string& channelName)
{
	std::map<std::string, Channel>::iterator it = _channels.find(channelName);
	if (it != _channels.end())
		return &it->second;
	return NULL;
}

void Server::checkIfRegistered(Client &client)
{
	if (!client.isRegistered && client.passCheck && !client.nickName.empty() && !client.user.empty())
	{
		client.isRegistered = true; // Client kayıtlı ise, isRegistered'i true yapar.
		writeReply(client.cliFd, RPL_WELCOME(client.nickName)); // Hoşgeldin mesajı gönderir.
		std::cout << GREEN << "Client " << client.nickName << " registered successfully." << RESET << std::endl;
	}
}

const Server::Command Server::commandTable[] = {
	{"PASS", &Server::handlePass},
	{"NICK", &Server::handleNick},
	{"USER", &Server::handleUser},
	{"JOIN", &Server::handleJoin},
	{"PRIVMSG", &Server::handlePrivmsg},
	{"QUIT", &Server::handleQuit},
	{"TOPIC", &Server::handleTopic},
	{"KICK", &Server::handleKick},
	{"CAP", &Server::handleCap},
	{"INVITE", &Server::handleInvite},
	{"MODE", &Server::handleMode},
	{"PING", &Server::handlePing},
	{"NAMES", &Server::handleNames},
};

void Server::createSocket()
{
	if ((this->_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //Socketi oluştur.
		std::cerr << RED << "Error creating socket." << RESET << std::endl;
	else
		std::cout << GREEN << "Socket created." << RESET << std::endl;

	setsockopt(this->_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &this->_reuse, sizeof(int)); //Socket'i yeniden kullanabilmek için ayarla. Address already in use hatasını önler. 
}

void Server::setAdressAndBind()
{
	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET; //IPv4
	server_address.sin_addr.s_addr = INADDR_ANY; //Herhangi bir IP adresi üzerinden bağlanılabilme
	server_address.sin_port = htons(this->_port); //Port numarasını ayarla, htons ile ağ byte sırasına çevir(Little-endian to Big-endian).

	//Bind Socket
	if (bind(this->_server_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) //Socket'i adresle bağla.
		std::cerr << RED << "Error binding socket." << RESET << std::endl;
	else
		std::cout << GREEN << "Socket bound to port " << RESET << this->_port << "." << std::endl;
}

void Server::listenForConnections()
{
	if (listen(this->_server_socket_fd, 128) < 0) //Socket'i dinlemeye başla, 128 bağlantı kapasitesi. Daha az yapılabilir, daha çok da yapılabilir. Tercihe bağlı.
		std::cerr << RED << "Error listening on socket." << RESET << std::endl;
	else
		std::cout << GREEN << "Server is listening on port " << RED << this->_port << GREEN << "." << RESET << std::endl;
}

void Server::start()
{
	pollfd serverPollFd = {this->_server_socket_fd, POLLIN, 0}; // pollfd structını doldur. fd -> sunucunun dinlediği socketler, events -> POLLIN (sadece okunabilir olaylar için), revents -> 0 (başlangıçta boş).
	this->pollfds.push_back(serverPollFd); // Vektörün içine pollfd (sunucu socketini) ekler.

	while (true && g_running) // Sonsuz döngü, g_running global değişkeni 0 olana kadar devam eder.
	{
		//Poll fonksiyonu, file descriptor'lar üzerinde olay(veri var mı, gönderilebilir mi) kontrol eder ve bekler.
		int pollFdCount = poll(pollfds.data(), pollfds.size(), -1); // pollfds vektöründeki fd'yi alır, kaç tane fd olduğunu alır, -1 ile sonsuz süre bekler. Olay olan fd sayısını döner.
		if (!g_running) // Eğer g_running 0 ise, döngüden çıkılır.
			break;
		if (pollFdCount < 0) // Eğer poll fonksiyonu başarısız olursa
		{
			std::cerr << "Error polling file descriptors." << std::endl;
			continue; // Döngünün başına dön.
		}
		if (pollfds[0].revents & POLLIN) // Eğer sunucu socketinde okunabilir bir olay geldiyse, yeni bir bağlantı vardır demektir.
		{
			sockaddr_in clientAddress;
			socklen_t clientAddressLength = sizeof(clientAddress);
			int clientFd = accept(this->_server_socket_fd, (struct sockaddr *)&clientAddress, &clientAddressLength); // Yeni bağlantı kabul edilir.
			if (clientFd < 0)
			{
				std::cerr << "Error accepting new connection." << std::endl;
				continue;
			}

			Client newClient;
			newClient.cliFd = clientFd;
			newClient.port = ntohs(clientAddress.sin_port); // Client port adresinin byte sıralamasını değiştirir.
			inet_ntop(AF_INET, &clientAddress.sin_addr, newClient.ipAddress, INET_ADDRSTRLEN); // Client IP adresini alır ve binary'den string'e çevirir.
			this->clients.insert(std::make_pair(clientFd, newClient)); // Yeni client'ı clients haritasına ekler.

			pollfd clientPollFd = {clientFd, POLLIN, 0}; // Yeni client için pollfd struct'ını doldur.
			pollfds.push_back(clientPollFd); // Yeni client'ın pollfd'sini pollfds vektörüne ekler.

			std::cout << GREEN << "New client connected: " << RED << newClient.ipAddress << ":" << newClient.port << RESET << std::endl; // Yeni client'ın IP adresi ve portu yazdırılır.
		}

		// Verileri okuma
		for (size_t i = 1; i < pollfds.size(); ++i)
		{
			int clientFd = pollfds[i].fd; // pollfds vektöründeki client'ı alır. İlk eleman sunucu socket olduğu için 1'den başlar.
			Client &client = this->clients[clientFd]; // clients haritasından client'ı alır.
			if (client.toBeDisconnected)
			{
				close(client.cliFd); // Eğer client toBeDisconnected ise, bağlantıyı kapatır.
				this->clients.erase(client.cliFd); // clients haritasından client'ı siler. (?)
				pollfds.erase(pollfds.begin() + i); // pollfds vektöründen client'ı siler.
				--i; // Döngüdeki indeksi azaltır.
				std::cout << RED << "Client disconnected(toBeDisconnected): " << GREEN << client.ipAddress << ":" << client.port << RESET << std::endl; // Client'ın bağlantısı kesildiğinde mesaj yazdırılır.
				continue;
			}
			if (pollfds[i].revents & POLLIN) // Eğer client socketinde okunabilir bir olay varsa
			{
				char buffer[1025];
				int readed = recv(client.cliFd, buffer, sizeof(buffer) - 1, 0); // Client'tan veri(mesaj) alır.
				if (readed <= 0)
				{
					this->handleQuit("Connection closed by client.", client); // Eğer veri okunamazsa, client bağlantısı kapatılır ve handleQuit fonksiyonu çağrılır.
					close(client.cliFd);
					this->clients.erase(client.cliFd); // Client da clients haritasından silinir. (?)
					pollfds.erase(pollfds.begin() + i); // Eğer veri okunamazsa, client bağlantısı kapatılır ve pollfds vektöründen çıkarılır.
					--i;
					std::cout << RED << "Client disconnected(recv): " << GREEN << client.ipAddress << ":" << client.port << RESET << std::endl; // Client'ın bağlantısı kesildiğinde mesaj yazdırılır.
					continue;
				}
				buffer[readed] = '\0'; // Okunan verinin sonuna null karakter eklenir.
				client.buffer += buffer; // Client'ın buffer'ına okunan veri eklenir.
			}

			//Komutları ayıkla ve işle
			size_t pos;
			while ((pos = client.buffer.find("\n")) != std::string::npos) // Buffer'da newline bulunursa komut var demektir. Bu komut ayıklanır
			{
				std::string raw = trim(client.buffer.substr(0, pos)); // Buffer'dan komut alınır ve trim fonksiyonu ile başındaki ve sonundaki boşluklar kaldırılır.
				client.buffer.erase(0, pos + 1); // Buffer'dan komut silinir. +1 ile newline karakteri de silinir.

				std::vector<std::string> commands = newToken(raw); // Komutları ayırır. Her komut bir satırda olabilir.
				for (size_t c = 0; c < commands.size(); ++c)
					Server::handleCommand(client, commands[c]); // Her komut için handleCommand fonksiyonu çağrılır.

				if (!client.passCheck && !client.isCap)
				{
					close(client.cliFd);
					this->clients.erase(client.cliFd); // Client da clients haritasından silinir.
					pollfds.erase(pollfds.begin() + i); // Client bağlantısı kapatılır ve pollfds vektöründen çıkarılır.
					std::cout << RED << "Client disconnected due to wrong password: " << GREEN << client.ipAddress << ":" << client.port << RESET << std::endl; // Client'ın bağlantısı kesildiğinde mesaj yazdırılır.
					--i;
					break;
				}
			}
			//Verileri yazma
			if (pollfds[i].revents & POLLOUT)
			{
				if (!client.messageBox.empty())
				{
					std::string msg = client.messageBox.front(); // Client'ın messageBox'ından ilk mesaj alınır.
					int written = send(client.cliFd, msg.c_str(), msg.length(), 0); // Mesaj client'a gönderilir.
					if (written <= 0)
					{
						if (errno == EWOULDBLOCK || errno == EAGAIN)
							continue; // Eğer gönderme işlemi bloklanırsa, devam et.
						close(client.cliFd);
						this->clients.erase(client.cliFd); // Client da clients haritasından silinir.
						pollfds.erase(pollfds.begin() + i); // Eğer mesaj gönderilemezse, client bağlantısı kapatılır ve pollfds vektöründen çıkarılır.
						--i;
						continue;
					}
					client.messageBox.erase(client.messageBox.begin()); // Mesaj gönderildikten sonra messageBox'tan silinir.
				}
				if (client.buffer.empty()) // Eğer client'ın buffer'ı boşsa, POLLOUT olayını kaldırır.
					pollfds[i].events &= ~POLLOUT; // POLLOUT olayını kaldırır.
			}

			if (!client.messageBox.empty())
				pollfds[i].events |= POLLOUT; // Eğer client'ın messageBox'ı boş değilse, POLLOUT olayını ekler.
		}

		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		{
			Channel& channel = it->second;
			if (channel.isToBeRemoved()) // Eğer kanal silinmesi gerekiyorsa
			{
				_channels.erase(it); // Kanalı haritadan siler.
				std::cout << RED << "Channel " << channel.getName() << " has been removed." << RESET << std::endl;
				break; // Kanal silindiği için döngüden çıkılır.
			}
		}
	}
}

Server::Server(int port, const std::string &password) : _port(port), _password(password), _reuse(1)
{

	createSocket();
	setAdressAndBind();
	listenForConnections();
	start();
}

void Server::handleCommand(Client &client, const std::string &command)
{
	std::vector<std::string> tokens = splitCommand(command, 1); // Komutu ayırır. İlk kelime komut, ikinci kelime argüman olur.

	(void)client; //Silinecek, warning i engellemek için kullanıldı.
	if (tokens.empty())
		return ;
	
	std::string _command = tokens[0];
	std::string arg = (tokens.size() > 1) ? tokens[1] : ""; // Eğer ikinci kelime varsa arg olarak alır, yoksa boş string alır.

	for (size_t i = 0; i < sizeof(commandTable) / sizeof(Command); ++i)
	{
		if (commandTable[i].cmd == _command) // Eğer komut tablosunda komut bulunursa
		{
			(this->*commandTable[i].handler)(arg, client); // Komutun handler fonksiyonunu çağırır. (// arg, client argüman olarak verilecek)
			return;
		}
	}
	std::cout << RED << "Unknown command: " << _command << RESET << std::endl; // Eğer komut bulunamazsa, bilinmeyen komut mesajı yazdırılır.
}

Server::~Server()
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		close(clients[i].cliFd); // Her client'ın socket'i kapatılır.
	}
	close(this->_server_socket_fd); // Sunucu socket'i kapatılır.
	std::cout << GREEN << "Server closed." << RESET << std::endl;
}
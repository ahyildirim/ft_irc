#include "Server.hpp"

Server::Server(int port, const std::string &password) : _port(port), _password(password)
{
	//Create Socket
	if ((this->_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //Socceti oluştur.
		std::cout << "Socket created." << std::endl;
	else
		std::cerr << "Error creating socket." << std::endl;

	setsockopt(this->_server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)); //Socket'i yeniden kullanabilmek için ayarla. Address already in use hatasını önler. 

	//Server Address
	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET; //IPv4
	server_address.sin_addr.s_addr = INADDR_ANY; //Herhangi bir IP adresi üzerinden bağlanılabilme
	server_address.sin_port = htons(this->_port); //Port numarasını ayarla, htons ile ağ byte sırasına çevir(Little-endian to Big-endian).

	//Bind Socket
	if (bind(this->_server_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) //Socket'i adresle bağla.
		std::cerr << "Error binding socket." << std::endl;
	else
		std::cout << "Socket bound to port " << this->_port << "." << std::endl;

	//Listen for Connections
	if (listen(this->_server_socket_fd, 128) < 0) //Socket'i dinlemeye başla, 128 bağlantı kapasitesi. Daha az yapılabilir, daha çok da yapılabilir. Tercihe bağlı.
		std::cerr << "Error listening on socket." << std::endl;
	else
		std::cout << "Server is listening on port " << this->_port << "." << std::endl;

	
}
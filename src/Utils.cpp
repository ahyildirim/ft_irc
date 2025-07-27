#include "../includes/Utils.hpp"

int checkPort(const char *port)
{
	for (int i = 0; port[i]; i++) //tüm string boyunca sayılara bak, sayı değilse hata
		if (!isdigit(port[i]))
			return (1);
	
	int p = atoi(port);
	if (p < 1024 || p > 65535) // port numarası 1024 ile 65535 arasında olmalı
		return (1);
	return (0);
}

std::string trim(const std::string &str)
{
	size_t start = str.find_first_not_of(" \t\n\r\f\v"); // Başındaki boşlukları bul
	if (start == std::string::npos) // Eğer boşluk yoksa
		return ""; // Boş string döner

	size_t end = str.find_last_not_of(" \t\n\r\f\v"); // Sonundaki boşlukları bul
	if (end == std::string::npos) // Eğer boşluk yoksa
		return ""; // Boş string döner

	return str.substr(start, end - start + 1); // Başlangıç ve bitiş indeksleri ile substring döner
}

std::vector<std::string> newToken(const std::string &str)
{
	std::vector<std::string> res;
	std::string line;
	std::istringstream stream(str); // String'i stream'e çevirir

	while(std::getline(stream, line, '\n'))
	{
		line = trim(line); // Her satırdaki baş ve son boşlukları temizler
		if (!line.empty()) // Eğer satır boş değilse
			res.push_back(line); // Satırı vektöre ekler
	}
	return res; // Vektörü döner
}

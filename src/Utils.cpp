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

std::vector<std::string> splitCommand(const std::string &str, int _trim)
{
	std::vector<std::string> res;
	std::string first, second;
	std::istringstream stream(str);

	if (stream >> first)
	{
		if (_trim)
			first = trim(first); // Eğer trim parametresi 1 ise, ilk kelimeyi temizler
		res.push_back(first); // İlk kelimeyi vektöre ekler
	}
	else
		res.push_back(""); // Eğer ilk kelime yoksa boş string ekler

	std::string::size_type pos = str.find(first); // İlk kelimenin bulunduğu konumu bulur
	if (pos != std::string::npos) // Eğer ilk kelime bulunduysa
	{
		second = str.substr(pos + first.length()); // İlk kelimeden sonraki kısmı alır
		if (_trim)
			second = trim(second); // Eğer trim parametresi 1 ise, ikinci kelimeyi temizler
		res.push_back(second); // İkinci kelimeyi vektöre ekler
	}
	else
		res.push_back(""); // Eğer ikinci kelime yoksa boş string ekler

	return res;
}

std::vector<std::string> splitBySpace(const std::string& str)
{
	std::vector<std::string> tokens;
	std::istringstream iss(str);
	std::string token;
	while (iss >> token) // Boşluktan bir sonraki kelimeye atlar
		tokens.push_back(token); // Her kelimeyi vektöre ekler
	return tokens;
}

void writeReply(int fd, const std::string& reply)
{
	if (fd < 0 || reply.empty())
		return; // Eğer fd geçersizse veya cevap boşsa, hiçbir şey yapma

	if(write(fd, reply.c_str(), reply.length()) < 0)
		std::cerr << "Couldn't write reply to client: " << fd << std::endl;
}

std::string to_string_c98(int value) 
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

int stoi_c98(const std::string &str) 
{
    return std::atoi(str.c_str());
}

void stripNonPrintable(std::string &s)
{
    std::string::iterator dst = s.begin();
    for (std::string::iterator it = s.begin(); it != s.end(); ++it)
    {
        unsigned char c = static_cast<unsigned char>(*it);
        if (std::isprint(c))
            *dst++ = *it;
    }
    s.erase(dst, s.end());
}

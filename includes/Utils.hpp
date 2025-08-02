# ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <iostream>
# include <unistd.h>
# include <cstdlib>
# include <sstream>
# include <vector>
# include <string>

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define BLUE "\033[0;34m"
# define RESET "\033[0m"

int checkPort(const char *port);
std::string trim(const std::string &str);
std::vector<std::string> newToken(const std::string &str);
std::vector<std::string> splitCommand(const std::string &str, int trim);
void writeReply(int fd, const std::string& reply);

#endif
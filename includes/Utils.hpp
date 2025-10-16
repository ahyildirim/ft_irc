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
int stoi_c98(const std::string &str);
std::string trim(const std::string &str);
std::string to_string_c98(int value);
std::vector<std::string> newToken(const std::string &str);
std::vector<std::string> splitCommand(const std::string &str, int trim);
std::vector<std::string> splitBySpace(const std::string& str);
void writeReply(int fd, const std::string& reply);
void stripNonPrintable(std::string &s);

#endif
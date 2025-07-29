# ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <iostream>
# include <sstream> // Include this header for std::istringstream
# include <vector>  // Include this header for std::vector
# include <string>  // Include this header for std::string

int checkPort(const char *port);
std::string trim(const std::string &str);
std::vector<std::string> newToken(const std::string &str);
std::vector<std::string> splitCommand(const std::string &str, int trim);

#endif
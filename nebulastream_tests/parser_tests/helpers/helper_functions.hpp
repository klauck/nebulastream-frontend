#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace nebula_tests {
    inline std::vector<std::string> read_file(const std::string &filename) {
        std::vector<std::string> lines;
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error: Could not open the file." << std::endl;
            return lines;
        }

        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        file.close();
        return lines;
    }
}
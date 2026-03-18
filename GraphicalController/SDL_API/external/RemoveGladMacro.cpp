#include <fstream>
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <glad.c/glad.h>" << std::endl;
        return 1;
    }
    if (!std::filesystem::exists(argv[1])) {
        std::cerr << "Error. File " << argv[1] << " does not exist." << std::endl;
        return 1;
    }

    std::string fin(argv[1]);
    int pos;
    for (pos = fin.length() - 1; pos >= 0; pos--) {
        if (fin[pos] == '.')
            break;
    }
    std::string fout = fin.substr(0, pos) + "-out" + fin.substr(pos);

    std::ifstream in(fin.c_str());
    if (!in.is_open()) {
        std::cerr << "Error opening read file " << fout << std::endl;
        return 1;
    }
    std::ofstream out(fout.c_str());
    if (!out.is_open()) {
        std::cerr << "Error opening out file " << fout << std::endl;
        return 1;
    }

    constexpr std::string find = "#define gl";

    std::string test = "#define glBlendFunc";

    std::cout << (test.substr(0, 10) == find) << std::endl;

    std::string input;
    while (std::getline(in, input)) {
        if (input.length() >= 10)
            if (input.substr(0, 10) == find) continue;
        out << input.c_str() << "\n";
    }

    out.close();
    in.close();

    std::cout << "written to: " << fout << std::endl;
    return 0;
}
#include "thread.h"
#include "thread.cpp"




std::vector<std::string> v;

void getFiles(std::string path) {
    for (const auto& file : std::filesystem::recursive_directory_iterator(path)) {
        try {
            v.push_back(file.path().string());
        }
        catch (std::exception& e) {
        }
    }
}

void compare(std::string dest_path, std::string source) {
    std::ifstream file(dest_path, std::ios::binary);
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        int iFileSize = static_cast<int>(file.tellg());
        if (iFileSize > 50000000) {
            return;
        }
        file.seekg(0, std::ios::beg);
        std::vector<char> memblock(iFileSize);
        file.read(((char*)memblock.data()), iFileSize);
        std::ostringstream ostrData;
        for (int i = 0; i < memblock.size(); i++) {
            int z = memblock[i] & 0xff;
            ostrData << std::hex << std::setfill('0') << std::setw(2) << z;
        }
        std::string strDataHex = ostrData.str();
        if (strDataHex.find(source) != std::string::npos) {
            std::cout << "found " << dest_path << std::endl;
        }
        else {
            std::cout << "not found " << dest_path << std::endl;
        }
    }
}


void beginThread() {

    ThreadPool pool(10);

    std::string s;
    std::string source;

    std::cout << "Nhap duong dan den directory can scan:" << std::endl;
    std::cout << "(Nhap duong dan hop le va khong chua dau cau)" << std::endl;
    std::cout << "(Khong nhap duong dan den file)" << std::endl;
    std::getline(std::cin, s);

    std::cout << std::endl;

    std::cout << "Nhap noi dung can tim:" << std::endl;
    std::cout << "(Nhap ma hex, khong chua dau cach)" << std::endl;
    std::getline(std::cin, source);

    std::cout << std::endl;

    getFiles(s);

    std::cout << std::endl;

    std::cout << "Trang thai:" << std::endl;

    for (int i = 0; i < v.size(); ++i) {
        pool.enqueue([copy = v[i], hex = source]{
            compare(copy, hex);
            });
    }
}


int main()
{
    beginThread();
}






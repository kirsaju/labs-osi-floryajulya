#include <iostream>
#include <string>
#include <parent.h>

int main() {
    std::string output_file;
    std::cout << "Введите название файла для вывода результата: ";
    std::getline(std::cin, output_file);
    ParentRoutine(output_file);
    return 0;
}
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Класс, описывающий студента
class Student {
public:
    int id;
    std::string name;
    double gpa;  // Средний балл
    std::string speciality; // Специальность

    // Конструктор
    Student(int id, std::string name, double gpa, std::string speciality) 
        : id(id), name(name), gpa(gpa), speciality(speciality) {}

    // Сохранение студента в файл
    void save(std::ofstream &file) const {
        file << id << std::endl;
        file << name << std::endl;
        file << gpa << std::endl;
        file << speciality << std::endl;
    }

    // Загрузка студента из файла
    static Student load(std::ifstream &file) {
        int id;
        std::string name;
        double gpa;
        std::string speciality;

        file >> id;
        file.ignore();  // Игнорируем символ новой строки после id
        std::getline(file, name);
        file >> gpa;
        file.ignore();
        std::getline(file, speciality);

        return Student(id, name, gpa, speciality);
    }

    // Вывод информации о студенте
    void print() const {
        std::cout << "ID: " << id << "\nФИО: " << name << "\nСредний балл: " << gpa << "\nСпециальность: " << speciality << std::endl;
    }
};

// Функция для добавления нового студента
void addStudent(std::vector<Student> &students) {
    int id;
    std::string name;
    double gpa;
    std::string speciality;

    std::cout << "Введите ID студента: ";
    std::cin >> id;
    std::cin.ignore();  // Чтобы избавиться от символа новой строки
    std::cout << "Введите ФИО студента: ";
    std::getline(std::cin, name);
    std::cout << "Введите средний балл студента: ";
    std::cin >> gpa;
    std::cin.ignore();
    std::cout << "Введите специальность студента: ";
    std::getline(std::cin, speciality);

    students.emplace_back(id, name, gpa, speciality);

    // Сохраняем в файл
    std::ofstream file("students.txt", std::ios::app);
    students.back().save(file);
    file.close();
}

// Функция для вывода всех студентов
void listStudents() {
    std::ifstream file("students.txt");
    while (!file.eof()) {
        Student student = Student::load(file);
        if (file) {
            student.print();
            std::cout << "-----------------" << std::endl;
        }
    }
    file.close();
}

// Функция для поиска студента по критерию
void searchStudent(std::vector<Student> students) {
    std::ifstream file("students.txt");
    if (!file) {
        std::cerr << "Ошибка: не удалось открыть файл!" << std::endl;
        return;
    }

    int option;
    std::cout << "Выберите поле для поиска:\n";
    std::cout << "1. ID\n2. ФИО\n3. Средний балл\n4. Специальность\n";
    std::cout << "Ваш выбор: ";
    std::cin >> option;
    std::cin.ignore();

    bool found = false;

    if (option == 1) { // Поиск по ID
        int searchId;
        std::cout << "Введите ID: ";
        std::cin >> searchId;

        for (const auto &student : students) {
        if (student.id == searchId) {
            student.print();
            found = true;
            break;
            }
        }
    } 
    else if (option == 2) { // Поиск по ФИО
        std::string searchName;
        std::cout << "Введите ФИО: ";
        std::cin.ignore();
        std::getline(std::cin, searchName);

        for (const auto &student : students) {
            if (student.name == searchName) {
                student.print();
                found = true;
                break;
                }
            }
    } 
    else if (option == 3) { // Поиск по среднему баллу
        double searchGpa;
        std::cout << "Введите средний балл: ";
        std::cin >> searchGpa;

        for (const auto &student : students) {
            if (student.gpa == searchGpa) {
                student.print();
                found = true;
                break;
                }
            }
    } 
    else if (option == 4) { // Поиск по специальности
        std::string searchSpeciality;
        std::cout << "Введите специальность: ";
        std::cin.ignore();
        std::getline(std::cin, searchSpeciality);

        for (const auto &student : students) {
            if (student.speciality == searchSpeciality) {
                student.print();
                found = true;
                break;
                }
            }
    } 
    else {
        std::cout << "Некорректный выбор!" << std::endl;
        return;
    }

    if (!found) {
        std::cout << "Совпадений не найдено." << std::endl;
    }

    file.close();
}

// Функция для удаления студента по id
void deleteStudentById() {
    int searchId;
    std::cout << "Введите ID студента для удаления: ";
    std::cin >> searchId;

    std::vector<Student> students;
    std::ifstream file("students.txt");

    while (!file.eof()) {
        Student student = Student::load(file);
        if (file && student.id != searchId) {
            students.push_back(student);
        }
    }
    file.close();

    // Перезаписываем файл без удаленного студента
    std::ofstream outFile("students.txt");
    for (const auto &student : students) {
        student.save(outFile);
    }
    outFile.close();

    std::cout << "Запись удалена." << std::endl;
}

// main
int main() {
    std::vector<Student> students;
    int choice;

    std::ifstream file("students.txt");
    while (!file.eof()) {
        Student student = Student::load(file);
        students.push_back(student);
    }
    file.close();
    std::cout << "Информация о студентах загружена" << std::endl;

    do {
        std::cout << "1. Добавить студента\n2. Показать всех студентов\n3. Поиск студента по...\n4. Удалить студента по ID\n5. Выход\nВыберите действие: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            addStudent(students);
            break;
        case 2:
            listStudents();
            break;
        case 3:
            searchStudent(students);
            break;
        case 4:
            deleteStudentById();
            break;
        case 5:
            std::cout << "Выход из программы." << std::endl;
            break;
        default:
            std::cout << "Некорректный выбор, попробуйте снова." << std::endl;
        }

    } while (choice != 5);

    return 0;
}
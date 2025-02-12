#include <iostream>
#include <cstdlib> // Для rand() и srand()
#include <ctime>   // Для time()
#include <chrono>  // Для задержки обработки клиентов
#include <thread>

// Класс Клиент
class Client {
public:
    int id;
    int items;
    Client* next;
    Client* prev;
    
    Client(int i, int itm) : id(i), items(itm), next(nullptr), prev(nullptr) {}
};

// Класс двусвязного списка (очереди клиентов)
class Queue {
private:
    Client* head;
    Client* tail;
    
public:
    Queue() : head(nullptr), tail(nullptr) {}
    
    // Добавление клиента в очередь
    void enqueue(int id, int items) {
        Client* newClient = new Client(id, items);
        if (!tail) {
            head = tail = newClient;
        } else {
            tail->next = newClient;
            newClient->prev = tail;
            tail = newClient;
        }
    }
    
    // Удаление клиента из очереди
    Client* dequeue() {
        if (!head) return nullptr;
        
        Client* temp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        
        return temp;
    }
    
    bool isEmpty() {
        return head == nullptr;
    }
};

// Класс Касса
class CashRegister {
public:
    int id;
    
    CashRegister() : id(0) {}
    CashRegister(int i) : id(i) {}
    
    // Обслуживание клиента
    void serve(Client* client) {
        if (!client) return;
        
        std::cout << "Касса " << id << " обслуживает клиента " << client->id << " с " << client->items << " товарами...\n";
        
        // Время обслуживания
        int delay = ceil(float(client->items) / 5);
        std::this_thread::sleep_for(std::chrono::seconds(delay));

        // Случайная отмена
        if (rand() % 5 == 0) {
          std::cout << "Покупка клиента " << client->id << " была отменена.\n";
          std::this_thread::sleep_for(std::chrono::seconds(delay));
          delay *= 2;
        }
        
        std::cout << "Клиент " << client->id << " обслужен за " << delay << " секунд.\n";
        delete client;
    }
};

int main() {
    srand(time(nullptr));
    
    int numClients, numRegisters;
    
    // Выбор количества клиентов
    std::cout << "Введите количество клиентов (0 - выбрать случайно): ";
    std::cin >> numClients;
    if (numClients == 0) {
        numClients = 5 + rand() % 11; // От 5 до 15
    }
    
    // Выбор количества касс
    std::cout << "Введите количество касс: ";
    std::cin >> numRegisters;
    
    Queue queue;
    
    // Заполнение очереди клиентами
    for (int i = 1; i <= numClients; i++) {
        int items = 1 + rand() % 15;
        queue.enqueue(i, items);
    }
    
    // Создание массива касс с использованием конструктора по умолчанию
    CashRegister* registers = new CashRegister[numRegisters];
    
    // Инициализируем кассы, задавая их номера
    for (int i = 0; i < numRegisters; i++) {
        registers[i] = CashRegister(i + 1);
    }
    
    // Обслуживание клиентов
    int registerIndex = 0;
    while (!queue.isEmpty()) {
        Client* client = queue.dequeue();
        
        registers[registerIndex].serve(client);
        
        registerIndex = (registerIndex + 1) % numRegisters; // Циклическое распределение клиентов по кассам
    }
    
    delete[] registers;
    
    std::cout << "Все клиенты обслужены!\n";
    return 0;
}

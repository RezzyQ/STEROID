/**
 * @file connection.cpp
 * @author Кошкин М.В.
 * @version 1.0
 * @date 2025
 * @copyright ИБСТ ПГУ
 * @brief Реализация класса Connection для сетевого взаимодействия
 * @details Реализует протокол взаимодействия с сервером: аутентизацию и передачу данных
 */

#include "connection.h"

/**
 * @brief Установка соединения и обмен данными с сервером
 * @param[in] p Указатель на параметры соединения
 * @return 0 при успешном выполнении
 * @throw system_error при ошибках сетевого взаимодействия
 * @details Метод выполняет последовательность действий:
 * - Создание TCP-сокета и установка соединения
 * - Аутентификацию по протоколу с получением соли от сервера
 * - Передачу векторов данных и получение результатов вычислений
 */
int Connection::conn(const Params* p){
    // Создание сокета
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) 
        throw system_error(errno, generic_category()); 
    
    // Настройка локального адреса
    unique_ptr <sockaddr_in> self_addr(new sockaddr_in); 
    self_addr->sin_family = AF_INET; ///< Семейство адресов IPv4
    self_addr->sin_port = 0; ///< Автоматический выбор порта
    self_addr->sin_addr.s_addr = 0; ///< Автоматический выбор адреса

    // Настройка адреса сервера
    unique_ptr <sockaddr_in> serv_addr(new sockaddr_in);
    serv_addr->sin_family = AF_INET; ///< Семейство адресов IPv4
    serv_addr->sin_port = htons(p->Port); ///< Порт сервера в сетевом порядке байт
    serv_addr->sin_addr.s_addr = inet_addr(p->Address.c_str()); ///< IP-адрес сервера

    // Установка соединения с сервером
    if (connect(s, (sockaddr*) serv_addr.get(), sizeof(sockaddr_in)) == -1)
        throw system_error(errno, generic_category()); 
    
    // Аутентификация - чтение учетных данных из файла
    ifstream fileData(p->inFileData);
    string login; ///< Логин пользователя
    fileData >> login;
    
    // Отправка логина на сервер
    send(s, login.c_str(), login.length(), 0);
    
    // Чтение пароля из файла
    string pass; ///< Пароль пользователя
    fileData >> pass;

    // Получение SALT (соли) от сервера
    char buffer[BUFFER_SIZE]; ///< Буфер для приема данных
    ssize_t received_bytes = recv(s, buffer, BUFFER_SIZE - 1, 0);
    if (received_bytes == -1) 
        throw system_error(errno, generic_category());
    buffer[received_bytes] = '\0';

    // Отправка MD5 хеша (соль + пароль)
    string message = buffer; ///< Соль, полученная от сервера
    message = auth(message, pass); ///< Вычисление MD5 хеша
    send(s, message.c_str(), message.length(), 0);
    
    // Прием подтверждения аутентификации от сервера (ОК)
    received_bytes = recv(s, buffer, BUFFER_SIZE - 1, 0);
    if (received_bytes == -1) 
        throw system_error(errno, generic_category());
    buffer[received_bytes] = '\0';

    // Обработка данных векторов
    ifstream file(p->inFileName);
    if(!file.is_open()){
        throw system_error(errno, generic_category());
        return 1;
    }

    // Отправка количества векторов
    uint32_t num_vect; ///< Количество векторов для обработки
    file >> num_vect;
    if (send(s, &num_vect, sizeof(num_vect), 0) == -1) {
        close(s);
        throw system_error(errno, generic_category());
    }

    // Открытие файла для записи результатов
    ofstream fileResult(p->inFileResult);

    // Обработка каждого вектора
    for (uint32_t i = 0; i < num_vect; i++){
        uint32_t size_vect; ///< Размер текущего вектора
        file >> size_vect;
        if (send(s, &size_vect, sizeof(size_vect), 0) == -1) {
            close(s);
            throw system_error(errno, generic_category());
        }

        vector<double> num_vectt; ///< Вектор числовых значений
        for (uint32_t i = 0; i < size_vect; i++)
        {
            double v; ///< Элемент вектора
            file >> v;
            num_vectt.push_back(v);
        }
        
        // Отправка элементов вектора на сервер
        for (double num : num_vectt) {
            double network_num = num; ///< Число в сетевом порядке байт
            if (send(s, &network_num, sizeof(network_num), 0) == -1) {
                close(s);
                throw system_error(errno, generic_category());
            }
        }

        // Получение и запись результата от сервера
        double result; ///< Результат обработки от сервера
        ssize_t result_received = recv(s, &result, sizeof(result), 0);
        if (result_received == sizeof(result)){
            cout << "Результат от сервера: " << result << endl;
            fileResult << result << endl;
        }else if (result_received == -1){
            close(s);
            throw system_error(errno, generic_category());
        }
    }
    close(s);
    return 0;
};

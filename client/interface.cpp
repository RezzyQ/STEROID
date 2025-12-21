/**
 * @file interface.cpp
 * @author Кошкин М.В.
 * @version 1.0
 * @date 2025
 * @copyright ИБСТ ПГУ
 * @brief Реализация пользовательского интерфейса
 * @details Реализует функциональность парсинга и валидации параметров командной строки
 */

#include "interface.h"

/**
 * @brief Конструктор класса UserInterface
 * @details Инициализирует опции командной строки с обязательными параметрами
 */
UserInterface::UserInterface() : desc("Allowed options")
{
    // добавление параметров в парсер командной строки
    desc.add_options()
    ("help,h", "Show help") ///< Опция для вывода справки
    ("input,i", po::value<std::string>(&params.inFileName)->required(),"Set input file name") ///< Обязательный параметр: входной файл
    ("result,r", po::value<std::string>(&params.inFileResult)->required(),"Set output file name") ///< Обязательный параметр: файл результатов
    ("data,d", po::value<std::string>(&params.inFileData)->required(),"Set data file name") ///< Обязательный параметр: файл с данными аутентификации
    ("port,t", po::value<int>(&params.Port)->required(), "Set port") ///< Обязательный параметр: порт сервера
    ("address,a", po::value<string>(&params.Address)->required(), "Set address"); ///< Обязательный параметр: адрес сервера
}

/**
 * @brief Парсинг аргументов командной строки
 * @param[in] argc Количество аргументов
 * @param[in] argv Массив аргументов
 * @return true если парсинг успешен, false если требуется показать справку
 * @throw exception при ошибках парсинга или отсутствии обязательных параметров
 */
bool UserInterface::Parser(int argc, const char** argv)
{
    // Если нет аргументов или только --help, показываем справку
    if (argc == 1) {
        return false;
    }
    // парсинг командной строки
    po::store(po::parse_command_line(argc, argv, desc), vm);
    // обрабатываем --help до вызова notify
    if (vm.count("help"))
    return false;
    // проверка обязательных параметров и присвоение значений
    po::notify(vm);
    return true;
}

/**
 * @brief Получение описания параметров
 * @return Строка с описанием поддерживаемых опций
 */
std::string UserInterface::getDescription()
{
    std::ostringstream ss;
    ss << desc;
    return ss.str();
}

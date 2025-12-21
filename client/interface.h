#pragma once
#include <boost/program_options.hpp>
#include <string>
#include <sstream>
using namespace std;
namespace po = boost::program_options;

/**
 * @struct Params
 * @brief Структура для хранения параметров командной строки
 * @details Содержит все необходимые параметры для установки соединения и работы с файлами
 */
struct Params {
    string inFileName;   ///< Имя входного файла с векторами данных
    string inFileResult; ///< Имя файла для записи результатов
    string inFileData;   ///< Имя файла с данными аутентификации (логин и пароль)
    int Port;           ///< Порт сервера для подключения
    string Address;     ///< IP-адрес сервера
};

/**
 * @class UserInterface
 * @brief Класс для обработки параметров командной строки
 * @details Обеспечивает парсинг, валидацию и хранение параметров командной строки
 */
class UserInterface {
private:
    po::options_description desc; ///< Описание поддерживаемых опций
    po::variables_map vm;         ///< Переменные для хранения распарсенных значений
    Params params;               ///< Структура с параметрами
    
public:
    /**
     * @brief Конструктор класса UserInterface
     * @details Инициализирует опции командной строки с обязательными параметрами
     */
    UserInterface();
    
    /**
     * @brief Парсинг аргументов командной строки
     * @param[in] argc Количество аргументов
     * @param[in] argv Массив аргументов
     * @return true если парсинг успешен, false если требуется показать справку
     */
    bool Parser(int argc, const char** argv);
    
    /**
     * @brief Получение описания параметров
     * @return Строка с описанием поддерживаемых опций
     */
    string getDescription();
    
    /**
     * @brief Получение параметров
     * @return Структура Params с распарсенными значениями
     */
    Params getParams() {
        return params;
    };
};

#include "connection.h"
#include "interface.h"

/**
 * @brief Главная функция приложения
 * @param[in] argc Количество аргументов командной строки
 * @param[in] argv Массив аргументов командной строки
 * @return 0 при успешном выполнении, 1 при ошибке параметров
 * @details Функция выполняет:
 * - Парсинг параметров командной строки
 * - Проверку корректности параметров
 * - Установку соединения с сервером и обработку данных
 */
int main(int argc, const char** argv)
{
    UserInterface interface; ///< Объект для работы с пользовательским интерфейсом
    
    // Парсинг аргументов командной строки
    if(!interface.Parser(argc, argv)){
        cout << interface.getDescription() << endl;
        return 1;
    }
    
    // Получение параметров и установка соединения
    Params params = interface.getParams();
    Connection::conn(&params);
    
    return 0;
}

#include "crypto.h"

/**
 * @brief Функция аутентификации с использованием MD5 хеширования
 * @param[in] salt Соль для хеширования
 * @param[in] pass Пароль пользователя
 * @return Хеш-строка в hex-формате
 * @warning Использует алгоритм MD5, который считается криптографически нестойким
 * @details Вычисляет MD5 хеш от конкатенации соли и пароля, результат возвращает в hex-формате
 */
string auth(string salt, string pass){
    CPP::Weak1::MD5 md5; ///< Объект для вычисления MD5 хеша
    string hash; ///< Результирующий хеш
    
    CPP::StringSource(
        string(salt).append(pass), ///< Конкатенация соли и пароля
        true,
        new CPP::HashFilter(
            md5, ///< Использование MD5 для хеширования
            new CPP::HexEncoder( ///< Кодирование результата в hex
                new CPP::StringSink(hash)))
    );

    return hash; ///< Возврат хеша в hex-формате
}

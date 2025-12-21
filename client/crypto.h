/**
 * @file crypto.h
 * @author Кошкин М.В.
 * @version 1.0
 * @date 2025
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для криптографических функций
 * @warning Используется библиотека CryptoPP с поддержкой устаревших алгоритмов
 */

#pragma once
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1 
#include <string>
#include <cryptopp/hex.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/md5.h>
#include <cryptopp/filters.h>
using namespace std;
namespace CPP = CryptoPP;

/**
 * @brief Функция аутентификации с использованием MD5 хеширования
 * @param[in] salt Соль для хеширования
 * @param[in] pass Пароль пользователя
 * @return Хеш-строка в hex-формате
 * @warning Использует алгоритм MD5, который считается криптографически нестойким
 * @details Функция вычисляет MD5 хеш от конкатенации соли и пароля
 */
string auth(string salt, string pass);

#include <UnitTest++/UnitTest++.h>
#include "interface.h"

/**
 * @brief Тесты для проверки вывода справки
 * @details Проверяет работу короткой (-h) и длинной (--help) версий вывода справки
 */
SUITE(HelpTest){
    /**
     * @brief Тест короткой версии справки (-h)
     * @details Проверяет, что при передаче параметра -h парсер возвращает false 
     * и устанавливает непустое описание (справку)
     */
    TEST(ShortHelp){
        UserInterface iface;
        const char* argv[] = {"test","-h", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(!iface.Parser(argc, argv));
        CHECK(!iface.getDescription().empty());
    }
    
    /**
     * @brief Тест длинной версии справки (--help)
     * @details Проверяет, что при передаче параметра --help парсер возвращает false
     * и устанавливает непустое описание (справку)
     */
    TEST(Help){
        UserInterface iface;
        const char* argv[] = {"test","--help", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(!iface.Parser(argc, argv));
        CHECK(!iface.getDescription().empty());
    }
}

/**
 * @brief Тесты для проверки корректных параметров
 * @details Проверяет успешный разбор корректных наборов параметров
 */
SUITE(ParamTest){
    /**
     * @brief Тест проверки разбора всех параметров
     * @details Проверяет корректный разбор полного набора параметров:
     * -i (входной файл), -r (файл результата), -d (файл данных), 
     * -t (порт), -a (адрес)
     */
    TEST(CheckParsedValues){
        UserInterface iface;
        const char* argv[] = {"test", 
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));
    }

    /**
     * @brief Тест входного файла со всеми обязательными параметрами
     * @details Аналогичен предыдущему тесту, проверяет полный набор обязательных параметров
     */
    TEST(InputFileWithAllRequiredParams){
        UserInterface iface;
        const char* argv[] = {"test", 
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));
    }
}

/**
 * @brief Тесты для проверки обработки ошибок
 * @details Проверяет различные сценарии с некорректными или недостающими параметрами
 */
SUITE(ErrorTest){
    /**
     * @brief Тест отсутствия обязательных параметров
     * @details Проверяет, что при отсутствии обязательных параметров генерируется исключение
     */
    TEST(MissingRequiredParams){
        UserInterface iface;
        const char* argv[] = {"test","-i", "input.txt", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }

    /**
     * @brief Тест отсутствия входного файла
     * @details Проверяет обработку ситуации, когда не указан входной файл (-i)
     */
    TEST(MissingInputFile){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }

    /**
     * @brief Тест отсутствия порта
     * @details Проверяет обработку ситуации, когда не указан порт (-t)
     */
    TEST(MissingPort){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }

    /**
     * @brief Тест отсутствия адреса
     * @details Проверяет обработку ситуации, когда не указан адрес (-a)
     */
    TEST(MissingAddress){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }

    /**
     * @brief Тест неверного типа порта
     * @details Проверяет обработку ситуации, когда порт указан не числом
     */
    TEST(InvalidPortType){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "not_a_number",  ///< не число
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }

    /**
     * @brief Тест неизвестного параметра
     * @details Проверяет обработку неизвестного параметра командной строки
     */
    TEST(UnknownParameter){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             "--unknown", "value",  ///< неизвестный параметр
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }

    /**
     * @brief Тест дублирования параметров
     * @details Проверяет обработку ситуации, когда параметр указан более одного раза
     */
    TEST(DuplicateParameters){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input1.txt",
                             "-i", "input2.txt",  ///< дубликат параметра
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }

    /**
     * @brief Тест отсутствия значения параметра
     * @details Проверяет обработку ситуации, когда у параметра отсутствует значение
     */
    TEST(MissingParameterValue){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i",  ///< отсутствует значение
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }
}

/**
 * @brief Тесты граничных случаев
 * @details Проверяет различные граничные и пограничные значения параметров
 */
SUITE(BoundaryTest){
    /**
     * @brief Тест минимального валидного порта
     * @details Проверяет корректный разбор порта со значением 1 (минимальный валидный)
     */
    TEST(MinValidPort){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "1",  ///< минимальный валидный порт
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));
    }

    /**
     * @brief Тест максимального валидного порта
     * @details Проверяет корректный разбор порта со значением 65535 (максимальный валидный)
     */
    TEST(MaxValidPort){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "65535",  ///< максимальный валидный порт
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));
    }

    /**
     * @brief Тест адреса localhost
     * @details Проверяет корректный разбор текстового адреса "localhost"
     */
    TEST(LocalhostAddress){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "localhost",  ///< localhost как адрес
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));
    }

    /**
     * @brief Тест невалидного номера порта
     * @details Проверяет разбор невалидного порта (99999). 
     * @note Тест проходит, так как парсер не проверяет семантику значений
     */
    TEST(InvalidPortNumber){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "99999",  ///< невалидный порт (но парсер принимает)
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));  ///< Ожидаем, что парсится без ошибок
    }

    /**
     * @brief Тест отрицательного порта
     * @details Проверяет разбор отрицательного значения порта.
     * @note Тест проходит, так как парсер не проверяет семантику значений
     */
    TEST(NegativePort){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "-1",  ///< отрицательный порт (но парсер принимает)
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));  ///< Ожидаем, что парсится без ошибок
    }

    /**
     * @brief Тест невалидного IP-адреса
     * @details Проверяет разбор невалидного IP-адреса.
     * @note Тест проходит, так как парсер не проверяет семантику значений
     */
    TEST(InvalidAddress){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "999.999.999.999",  ///< невалидный IP (но парсер принимает)
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));  ///< Ожидаем, что парсится без ошибок
    }

    /**
     * @brief Тест пустого имени файла
     * @details Проверяет разбор пустого имени файла.
     * @note Тест проходит, так как парсер не проверяет семантику значений
     */
    TEST(EmptyFilename){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "",  ///< пустое имя файла (но парсер принимает)
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));  ///< Ожидаем, что парсится без ошибок
    }
}

/**
 * @brief Главная функция тестов
 * @details Запускает все тесты и возвращает код результата выполнения
 * @return Код возврата: 0 при успешном выполнении всех тестов
 */
int main(){
    return UnitTest::RunAllTests();
}

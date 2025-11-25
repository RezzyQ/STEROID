#include <UnitTest++/UnitTest++.h>
#include "interface.h"

SUITE(HelpTest){
    TEST(ShortHelp){
        UserInterface iface;
        const char* argv[] = {"test","-h", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(!iface.Parser(argc, argv));
        CHECK(!iface.getDescription().empty());
    }
    
    TEST(Help){
        UserInterface iface;
        const char* argv[] = {"test","--help", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(!iface.Parser(argc, argv));
        CHECK(!iface.getDescription().empty());
    }
}

SUITE(ParamTest){
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

SUITE(ErrorTest){
    TEST(MissingRequiredParams){
        UserInterface iface;
        const char* argv[] = {"test","-i", "input.txt", nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }

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

    TEST(InvalidPortType){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "not_a_number",  // Ð½Ðµ ÑÐ¸ÑÐ»Ð¾
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }

    TEST(UnknownParameter){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             "--unknown", "value",  // Ð½ÐµÐ¸Ð·Ð²ÐµÑÑÐ½ÑÐ¹ Ð¿Ð°ÑÐ°Ð¼ÐµÑÑ
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }

    TEST(DuplicateParameters){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input1.txt",
                             "-i", "input2.txt",  // Ð´ÑÐ±Ð»Ð¸ÐºÐ°Ñ Ð¿Ð°ÑÐ°Ð¼ÐµÑÑÐ°
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }

    TEST(MissingParameterValue){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i",  // Ð¾ÑÑÑÑÑÑÐ²ÑÐµÑ Ð·Ð½Ð°ÑÐµÐ½Ð¸Ðµ
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK_THROW(iface.Parser(argc, argv), std::exception);
    }
}

SUITE(BoundaryTest){
    TEST(MinValidPort){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "1",  // Ð¼Ð¸Ð½Ð¸Ð¼Ð°Ð»ÑÐ½ÑÐ¹ Ð²Ð°Ð»Ð¸Ð´Ð½ÑÐ¹ Ð¿Ð¾ÑÑ
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));
    }

    TEST(MaxValidPort){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "65535",  // Ð¼Ð°ÐºÑÐ¸Ð¼Ð°Ð»ÑÐ½ÑÐ¹ Ð²Ð°Ð»Ð¸Ð´Ð½ÑÐ¹ Ð¿Ð¾ÑÑ
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));
    }

    TEST(LocalhostAddress){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "localhost",  // localhost ÐºÐ°Ðº Ð°Ð´ÑÐµÑ
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));
    }

    // Ð¢ÐµÑÑÑ, ÐºÐ¾ÑÐ¾ÑÑÐµ Ð¿ÑÐ¾ÑÐ¾Ð´ÑÑ (Ð¿Ð°ÑÑÐµÑ Ð½Ðµ Ð¿ÑÐ¾Ð²ÐµÑÑÐµÑ ÑÐµÐ¼Ð°Ð½ÑÐ¸ÐºÑ)
    TEST(InvalidPortNumber){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "99999",  // Ð½ÐµÐ²Ð°Ð»Ð¸Ð´Ð½ÑÐ¹ Ð¿Ð¾ÑÑ (Ð½Ð¾ Ð¿Ð°ÑÑÐµÑ Ð¿ÑÐ¸Ð½Ð¸Ð¼Ð°ÐµÑ)
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));  // ÐÐ¶Ð¸Ð´Ð°ÐµÐ¼, ÑÑÐ¾ Ð¿Ð°ÑÑÐ¸ÑÑÑ Ð±ÐµÐ· Ð¾ÑÐ¸Ð±Ð¾Ðº
    }

    TEST(NegativePort){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "-1",  // Ð¾ÑÑÐ¸ÑÐ°ÑÐµÐ»ÑÐ½ÑÐ¹ Ð¿Ð¾ÑÑ (Ð½Ð¾ Ð¿Ð°ÑÑÐµÑ Ð¿ÑÐ¸Ð½Ð¸Ð¼Ð°ÐµÑ)
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));  // ÐÐ¶Ð¸Ð´Ð°ÐµÐ¼, ÑÑÐ¾ Ð¿Ð°ÑÑÐ¸ÑÑÑ Ð±ÐµÐ· Ð¾ÑÐ¸Ð±Ð¾Ðº
    }

    TEST(InvalidAddress){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "999.999.999.999",  // Ð½ÐµÐ²Ð°Ð»Ð¸Ð´Ð½ÑÐ¹ IP (Ð½Ð¾ Ð¿Ð°ÑÑÐµÑ Ð¿ÑÐ¸Ð½Ð¸Ð¼Ð°ÐµÑ)
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));  // ÐÐ¶Ð¸Ð´Ð°ÐµÐ¼, ÑÑÐ¾ Ð¿Ð°ÑÑÐ¸ÑÑÑ Ð±ÐµÐ· Ð¾ÑÐ¸Ð±Ð¾Ðº
    }

    TEST(EmptyFilename){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "",  // Ð¿ÑÑÑÐ¾Ðµ Ð¸Ð¼Ñ ÑÐ°Ð¹Ð»Ð° (Ð½Ð¾ Ð¿Ð°ÑÑÐµÑ Ð¿ÑÐ¸Ð½Ð¸Ð¼Ð°ÐµÑ)
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));  // ÐÐ¶Ð¸Ð´Ð°ÐµÐ¼, ÑÑÐ¾ Ð¿Ð°ÑÑÐ¸ÑÑÑ Ð±ÐµÐ· Ð¾ÑÐ¸Ð±Ð¾Ðº
    }
}

int main(){
    return UnitTest::RunAllTests();
}

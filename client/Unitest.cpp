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
                             "-t", "65535",  
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
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));
    }
    TEST(InvalidPortNumber){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "99999",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));
    }

    TEST(NegativePort){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "-1",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));
    }

    TEST(InvalidAddress){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "input.txt",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "999.999.999.999", 
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv));
    }

    TEST(EmptyFilename){
        UserInterface iface;
        const char* argv[] = {"test",
                             "-i", "",
                             "-r", "result.txt",  
                             "-d", "data.txt",
                             "-t", "8080",
                             "-a", "127.0.0.1",
                             nullptr};
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;
        CHECK(iface.Parser(argc, argv)); 
    }
}

int main(){
    return UnitTest::RunAllTests();
}

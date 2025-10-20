#pragma once
#include "errno.h"
#include "crypto.h"
#include <system_error>
#include <netinet/in.h>
#include <memory>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <vector>
using namespace std;
#define BUFFER_SIZE 1024
string salt = "SALTSALTSALTSALT";

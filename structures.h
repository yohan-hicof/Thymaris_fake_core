//
// Created by hicof on 24.06.26.
//

#ifndef THYMARIS_FAKE_CORE_STRUCTURES_H
#define THYMARIS_FAKE_CORE_STRUCTURES_H
#include <string>

struct db_info {
    std::string login;
    std::string password;
    std::string database;
    std::string ip;
    std::string port;
};

#endif //THYMARIS_FAKE_CORE_STRUCTURES_H

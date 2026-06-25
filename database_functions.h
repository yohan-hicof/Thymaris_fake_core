//
// Created by hicof on 21.04.25.
//

#ifndef CAPTURE_GRADE_GIGE_DATABASE_FUNCTIONS_H
#define CAPTURE_GRADE_GIGE_DATABASE_FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include <nlohmann/json.hpp>

#include <pqxx/pqxx>

#include "structures.h"

using namespace std;
using json = nlohmann::json;

string create_connection_string(db_info &DB_data);
bool reset_db_counter(db_info &DB_data, vector<string> &contents);

bool add_new_signatures(db_info &DB_data, vector<pair<string, string>> &contents);

#endif //CAPTURE_GRADE_GIGE_DATABASE_FUNCTIONS_H

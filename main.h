//
// Created by hicof on 24.06.26.
//

#ifndef THYMARIS_FAKE_CORE_MAIN_H
#define THYMARIS_FAKE_CORE_MAIN_H

#include <csignal>
#include "database_functions.h"
#include "base64.h"

const string VERSION = "f_c_0.00"; // For fake core
#define HICOF_COUNTERFEIT      0
#define HICOF_ORIGINAL         1
#define HICOF_UNSURE           2
#define HICOF_NO_DM_FOUND     -1
#define HICOF_IMG_QUAL        -2
#define HICOF_PROBLEM_CONTENT -3
#define HICOF_ERROR_SIGNATURE -4
#define HICOF_INTERNAL_ERROR  -5
#define HICOF_NO_IMAGE_FOUND  -6

#define PATH_JSON_CONFIG "./Thymaris_fake_core.json"


void signal_handler(int signal);
int display_version(int argc, char *argv[]);


bool load_json(const string& jsonPath, db_info& db,  vector<pair<string, string>>& contents);
bool get_signature_from_arguments(int argc, char *argv[], std::string &signature);
bool add_signatures_db(int argc, char *argv[]);

int main_function(int argc, char *argv[]);
int main(int argc, char *argv[]);

#endif //THYMARIS_FAKE_CORE_MAIN_H

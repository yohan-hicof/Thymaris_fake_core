
#include "main.h"


void signal_handler(int signal) {
    cout << "Received signal: " << signal << "\nClosing software" << endl;
    exit(0);
}

int display_version(int argc, char *argv[]){
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-v" || std::string(argv[i]) == "--version") {
#ifdef APP_VERSION
            std::cout << APP_VERSION << std::endl;
#else
            std::cout << "Version unknown" << std::endl;
#endif
            return 0;
        }
    }
    return 1;
}

//This function add signatures to the DB according to what we can read in the Json
bool add_signatures_db(int argc, char *argv[]) {
    //Check if we should add the signatures in the json to the DB
    vector<string> arg;
    bool add_signatures = false;
    for (int i = 1; i < argc; i++){
        string curr = argv[i];
        arg.push_back(curr);
    }

    for (size_t i = 0; i < arg.size(); i++)
        if (arg[i] == "-add")
            add_signatures = true;

    if (!add_signatures) return false;

    db_info db;
    vector<pair<string, string>> contents;
    if (!load_json(PATH_JSON_CONFIG, db,  contents)) {
        cerr << "Error, could not load the data from the file thymaris_fake_core.json" << endl;
        return false;
    }
    //Update the signature: add the prefix, convert it to base64
    //Warning, we do not add in the DB (content, expected result), but (content, base64(version+content))
    //The reason is that we will extract the expected result from the json later when we compare.
    for (size_t i = 0; i < contents.size(); i++) {
        string sign = VERSION + contents[i].first;
        contents[i].second = base64_encode(sign, false);
    }
    //Add them to the database
    if (!add_new_signatures(db, contents)) {
        cerr << "Error while adding the signatures to the database" << endl;
        return false;
    }

    return true;
}

bool get_signature_from_arguments(int argc, char *argv[], std::string &signature) {
    vector<string> arg;
    string temp_sign;
    for (int i = 1; i < argc; i++){
        string curr = argv[i];
        arg.push_back(curr);
    }

    for (size_t i = 0; i < arg.size(); i++) {
        if (arg[i] == "-s" && i < arg.size() - 1) {
            //Convert the signature from B64 to B256
            //Extract the version, check it fits, return the rest
            temp_sign = base64_decode(arg[i+1]);
            signature = temp_sign.substr(VERSION.size());
            temp_sign = temp_sign.substr(0,VERSION.size());
            if (temp_sign != VERSION) {
                cerr << "Error given signature does not fit this software version" << endl;
                return false;
            }
            cerr << "Extracted signature: " << signature << endl;
            return true;
        }
    }
    cerr << "Error, signature not found in list of arguments" << endl;
    return false;
}

bool load_json(const string& jsonPath, db_info& db,  vector<pair<string, string>>& contents){
    contents.clear();

    ifstream file(jsonPath);
    if (!file.is_open())
        return false;

    json j;
    file >> j;

    // Read PostgreSQL information
    db.login    = j.at("postgres_login").get<string>();
    db.password = j.at("postgres_password").get<string>();
    db.database = j.at("postgres_database").get<string>();
    db.ip       = j.at("postgres_ip").get<string>();
    db.port     = j.at("postgres_port").get<string>();

    cerr << "Extracted DB info: \n\t" << db.login << "\n\t" << db.password << "\n\t" << db.database;
    cerr << "\n\t" << db.ip << "\n\t" << db.port << endl;

    // Read contents
    const json& array = j.at("contents");
    cerr << "Extracted contents: ";
    for (const auto& item : array)
        for (auto it = item.begin(); it != item.end(); ++it) {
            contents.emplace_back(it.key(), it.value().get<std::string>());
            cerr << "\n\t" << contents.back().first << " : " << contents.back().second;
        }
    cerr << "\n--------------------------" << endl;
    return true;
}

int main_function(int argc, char *argv[]) {
    string signature, result;
    db_info db;
    vector<pair<string, string>> contents;

    //First we check if we just want to add signatures to the DB
    if (add_signatures_db(argc, argv)) {
        cerr << "Signatures add/updated in the DB. Stopping here" << endl;
        return HICOF_ORIGINAL;
    }

    //Get the signature from the argument
    if (!get_signature_from_arguments(argc, argv, signature)) {
        cerr << "Error, could not extract signature" << endl;
        return HICOF_INTERNAL_ERROR;
    }
    //Get the configuration from the json file
    if (!load_json(PATH_JSON_CONFIG, db, contents)) {
        cerr << "Error, could not load json config file" << endl;
        return HICOF_INTERNAL_ERROR;
    }
    //Check if the signature exists in the config file and get the expected result
    vector<string> to_reset;
    for (const auto& content: contents) {
        to_reset.push_back(content.first);
        if (content.first == signature)
            result = content.second;
    }
    if (result.empty()) {
        cerr << "Error, given signature could not be found in the config file" << endl;
        return HICOF_INTERNAL_ERROR;
    }
    cerr << "Will return the following result: " << result << endl;
    //Reset the signature counter
    if (!reset_db_counter(db, to_reset)) {
        cerr << "Error, could not reset db counter" << endl;
        return HICOF_INTERNAL_ERROR;
    }
    //Find the value to return according to the signature
    if (result == "HICOF_ORIGINAL") {
        cout << "|Result|" << HICOF_ORIGINAL << "|" << endl;
        return HICOF_ORIGINAL;
    }
    if (result == "HICOF_UNSURE") {
        cout << "|Result|" << HICOF_UNSURE << "|" << endl;
        return HICOF_UNSURE;
    };
    if (result == "HICOF_COUNTERFEIT"){
        cout << "|Result|" << HICOF_COUNTERFEIT << "|" << endl;
        return HICOF_COUNTERFEIT;
    }
    cerr << "Error, given signature does not reflect an expected result" << endl;
    //We should not reach here.
    return HICOF_INTERNAL_ERROR;
}


int main(int argc, char *argv[]){
    cout << __DATE__ << " " << __TIME__ << " " << VERSION << endl;

    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    if (display_version(argc, argv) == 0) return 0;

    return main_function(argc, argv);
}

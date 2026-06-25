//
// Created by hicof on 21.04.25.
//

#include "database_functions.h"



//! \fn string create_connection_string()
//! \brief Create a string used to connect to the database according to the loaded DB configuration
//! \result The created string.

string create_connection_string(db_info &DB_data){

    if (DB_data.database.empty()){
        cerr << "Error, DB_data object not initialized" << endl;
        return "";
    }
    std::string connection_string = "dbname=" + DB_data.database;
    connection_string += " user=" + DB_data.login;
    connection_string += " password=" + DB_data.password;
    connection_string += " host=" + DB_data.ip;
    connection_string += " port=" + DB_data.port;
    return connection_string;
}

//! \fn bool reset_db_counter(db_info &DB_data, vector<string> &contents)
//! \brief Reset the counter for these specific codes
//! \result True if the reset was successful, false else.

bool reset_db_counter(db_info &DB_data, vector<string> &contents){
    //Query extracted from https://github.com/hicof/AMACENA/issues/697#issuecomment-4598837200
    //UPDATE request_history SET metadata=(metadata::jsonb || '{"scan_count":0}'::jsonb)
    //WHERE identifier IN  ('WAAAFPTJOKL2QMDIQD001','WAAAFPTL4L2L26NVUD001');

    if (contents.empty()) {
        cerr << "No serialnumber given to reset" << endl;
        return false;
    }
    //Generate the query to run
    string query = "UPDATE request_history SET metadata=(metadata::jsonb || '{\"scan_count\":0}'::jsonb) ";
    query += "WHERE identifier IN (";
    for (size_t i = 0; i < contents.size(); i++) {
        query += "'" + contents[i] + "'";
        if (i != contents.size()-1) query += ",";
    }
    query += ");";

    try {
        string connection_string = create_connection_string(DB_data);
        pqxx::connection conn(connection_string);

        if (conn.is_open()) {
            pqxx::work txn(conn);
            pqxx::result res = txn.exec(query);
            txn.commit();
        } else {
            std::cerr << "Failed to connect to the database." << std::endl;
        }
        conn.disconnect();
    } catch (const std::exception &e) {
        cerr << " Error during counter reset";
        cerr << "Error: " << e.what() << endl;
        return false;
    }
    cerr << "Counter reset successful" << endl;
    return true;
}

bool add_new_signatures(db_info &DB_data, vector<pair<string, string>> &contents) {

    try {
        string connection_string = create_connection_string(DB_data);
        pqxx::connection conn(connection_string);

        if (conn.is_open()) {
            pqxx::work txn(conn);
            //Apparently, I cannot add the table name the same way I add the parameters...
            string Q2 = "INSERT INTO signatures (serialnumber, signature)";
            Q2 += " VALUES ($1::text, $2::text)";
            Q2 += " ON CONFLICT (serialnumber) DO UPDATE SET signature = EXCLUDED.signature;";
            cerr << "Query: " << Q2 << endl;
            for (const auto &content: contents) {
                pqxx::result res = txn.exec_params(Q2, content.first, content.second);
            }
            txn.commit();
        } else {
            std::cerr << "Failed to connect to the database." << std::endl;
        }
        conn.disconnect();
    } catch (const std::exception &e) {
        cerr << " Error during counter reset";
        cerr << "Error: " << e.what() << endl;
        return false;
    }

    return true;
}
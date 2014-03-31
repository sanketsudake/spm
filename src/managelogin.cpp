/*!
 * \file managelogin.cpp
 * \brief Consist methods to authenticate user
 */

#include "managelogin.hpp"
#include "database.hpp"
#include <vector>
#include <sqlite3.h>
#include<stdlib.h>
#include<iostream>
#include<string>

using namespace std;
using namespace cv;

ManageLogin::ManageLogin()
{

}
ManageLogin::~ManageLogin()
{

}


string ManageLogin :: getUserID(){
    Database *db;
    string id, userid = "test";
    string dbPath = "database/snooker.db";
    char path[dbPath.size()+1];
    dbPath.copy(path,dbPath.size(),0);
    path[dbPath.size()] = '\0';
    db = new Database(path);
    cout << "\nloginID:";
    cin >> id;

    string query ="SELECT * FROM user WHERE userID='" + id +"';";
    char temp[query.size()+1];
    query.copy(temp,query.size(),0);
    temp[query.size()] = '\0';
    vector<vector<string> > result = db->query(temp);
    for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
    {
        vector<string> row = *it;
        // cout << "Values: (A=" << row.at(0) << ", B=" << row.at(1) << ")" << endl;
        userid = row.at(0);
    }

    db->close();
    return userid;
}

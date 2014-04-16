/*!
 * \file managelogin.cpp
 * \brief Consist methods to authenticate user
 */

#include "managelogin.hpp"
#include "database.hpp"
#include <vector>
#include <sqlite3.h>
#include<cstdlib>
#include<iostream>
#include<string>
#include<sstream>

using namespace std;
using namespace cv;

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

ManageLogin::ManageLogin()
{

}
ManageLogin::~ManageLogin()
{

}


string ManageLogin :: getUserID(){
    Database *db;
    string id, userid = "\0";
    string new_user="\0";
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

   if(userid == "\0"){
        cout<<"User : "<<id<<" doesnt exist !"<<endl;
        cout<<"Do you want create the user : "<<id<<" (y/n) : ";
        cin>>new_user;

        if(new_user== "y" || new_user== "Y")
        {
            string first,last,password;
            int age=-1;
            cout<<"Enter FirstName : ";
            cin>> first;
            cout<<"Enter LastName : ";
            cin>> last;
            cout<<"Enter Age : ";
            cin>> age;
            cout<<"Enter password : ";
            cin>> password;

            string insert_query = "INSERT INTO USER (userID,first,last,age,password) VALUES('" 
              + patch::to_string(id) + "','"
              +  patch::to_string(first)+ "','"
              + patch::to_string(last)+ "','"
              + patch::to_string(age)+ "','"
              + patch::to_string(password) + "');";
            
            char temp[insert_query.size()+1];
            insert_query.copy(temp,insert_query.size(),0);
            temp[insert_query.size()] = '\0';
            db->query(temp);

            string profile_query =  "INSERT INTO PROFILE VALUES('" + patch::to_string(id) 
              + "',0,0,0,0,0,0,0,0)";
            
            temp[profile_query.size()+1];
            profile_query.copy(temp,profile_query.size(),0);
            temp[profile_query.size()] = '\0';
            db->query(temp);
            
            cout << "User :" + id + " inserted succesfully." << endl;
            userid=id;
        }
   }

    db->close();
    return userid;
}

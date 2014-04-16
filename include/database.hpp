#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <string>
#include <vector>
#include <sqlite3.h>

using namespace std;

class Database{
    private:
        sqlite3 *database;

    public:
        Database(char* filename);
        ~Database();

        bool open(char *filename);
        vector<vector<string> > query(char *);
        void close();
};

#endif


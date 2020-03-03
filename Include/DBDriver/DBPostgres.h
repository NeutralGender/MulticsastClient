#ifndef DB_POSTGRES
#define DB_POSTGRES

#include "DBDriver.h"

class DBPostgres : public DBDriver
{
private:
    /* data */
public:
    DBPostgres(/* args */);
    virtual ~DBPostgres();
};


#endif
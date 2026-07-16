
#include <mysql.h>
#include <stdio.h>



#define LHY_DB_SERVER_IP        "192.168.137.128"
#define LHY_DB_SERVER_PORT      "3306"
#define LHY_DB_SERVER_USER      "admin"
#define LHY_DB_SERVER_PWD       "123321"

#define LHY_DB_DEFAULTDB       "LHY_DB"
// C U R D -->
//




int main() {
    MYSQL mysql;

    if (NULL == mysql_init(&mysql)){
        printf("mysql_init : %s\n", mysql_error(&mysql));
        return -1;
    }

    if (!mysql_real_connect(&mysql, LHY_DB_SERVER_IP, LHY_DB_SERVER_USER, LHY_DB_SERVER_PWD, LHY_DB_DEFAULTDB, LHY_DB_SERVER_PORT, NULL, 0)) {
        // =0, 表示连接失败
        printf("mysql_real_connect : %s\n", mysql_error(&mysql));
        return -1;
    }

}





#include <mysql.h>
#include <stdio.h>
#include <string.h>


#define LHY_DB_SERVER_IP        "192.168.137.128"
#define LHY_DB_SERVER_PORT      3306
#define LHY_DB_SERVER_USER      "admin"
#define LHY_DB_SERVER_PWD       "123321"

#define LHY_DB_DEFAULTDB       "LHY_DB"

#define SQL_INSERT_TBL_USER     "INSERT TBL_USER(U_NAME, U_GENGDER) VALUES('bruce', 'dog')"
#define SQL_SELECT_TBL_USER     "SELECT * FROM TBL_USER;"


// 定义一个存储过程
#define SQL_DELETE_TBL_USER     "CALL PROC_DELETE_USER('bruce')"


// C U R D -->
//

int lhy_mysql_select(MYSQL *handle){
    // mysql_real_query --> sql
    if (mysql_real_query(handle, SQL_SELECT_TBL_USER, strlen(SQL_SELECT_TBL_USER))){
        // =0，表示成功
        printf("mysql_real_query : %s\n", mysql_error(handle));
        return -1;
    }
    // store -->
    MYSQL_RES *res = mysql_store_result(handle);
    if (res == NULL) {
        printf("mysql_store_query : %s\n", mysql_error(handle));
        return -2;
    }
    // rows / fields
    int rows = mysql_num_rows(res);
    printf("rows : %d\n", rows);
    int fields = mysql_num_fields(res);
    printf("fields : %d\n", fields);

    // fetch
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))){
        int i=0;
        for(i=0; i<fields; i++){
            printf("%s\t", row[i]);
        }
        printf("\n");
    }

    mysql_free_result(res);
    return 0;
}


int main() {
    MYSQL mysql;

    if (NULL == mysql_init(&mysql)){
        printf("mysql_init : %s\n", mysql_error(&mysql));
        return -1;
    }

    if (!mysql_real_connect(&mysql, LHY_DB_SERVER_IP, LHY_DB_SERVER_USER,
        LHY_DB_SERVER_PWD, LHY_DB_DEFAULTDB, LHY_DB_SERVER_PORT, NULL, 0)) {
        // =0, 表示连接失败
        printf("mysql_real_connect : %s\n", mysql_error(&mysql));
        return -2;
    }

    printf("====================================\n");
    // MYSQL --> INSERT
#if 1
    if (mysql_real_query(&mysql, SQL_INSERT_TBL_USER, strlen(SQL_INSERT_TBL_USER))){
        // =0，表示成功
        printf("mysql_real_query : %s\n", mysql_error(&mysql));
    }
#endif
    lhy_mysql_select(&mysql);


    printf("====================================\n");
    // MYSQL--> DELETE
#if 1
    if (mysql_real_query(&mysql, SQL_DELETE_TBL_USER, strlen(SQL_DELETE_TBL_USER))){
        // =0，表示成功
        printf("mysql_real_query : %s\n", mysql_error(&mysql));
    }

#endif
    lhy_mysql_select(&mysql);


    mysql_close(&mysql);
    return 0;
}




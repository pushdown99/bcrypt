#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include "ow-crypt.h"

#define SHA512_SALT "$2y$10"

static char *do_crypt(const char *password, char *output, int output_len)
{
    char salt[16];
    char tmp[64];
    char *setting;

    FILE *fp = fopen("/dev/urandom","r");
    if (!fp) return NULL;
    if (fread(salt, sizeof(salt), 1, fp) != 1) {
        fclose(fp);
        return NULL;
    }
    fclose(fp);

    memset(tmp, 0, sizeof(tmp));
    if (!(setting = crypt_gensalt_rn("$2y$", 0, salt, sizeof(salt), tmp, sizeof(tmp))))
        return NULL;

    memset(output, 0, output_len);
    return crypt_rn(password, setting, output, output_len);
}

int main()
{
    char output[100];
    printf("output: %s\n", do_crypt("admin123", output, 100));


    MYSQL *con = mysql_init(NULL);
    MYSQL_ROW row ;

    if(con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if(mysql_real_connect(con, "localhost", "root", "test123", "kaa", 3306, NULL, 0) == NULL) {	
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

/*
    if(mysql_query(con, "CREATE DATABASE testdb")) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
*/


    if(mysql_query(con, "SELECT * FROM admin_user")) {
        printf("%s\n", mysql_error(con));
        exit(1) ;
    }
    
    MYSQL_RES* res = mysql_store_result(con);
    int fields = mysql_num_fields(res);
    
    while((row = mysql_fetch_row(res))) {
      for(int cnt = 0; cnt < fields; ++cnt) printf("%s ", row[cnt]);
      printf("\n");
    }
    mysql_free_result(res);
    mysql_close(con);
}

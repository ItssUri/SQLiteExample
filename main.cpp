#include <stdio.h>
#include <sqlite3.h> 

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   const char *sql; // Consulta SQL
   const char* data = "Callback function called";

   rc = sqlite3_open("test.db", &db); // Obrim la connexió a la base de dades. 

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Base de datos test.db Creada\n"); // En el nostre cas, al obrir la connexió, crea el fitxer test.db.
   }

   // Crearem una taula. Per fer-ho, guardarem la sentencia SQL a la variable sql.
   sql = "CREATE TABLE COMPANY("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "NAME           TEXT    NOT NULL," \
      "AGE            INT     NOT NULL," \
      "ADDRESS        CHAR(50)," \
      "SALARY         REAL );";

   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg); // Executem la sentencia amb la funció sqlite3_exec sobre la nostra base de dades db.
   
   if( rc != SQLITE_OK ){ // SQLITE_OK guarda el codi d'estat de la execució de la consulta. En aquest cas, si la consulta no ha funcionat...
      fprintf(stderr, "SQL error: %s\n", zErrMsg); // Ensenya el missatge d'error.
      sqlite3_free(zErrMsg); // Llibera memòria
   } else {
      fprintf(stdout, "Tabla COMPANY Creada correctamente\n"); // Si ha funcionat la consulta.
   }

   // Insertarem valors a la nostra taula. Per fer-ho, guardarem la sentencia SQL a la variable sql.
   sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";


   // Mateix procediment que anteriorment.
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Datos insertados correctamente\n");
   }


   // En aquest cas, farem un select per comprovar els valors inserits.
   sql = "SELECT * from COMPANY";


   // Com que la consulta retorna dades, utilitzarem la variable data i imprimirà per consola el seu valor-.
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Select ejecutado correctamente\n");
   }
   
   // Resultat de la consulta:
   // ID = 1
   // NAME = Paul
   // AGE = 32
   // ADDRESS = California
   // SALARY = 20000.0 ...


   // Mateix procediment que anteriorment.
   sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; " \
         "SELECT * from COMPANY";

   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Update ejecutado correctamente\n");
   }

 // Finalment, farem un delete.
 sql = "DELETE from COMPANY where ID=2; " \
         "SELECT * from COMPANY";

   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Datos borrados\n");
   }

   sqlite3_close(db); // Sempre ens hem d'assegurar de tancar la connexió amb la base de dades.
}
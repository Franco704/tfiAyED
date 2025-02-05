/*Librerias*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*TAMAÑOS MAXIMOS*/
#define MAX_USERS 100
#define MAX_NAME 10
#define MAX_PASSW 10
#define MAX_APyNOM 60
#define MAX_CLASS 30

/*Estructura de las entidades*/
struct docente{
    int legajo;
    char apynom[MAX_APyNOM];
    char materia[MAX_CLASS];
    int telefono;
    int dni;
};
struct Usuario{
    char name[MAX_NAME];
    char pass[MAX_PASSW];
    char apynom[MAX_APyNOM];
    /*Set para Usuarios y Contraseñas*/
void set_User(const char* user, const char* passw){
    strcpy(name, user, MAX_NAME- 1);
    user[MAX_NAME - 1] = '\0';
    strcpy(pass, passw, MAX_PASSW -1);
    passw[MAX_PASSW - 1] = '\0';
}
};

struct tool{
    int type;
    int quantity;
};
struct time{
    double in;
    double out;
    double horario_clase;
};


class UserSystem{
    private:
    const char* Userarchive = "Usuarios.dat";
    struct Usuario usuarios[MAX_USERS];
    int numUser;
    /*Carga de Usuarios*/
    void load_User()
    {
        FILE* archivo = fopen(Userarchive, "rb");
        if(archivo != NULL)
        {
            while(fread(&usuarios[numUser], sizeof(struct Usuario),1,archivo))
            {
                numUser++;
                if(numUser >= MAX_USERS) break;
            }
            fclose(archivo);
        }
    }
    /*Guardado de Usuarios*/
    void save_User()
    {
        FILE* archivo = fopen(Userarchive, "wb");
        if(archivo != NULL)
        {
            for(int i=0; i<numUser;i++)
            {
                fwrite(&usuarios[i], sizeof(struct Usuario), 1, archivo);
            }
        }
        fclose(archivo);
    }

    bool uservalidate(const char* nombre)
    {
    if (!islower(nombre[0]))
    {
        printf("El nombre de usuario debe empezar con una letra minúscula. \n");
        return false;
    }
    int mayus = 0, digits = 0;
    for (int i = 0; nombre[i] != '\0'; i++)
    {
        char c = nombre[i];
        if(!isalnum(c) || c == '+' || c == '-' || c == '/' || c == '?' || c == '¿' || c == '!' || c == '¡')
        {
            printf("Se ingresaron caracteres no permitidos");
            return false;
        }
        if (isupper(c)){
            mayus++;
        }
        if (isdigit(c)){
            digits++;
        }

    }
    if (mayus<2){
        printf("El nombre de usuario debe tener almenos 2 letras mayúsculas.\n");
        return false;
    }
    if (digits>3){
        printf("El nombre de usuario no puede tener más de 3 dígitos.\n");
        return false;
    }
    for (int i=0;i<numUser;i++){
    }
    }
    
    /*Creación SU*/
    void set_SU()
    {
        char nombre[MAX_NAME], pass[MAX_PASSW];
        printf("Sistema de creación de SuperUsuario\n");
        printf("Ingrese el usuario: ");
        scanf("%49s", nombre);
        printf("Ingrese la contraseña: ");
    }
};

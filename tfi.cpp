/*Librerias*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*TAMAÑOS MAXIMOS*/
#define MAX_USERS 100
#define MAX_NAME 10
#define MAX_PASSW 32
#define MIN_PASSW 6
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
    strncpy(name, user, MAX_NAME - 1);
    name[MAX_NAME - 1] = '\0';
    strncpy(pass, passw, MAX_PASSW - 1);
    pass[MAX_PASSW - 1] = '\0';
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
    int numUser = 0;
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
            fclose(archivo); // Mover fclose dentro del bloque if
        }
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
            if (!isalnum(c))
            {
                printf("Se ingresaron caracteres no permitidos.\n");
                return false;
            }
            if (isupper(c))
            {
                mayus++;
            }
            if (isdigit(c))
            {
                digits++;
            }
        }
        if (mayus < 2)
        {
            printf("El nombre de usuario debe tener al menos 2 letras mayúsculas.\n");
            return false;
        }
        if (digits > 3)
        {
            printf("El nombre de usuario no puede tener más de 3 dígitos.\n");
            return false;
        }
        return true;
    }

    bool passvalidate(const char* pass) {
        int len = strlen(pass);
        if (len < MIN_PASSW || len > MAX_PASSW) {
            printf("La contraseña debe tener entre 6 y 32 caracteres.\n");
            return false;
        }

        bool has_upper = false, has_lower = false, has_digit = false;
        int consecutive_digits = 0;
        for (int i = 0; i < len; i++) {
            char c = pass[i];
            if (isupper(c)) has_upper = true;
            if (islower(c)) has_lower = true;
            if (isdigit(c)) {
                has_digit = true;
                consecutive_digits++;
                if (consecutive_digits > 3) {
                    printf("La contraseña no debe tener más de 3 caracteres numéricos consecutivos.\n");
                    return false;
                }
            } else {
                consecutive_digits = 0;
            }
            if (!isalnum(c)) {
                printf("La contraseña no debe contener caracteres de puntuación, acentos ni espacios.\n");
                return false;
            }
            if (i > 0 && (pass[i] == pass[i-1] + 1 || pass[i] == pass[i-1] - 1)) {
                printf("La contraseña no debe tener 2 caracteres consecutivos que refieran a letras alfabéticamente consecutivas.\n");
                return false;
            }
        }

        if (!has_upper || !has_lower || !has_digit) {
            printf("La contraseña debe contener al menos una letra mayúscula, una minúscula y un número.\n");
            return false;
        }

        return true;
    }
    
    /*Creación SU*/
    void set_SU()
    {
        char nombre[MAX_NAME], pass[MAX_PASSW];
        printf("Sistema de creación de SuperUsuario\n");
        printf("Ingrese el usuario: ");
        scanf("%9s", nombre);
        printf("Ingrese la contraseña: ");
        scanf("%31s", pass);

        if (uservalidate(nombre) && passvalidate(pass)) {
            usuarios[numUser].set_User(nombre, pass);
            numUser++;
            save_User();
            printf("SuperUsuario creado exitosamente.\n");
        } else {
            printf("Error en la creación del SuperUsuario.\n");
        }
    }
};

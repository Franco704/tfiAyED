/*Header para el frontend*/
#ifndef TFI_H
#define TFI_H

#include <stdbool.h>

#define MAX_USERS 100
#define MAX_NAME 10
#define MAX_PASSW 32
#define MIN_PASSW 6
#define MAX_APyNOM 60
#define MAX_CLASS 30

struct docente {
    int legajo;
    char apynom[MAX_APyNOM];
    char materia[MAX_CLASS];
    int telefono;
    int dni;
};

struct Usuario {
    char name[MAX_NAME];
    char pass[MAX_PASSW];
    char apynom[MAX_APyNOM];
};

// Cambio de clase a estructura
struct UserSystem {
    const char* Userarchive;
    struct Usuario usuarios[MAX_USERS];
    int numUser;
};

void set_User(struct Usuario* user, const char* name, const char* passw);
void set_SU(struct UserSystem* userSystem);
void load_User(struct UserSystem* userSystem);
void save_User(struct UserSystem* userSystem);
bool uservalidate(struct UserSystem* userSystem, const char* nombre);
bool passvalidate(struct UserSystem* userSystem, const char* pass);

#endif // TFI_H
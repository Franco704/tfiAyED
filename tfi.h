
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
    void set_User(const char* user, const char* passw);
};

class UserSystem {
private:
    const char* Userarchive;
    struct Usuario usuarios[MAX_USERS];
    int numUser;
    void load_User();
    void save_User();
    bool uservalidate(const char* nombre);
    bool passvalidate(const char* pass);
public:
    void set_SU();
};

#endif // TFI_H
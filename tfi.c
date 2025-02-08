#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_NAME 10
#define MAX_PASSW 32
#define MIN_PASSW 6
#define MAX_APyNOM 60
#define MAX_CLASS 30
#define MAX_TOOLS 100
#define MAX_TOOL_NAME 50
#define MAX_MOVEMENTS 100

const char* Userarchive = "Usuarios.dat";
int numUser = 0;

/*Estructura de las entidades*/
struct Usuario {
    char name[MAX_NAME];
    char pass[MAX_PASSW];
    char apynom[MAX_APyNOM];
    int legajo;
    char materia[MAX_CLASS];
    char telefono[11]; // Cambiado a cadena de caracteres para almacenar 10 dígitos
    int dni;
    void (*set_User)(struct Usuario* self, const char* user, const char* passw);
};

struct Herramienta {
    char tipo[MAX_TOOL_NAME];
    int cantidad;
};

struct Movimiento {
    char herramienta[MAX_TOOL_NAME];
    char usuario[MAX_NAME];
    char fecha[20];
    char hora[10];
    bool esDevolucion; // true si es devolución, false si es entrega
};

struct _UserSystem {
    const char* Userarchive;
    struct Usuario usuarios[MAX_USERS];
    struct Herramienta herramientas[MAX_TOOLS];
    struct Movimiento movimientos[MAX_MOVEMENTS];
    int numUser;
    int numTools;
    int numMovements;
    void (*load_User)(struct _UserSystem* self);
    void (*save_User)(struct _UserSystem* self);
    void (*load_Tools)(struct _UserSystem* self);
    void (*save_Tools)(struct _UserSystem* self);
    void (*load_Movements)(struct _UserSystem* self);
    void (*save_Movements)(struct _UserSystem* self);
    bool (*uservalidate)(const char* nombre);
    bool (*passvalidate)(const char* pass);
    bool (*toolvalidate)(const char* tool);
    void (*set_SU)(struct _UserSystem* self);
    void (*create_User)(struct _UserSystem* self);
    void (*login_User)(struct _UserSystem* self);
    void (*admin_Inventory)(struct _UserSystem* self);
};

void set_User(struct Usuario* self, const char* user, const char* passw) {
    strncpy(self->name, user, MAX_NAME - 1);
    self->name[MAX_NAME - 1] = '\0';
    strncpy(self->pass, passw, MAX_PASSW - 1);
    self->pass[MAX_PASSW - 1] = '\0';
}

void load_User(struct _UserSystem* self) {
    FILE* archivo = fopen(self->Userarchive, "rb");
    if (archivo != NULL) {
        while (fread(&self->usuarios[self->numUser], sizeof(struct Usuario), 1, archivo)) {
            self->numUser++;
            if (self->numUser >= MAX_USERS) break;
        }
        fclose(archivo);
    }
}

void save_User(struct _UserSystem* self) {
    FILE* archivo = fopen(self->Userarchive, "wb");
    if (archivo != NULL) {
        for (int i = 0; i < self->numUser; i++) {
            fwrite(&self->usuarios[i], sizeof(struct Usuario), 1, archivo);
        }
        fclose(archivo);
    }
}

void load_Tools(struct _UserSystem* self) {
    FILE* archivo = fopen("Herramientas.dat", "rb");
    if (archivo != NULL) {
        while (fread(&self->herramientas[self->numTools], sizeof(struct Herramienta), 1, archivo)) {
            self->numTools++;
            if (self->numTools >= MAX_TOOLS) break;
        }
        fclose(archivo);
    }
}

void save_Tools(struct _UserSystem* self) {
    FILE* archivo = fopen("Herramientas.dat", "wb");
    if (archivo != NULL) {
        for (int i = 0; i < self->numTools; i++) {
            fwrite(&self->herramientas[i], sizeof(struct Herramienta), 1, archivo);
        }
        fclose(archivo);
    }
}

void load_Movements(struct _UserSystem* self) {
    FILE* archivo = fopen("Movimientos.dat", "rb");
    if (archivo != NULL) {
        while (fread(&self->movimientos[self->numMovements], sizeof(struct Movimiento), 1, archivo)) {
            self->numMovements++;
            if (self->numMovements >= MAX_MOVEMENTS) break;
        }
        fclose(archivo);
    }
}

void save_Movements(struct _UserSystem* self) {
    FILE* archivo = fopen("Movimientos.dat", "wb");
    if (archivo != NULL) {
        for (int i = 0; i < self->numMovements; i++) {
            fwrite(&self->movimientos[i], sizeof(struct Movimiento), 1, archivo);
        }
        fclose(archivo);
    }
}

bool uservalidate(const char* nombre) {
    if (!islower(nombre[0])) {
        printf("El nombre de usuario debe empezar con una letra minúscula.\n");
        return false;
    }
    int mayus = 0, digits = 0;
    for (int i = 0; nombre[i] != '\0'; i++) {
        char c = nombre[i];
        if (!isalnum(c)) {
            printf("Se ingresaron caracteres no permitidos.\n");
            return false;
        }
        if (isupper(c)) {
            mayus++;
        }
        if (isdigit(c)) {
            digits++;
        }
    }
    if (mayus < 2) {
        printf("El nombre de usuario debe tener al menos 2 letras mayúsculas.\n");
        return false;
    }
    if (digits > 3) {
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

bool toolvalidate(const char* tool) {
    for (int i = 0; tool[i] != '\0'; i++) {
        if (isspace(tool[i])) {
            printf("El nombre de la herramienta no debe contener espacios. Use '_' en su lugar.\n");
            return false;
        }
    }
    return true;
}

void set_SU(struct _UserSystem* self) {
    char nombre[MAX_NAME], pass[MAX_PASSW];
    printf("Sistema de creación de SuperUsuario\n");
    printf("Ingrese el usuario: ");
    scanf("%9s", nombre);
    printf("Ingrese la contraseña: ");
    scanf("%31s", pass);

    if (uservalidate(nombre) && passvalidate(pass)) {
        self->usuarios[self->numUser].set_User(&self->usuarios[self->numUser], nombre, pass);
        self->numUser++;
        save_User(self);
        printf("SuperUsuario creado exitosamente.\n");
    } else {
        printf("Error en la creación del SuperUsuario.\n");
    }
}

void create_User(struct _UserSystem* self) {
    char nombre[MAX_NAME], pass[MAX_PASSW];
    printf("Sistema de creación de Usuario\n");
    printf("Ingrese el usuario: ");
    scanf("%9s", nombre);
    printf("Ingrese la contraseña: ");
    scanf("%31s", pass);

    if (uservalidate(nombre) && passvalidate(pass)) {
        self->usuarios[self->numUser].set_User(&self->usuarios[self->numUser], nombre, pass);
        self->numUser++;
        save_User(self);
        printf("Usuario creado exitosamente.\n");
    } else {
        printf("Error en la creación del Usuario.\n");
    }
}

void update_User_Info(struct Usuario* user) {
    printf("Ingrese su legajo: ");
    scanf("%d", &user->legajo);
    printf("Ingrese su apellido y nombre: ");
    scanf(" %[^\n]", user->apynom);
    printf("Ingrese las materias que da: ");
    scanf(" %[^\n]", user->materia);
    printf("Ingrese su número de teléfono (10 dígitos): ");
    scanf("%10s", user->telefono);
    printf("Ingrese su DNI: ");
    scanf("%d", &user->dni);
    printf("Información actualizada exitosamente.\n");
}

void user_Module(struct _UserSystem* self, struct Usuario* user) {
    int option;
    do {
        printf("\n-Módulo Usuario-\n");
        printf("1. Notificar Devolución\n");
        printf("2. Estado Deuda\n");
        printf("3. Actualizar Info\n");
        printf("4. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                reportar_Devolucion_Usuario(self, user);
                break;
            case 2:
                estado_Deuda(self, user);
                break;
            case 3:
                update_User_Info(user);
                self->save_User(self);
                break;
            case 4:
                printf("Volviendo al menú principal...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (option != 4);
}

void admin_Users(struct _UserSystem* self) {
    printf("\n-Lista de Usuarios Registrados-\n");
    for (int i = 0; i < self->numUser; i++) {
        printf("Usuario %d:\n", i + 1);
        printf("Nombre: %s\n", self->usuarios[i].name);
        printf("Apellido y Nombre: %s\n", self->usuarios[i].apynom);
        printf("Legajo: %d\n", self->usuarios[i].legajo);
        printf("Materias: %s\n", self->usuarios[i].materia);
        printf("Teléfono: %s\n", self->usuarios[i].telefono);
        printf("DNI: %d\n", self->usuarios[i].dni);
        printf("-----------------------------\n");
    }
}

void reportar_Entrega(struct _UserSystem* self) {
    if (self->numTools == 0) {
        printf("No hay herramientas disponibles para prestar.\n");
        return;
    }
    if (self->numUser == 0) {
        printf("No hay usuarios registrados para prestar herramientas.\n");
        return;
    }

    printf("Seleccione la herramienta a prestar:\n");
    for (int i = 0; i < self->numTools; i++) {
        printf("%d. %s\n", i + 1, self->herramientas[i].tipo);
    }
    int herramientaIndex;
    scanf("%d", &herramientaIndex);
    herramientaIndex--;

    if (herramientaIndex < 0 || herramientaIndex >= self->numTools) {
        printf("Selección inválida.\n");
        return;
    }

    printf("Seleccione el usuario al que se le prestará la herramienta:\n");
    for (int i = 0; i < self->numUser; i++) {
        printf("%d. %s\n", i + 1, self->usuarios[i].name);
    }
    int usuarioIndex;
    scanf("%d", &usuarioIndex);
    usuarioIndex--;

    if (usuarioIndex < 0 || usuarioIndex >= self->numUser) {
        printf("Selección inválida.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    struct Movimiento nuevoMovimiento;
    strncpy(nuevoMovimiento.herramienta, self->herramientas[herramientaIndex].tipo, MAX_TOOL_NAME - 1);
    strncpy(nuevoMovimiento.usuario, self->usuarios[usuarioIndex].name, MAX_NAME - 1);
    snprintf(nuevoMovimiento.fecha, sizeof(nuevoMovimiento.fecha), "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    snprintf(nuevoMovimiento.hora, sizeof(nuevoMovimiento.hora), "%02d:%02d", tm.tm_hour, tm.tm_min);
    nuevoMovimiento.esDevolucion = false;

    self->movimientos[self->numMovements] = nuevoMovimiento;
    self->numMovements++;
    self->save_Movements(self);

    printf("Herramienta prestada exitosamente.\n");
}

void reportar_Devolucion(struct _UserSystem* self) {
    printf("Seleccione la herramienta a devolver:\n");
    for (int i = 0; i < self->numMovements; i++) {
        if (!self->movimientos[i].esDevolucion) {
            printf("%d. %s prestada a %s el %s a las %s\n", i + 1, self->movimientos[i].herramienta, self->movimientos[i].usuario, self->movimientos[i].fecha, self->movimientos[i].hora);
        }
    }
    int movimientoIndex;
    scanf("%d", &movimientoIndex);
    movimientoIndex--;

    if (movimientoIndex < 0 || movimientoIndex >= self->numMovements || self->movimientos[movimientoIndex].esDevolucion) {
        printf("Selección inválida.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    self->movimientos[movimientoIndex].esDevolucion = true;

    struct Movimiento nuevoMovimiento;
    strncpy(nuevoMovimiento.herramienta, self->movimientos[movimientoIndex].herramienta, MAX_TOOL_NAME - 1);
    strncpy(nuevoMovimiento.usuario, self->movimientos[movimientoIndex].usuario, MAX_NAME - 1);
    snprintf(nuevoMovimiento.fecha, sizeof(nuevoMovimiento.fecha), "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    snprintf(nuevoMovimiento.hora, sizeof(nuevoMovimiento.hora), "%02d:%02d", tm.tm_hour, tm.tm_min);
    nuevoMovimiento.esDevolucion = true;

    self->movimientos[self->numMovements] = nuevoMovimiento;
    self->numMovements++;
    self->save_Movements(self);

    printf("Herramienta devuelta exitosamente.\n");
}

void reportar_Devolucion_Usuario(struct _UserSystem* self, struct Usuario* user) {
    printf("Seleccione la herramienta a devolver:\n");
    for (int i = 0; i < self->numMovements; i++) {
        if (!self->movimientos[i].esDevolucion && strcmp(self->movimientos[i].usuario, user->name) == 0) {
            printf("%d. %s prestada el %s a las %s\n", i + 1, self->movimientos[i].herramienta, self->movimientos[i].fecha, self->movimientos[i].hora);
        }
    }
    int movimientoIndex;
    scanf("%d", &movimientoIndex);
    movimientoIndex--;

    if (movimientoIndex < 0 || movimientoIndex >= self->numMovements || self->movimientos[movimientoIndex].esDevolucion) {
        printf("Selección inválida.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    self->movimientos[movimientoIndex].esDevolucion = true;

    struct Movimiento nuevoMovimiento;
    strncpy(nuevoMovimiento.herramienta, self->movimientos[movimientoIndex].herramienta, MAX_TOOL_NAME - 1);
    strncpy(nuevoMovimiento.usuario, self->movimientos[movimientoIndex].usuario, MAX_NAME - 1);
    snprintf(nuevoMovimiento.fecha, sizeof(nuevoMovimiento.fecha), "%02d-%02d-%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    snprintf(nuevoMovimiento.hora, sizeof(nuevoMovimiento.hora), "%02d:%02d", tm.tm_hour, tm.tm_min);
    nuevoMovimiento.esDevolucion = true;

    self->movimientos[self->numMovements] = nuevoMovimiento;
    self->numMovements++;
    self->save_Movements(self);

    printf("Herramienta devuelta exitosamente.\n");
}

void resumen_Movimientos(struct _UserSystem* self) {
    printf("\n-Resumen de Movimientos-\n");
    for (int i = 0; i < self->numMovements; i++) {
        printf("%d. %s %s a %s el %s a las %s\n", i + 1,
               self->movimientos[i].herramienta,
               self->movimientos[i].esDevolucion ? "devuelta por" : "prestada a",
               self->movimientos[i].usuario,
               self->movimientos[i].fecha,
               self->movimientos[i].hora);
    }
}

void resumen_Deudas(struct _UserSystem* self) {
    printf("\n-Resumen de Deudas-\n");
    for (int i = 0; i < self->numMovements; i++) {
        if (!self->movimientos[i].esDevolucion) {
            printf("Herramienta: %s, Usuario: %s, Fecha: %s, Hora: %s\n",
                   self->movimientos[i].herramienta,
                   self->movimientos[i].usuario,
                   self->movimientos[i].fecha,
                   self->movimientos[i].hora);
        }
    }
}

void control_Stock(struct _UserSystem* self) {
    int option;
    do {
        printf("\n-Control de Stock-\n");
        printf("1. Reportar Entrega\n");
        printf("2. Reportar Devolución\n");
        printf("3. Resumen de Movimientos\n");
        printf("4. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                reportar_Entrega(self);
                break;
            case 2:
                reportar_Devolucion(self);
                break;
            case 3:
                resumen_Movimientos(self);
                break;
            case 4:
                printf("Volviendo al menú de SuperUsuario...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (option != 4);
}

void admin_Inventory(struct _UserSystem* self) {
    int option;
    do {
        printf("\n-Administrar Inventario-\n");
        printf("1. Agregar Herramienta\n");
        printf("2. Mostrar Herramientas\n");
        printf("3. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &option);

        switch (option) {
            case 1: {
                if (self->numTools >= MAX_TOOLS) {
                    printf("No se pueden agregar más herramientas. Inventario lleno.\n");
                    break;
                }
                struct Herramienta nuevaHerramienta;
                printf("Ingrese el tipo de herramienta: ");
                scanf("%49s", nuevaHerramienta.tipo);
                if (!self->toolvalidate(nuevaHerramienta.tipo)) {
                    break;
                }
                printf("Ingrese la cantidad: ");
                scanf("%d", &nuevaHerramienta.cantidad);
                self->herramientas[self->numTools] = nuevaHerramienta;
                self->numTools++;
                self->save_Tools(self);
                printf("Herramienta agregada exitosamente.\n");
                break;
            }
            case 2:
                printf("\n-Lista de Herramientas-\n");
                for (int i = 0; i < self->numTools; i++) {
                    printf("Herramienta %d:\n", i + 1);
                    printf("Tipo: %s\n", self->herramientas[i].tipo);
                    printf("Cantidad: %d\n", self->herramientas[i].cantidad);
                    printf("-----------------------------\n");
                }
                break;
            case 3:
                printf("Volviendo al menú de Inventario...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (option != 3);
}

void inventory_Menu(struct _UserSystem* self) {
    int option;
    do {
        printf("\n-Menú Inventario-\n");
        printf("1. Administrar Inventario\n");
        printf("2. Resumen Deudas\n");
        printf("3. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                self->admin_Inventory(self);
                break;
            case 2:
                resumen_Deudas(self);
                break;
            case 3:
                printf("Volviendo al menú de SuperUsuario...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (option != 3);
}

void su_Module(struct _UserSystem* self) {
    int option;
    do {
        printf("\n-Módulo SuperUsuario-\n");
        printf("1. Control Stock\n");
        printf("2. Administrar Usuarios\n");
        printf("3. Menú Inventario\n");
        printf("4. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                control_Stock(self);
                break;
            case 2:
                admin_Users(self);
                break;
            case 3:
                inventory_Menu(self);
                break;
            case 4:
                printf("Volviendo al menú principal...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (option != 4);
}

void login_User(struct _UserSystem* self) {
    char nombre[MAX_NAME], pass[MAX_PASSW];
    printf("Ingrese su nombre de usuario: ");
    scanf("%9s", nombre);
    printf("Ingrese su contraseña: ");
    scanf("%31s", pass);

    for (int i = 0; i < self->numUser; i++) {
        if (strcmp(self->usuarios[i].name, nombre) == 0 && strcmp(self->usuarios[i].pass, pass) == 0) {
            printf("Inicio de sesión exitoso.\n");
            if (i == 0) { // Asumimos que el primer usuario es el SuperUsuario
                su_Module(self);
            } else {
                user_Module(self, &self->usuarios[i]);
            }
            return;
        }
    }
    printf("Nombre de usuario o contraseña incorrectos.\n");
}

void estado_Deuda(struct _UserSystem* self, struct Usuario* user) {
    printf("\n-Estado de Deuda-\n");
    bool tieneDeuda = false;
    for (int i = 0; i < self->numMovements; i++) {
        if (!self->movimientos[i].esDevolucion && strcmp(self->movimientos[i].usuario, user->name) == 0) {
            printf("Herramienta: %s, Fecha: %s, Hora: %s\n",
                   self->movimientos[i].herramienta,
                   self->movimientos[i].fecha,
                   self->movimientos[i].hora);
            tieneDeuda = true;
        }
    }
    if (!tieneDeuda) {
        printf("No tiene herramientas pendientes de devolución.\n");
    }
}

int main() {
    struct _UserSystem UserSystem = {
        .Userarchive = "Usuarios.dat",
        .numUser = 0,
        .numTools = 0,
        .numMovements = 0,
        .load_User = load_User,
        .save_User = save_User,
        .load_Tools = load_Tools,
        .save_Tools = save_Tools,
        .load_Movements = load_Movements,
        .save_Movements = save_Movements,
        .uservalidate = uservalidate,
        .passvalidate = passvalidate,
        .toolvalidate = toolvalidate,
        .set_SU = set_SU,
        .create_User = create_User,
        .login_User = login_User,
        .admin_Inventory = admin_Inventory
    };

    // Inicializar el puntero de función set_User para cada usuario
    for (int i = 0; i < MAX_USERS; i++) {
        UserSystem.usuarios[i].set_User = set_User;
    }

    UserSystem.load_User(&UserSystem);
    UserSystem.load_Tools(&UserSystem);
    UserSystem.load_Movements(&UserSystem);

    int option;
    do {
        printf("\n-Módulo Inicio-\n");
        printf("1. Iniciar Sesión\n");
        printf("2. Crear Usuario\n");
        printf("3. Cerrar\n");
        printf("Seleccione una opción: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                UserSystem.login_User(&UserSystem);
                break;
            case 2:
                if (UserSystem.numUser == 0) {
                    printf("No existen usuarios. Creando SuperUsuario...\n");
                    UserSystem.set_SU(&UserSystem);
                } else {
                    UserSystem.create_User(&UserSystem);
                }
                break;
            case 3:
                printf("Cerrando...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (option != 3);

    return 0;
}
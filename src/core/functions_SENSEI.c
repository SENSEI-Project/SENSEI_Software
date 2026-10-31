#include "functions_SENSEI.h"  // Incluir el archivo de cabecera
#include "messages_SENSEI.h"   // Como vamos a imprimir mensajes, incluímos también este .h
#include <stdlib.h>   // getenv
#include <stdio.h>

// ------------------------------------------------------------------
//  Interactive stepping (see functions_SENSEI.h)
// ------------------------------------------------------------------

int sensei_interactive = -1;

void sensei_pause(void)
{
    if (sensei_interactive < 0) {
        const char *e = getenv("SENSEI_INTERACTIVE");
        sensei_interactive = (e && (*e == '1' || *e == 'y' || *e == 'Y' ||
                                    *e == 't' || *e == 'T')) ? 1 : 0;
    }
    if (sensei_interactive) {
        fputs("\n[SENSEI] Press Enter to continue...", stdout);
        fflush(stdout);
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
}





// FUNCIONES DE LECTURA DE ARCHIVOS .txt DE CONFIGURACIÓN
// ------------------------------------------------------

/**
 * @brief Lee el archivo de configuración en el que se especifican el modelo,
 * los archivos de medidas / pseudomedidas, archivo de opciones de calibración,
 * archivos extra para opciones de calibración e hiperparámetros del algoritmo.
 * 
 * @note CALIBRACIÓN
 */
void read_config_file_calibration(const char* config_File, char **base_Path, char **info_File, 
    char **dm_File, char **patterns_File, char **wlm_File, char **pm_File, char **fm_File, 
    char **fm_avg_File, char **clusters_pipes_File, char** common_HW_File, 
    char** common_DW_File, char** main_pipes_File, char** options_File, double* epsilon, 
    double* lambda_init, int* max_loops, int* max_it, int* max_attempts, double* tol, int *lang) {

    *lang = 1;

    FILE* file = fopen(config_File, "r"); 
    if (!file) { 
        printf("\033[0;31mNo se ha podido abrir el archivo de configuración.\n"
            "Asegúrate de que el archivo (%s) existe y está en la misma carpeta que el ejecutable\n\n"
            "The configuration file could not be opened.\n"
            "Make sure the file (%s) exists and is in the same folder as the executable\033[0m\n\n");
        SENSEI_PAUSE(); 
        exit(EXIT_FAILURE); 
    }

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        // Eliminar salto de línea
        line[strcspn(line, "\r\n")] = 0;

        char* key = strtok(line, ";");
        char* value = strtok(NULL, ";");

        // limpiar espacios
        if (value) {
            while (*value == ' ' || *value == '\t') value++;
            size_t len = strlen(value);
            while (len > 0 && (value[len-1] == ' ' || value[len-1] == '\t')) {
                value[len-1] = '\0';
                len--;
            }
        }

        if (!key) continue;

        //Lectura de los campos necesarios en config_calibration.txt y
        //rellenamos las variables asociadas (algunas se pueden quedar con los
        //valores por defecto si tienen "-")
        if (strcmp(key, "PATH") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *base_Path = strdup(value);
            } else {
                *base_Path = NULL;
            } 
        }
        else if (strcmp(key, "info_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *info_File = strdup(tmp);
            } else {
                *info_File = NULL;
            }
        }
        else if (strcmp(key, "patterns_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *patterns_File = strdup(tmp);
            } else {
                *patterns_File = NULL;
            }
        }
        else if (strcmp(key, "dm_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *dm_File = strdup(tmp);
            } else {
                *dm_File = NULL;
            }
        }
        else if (strcmp(key, "wlm_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *wlm_File = strdup(tmp);
            } else {
                *wlm_File = NULL;
            }
        }
        else if (strcmp(key, "pm_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *pm_File = strdup(tmp);
            } else {
                *pm_File = NULL;
            }
        }
        else if (strcmp(key, "fm_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *fm_File = strdup(tmp);
            } else {
                *fm_File = NULL;
            }
        }
        else if (strcmp(key, "fm_avg_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *fm_avg_File = strdup(tmp);
            } else {
                *fm_avg_File = NULL;
            }
        }
        else if (strcmp(key, "clusters_pipes_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *clusters_pipes_File = strdup(tmp);
            } else {
                *clusters_pipes_File = NULL;
            }
        }
        else if (strcmp(key, "common_HW_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *common_HW_File = strdup(tmp);
            } else {
                *common_HW_File = NULL;
            }
        } 
        // else if (strcmp(key, "age_materials_pipes_File") == 0) {
        //     if (value && strcasecmp(value, "-") != 0 && *base_Path) {
        //         char tmp[512];
        //         snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
        //         *age_materials_pipes_File = strdup(tmp);
        //     } else {
        //         *age_materials_pipes_File = NULL;
        //     }
        // } 
        else if (strcmp(key, "common_DW_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *common_DW_File = strdup(tmp);
            } else {
                *common_DW_File = NULL;
            }
        }
        else if (strcmp(key, "main_pipes_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *main_pipes_File = strdup(tmp);
            } else {
                *main_pipes_File = NULL;
            }
        } else if (strcmp(key, "options_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *options_File = strdup(tmp);
            } else {
                *options_File = NULL;
            }
        }
        else if (strcmp(key, "epsilon") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *epsilon = atof(value);
            } else {
                *epsilon = 0.2;
            }
        }
        else if (strcmp(key, "lambda_init") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *lambda_init = atof(value);
            } else {
                *lambda_init = 0.01;
            }
        }
        else if (strcmp(key, "max_loops") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *max_loops = atoi(value);
            } else {
                *max_loops = 20;
            }
        }
        else if (strcmp(key, "max_it") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *max_it = atoi(value);
            } else {
                *max_it = 5;
            }
        }
        else if (strcmp(key, "max_attempts") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *max_attempts = atoi(value);
            } else {
                *max_attempts = 5;
            }
        }
        else if (strcmp(key, "tol") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *tol = atof(value);
            } else {
                *tol = 1e-5;
            }
        }
        else if (strcmp(key, "LANGUAGE (ENG/ESP)") == 0) {
            if (strcasecmp(value, "ENG") == 0 || strcasecmp(value, "-") == 0) {
                *lang = 1;   // Inglés
            } else if (strcasecmp(value, "ESP") == 0) {
                *lang = 0;   // Español
            } else {
                // Valor no válido -> mensaje y fallback
                printf("\033[0;31mERROR: Unrecognized language value: '%s'\033[0m\n", value);
                printf("\033[0;31mUse 'ESP' or 'ENG'. English will be used by default.\033[0m\n\n");
                SENSEI_PAUSE();
                *lang = 1;   // Por defecto ENG
            }
        }
    }

    fclose(file);
}




/**
 * @brief Lee el archivo de configuración en el que
 * se especifican el modelo, los archivos de medidas / pseudomedidas y los
 * hiperparámetros del algoritmo.
 * 
 * @note EE MULTIPERIODO
 */
void read_config_file_multiperiod_SE(const char* config_File, char **base_Path, char **info_File, 
    char **dm_File, char **wlm_File, char **pm_File, char **fm_File, char **fm_avg_File, 
    double* epsilon, int* max_it, double* tol, double* lambda_init, int* max_attempts, 
    char **emitter_ID, double* emitter_coef, int* period_to_add_emitter, int *lang) {

    FILE* file = fopen(config_File, "r"); 
    if (!file) { 
        printf("\033[0;31mNo se ha podido abrir el archivo de configuración.\n"
            "Asegúrate de que el archivo (%s) existe y está en la misma carpeta que el ejecutable\n\n"
            "The configuration file could not be opened.\n"
            "Make sure the file (%s) exists and is in the same folder as the executable\033[0m\n\n");
        SENSEI_PAUSE(); 
        exit(EXIT_FAILURE); 
    }

    *lang = 1;

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        // Eliminar salto de línea
        line[strcspn(line, "\r\n")] = 0;

        char* key = strtok(line, ";");
        char* value = strtok(NULL, ";");

        // limpiar espacios
        if (value) {
            while (*value == ' ' || *value == '\t') value++;
            size_t len = strlen(value);
            while (len > 0 && (value[len-1] == ' ' || value[len-1] == '\t')) {
                value[len-1] = '\0';
                len--;
            }
        }

        if (!key) continue;

        //Lectura de los campos necesarios en config_multiperiod_SE.txt y
        //rellenamos las variables asociadas (algunas se pueden quedar con los
        //valores por defecto si tienen "-")
        if (strcmp(key, "PATH") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *base_Path = strdup(value);
            } else {
                *base_Path = NULL;
            } 
        }
        else if (strcmp(key, "info_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *info_File = strdup(tmp);
            } else {
                *info_File = NULL;
            }
        }
        else if (strcmp(key, "dm_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *dm_File = strdup(tmp);
            } else {
                *dm_File = NULL;
            }
        }
        else if (strcmp(key, "wlm_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *wlm_File = strdup(tmp);
            } else {
                *wlm_File = NULL;
            }
        }
        else if (strcmp(key, "pm_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *pm_File = strdup(tmp);
            } else {
                *pm_File = NULL;
            }
        }
        else if (strcmp(key, "fm_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *fm_File = strdup(tmp);
            } else {
                *fm_File = NULL;
            }
        }
        else if (strcmp(key, "fm_avg_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *fm_avg_File = strdup(tmp);
            } else {
                *fm_avg_File = NULL;
            }
        }
        else if (strcmp(key, "epsilon") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *epsilon = atof(value);
            } else {
                *epsilon = 0.2;
            }
        }
        else if (strcmp(key, "max_it") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *max_it = atoi(value);
            } else {
                *max_it = 5;
            }
        }
        else if (strcmp(key, "tol") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *tol = atof(value);
            } else {
                *tol = 1e-5;
            }
        }
        else if (strcmp(key, "lambda_init") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *lambda_init = atof(value);
            } else {
                *lambda_init = 0.01;
            }
        }
        else if (strcmp(key, "max_attempts") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *max_attempts = atoi(value);
            } else {
                *max_attempts = 5;
            }
        }
        else if (strcmp(key, "emitter_ID") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *emitter_ID = strdup(value);
            } else {
                *emitter_ID = NULL;
            }
        }
        else if (strcmp(key, "emitter_coef") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *emitter_coef = atof(value);
            } else {
                *emitter_coef = 0;
            }
        }
        else if (strcmp(key, "period_to_add_emitter") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *period_to_add_emitter = atoi(value);
            } else {
                *period_to_add_emitter = 0;
            }
        }
        else if (strcmp(key, "LANGUAGE (ENG/ESP)") == 0) {
            if (strcasecmp(value, "ENG") == 0 || strcasecmp(value, "-") == 0) {
                *lang = 1;   // Inglés
            } else if (strcasecmp(value, "ESP") == 0) {
                *lang = 0;   // Español
            } else {
                // Valor no válido -> mensaje y fallback
                printf("\033[0;31mERROR: Unrecognized language value: '%s'\033[0m\n", value);
                printf("\033[0;31mUse 'ESP' or 'ENG'. English will be used by default.\033[0m\n\n");
                SENSEI_PAUSE();
                *lang = 1;   // Por defecto ENG
            }
        }
        
    }


    fclose(file);
}




/**
 * @brief Lee el archivo de configuración en el que
 * se especifican el modelo, los archivos de medidas / pseudomedidas y los
 * hiperparámetros del algoritmo.
 * 
 * @note EE INSTANTÁNEA
 */
void read_config_file_snapshot_SE(const char* config_File, char **base_Path, char **info_File, 
    char **dm_File, char **wlm_File, char **pm_File, char **fm_File, double* epsilon, 
    int* max_it, double* tol, char **emitter_ID, int *lang) {

    FILE* file = fopen(config_File, "r"); 
    if (!file) { 
        printf("\033[0;31mNo se ha podido abrir el archivo de configuración.\n"
            "Asegúrate de que el archivo (%s) existe y está en la misma carpeta que el ejecutable\n\n"
            "The configuration file could not be opened.\n"
            "Make sure the file (%s) exists and is in the same folder as the executable\033[0m\n\n");
        SENSEI_PAUSE(); 
        exit(EXIT_FAILURE); 
    }

    *lang = 1;

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        // Eliminar salto de línea
        line[strcspn(line, "\r\n")] = 0;

        char* key = strtok(line, ";");
        char* value = strtok(NULL, ";");

        // limpiar espacios
        if (value) {
            while (*value == ' ' || *value == '\t') value++;
            size_t len = strlen(value);
            while (len > 0 && (value[len-1] == ' ' || value[len-1] == '\t')) {
                value[len-1] = '\0';
                len--;
            }
        }

        if (!key) continue;

        //Lectura de los campos necesarios en config_snapshot_SE.txt y
        //rellenamos las variables asociadas (algunas se pueden quedar con los
        //valores por defecto si tienen "-")
        if (strcmp(key, "PATH") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *base_Path = strdup(value);
            } else {
                *base_Path = NULL;
            } 
        }
        else if (strcmp(key, "info_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *info_File = strdup(tmp);
            } else {
                *info_File = NULL;
            }
        }
        else if (strcmp(key, "dm_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *dm_File = strdup(tmp);
            } else {
                *dm_File = NULL;
            }
        }
        else if (strcmp(key, "wlm_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *wlm_File = strdup(tmp);
            } else {
                *wlm_File = NULL;
            }
        }
        else if (strcmp(key, "pm_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *pm_File = strdup(tmp);
            } else {
                *pm_File = NULL;
            }
        }
        else if (strcmp(key, "fm_File") == 0) {
            if (value && strcasecmp(value, "-") != 0 && *base_Path) {
                char tmp[512];
                snprintf(tmp, sizeof(tmp), "%s/%s", *base_Path, value);
                *fm_File = strdup(tmp);
            } else {
                *fm_File = NULL;
            }
        }
        else if (strcmp(key, "epsilon") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *epsilon = atof(value);
            } else {
                *epsilon = 0.2;
            }
        }
        else if (strcmp(key, "max_it") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *max_it = atoi(value);
            } else {
                *max_it = 5;
            }
        }
        else if (strcmp(key, "tol") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *tol = atof(value);
            } else {
                *tol = 0.0003;
            }
        }
        else if (strcmp(key, "emitter_ID") == 0) {
            if (value && strcasecmp(value, "-") != 0) {
                *emitter_ID = strdup(value);
            } else {
                *emitter_ID = NULL;
            }
        }
        else if (strcmp(key, "LANGUAGE (ENG/ESP)") == 0) {
            if (strcasecmp(value, "ENG") == 0 || strcasecmp(value, "-") == 0) {
                *lang = 1;   // Inglés
            } else if (strcasecmp(value, "ESP") == 0) {
                *lang = 0;   // Español
            } else {
                // Valor no válido -> mensaje y fallback
                printf("\033[0;31mERROR: Unrecognized language value: '%s'\033[0m\n", value);
                printf("\033[0;31mUse 'ESP' or 'ENG'. English will be used by default.\033[0m\n\n");
                SENSEI_PAUSE();
                *lang = 1;   // Por defecto ENG
            }
        }
        
    }

    fclose(file);
}




// FUNCIONES DE PRECONFIGURACIÓN DE CALIBRACIÓN
// --------------------------------------------

/**
 * @brief Analiza si dm_File y patterns_File han sido declarados y define en
 * base a ello el enfoque para trabajar con las demandas. También revisa que al
 * menos uno de ellos esté definido
 * 
 * @note CALIBRACIÓN
 */
int set_all_demands_nodes_rows(const char* dm_File, const char* patterns_File, int lang){

    int all_demands_nodes_rows;

    //Sobre filas relativas a demandas 
    if (dm_File != NULL && patterns_File == NULL){
        all_demands_nodes_rows = 1;
        // printf("Vamos a incluir como filas de demandas las demandas de TODOS los nodos\n\n");
    } else if (dm_File == NULL && patterns_File != NULL){
        all_demands_nodes_rows = 0;
        // printf("Vamos a incluir como filas de demandas SOLAMENTE los PATRONES con los que trabajemos\n\n");
    } else if (dm_File != NULL && patterns_File != NULL){
        printf("%s", messages[MSG_ERR_DM_AND_PATTERNS_CONFLICT][lang]);
        all_demands_nodes_rows = 0;
        SENSEI_PAUSE();
    } else {
        // printf("\033[0;31mDebes incluir o bien un archivo con info de patrones o bien un archivo con info. de demandas en los nodos de demanda!!  \033[0m \n");
        // printf("\033[0;31mNo has incluido ninguno de los dos. CIERRA Y ARREGLALO!!!  \033[0m \n");
        printf("%s", messages[MSG_ERR_MISSING_DM_FILE][lang]);
        SENSEI_PAUSE();
        all_demands_nodes_rows = -1;
    }

    return all_demands_nodes_rows;
    
}




/**
 * @brief Lee el archivo .csv de opciones y activa las variables que el usuario
 * ha decidido optimizar
 * 
 * @note CALIBRACIÓN
 */
int read_options_variables(Options *Opt, const char* options_File, int*** CM, int lang) {

    FILE* file = fopen(options_File, "r");

    //Inicializamos en 0 todas las variables binarias de la estructura Opt
    Opt->mod_patterns = 0;
    Opt->mod_base_demands = 0;
    Opt->mod_curves_X = 0;
    Opt->mod_curves_Y = 0;
    Opt->mod_valves_settings = 0;
    Opt->mod_individual_roughness = 0; 
    Opt->mod_common_roughness_DW = 0; 
    Opt->mod_common_roughness_HW = 0;
    Opt->mod_init_tank_levels = 0;
    Opt->mod_tank_diameters = 0;

    if (!file) {
        printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "options_File", options_File);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char line[256];
    int row = 0;
    int max_position = 0;


    // Definimos matriz 10x10
    //Las columnas indican el tipo de variable, y las filas indican la etapa en
    //la que modificaremos esa variable (si se decide hacerlo)
    *CM = (int**)malloc(10 * sizeof(int*));  //Matriz de cronología
    for (int i = 0; i < 10; i++) {
        (*CM)[i] = (int*)malloc(10 * sizeof(int));
        for (int j = 0; j < 10; j++) {
            (*CM)[i][j] = 0;  // Inicializar a 0
        }
    }



    while (fgets(line, sizeof(line), file) && row < 10) {
        // Buscar el valor de la posición después del ';'
        char* token = strrchr(line, ';');
        if (token != NULL) {
            int position = atoi(token + 1);  // Leemos la segunda celda de cada línea
            if (position >= 1 && position <= 10) {
                (*CM)[position - 1][row] = 1;

                // Activamos la variable binaria correspondiente según la fila en Options_File
                if (row == 0) Opt->mod_patterns = 1;
                else if (row == 1) Opt->mod_base_demands = 1;
                else if (row == 2) Opt->mod_curves_X = 1;
                else if (row == 3) Opt->mod_curves_Y = 1;
                else if (row == 4) Opt->mod_valves_settings = 1;
                else if (row == 5) Opt->mod_individual_roughness = 1;
                else if (row == 6) Opt->mod_common_roughness_DW = 1;
                else if (row == 7) Opt->mod_common_roughness_HW = 1;
                else if (row == 8) Opt->mod_init_tank_levels = 1;
                else if (row == 9) Opt->mod_tank_diameters = 1;

                if (position > max_position) {
                    max_position = position;
                }

            }
        }
        row++;
    }

    fclose(file);

    return max_position; //max_position es el número de ETAPAS del proceso de calibración

}





/**
 * @brief Revisa los archivos definidos para las variables relativas a
 * rugosidad, depura errores, alerta de incompatibilidades, etc.
 * 
 * @note CALIBRACIÓN
 */
int check_roughness_options_set_mod_MP(Options Opt, const char* clusters_pipes_File, 
    const char* common_DW_File, const char* common_HW_File, const char* main_pipes_File, int lang){

    //Variable para considerar rugosidades de todas las tuberías o solo de las principales
    int mod_MP;  

    if (main_pipes_File != NULL){
        mod_MP = 1;  //Si se detecta nombre de archivo de tuberías principales, se activa
    } else {
        mod_MP = 0;
    }

    //No se pueden seleccionar a la vez las opciones de coef. de rugosidad común 
    //por grupo (ya sea HW o DW) y coef. individual
    if ((Opt.mod_individual_roughness+Opt.mod_common_roughness_DW+Opt.mod_common_roughness_HW) >= 2){
        printf("%s", messages[MSG_ERR_INCOMPATIBLE_ROUGHNESS_OPTIONS][lang]);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    //Si se selecciona opción de coef. de rugosidad común por grupo pero se ha
    //dado un archivo de tuberías principales, se avisa de que no se tendrá en
    //cuenta ese archivo
    if (Opt.mod_individual_roughness == 0 && (Opt.mod_common_roughness_DW == 1 || Opt.mod_common_roughness_HW == 1) && mod_MP == 1){
        if (Opt.mod_common_roughness_DW == 1){
            printf("%s", messages[MSG_WARN_MAIN_PIPES_IGNORED_DW][lang]);
        } else if (Opt.mod_common_roughness_HW == 1){
            printf("%s", messages[MSG_WARN_MAIN_PIPES_IGNORED_HW][lang]);
        }
        mod_MP = 0; //Cambiamos a 0 para evitar futuros problemas
        SENSEI_PAUSE();
    }

    //Si se elige la opción de coef. de rugosidad común por grupos
    //Es necesario especificar archivo de grupos de tuberías y archivo de edad y material
    if (Opt.mod_common_roughness_DW == 1){
        if (clusters_pipes_File == NULL && common_DW_File == NULL){
            printf("%s", messages[MSG_ERR_DW_GROUPS_MISSING_BOTH][lang]);
            SENSEI_PAUSE();
            exit(EXIT_FAILURE);
        } else if (clusters_pipes_File == NULL){
            printf("%s", messages[MSG_ERR_DW_GROUPS_MISSING_CLUSTERS_PIPES_FILE][lang]);
            SENSEI_PAUSE();
            exit(EXIT_FAILURE);
        } else if (common_DW_File == NULL){
            printf("%s", messages[MSG_ERR_DW_GROUPS_MISSING_DW_FILE][lang]);
            SENSEI_PAUSE();
            exit(EXIT_FAILURE);
        }
    }

    if (Opt.mod_common_roughness_HW == 1){
        if (clusters_pipes_File == NULL && common_HW_File == NULL){
            printf("%s", messages[MSG_ERR_HW_GROUPS_MISSING_BOTH][lang]);
            SENSEI_PAUSE();
            exit(EXIT_FAILURE);
        } else if (clusters_pipes_File == NULL){
            printf("%s", messages[MSG_ERR_HW_GROUPS_MISSING_CLUSTERS_PIPES_FILE][lang]);
            SENSEI_PAUSE();
            exit(EXIT_FAILURE);
        } else if (common_HW_File == NULL){
            printf("%s", messages[MSG_ERR_HW_GROUPS_MISSING_HW_FILE][lang]);
            SENSEI_PAUSE();
            exit(EXIT_FAILURE);
        }
    }


    if ((Opt.mod_common_roughness_DW+Opt.mod_common_roughness_HW) == 0 && clusters_pipes_File != NULL){
        printf(messages[MSG_WARN_GENERAL_FILE_IGNORED][lang], "clusters_pipes_File");
        SENSEI_PAUSE();
    }


    if (Opt.mod_common_roughness_HW == 0 && common_HW_File != NULL){
        printf(messages[MSG_WARN_GENERAL_FILE_IGNORED][lang], "common_HW_File");
        SENSEI_PAUSE();
    }


    if (Opt.mod_common_roughness_DW == 0 && common_DW_File != NULL){
        printf(messages[MSG_WARN_GENERAL_FILE_IGNORED][lang], "common_DW_File");
        SENSEI_PAUSE();
    }


    return mod_MP;
}



// FUNCIONES DE CONTEO Y CLASIFICACIÓN DE ELEMENTOS
// ------------------------------------------------



/**
 * @brief Cuenta los nodos (junctions, embalses y tanques) y los enlaces
 * (tuberías, bombas y vávlulas) que hay en la red
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
void count_elements_in_network(int* nN, int* nL, int* nJ, int* nR, int* nT, int* nPI, int* nPU, 
    int* nV, EN_Project ph){

    *nJ = 0, *nR = 0, *nT = 0, *nPI = 0, *nPU = 0, *nV = 0;

    EN_getcount(ph, EN_NODECOUNT, nN);  // Número total de nodos
    EN_getcount(ph, EN_LINKCOUNT, nL);  // Número total de enlaces (tuberías)
 
    int nodeType, linkType;

    // DESGLOSAR EL CONTEO DE CADA NODO
    for (int i = 1; i <= *nN; i++) {
        EN_getnodetype(ph, i, &nodeType);

        if (nodeType == EN_JUNCTION) {  // Nodo tipo JUNCTION
            (*nJ)++;  
        } else if (nodeType == EN_RESERVOIR) {  // Nodo tipo EMBALSE
            (*nR)++;
        } else if (nodeType == EN_TANK) { // Nodo tipo TANQUE
            (*nT)++;
        }
    }

    // DESGLOSAR EL CONTEO DE CADA LINK
    for (int i = 1; i <= *nL; i++) {
        EN_getlinktype(ph, i, &linkType);
        if (linkType == EN_PIPE || linkType == EN_CVPIPE) {  // Link tipo TUBERÍA o TUBERÍA con CV
            (*nPI)++;  
        } else if (linkType == EN_PUMP) { // Link tipo BOMBA
            (*nPU)++;
        } else {  // Link tipo VÁLVULA (ponemos "else" porque hay 6 tipos de válvula: TCV, FCV, PRV, etc.)
            (*nV)++;
        }
    }
}




/**
 * @brief Clasifica los junctions como de demanda y de tránsito (según su
 * demanda base). Cuenta la cantidad de cada tipo y crea listas con los índices
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
void classify_transit_demand_junctions(int nJ, int *nJ_Trans, int *nJ_Dem, int **ind_J_Trans, int **ind_J_Dem, EN_Project ph) {
    *nJ_Trans = 0;
    *nJ_Dem = 0;

    double base_demand;

    for (int i = 0; i < nJ; i++) {  //Primer bucle para contar cuántos hay de tránsito y de demanda
        EN_getnodevalue(ph, i+1, EN_BASEDEMAND, &base_demand);  //El criterio es la demanda base (nula o no nula)
        if (base_demand == 0){
            (*nJ_Trans)++;
        } else {
            (*nJ_Dem)++;
        }
    }

    int pos_Trans = 0;
    int pos_Dem = 0;

    *ind_J_Trans = (int*)malloc((*nJ_Trans) * sizeof(int));
    *ind_J_Dem = (int*)malloc((*nJ_Dem) * sizeof(int));


    for (int i = 0; i < nJ; i++){  //Segundo bucle para almacenar índices
        EN_getnodevalue(ph, i+1, EN_BASEDEMAND, &base_demand);
        if (base_demand == 0){
            (*ind_J_Trans)[pos_Trans] = i+1;
            pos_Trans++;
        } else {
            (*ind_J_Dem)[pos_Dem] = i+1;
            pos_Dem++;
        }
    }

}



/**
 * @brief Cuenta el número de patrones de demanda en un .inp
 * 
 * @note CALIBRACIÓN
 */
int count_demand_patterns_from_inp(int nJ, int** unique_patterns, char*** demand_patterns_id, EN_Project ph) {

    int nDemPat = 0; 

    // Asumimos que como mucho habrá nJ patrones distintos
    (*unique_patterns) = (int*)malloc(nJ * sizeof(int));

    for (int i = 0; i < nJ; i++){
        (*unique_patterns)[i] = 0;
    }


    // Contar patrones únicos asociados a nodos (junctions)
    for (int i = 0; i < nJ; i++) {
        double pattern_index;
        EN_getnodevalue(ph, i + 1, EN_PATTERN, &pattern_index);

        if (pattern_index > 0) {
            int exists = 0;
            for (int j = 0; j < nDemPat; j++) {
                if ((*unique_patterns)[j] == (int)pattern_index) {
                    exists = 1;
                    break;
                }
            }
            if (!exists) {
                (*unique_patterns)[nDemPat] = (int)pattern_index;
                nDemPat++;
            }
        }
    }

    sort_first_n(*unique_patterns, nDemPat);

    // Reservar memoria para los IDs de los patrones
    *demand_patterns_id = malloc(nDemPat * sizeof(char*));

    // Obtener los IDs de cada patrón 
    char pattern_id[64];  

    for (int i = 0; i < nDemPat; i++) {
        EN_getpatternid(ph, (*unique_patterns)[i], pattern_id);
        (*demand_patterns_id)[i] = strdup(pattern_id);  // duplicar string dinámicamente
    } 

    return nDemPat;

}









/**
 * @brief Obtiene los niveles mínimos y máximos de los tanques de la red
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
void get_min_max_tank_levels(int nJ, int nR, int nT, double** min_tank_levels, 
    double** max_tank_levels, EN_Project ph){
    
    double min_level, max_level;
    for (int i = 0; i < nT; i++){
        EN_getnodevalue(ph, i+nJ+nR+1, EN_MINLEVEL, &min_level);
        EN_getnodevalue(ph, i+nJ+nR+1, EN_MAXLEVEL, &max_level);
        (*min_tank_levels)[i] = min_level;
        (*max_tank_levels)[i] = max_level;
    }
}






/**
 * @brief Almacena, ordenado por intervalos, los factores multiplicadores de los
 * patrones de demanda declarados en el .inp
 * 
 * @see Devuelve matriz de tamaño T x nDemPat, con los factores multiplicadores
 * en cada intervalo (fila) de cada patrón de demanda (columna)
 * 
 * @note CALIBRACIÓN
 */
double** get_demand_patterns_from_inp(int nDemPat, int T, char** demand_patterns_id,
    int* unique_patterns, EN_Project ph, int lang) {
    
    // Paso 1: Reservar espacio para guardar los valores de los patrones 
    //(matriz tamaño T x nDemPat)
    double **patterns = (double**)malloc(T * sizeof(double*));

    for (int i = 0; i < T; i++) {
        patterns[i] = (double*)malloc(nDemPat*sizeof(double));
    }


    // Paso 2: Leer los valores de cada patrón y guardarlos
    for (int i = 0; i < nDemPat; i++) {
        int pat_index = unique_patterns[i];  // índice real del patrón en EPANET

        int pat_len;
        EN_getpatternlen(ph, unique_patterns[i], &pat_len);
        if (pat_len < T){
            printf(messages[MSG_ERR_PATTERN_TOO_SHORT][lang], demand_patterns_id[i], pat_len, T);
            SENSEI_PAUSE();
            exit(EXIT_FAILURE);
        }

        for (int t = 0; t < T; t++) {
            double pattern_value;
            EN_getpatternvalue(ph, pat_index, t + 1, &pattern_value);

            // Almacenamos por filas: factor multiplicador para cada instante
            // (columnas por patrón)
            patterns[t][i] = pattern_value;
        }
    }

    return patterns;
}




/**
 * @brief Lee main_pipes_File y almacena las posiciones de las tuberías "principales"
 * 
 * @note CALIBRACIÓN
 */
int store_main_pipes(int nL, int** indMP, const char* main_pipes_File, EN_Project ph, int lang) {
    FILE* file = fopen(main_pipes_File, "r");
    if (!file) {
        printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "main_pipes_File", main_pipes_File);  
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char line[50];
    int pipe_index;
    int nMP = 0;  // Número de tuberías principales

    // Vector lógico: 1 si la tubería 'i'-ésima (en el orden del .inp) es principal, 0 si no
    int* main_pipes_logic = (int*)malloc(nL * sizeof(int));
    for (int i = 0; i < nL; i++) {
        main_pipes_logic[i] = 0;
    }

    // Leer línea por línea
    while (fgets(line, sizeof(line), file)) {
        
        // Manejar BOM si existe (sólo afecta a la primera línea)
        if (strncmp(line, "\xEF\xBB\xBF", 3) == 0) {
            memmove(line, line + 3, strlen(line) - 2);  // Elimina los 3 bytes del BOM
        }

        line[strcspn(line, "\n")] = 0;  // Eliminar salto de línea

        if (strlen(line) == 0) continue; // Saltar líneas vacías

        // Obtener índice de la tubería desde el ID
        int err = EN_getlinkindex(ph, line, &pipe_index);
        if (err != 0) {
            printf(messages[MSG_ERR_PIPE_NOT_FOUND][lang], line);
            SENSEI_PAUSE();
        } else if (pipe_index > 0 && pipe_index <= nL) {
            main_pipes_logic[pipe_index - 1] = 1;
            nMP++;
        }
    }

    fclose(file);

    // Crear vector con índices de las tuberías principales
    (*indMP) = (int*)malloc(nMP * sizeof(int));
    int j = 0;
    for (int i = 0; i < nL; i++) {
        if (main_pipes_logic[i] == 1) {
            (*indMP)[j++] = i+1;
        }
    }

    free(main_pipes_logic);
    return nMP;  // Retornar número de tuberías principales
}




/**
 * @brief Obtiene los coeficientes de rugosidad de las tuberías del inp, bien de
 * todas o bien solamente de las 'principales' (según se haya configurado)
 * 
 * @note CALIBRACIÓN
 */
double* get_roughness_coefficients_from_inp(int nPI, int nMP, int mod_MP, int* indMP, EN_Project ph){

    double* roughness_coefficients = NULL;

    if (mod_MP == 0){
        roughness_coefficients = (double*)malloc(nPI*sizeof(double));

        double roughness;
        for (int i = 0; i < nPI; i++){
            EN_getlinkvalue(ph, i+1, EN_ROUGHNESS, &roughness);
            roughness_coefficients[i] = roughness;
        }

    } else {

        roughness_coefficients = (double*)malloc(nMP*sizeof(double));

        double roughness;
        for (int i = 0; i < nMP; i++){
            EN_getlinkvalue(ph, indMP[i], EN_ROUGHNESS, &roughness);
            roughness_coefficients[i] = roughness;
        }
    }

    return roughness_coefficients;

}




/**
 * @brief Lee el archivo cluster_pipes_File y clasifica las tuberías en los
 * grupos definidos (dentro de la estructura Clusters_Pipes_Info)
 * 
 * @note CALIBRACIÓN
 */
int classify_pipes_by_cluster(const char *clusters_pipes_File, EN_Project ph, 
    Cluster_Pipes_Info *Clusters_Pipes_Info, int lang) {
    FILE *file = fopen(clusters_pipes_File, "r");
    if (!file) {
        printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "clusters_pipes_File", clusters_pipes_File);  
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char line[256];
    int pipe_index;
    int nCP = 0; // Contador de clusters de tuberías registrados

    double diam; //Para almacenar el diámetro

    while (fgets(line, sizeof(line), file)) {
        // Eliminar salto de línea
        line[strcspn(line, "\n")] = 0;

        // Manejar BOM si existe
        if (strncmp(line, "\xEF\xBB\xBF", 3) == 0) {
            memmove(line, line + 3, strlen(line) - 2);
        }

        // Ignorar líneas vacías
        if (strlen(line) == 0) continue;

        // Parsear la línea
        char *pipe_id = strtok(line, ";");
        char *cluster_id = strtok(NULL, "\n");

        if (!pipe_id || !cluster_id) {
            printf(messages[MSG_ERR_INVALID_CSV_LINE][lang], clusters_pipes_File, line);
            continue;
        }

        // Obtener el índice de la tubería
        int err = EN_getlinkindex(ph, pipe_id, &pipe_index);
        if (err != 0) {
            printf(messages[MSG_ERR_PIPE_NOT_FOUND][lang], pipe_id);
            continue; // Saltar esta línea
        }

        // Buscar si el cluster ya existe en el arreglo
        int CP_index = -1;
        for (int i = 0; i < nCP; i++) {
            if (strcmp(Clusters_Pipes_Info[i].cluster_id, cluster_id) == 0) {
                CP_index = i;
                break;
            }
        }

        // Si el cluster no existe, agregarlo
        if (CP_index == -1) {
            strncpy(Clusters_Pipes_Info[nCP].cluster_id, cluster_id, sizeof(Clusters_Pipes_Info[nCP].cluster_id));
            Clusters_Pipes_Info[nCP].pipe_count = 0;
            CP_index = nCP++;
        }

        // Agregar el índice del nodo al cluster correspondiente
        Clusters_Pipes_Info[CP_index].pipe_index[Clusters_Pipes_Info[CP_index].pipe_count] = pipe_index;
        
        //Asignar a la tubería en cuestión su diámetro (leyéndolo del .inp)
        EN_getlinkvalue(ph, pipe_index, EN_DIAMETER, &diam);
        Clusters_Pipes_Info[CP_index].diameter[Clusters_Pipes_Info[CP_index].pipe_count] = diam;

        Clusters_Pipes_Info[CP_index].pipe_count++;

    }

    fclose(file);
    return nCP; // Devuelve el número total de CLUSTERS DE TUBERÍAS encontrados
}



/**
 * @brief Lee el coeficiente de rugosidad común para cada grupo:
 *  - Puede ser el coef. de Hazen-Williams (desde el archivo .csv common_HW_File)
 *  - o el coef. de Darcy-Weisbach (desde common_DW_File)
 * 
 * @note CALIBRACIÓN
 */
void classify_common_group_coef(int nCP, int HW, const char *common_coef_File, 
    Cluster_Pipes_Info *Clusters_Pipes_Info, EN_Project ph, int lang) {

    
    FILE *file = fopen(common_coef_File, "r");
    if (!file) {
        if (HW == 1){
            printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "common_HW_File", common_coef_File);  
        } else {
            printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "common_DW_File", common_coef_File);
        }
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char line[256];

    while (fgets(line, sizeof(line), file)) {
        // Eliminar salto de línea
        line[strcspn(line, "\n")] = 0;

        // Manejar BOM si existe
        if (strncmp(line, "\xEF\xBB\xBF", 3) == 0) {
            memmove(line, line + 3, strlen(line) - 2);
        }

        // Ignorar líneas vacías
        if (strlen(line) == 0) continue;

        // Parsear la línea
        char *cluster_id = strtok(line, ";");
        double common_group_coef = atof(strtok(NULL, "\n"));

        if (!cluster_id || !common_group_coef) {
            printf(messages[MSG_ERR_INVALID_CSV_LINE][lang], common_coef_File, line);
            continue;
        }


        // Asocia cluster_id al ID de cada cluster en Clusters_Pipes_Info y
        // rellena el campo correspondiente
        for (int i = 0; i < nCP; i++) {
            if (strcmp(Clusters_Pipes_Info[i].cluster_id, cluster_id) == 0) {   
                if (HW == 1){
                    Clusters_Pipes_Info[i].common_HW_coef = common_group_coef;  
                } else {
                    Clusters_Pipes_Info[i].common_DW_coef = common_group_coef;  
                }
                
            }
        }
    }

    fclose(file);
}




/**
 * @brief Convierte el coeficiente de rugosidad de Darcy-Weisbach de cada grupo
 * a Hazen-Williams para cada tubería del grupo (mediante una expresión en la
 * que interviene el diámetro de la tubería, en metros)
 * 
 * @note CALIBRACIÓN
 */
double* convert_roughness_coefs_from_DW_to_HW(int nPI, int nCP, double* common_DW_coefs, 
    Cluster_Pipes_Info* Clusters_Pipes_Info, EN_Project ph){

    double* roughness_coefficients = (double*)malloc(nPI*sizeof(double));

    double A, B, C, C_HW;
    double diam_m;

    for (int i = 0; i < nCP; i++){
        for (int j = 0; j < Clusters_Pipes_Info[i].pipe_count; j++){
            diam_m = Clusters_Pipes_Info[i].diameter[j]/1000;  //Diámetro en metros
            A = 157-12*exp(-5*diam_m);
            B = -4.89-1.15*log10(diam_m);
            C = 1.97-0.263*log10(diam_m);
            C_HW = A+B*pow(log10(common_DW_coefs[i])+3,C);

            roughness_coefficients[Clusters_Pipes_Info[i].pipe_index[j]-1] = C_HW;

        }

    }

    return roughness_coefficients;

}



/**
 * @brief Para las opciones de coef. de rugosidad por grupos (ya sea en HW o en DW),  
 * extiende el coeficiente común a las tuberías (directamente si se ha elegido HW 
 * o mediante conversión si se ha elegido DW)
 * 
 * @note CALIBRACIÓN
 */
double* extend_common_roughness_coefs_to_all_pipes(int nPI, int nCP, double* common_HW_coefs, 
    Cluster_Pipes_Info* Clusters_Pipes_Info, EN_Project ph){

    double* roughness_coefficients = (double*)malloc(nPI*sizeof(double));

    //Simplemente se replica el coef. común de cada grupo en cada una de las
    //tuberías del grupo, de manera que luego sea sencillo modificar los valores
    //en el ph desde el vector resultante
    for (int i = 0; i < nCP; i++){
        for (int j = 0; j < Clusters_Pipes_Info[i].pipe_count; j++){
            roughness_coefficients[Clusters_Pipes_Info[i].pipe_index[j]-1] = common_HW_coefs[i];
        }
    }

    return roughness_coefficients;

}




/**
 * @brief Obtiene en forma de vector la demanda base de los junctions de demanda
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
double* get_base_demands_vector_from_inp(int nJ_Dem, int* ind_J_Dem, EN_Project ph) {

    double* base_demands_vector = (double*)malloc(nJ_Dem*sizeof(double)); 

    double bd;

    for (int i = 0; i < nJ_Dem; i++){
        EN_getnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, &bd);
        base_demands_vector[i] = bd;
    }

    return base_demands_vector; // Devuelve el vector de demandas base, en orden según los nodos de demanda están en el ph

}




/**
 * @brief Guarda las coordenadas X e Y de las curvas de las bombas
 * 
 * @warning Asume que las curvas de las bombas se componen de 3 puntos: (x1,y1),
 * (x2,y2), (x3,y3)
 * 
 * @note CALIBRACIÓN
 */
int store_pump_curves_info(double*** xCurves, double*** yCurves, 
    int** pumpCurveIndices_out, EN_Project ph, int lang){

    int nTotalCurves, curve_type;
    int nPumpCurves = 0;
    double xValue, yValue;

    //Número total de curvas (algunas no tiene porqué ser de bombas)
    EN_getcount(ph, EN_CURVECOUNT, &nTotalCurves);  

    // Primero contar cuántas curvas son de tipo PUMP
    for (int i = 1; i <= nTotalCurves; i++) {
        EN_getcurvetype(ph, i, &curve_type);
        if (curve_type == EN_PUMP_CURVE) {
            nPumpCurves++;
        }
    }

    //Clasificar en pumpCurveIndices solamente los índices de las curvas que son de bombas
    int* pumpCurveIndices = (int*)malloc(nPumpCurves * sizeof(int));
    int pos = 0;
    for (int i = 1; i <= nTotalCurves; i++) {
        EN_getcurvetype(ph, i, &curve_type);
        if (curve_type == EN_PUMP_CURVE) {
            pumpCurveIndices[pos++] = i;
        }
    }

    // Reservamos matrices dinámicas de tamaño [nCurves][3]
    *xCurves = (double**)malloc(nPumpCurves * sizeof(double*));
    *yCurves = (double**)malloc(nPumpCurves * sizeof(double*));

    for (int i = 0; i < nPumpCurves; i++) {
        (*xCurves)[i] = (double*)malloc(3*sizeof(double));
        (*yCurves)[i] = (double*)malloc(3*sizeof(double));
    }

    // Rellenar los valores
    for (int i = 0; i < nPumpCurves; i++) {
        int curve_len;
        EN_getcurvelen(ph, pumpCurveIndices[i], &curve_len);
        if (curve_len != 3){
            printf("%s", messages[MSG_ERR_PUMP_CURVE_INCOMPLETE][lang]);
            SENSEI_PAUSE();
            exit(EXIT_FAILURE);
        } else {
            for (int j = 1; j <= 3; j++) {
                EN_getcurvevalue(ph, pumpCurveIndices[i], j, &xValue, &yValue);
                (*xCurves)[i][j-1] = xValue;
                (*yCurves)[i][j-1] = yValue;
            }
        }
    }

    *pumpCurveIndices_out = pumpCurveIndices;

    return nPumpCurves;
}




/**
 * @brief Obtiene y almacena en un vector los diámetros de los tanques
 * 
 * @note CALIBRACIÓN
 */
double* get_tank_diameters(int nJ, int nR, int nT, EN_Project ph){

    double* tank_diameters = (double*)malloc(nT*sizeof(double));

    double tank_diam;

    for (int i = 0; i < nT; i++){
        EN_getnodevalue(ph, nJ+nR+i+1, EN_TANKDIAM, &tank_diam);
        tank_diameters[i] = tank_diam;
    }

    return tank_diameters;

}





/**
 * @brief Obtiene los settings de las válvulas
 * 
 * @warning Se refiere al setting definido en la sección [VALVES] de info_File,
 * que DEBE COINCIDIR CON EL SETTING DE LAS REGLAS DE CONTROL QUE AFECTEN A LA
 * VÁLVULA (si las hay)
 * 
 * @note CALIBRACIÓN
 */
void get_valves_settings(int nPI, int nPU, int nV, double** valves_settings_out, EN_Project ph){

    double* valves_settings = (double*)malloc(nV*sizeof(double));

    char valve_id[10];

    for (int i = 0; i < nV; i++){
        double setting;
        EN_getlinkid(ph, nPI+nPU+i+1, valve_id);
        int err = EN_getlinkvalue(ph, nPI+nPU+i+1, EN_INITSETTING, &setting);
        valves_settings[i] = setting;
    }

    *valves_settings_out = valves_settings;
    
}




/**
 * @brief Clasifica aquellos controles que activen una válvula imponiendo un
 * setting (ni abrir ni cerrar)
 * 
 * @note CALIBRACIÓN
 */
int classify_control_settings_to_edit(int nControls, int nPI, int nPU, 
    Setting_Control_Info* Setting_Controls_Info, EN_Project ph){

    int nSetCtrls = 0; // Contador de cuántos setting en controles vamos a editar

    int out_type; 
    int out_linkIndex;
    double out_setting;
    int out_nodeIndex; 
    double out_level;

    for (int i = 0; i < nControls; i++){
        //Si el control afecta a válvula y no es de abrir (setting = 1) ni de cerrar (setting = 0)
        
        EN_getcontrol(ph, i+1, &out_type, &out_linkIndex, &out_setting, &out_nodeIndex, &out_level);
        if (out_linkIndex > nPI + nPU && out_setting != 1 && out_setting != 0){ 
            //Si es válvula y su setting no es ni de encender ni de apagar
            Setting_Controls_Info[nSetCtrls].index = i+1; // Índice del control según EPANET (1-based)
            Setting_Controls_Info[nSetCtrls].control_type = out_type; // Tipo: 0, 1, 2 ó 3
            Setting_Controls_Info[nSetCtrls].linkIndex = out_linkIndex; // Link al que afecta el control
            Setting_Controls_Info[nSetCtrls].setting = out_setting; // 
            Setting_Controls_Info[nSetCtrls].nodeIndex = out_nodeIndex; // Nodo de control 
            Setting_Controls_Info[nSetCtrls].level = out_level; // Nivel que mar
            Setting_Controls_Info[nSetCtrls].valve_index = out_linkIndex-nPI-nPU-1;

            nSetCtrls++;
        }
    }

    return nSetCtrls; // Devuelve el número de controles cuyo setting vamos a editar
    
}




/**
 * @brief Almacena la información de los controles: tanto asociados a tanques
 * (tipos 0 y 1) como temporales (tipos 2 y 3)
 * 
 * @note EE MULTIPERIODO
 */
void store_controls(int nPI, int nPU, int* nCtrl_01, int* nCtrl_23, long patternStep,
    Ctrl_01 *Controls_01, Ctrl_23 *Controls_23, EN_Project ph){

    int out_type; 
    int out_linkIndex;
    double out_setting;
    int out_nodeIndex; 
    double out_level;

    char node_id[100], link_id[100];

    long start_time, duration, end_time;

    int nTotalControls;
    EN_getcount(ph, EN_CONTROLCOUNT, &nTotalControls);  //número total de controles

    EN_gettimeparam(ph, EN_STARTTIME, &start_time);

    (*nCtrl_01) = 0;
    (*nCtrl_23) = 0;

    for (int i = 0; i < nTotalControls; i++){                

        EN_getcontrol(ph, i+1, &out_type, &out_linkIndex, &out_setting, &out_nodeIndex, &out_level);

        if (out_type == EN_LOWLEVEL || out_type == EN_HILEVEL){  //Control de tipo 0 ó 1

            Controls_01[*nCtrl_01].index = i+1; // Índice del control según EPANET (1-based)
            Controls_01[*nCtrl_01].control_type = out_type; // Tipo: 0 (EN_LOWLEVEL) ó 1 (EN_HILEVEL)
            Controls_01[*nCtrl_01].linkIndex = out_linkIndex; // Link al que afecta el control
            Controls_01[*nCtrl_01].setting = out_setting; 
            Controls_01[*nCtrl_01].nodeIndex = out_nodeIndex;
            Controls_01[*nCtrl_01].level = out_level;
            if (out_linkIndex > nPI + nPU){
                Controls_01[*nCtrl_01].is_valve = 1; 
            } else {
                Controls_01[*nCtrl_01].is_valve = 0; 
            }

            EN_getnodeid(ph, out_nodeIndex, node_id);
            EN_getlinkid(ph, out_linkIndex, link_id);
            const char *direction = (out_type == EN_LOWLEVEL) ? "BELOW" : "ABOVE";

            
            // Construir la frase según el tipo de acción
            if (out_setting == 0) {
                snprintf(Controls_01[*nCtrl_01].sentence,
                    sizeof(Controls_01[*nCtrl_01].sentence),
                    "LINK %s CLOSED IF NODE %s %s %.3f",
                    link_id, node_id, direction, out_level);
            } else if (out_setting == 1) {
                snprintf(Controls_01[*nCtrl_01].sentence,
                    sizeof(Controls_01[*nCtrl_01].sentence),
                    "LINK %s OPEN IF NODE %s %s %.3f",
                    link_id, node_id, direction, out_level);
            }
            else {
                snprintf(Controls_01[*nCtrl_01].sentence,
                    sizeof(Controls_01[*nCtrl_01].sentence),
                    "LINK %s SETTING %.3f IF NODE %s %s %.3f",
                    link_id, out_setting, node_id, direction, out_level);
            }
            
            (*nCtrl_01)++;
        }

        else if (out_type == EN_TIMER || out_type == EN_TIMEOFDAY){ //Solamente almacenamos los controles temporales

            Controls_23[*nCtrl_23].index = i+1; // Índice del control según EPANET (1-based)
            Controls_23[*nCtrl_23].control_type = out_type; // Tipo: 2 (EN_TIMER) ó 3 (EN_TIMEOFDAY)
            Controls_23[*nCtrl_23].linkIndex = out_linkIndex; // Link al que afecta el control
            Controls_23[*nCtrl_23].setting = out_setting; // Acción (encender, apagar, activar)

            if (out_type == EN_TIMER){
                double period = floor(out_level/patternStep);
                double sec_in_period = fmod(out_level,patternStep);
                Controls_23[*nCtrl_23].period = period; // Periodo en el que hay que activar esta regla
                Controls_23[*nCtrl_23].sec_in_period = sec_in_period; // Tiempo (en seg.) dentro del periodo para activar regla
            } else {
                double period = floor((out_level-start_time)/patternStep);
                double sec_in_period = fmod((out_level-start_time),patternStep);
                Controls_23[*nCtrl_23].period = period; // Periodo en el que hay que activar esta regla
                Controls_23[*nCtrl_23].sec_in_period = sec_in_period; // Tiempo (en seg.) dentro del periodo para activar regla
            }

            if (out_linkIndex > nPI + nPU){
                Controls_23[*nCtrl_23].is_valve = 1; 
            } else {
                Controls_23[*nCtrl_23].is_valve = 0; 
            }


            EN_getlinkid(ph, out_linkIndex, link_id);
            const char *direction = (out_type == EN_TIMER) ? "AT TIME" : "AT CLOCKTIME";
            const char *extra = (out_type == EN_TIMER) ? "(chronometer from start)" : "";

            int hours = floor(out_level/3600);
            int minutes = floor(fmod(out_level,3600)/60);
            int seconds = floor(fmod(out_level, 60));

            
            // Construir la frase según el tipo de acción
            if (out_setting == 0) {
                snprintf(Controls_23[*nCtrl_23].sentence, sizeof(Controls_23[*nCtrl_23].sentence),
                    "LINK %s CLOSED %s %d:%d:%d %s", link_id, direction, hours, minutes, seconds, extra);
            } else if (out_setting == 1) {
                snprintf(Controls_23[*nCtrl_23].sentence, sizeof(Controls_23[*nCtrl_23].sentence),
                    "LINK %s OPEN %s %d:%d:%d %s", link_id, direction, hours, minutes, seconds, extra);
            }
            else {
                snprintf(Controls_23[*nCtrl_23].sentence, sizeof(Controls_23[*nCtrl_23].sentence),
                    "LINK %s SETTING %.3f %s %d:%d:%d %s", 
                    link_id, out_setting, direction, hours, minutes, seconds, extra);
            }

            (*nCtrl_23)++;
        }
    }

}




/**
 * @brief Estructura que recoge SOLAMENTE las tuberías, bombas y válvulas
 * asociadas a ALGUNA regla de control. Sirve para realizar en todo momento
 * (o sea, a cualquier cambio que haya) el seguimiento del estado de dichos
 * elementos.
 * 
 * @note EE MULTIPERIODO
 */
void summarize_links_affected_by_controls(int nPI, int nPU, int nCtrl_01, int nCtrl_23, 
    Ctrl_01 *Controls_01, Ctrl_23 *Controls_23, Ctrl_Links_Summary* Controls_Links_Summary, EN_Project ph){

    double setting;

    int maxControls = nCtrl_01 + nCtrl_23;
    int *tempPipePump = (int*)malloc(maxControls * sizeof(int));
    int *tempValves   = (int*)malloc(maxControls * sizeof(int));
    int nPipesPumps = 0, nValves = 0;

    // Procesar controles tipo 0 ó 1 (EN_LOWLEVEL / EN_HILEVEL)
    for (int i = 0; i < nCtrl_01; i++) {
        int linkIndex = Controls_01[i].linkIndex;
        int already_added = 0;

        if (linkIndex > nPI + nPU) {
            // Válvula
            for (int j = 0; j < nValves; j++)
                if (tempValves[j] == linkIndex) { already_added = 1; break; }
            if (!already_added)
                tempValves[nValves++] = linkIndex;
        } else {
            // Tubería o bomba
            for (int j = 0; j < nPipesPumps; j++)
                if (tempPipePump[j] == linkIndex) { already_added = 1; break; }
            if (!already_added)
                tempPipePump[nPipesPumps++] = linkIndex;
        }
    }

    // Procesar controles tipo 2 ó 3 (EN_TIMER / EN_TIMEOFDAY)
    for (int i = 0; i < nCtrl_23; i++) {
        int linkIndex = Controls_23[i].linkIndex;
        int already_added = 0;

        if (linkIndex > nPI + nPU) {
            for (int j = 0; j < nValves; j++)
                if (tempValves[j] == linkIndex) { already_added = 1; break; }
            if (!already_added)
                tempValves[nValves++] = linkIndex;
        } else {
            for (int j = 0; j < nPipesPumps; j++)
                if (tempPipePump[j] == linkIndex) { already_added = 1; break; }
            if (!already_added)
                tempPipePump[nPipesPumps++] = linkIndex;
        }
    }

    // Copiar resultados únicos en vectores finales
    Controls_Links_Summary->pipe_pump_indices = (int*)malloc(nPipesPumps * sizeof(int));
    Controls_Links_Summary->valve_indices = (int*)malloc(nValves * sizeof(int));
    Controls_Links_Summary->last_valve_setting = (double*)malloc(nValves * sizeof(double));
    Controls_Links_Summary->pipe_pump_status = (int*)malloc(nPipesPumps * sizeof(int));
    Controls_Links_Summary->valve_status = (int*)malloc(nValves * sizeof(int));
    Controls_Links_Summary->nPipePump = nPipesPumps;
    Controls_Links_Summary->nValves = nValves;

    for (int i = 0; i < nPipesPumps; i++){
        Controls_Links_Summary->pipe_pump_indices[i] = tempPipePump[i];
        Controls_Links_Summary->pipe_pump_status[i] = 1;
    }
    for (int i = 0; i < nValves; i++){
        Controls_Links_Summary->valve_indices[i] = tempValves[i];
        Controls_Links_Summary->valve_status[i] = 2;
        Controls_Links_Summary->last_valve_setting[i] = -1;
    }

    free(tempPipePump);
    free(tempValves);

}





/**
 * @brief Sirve para:
 * 1) Almacenar el estado inicial declarado en el .inp de los elementos de control
 * 2) crear un nuevo .inp con todo igual excepto la sección STATUS, en la que las 
 * válvulas no aparecen para interpretarse siempre como "activas", y así luego poder 
 * imponer su estado (si hiciera falta, al ser cerrado / abierto)
 * 
 * @warning RELLENAAAAAR
 * 
 * @note EE MULTIPERIODO
 */
void get_initial_status_clean_section_intermediate_file(Ctrl_Links_Summary* Controls_Links_Summary, 
    const char* intermediate_File, EN_Project ph, int lang){
    
    double status;
    int status_integer;

    // --- Estados iniciales de tuberías y bombas
    for (int i = 0; i < Controls_Links_Summary->nPipePump; i++){
        EN_getlinkvalue(ph, Controls_Links_Summary->pipe_pump_indices[i], EN_INITSTATUS, &status);
        status_integer = (int)status;
        Controls_Links_Summary->pipe_pump_status[i] = status_integer;
    }


    //  Leer los IDs de las válvulas desde EPANET
    char **valveIDs = (char**)malloc(Controls_Links_Summary->nValves * sizeof(char*));
    for (int i = 0; i < Controls_Links_Summary->nValves; i++) {
        valveIDs[i] = (char*)malloc(100 * sizeof(char));
        EN_getlinkid(ph, Controls_Links_Summary->valve_indices[i], valveIDs[i]);
    }


    //  Abrir el archivo .inp
    FILE *file = fopen(intermediate_File, "r");
    if (!file) {
        printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "intermediate_File", intermediate_File);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char tmpFile[512];
    sprintf(tmpFile, "%s.tmp", intermediate_File);

    FILE *ftmp = fopen(tmpFile, "w");
    if (!ftmp) {
        printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "tmpFile", tmpFile);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char line[200];
    int inStatusSection = 0;

    // Por defecto, todas las válvulas empiezan ACTIVAS (si no aparecen en [STATUS])
    int *found = (int*)calloc(Controls_Links_Summary->nValves, sizeof(int));

    while (fgets(line, 200, file)) {

        // Detectar inicio / fin de sección
        if (strncmp(line, "[STATUS]", 8) == 0) {
            inStatusSection = 1;
            // printf("Estamos en [STATUS] \n\n");
            // system("pause");
            fputs(line, ftmp);
            continue;
        }
        if (line[0] == '[' && strncmp(line, "[STATUS]", 8) != 0) {
            // printf("Linea: %s \n", line);
            inStatusSection = 0;
        }

        int eliminar = 0;

        if (inStatusSection) {
            char id[200], status_str[200];
            if (sscanf(line, "%s %s", id, status_str) == 2) {
                for (int i = 0; i < Controls_Links_Summary->nValves; i++) {
                    if (strcmp(id, valveIDs[i]) == 0) {
                        found[i] = 1;
                        eliminar = 1;

                        if (strcasecmp(status_str, "OPEN") == 0) {
                            Controls_Links_Summary->valve_status[i] = 1;
                            // printf("Valvula %s declarada como OPEN -> eliminada\n", id);
                        }
                        else if (strcasecmp(status_str, "CLOSED") == 0) {
                            Controls_Links_Summary->valve_status[i] = 0;
                            // printf("Valvula %s declarada como CLOSED -> eliminada\n", id);
                        }
                        else {
                            eliminar = 0;
                        }
                        break;
                    }
                }
            }
        }

        if (!eliminar)
            fputs(line, ftmp);
    }

    fclose(file);
    fclose(ftmp);

    // Sustituir el archivo original por el temporal
    remove(intermediate_File);
    rename(tmpFile, intermediate_File);

    // --- Las válvulas no encontradas en [STATUS] están ACTIVAS
    for (int i = 0; i < Controls_Links_Summary->nValves; i++) {
        if (!found[i]) {
            Controls_Links_Summary->valve_status[i] = 2;
            // printf("Válvula %s no declarada en [STATUS] → se asume ACTIVA\n", valveIDs[i]);
        }
    }

    // --- Liberar memoria
    for (int i = 0; i < Controls_Links_Summary->nValves; i++)
        free(valveIDs[i]);
    free(valveIDs);
    free(found);
}




/**
 * @brief Almacena el nivel piezométrico de cada tanque que tenga asociado un
 * patrón de nivel piezométrico. Se impondrá el valor correspondiente al inicio
 * del ajuste de cada periodo
 * 
 * @note EE MULTIPERIODO
 */
int store_reservoir_patterns_info(int T, int nJ, int nR, 
    Reservoir_Pattern_Info* Reservoir_Patterns_Info, EN_Project ph){

    int pattern_index_int;
    double pattern_index_double;
    double base_head, pattern_value;
    int cont = 0;

    for (int i = 0; i < nR; i++){
        EN_getnodevalue(ph, nJ+i+1, EN_PATTERN, &pattern_index_double);
        pattern_index_int = (int)pattern_index_double; 
        
        if (pattern_index_int > 0){

            Reservoir_Patterns_Info[cont].reservoir_index = nJ+i+1;

            EN_getnodevalue(ph, nJ+i+1, EN_ELEVATION, &base_head);
            for (int t = 0; t < T; t++){
                EN_getpatternvalue(ph, pattern_index_int, t+1, &pattern_value);
                Reservoir_Patterns_Info[cont].head[t] = base_head*pattern_value;
            }
            printf("\n\n");

            cont++;
        }
    }

    return cont;

}





// FUNCIONES PARA LECTURA y ALMACENAMIENTO de MEDIDAS / PSEUDOMEDIDAS
// ------------------------------------------------------------------


/**
 * @brief Define matrices nulas de la dimenasión adecuada para rellenar datos de 
 * incertidumbre y medidas tras leer los csv de medidas / pseudomedidas. También
 * define un vector nulo donde indicaremos saber qué elementos tienen medida
 * (útil para definir futuras estructuras)
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
void define_initialize_vectors_matrices_for_sensors_info(int num_elements, int T, int** logic_vector,
    double*** info_matrix, double*** uncert_matrix){

    (*logic_vector) = (int*)malloc(num_elements*sizeof(int)); //Vector lógico para saber si hay medida o no en cada elemento
    (*info_matrix) = (double**) malloc(num_elements * sizeof(double*)); //Matriz para almacenar medidas 
    (*uncert_matrix) = (double**) malloc(num_elements * sizeof(double*)); //Matriz para almacenar CVs de cada medida

    //Inicialización
    for (int i = 0; i < num_elements; i++) {
        (*info_matrix)[i] = (double*) malloc(T * sizeof(double));
        (*uncert_matrix)[i] = (double*) malloc(T * sizeof(double));
        for (int t = 0; t < T; t++) {
            (*info_matrix)[i][t] = 0.0;  
            (*uncert_matrix)[i][t] = -1.0; 
        }
    }

}




/**
 * @brief Lee la información del csv de medidas / pseudomedidas de nivel de agua
 * en tanques y rellena las matrices y vectores necesarios
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
int store_wlm_info(int T, int nR, int nT, int nJ, const char* wlm_File, int* wlm_logic, 
    double** wlm_values, double** CV_wlm, EN_Project ph, int lang) {
    
    if (wlm_File == NULL) {
        printf("%s", messages[MSG_WARN_NO_WLM_FILE][lang]);
        SENSEI_PAUSE();
        return 0;
    }

    FILE* file = fopen(wlm_File, "r");
    if (!file) {
        printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "wlm_File", wlm_File);
        if (lang == 0) printf("\033[0;31mRevisa el nombre o el contenido del archivo\033[0m \n");
        else printf("\033[0;31mCheck the file name or its contents\033[0m \n");
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char line[20000];
    int num_sensors = 0;
    int element_index = -1;         // Índice EPANET (1-based)
    int current_logic_index = -1;   // Índice lógico en matrices = element_index - nJ - nR - 1

    double cv = 0.01, measured_value;
    int is_complementary_row = 0;

    for (int i = 0; i < nT; i++) {
        wlm_logic[i] = 0;
    }

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;


        // Eliminar BOM si está presente
        if (strncmp(line, "\xEF\xBB\xBF", 3) == 0) {
            memmove(line, line + 3, strlen(line) - 2);
        }

        char* token;
        char* ptr = line;
        int token_count = 0;

        while ((token = strsep(&ptr, ";")) != NULL) {
            token = trim(token);
            if (token_count == 0) {
                if (strlen(token) == 0) {
                    is_complementary_row = 1;
                    // usamos el último índice lógico leído
                } else {
                    is_complementary_row = 0;
                    int err = EN_getnodeindex(ph, token, &element_index);
                    if (err != 0) {
                        printf("\033[0;31mERROR: Nodo %s no encontrado en info_File\033[0m\n", token);
                        printf("\033[0;31mRevisa el contenido del wlm_File (%s) \033[0m\n", wlm_File);
                        SENSEI_PAUSE();
                        exit(EXIT_FAILURE);
                    }
                    current_logic_index = element_index - nJ - nR - 1;
                    if (current_logic_index < 0) {
                        printf("\033[0;31mERROR: Nodo %s no es un tanque\033[0m\n", token);
                        printf("\033[0;31mRevisa el contenido del wlm_File (%s) \033[0m\n", wlm_File);
                        SENSEI_PAUSE();
                        exit(EXIT_FAILURE);
                    }
                    wlm_logic[current_logic_index] = 1;
                }
            } else if (token_count == 1 && !is_complementary_row) {
                cv = (strlen(token) == 0) ? 0.01 : atof(token);
            } else {
                int t = token_count - 2;
                if (t < T && current_logic_index >= 0) {
                    if (is_complementary_row) {
                        if (strlen(token) > 0) {
                            double exceptional_cv = atof(token);
                            CV_wlm[current_logic_index][t] = exceptional_cv;
                        }
                    } else {
                        if (strlen(token) == 0) {
                            measured_value = 0.0;
                        } else {
                            measured_value = atof(token);
                            CV_wlm[current_logic_index][t] = cv;
                        }

                        wlm_values[current_logic_index][t] = measured_value;
                    }
                }
            }

            token_count++;
        }

        if (!is_complementary_row && token_count > 0) {
            num_sensors++;
        }
    }

    fclose(file);
    return num_sensors;
}




/**
 * @brief Lee la información del csv de medidas / pseudomedidas de demanda en
 * junctions de demanda y rellena las matrices y vectores necesarios
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
void store_dm_info(int T, int nDM, int nJ_Dem, int* ind_J_Dem, const char* dm_File, 
    double** dm_values, double** CV_dm, EN_Project ph, int lang) {
    
    FILE* file = fopen(dm_File, "r");
    if (!file) {
        printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "dm_File", dm_File);
        if (lang == 0) printf("\033[0;31mRevisa el nombre o el contenido del archivo\033[0m \n");
        else printf("\033[0;31mCheck the file name or its contents\033[0m \n");
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char line[20000];
    int num_sensors = 0;
    int element_index = -1;           // índice real en EPANET
    int current_logic_index = -1;   // índice j en [0, nJ_Dem)

    double cv = 0.01, measured_value;
    int is_complementary_row = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;


        // --- Eliminamos BOM si está presente ---
        if (strncmp(line, "\xEF\xBB\xBF", 3) == 0) {
            memmove(line, line + 3, strlen(line) - 2);
        }

        char* token;
        char* ptr = line;
        int token_count = 0;

        while ((token = strsep(&ptr, ";")) != NULL) {
            token = trim(token);
            if (token_count == 0) {
                // printf("%s) \n", token);
                if (strlen(token) == 0) {
                    is_complementary_row = 1;
                    // current_logic_index se conserva de línea anterior
                } else {
                    is_complementary_row = 0;
                    int err = EN_getnodeindex(ph, token, &element_index);
                    if (err != 0) {
                        printf(messages[MSG_ERR_NODE_NOT_FOUND][lang], token, "dm_File", dm_File);
                        SENSEI_PAUSE();
                        exit(EXIT_FAILURE);
                    }

                    // Buscar índice lógico correspondiente en ind_J_Dem[]
                    current_logic_index = -1;
                    for (int j = 0; j < nJ_Dem; j++) {
                        if (ind_J_Dem[j] == element_index) {
                            current_logic_index = j;
                            break;
                        }
                    }
                    if (current_logic_index == -1) {
                        printf(messages[MSG_ERR_NODE_NOT_DEMAND_JUNCTION][lang], token, dm_File);
                        SENSEI_PAUSE();
                        exit(EXIT_FAILURE);
                    }
                }
            } else if (token_count == 1 && !is_complementary_row) {
                cv = (strlen(token) == 0) ? 0.01 : atof(token);
            } else {
                int t = token_count - 2;
                if (t < T && current_logic_index != -1) {  
                    if (is_complementary_row) {
                        if (strlen(token) > 0) {
                            double exceptional_cv = atof(token);
                            CV_dm[current_logic_index][t] = exceptional_cv;
                        }
                    } else {
                        if (strlen(token) == 0) {
                            measured_value = 0.0;
                        } else {
                            measured_value = atof(token);
                            CV_dm[current_logic_index][t] = cv;
                        }

                        dm_values[current_logic_index][t] = measured_value;
                    }
                }
            }

            token_count++;
        }

        if (!is_complementary_row && token_count > 0) {
            num_sensors++;
        }
    }

    fclose(file);

    if (num_sensors != nDM) {
        printf(messages[MSG_ERR_DM_COUNT_MISMATCH][lang], nDM, num_sensors, dm_File);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }
}



/**
 * @brief Lee la información del csv de medidas / pseudomedidas de presión en
 * junctions rellena las matrices y vectores necesarios
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
int store_pm_info(int T, int nJ, const char* pm_File, int* pm_logic, double** pm_values, 
    double** CV_pm, EN_Project ph, int lang) {
    
    if (pm_File == NULL) {
        printf("%s", messages[MSG_WARN_NO_PM_FILE][lang]);
        SENSEI_PAUSE();
        return 0;
    }

    FILE* file = fopen(pm_File, "r");
    if (!file) {
        printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "pm_File", pm_File);
        if (lang == 0) printf("\033[0;31mRevisa el nombre o el contenido del archivo\033[0m \n");
        else printf("\033[0;31mCheck the file name or its contents\033[0m \n");
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char line[20000];
    int num_sensors = 0;
    int element_index = -1;
    int current_logic_index = -1;

    double cv = 0.01, measured_value;
    int is_complementary_row = 0;

    for (int i = 0; i < nJ; i++) {
        pm_logic[i] = 0;
    }

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        // --- Eliminamos BOM si está presente en la primera línea ---
        if (strncmp(line, "\xEF\xBB\xBF", 3) == 0) {
            memmove(line, line + 3, strlen(line) - 2);
        }

        char* token;
        char* ptr = line;
        int token_count = 0;

        while ((token = strsep(&ptr, ";")) != NULL) {
            token = trim(token);
            if (token_count == 0) {
                if (strlen(token) == 0) {
                    is_complementary_row = 1;
                } else {
                    is_complementary_row = 0;
                    int err = EN_getnodeindex(ph, token, &element_index);
                    if (err != 0) {
                        printf(messages[MSG_ERR_NODE_NOT_FOUND][lang], token, "pm_File", pm_File);
                        SENSEI_PAUSE();
                        exit(EXIT_FAILURE);
                    }
                    current_logic_index = element_index - 1;
                    if (current_logic_index >= nJ) {
                        printf(messages[MSG_ERR_NODE_NOT_JUNCTION][lang], token, pm_File);
                        SENSEI_PAUSE();
                        exit(EXIT_FAILURE);
                    }
                    pm_logic[current_logic_index] = 1;
                }
            } else if (token_count == 1 && !is_complementary_row) {
                cv = (strlen(token) == 0) ? 0.01 : atof(token);
            } else {
                int t = token_count - 2;
                if (t < T && current_logic_index >= 0) {
                    if (is_complementary_row) {
                        if (strlen(token) > 0) {
                            double exceptional_cv = atof(token);
                            CV_pm[current_logic_index][t] = exceptional_cv;
                        }
                    } else {
                        if (strlen(token) == 0) {
                            measured_value = 0.0;
                        } else {
                            measured_value = atof(token);
                            CV_pm[current_logic_index][t] = cv;
                        }

                        pm_values[current_logic_index][t] = measured_value;
                    }
                }
            }
            token_count++;
        }

        if (!is_complementary_row && token_count > 0) {
            num_sensors++;
        }
    }

    fclose(file);
    return num_sensors;
}






/**
 * @brief Lee la información del csv de medidas / pseudomedidas de flujo en
 * enlaces rellena las matrices y vectores necesarios.
 * 
 * @param avg Indica si se desea leer archivo de medidas de flujo INSTANTÁNEO
 * (avg = 0) o PROMEDIADO (avg = 1)
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
int store_fm_info(int T, int nL, const char* fm_File, int* fm_logic, double** fm_values, 
    double** CV_fm, int avg, EN_Project ph, int lang) {
    
    if (fm_File == NULL) {
        if (avg == 0){
            printf("%s", messages[MSG_WARN_NO_FM_FILE][lang]);
        } else {
            printf("%s", messages[MSG_WARN_NO_FMAVG_FILE][lang]);
        }
        SENSEI_PAUSE();
        return 0;
    }

    FILE* file = fopen(fm_File, "r");
    if (!file) {
        if (avg == 0){
            printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "fm_File", fm_File);
            if (lang == 0) printf("\033[0;31mRevisa el nombre o el contenido del archivo\033[0m \n");
            else printf("\033[0;31mCheck the file name or its contents\033[0m \n");
        } else {
            printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "fm_avg_File", fm_File);
            if (lang == 0) printf("\033[0;31mRevisa el nombre o el contenido del archivo\033[0m \n");
            else printf("\033[0;31mCheck the file name or its contents\033[0m \n");
        }
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char line[20000];
    int num_sensors = 0;
    int element_index = -1;
    int current_logic_index = -1;

    double cv = 0.01, measured_value;
    int is_complementary_row = 0;

    for (int i = 0; i < nL; i++) {
        fm_logic[i] = 0;
    }

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        // --- Eliminamos BOM si está presente en la primera línea ---
        if (strncmp(line, "\xEF\xBB\xBF", 3) == 0) {
            memmove(line, line + 3, strlen(line) - 2);
        }

        char* token;
        char* ptr = line;
        int token_count = 0;

        while ((token = strsep(&ptr, ";")) != NULL) {
            token = trim(token);
            if (token_count == 0) {
                if (strlen(token) == 0) {
                    is_complementary_row = 1;
                } else {
                    is_complementary_row = 0;
                    int err = EN_getlinkindex(ph, token, &element_index);
                    if (err != 0) {
                        if (avg == 0){
                            printf(messages[MSG_ERR_LINK_NOT_FOUND][lang], token, "fm_File", fm_File);
                        } else {
                            printf(messages[MSG_ERR_LINK_NOT_FOUND][lang], token, "fm_avg_File", fm_File);
                        }
                        SENSEI_PAUSE();
                        exit(EXIT_FAILURE);
                    }
                    current_logic_index = element_index - 1;
                    fm_logic[current_logic_index] = 1;
                }
            } else if (token_count == 1 && !is_complementary_row) {
                cv = (strlen(token) == 0) ? 0.01 : atof(token);
            } else {
                int t = token_count - 2;
                if (t < T && current_logic_index >= 0) {
                    if (is_complementary_row) {
                        if (strlen(token) > 0) {
                            double exceptional_cv = atof(token);
                            CV_fm[current_logic_index][t] = exceptional_cv;
                        }
                    } else {
                        if (strlen(token) == 0) {
                            measured_value = 0.0;
                        } else {
                            measured_value = atof(token);
                            CV_fm[current_logic_index][t] = cv;
                        }

                        fm_values[current_logic_index][t] = measured_value;
                    }
                }
            }

            token_count++;
        }

        if (!is_complementary_row && token_count > 0) {
            num_sensors++;
        }
    }

    fclose(file);
    return num_sensors;
}





/**
 * @brief Lee el archivo .csv de patrones y rellena la información necesaria
 * (análogo y alternativa a dm_File)
 * 
 * @warning FALTA ARREGLAR LOS MENSAJES!!
 * 
 * @note CALIBRACIÓN
 */
void store_patterns_info(int T, int nDemPat, const char* patterns_File, double** dm_values, 
    double** CV_dm, EN_Project ph) {

    FILE* file = fopen(patterns_File, "r");
    if (!file) {
        printf("\033[0;31mError abriendo archivo patterns_File\033[0m \n");
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char line[20000];
    int current_logical_index = -1;

    double cv = 0.01, measured_value;
    int num_valid_patterns = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        // --- Eliminar BOM si está presente (solo en la primera línea relevante) ---
        if (strncmp(line, "\xEF\xBB\xBF", 3) == 0) {
            memmove(line, line + 3, strlen(line) - 2);
        }

        char* token;
        char* ptr = line;
        int token_count = 0;
        int is_complementary_row = 0;
        char pattern_id[100] = "";
        int pattern_index = -1;

        while ((token = strsep(&ptr, ";")) != NULL) {
            token = trim(token);
            if (token_count == 0) {
                if (strlen(token) == 0) {
                    is_complementary_row = 1;
                } else {
                    strcpy(pattern_id, token);
                    is_complementary_row = 0;

                    // Obtener índice del patrón según el .inp
                    if (EN_getpatternindex(ph, pattern_id, &pattern_index) != 0 ||
                        pattern_index <= 0 || pattern_index > nDemPat) {
                        fprintf(stderr, "Patrón no encontrado o fuera de rango: %s\n", pattern_id);
                        pattern_index = -1;
                    } else {
                        pattern_index--;  // Convertimos a índice base 0
                        current_logical_index = pattern_index;
                        num_valid_patterns++;
                    }
                }
            } else if (token_count == 1 && !is_complementary_row) {
                cv = (strlen(token) == 0) ? 0.01 : atof(token);
            } else {
                int t = token_count - 2;
                if (t < T && current_logical_index != -1) {
                    if (is_complementary_row) {
                        if (strlen(token) > 0) {
                            double exceptional_cv = atof(token);
                            CV_dm[current_logical_index][t] = exceptional_cv;
                        }
                    } else {
                        measured_value = atof(token);
                        CV_dm[current_logical_index][t] = cv;
                        dm_values[current_logical_index][t] = measured_value;
                    }
                }
            }
            token_count++;
        }
    }

    fclose(file);

    if (num_valid_patterns != nDemPat) {
        printf("\033[0;31mADVERTENCIA: Se esperaban %d patrones, pero se leyeron %d correctamente del archivo. \033[0m\n",
               nDemPat, num_valid_patterns);
        SENSEI_PAUSE();
    }
}





/**
 * @brief Cuenta el número de medidas / pseudomedidas recogidas de cada tipo 
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
int count_measurements(int num_elements, int T, double** uncert_matrix){
    
    int measurements_count = 0; 
    for (int i = 0; i < num_elements; i++){
        for (int t = 0; t < T; t++){
            if (uncert_matrix[i][t] != -1.0) measurements_count++;
        }
    }

    return measurements_count;
}





/**
 * @brief Devuelve vector con los índices (NO SIEMPRE INDEXADOS DESDE EL INICIO) de 
 * los elementos que tienen medida de cada alguno de los tipos WLM, PM, FM o FM_avg 
 * (Útil para indexar filas del jacobiano, almacenar y manejar valores e imprimir
 * información por print)
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
int* get_sensor_positions_vector(int num_sensors, int num_elements, int* logic_vector){
    int* positions_vector = (int*)malloc(num_sensors * sizeof(int));

    int element_count = 0;
    for (int i = 0; i < num_elements; i++) {
        if (logic_vector[i] == 1) {
            positions_vector[element_count] = i+1;  
            element_count++;
            if (element_count >= num_sensors) break;  
        }
    }

    return positions_vector;
}



// FUNCIONES PARA DEFINIR ESTRUCTURAS DE CV, MEDIDAS / PSEUDOMEDIDAS y PESOS
// -------------------------------------------------------------------------



/**
 * @brief Almacena el CV de las variables medidas en cada periodo
 * considerado (en base a la lectura previa de los archivos .csv)
 * 
 * @param m Núm. de variables medidas (contando también pseudomedidas)
 * @param T Núm. de intervalos o instantes considerados
 * 
 * @return Matriz de tamaño (m,T)
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
double** get_CV_matrix(int T, int m, int nDM, int nWLM, int nPM, int nFM, int nFM_avg,
    int* wlm_positions, int* pm_positions, int* fm_positions, int* fm_avg_positions, 
    double** CV_dm, double** CV_wlm, double** CV_pm, 
    double** CV_fm, double** CV_fm_avg){

    double** CV_matrix = (double**)malloc(m * sizeof(double*));
    for (int i = 0; i < m; i++) {
        CV_matrix[i] = (double*)malloc(T * sizeof(double));
    }

    for (int t = 0; t < T; t++){
        for (int i = 0; i < nDM; i++){
            CV_matrix[i][t] = CV_dm[i][t]; 
        }

        for (int i = 0; i < nWLM; i++) {
            CV_matrix[i+nDM][t] = CV_wlm[wlm_positions[i]-1][t];
        }
 
        for (int i = 0; i < nPM; i++){
            CV_matrix[i+nDM+nWLM][t] = CV_pm[pm_positions[i]-1][t]; 
        }

        for (int i = 0; i < nFM; i++){
            CV_matrix[i+nDM+nWLM+nPM][t] = CV_fm[fm_positions[i]-1][t];  
        }

        for (int i = 0; i < nFM_avg; i++){
            CV_matrix[i+nDM+nWLM+nPM+nFM][t] = CV_fm_avg[fm_avg_positions[i]-1][t];  
        }
        
    }

    return CV_matrix;

}





/**
 * @brief Devuelve un vector de medidas / pseudomedidas ordenadas por periodos: "m" medidas en el
 * primer periodo, "m" en el segundo, etc.
 * 
 * @param m Núm. de variables medidas (contando también pseudomedidas)
 * @param T Núm. de intervalos o instantes considerados
 * 
 * @return Vector de tamaño m*T
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
double* get_measurements_vector(int T, int m, int nDM, int nWLM, int nPM, int nFM, int nFM_avg,
    int* wlm_positions, int* pm_positions, int* fm_positions, int* fm_avg_positions,
    double** dm_values, double** wlm_values, double** pm_values, double** fm_values, double** fm_avg_values){

    double* measurements_vector = (double*)malloc(m*T * sizeof(double));

    for (int t = 0; t < T; t++){
        for (int i = 0; i < nDM; i++){
            measurements_vector[i+m*t] = dm_values[i][t]; 
        }

        for (int i = 0; i < nWLM; i++) {
            measurements_vector[i+nDM+m*t] = wlm_values[wlm_positions[i]-1][t];
        }

        for (int i = 0; i < nPM; i++){
            measurements_vector[i+nDM+nWLM+m*t] = pm_values[pm_positions[i]-1][t]; 
        }

        for (int i = 0; i < nFM; i++){
            measurements_vector[i+nDM+nWLM+nPM+m*t] = fm_values[fm_positions[i]-1][t];  
        }     

        for (int i = 0; i < nFM_avg; i++){
            measurements_vector[i+nDM+nWLM+nPM+nFM+m*t] = fm_avg_values[fm_avg_positions[i]-1][t];  
        }   
    }

    return measurements_vector;
}







/**
 * @brief Devuelve un vector con el peso de cada medida / pseudomedida,
 * ordenadas por periodos: "m" pesos del primer periodo, "m" del segundo, etc.
 * 
 * @param m Núm. de variables medidas (contando también pseudomedidas)
 * @param T Núm. de intervalos o instantes considerados
 * 
 * @return Vector de tamaño m*T
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
double* compute_weights_vector(int T, int m, double* z, double** CV_matrix){

    double* weights_vector = (double*)malloc((m*T) * sizeof(double));

    double z_i_t;
    for (int i = 0; i < m; i++){
        for (int t = 0; t < T; t++){
            z_i_t = z[i+m*t];
            if (CV_matrix[i][t] != -1.0){ //Si el valor se mide:
                if (fabs(z_i_t) < 0.0001){ //Si medimos 0, ponemos peso 0.1
                    weights_vector[i+t*m] = 0.05;   //arbitrario  
                } else { //Si es no nulo, ponemos peso 1/(CV*z)^2. Es decir, la inversa de la varianza de la medida
                    weights_vector[i+t*m] = 1/pow(z_i_t*CV_matrix[i][t],2);
                }
            } else { //Si el valor NO se mide (ej: una presión no se mide en un determinado instante)
                weights_vector[i+t*m] = 0;  //Ponemos peso 0
            }
        }
    }
    
    return weights_vector;

}






/**
 * @brief Devuelve matriz diagonal (sparse) de pesos (W), raíz cuadrada de los
 * pesos (sqrt(W)) y varianza de las medidas / pseudomedidas o inversa de los
 * pesos (R)
 * 
 * @param diag_option 0 para W, 1 para sqrt(W) y 2 para R
 * @param m Núm. de variables medidas (contando también pseudomedidas)
 * @param T Núm. de intervalos o instantes considerados
 * 
 * @return Matriz diagonal dispersa (m,T)
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
cholmod_sparse* diagonal_matrix_from_vector(int T, int m, double* weights_vector, int diag_option, cholmod_common *c){

    int size = m*T;
    cholmod_sparse *diagonal_matrix = cholmod_allocate_sparse(size, size, size, 1, 1, 0, CHOLMOD_REAL, c);

    int *diagonal_matrix_p = (int *) diagonal_matrix->p;    // Índices de columna
    int *diagonal_matrix_i = (int *) diagonal_matrix->i;    // Índices de fila
    double *diagonal_matrix_x = (double *) diagonal_matrix->x;  // Valores de la matriz

    for (int i = 0; i < size; i++) {
        diagonal_matrix_p[i] = i;      
        diagonal_matrix_i[i] = i;    
        if (diag_option == 0){
            diagonal_matrix_x[i] = weights_vector[i];  
        } else if (diag_option == 1){
            diagonal_matrix_x[i] = sqrt(weights_vector[i]);  
        } else if (diag_option == 2){
            diagonal_matrix_x[i] = 1/weights_vector[i];  
        } else if (diag_option == 3){
            diagonal_matrix_x[i] = 1/sqrt(weights_vector[i]);  
        }  
    }

    diagonal_matrix_p[size] = size; 

    return diagonal_matrix;
}




// FUNCIONES QUE SE USAN EN LOS ALGORITMOS DE CALIBRACIÓN / EE
// -----------------------------------------------------------


/**
 * @brief Define la estructura para las variables provisionales (donde
 * cargaremos los valores calculados en cada iteración), siempre que se
 * hayan activado las opciones correspondientes
 * 
 * @note CALIBRACIÓN
 */
void define_provisional_variables_calibration(Calib_Variables *Prov_Vars, Options Opt, int nDemPat,
    int T, int nJ_Dem, int nPumpCurves, int nV, int nCP, int mod_MP, int nMP, int nPI, int nT){

    if (Opt.mod_patterns == 1){
        Prov_Vars->demand_patterns = (double**)malloc(T * sizeof(double*));
        for (int i = 0; i < T; i++) {
            Prov_Vars->demand_patterns[i] = (double*)malloc(nDemPat*sizeof(double));
        }
    }

    if (Opt.mod_base_demands == 1){
        Prov_Vars->base_demands = (double*) malloc(nJ_Dem*sizeof(double));
    }

    if (Opt.mod_curves_X == 1){
        Prov_Vars->xPumpCurves = (double**)malloc(nPumpCurves * sizeof(double*));
        for (int i = 0; i < nPumpCurves; i++) {
            Prov_Vars->xPumpCurves[i] = (double*)malloc(3*sizeof(double));
        }
    }

    if (Opt.mod_curves_Y == 1){
        Prov_Vars->yPumpCurves = (double**)malloc(nPumpCurves * sizeof(double*));
        for (int i = 0; i < nPumpCurves; i++) {
            Prov_Vars->yPumpCurves[i] = (double*)malloc(3*sizeof(double));
        }
    }

    if (Opt.mod_valves_settings == 1){
        Prov_Vars->valves_settings = (double*)malloc(nV*sizeof(double));
    }

    if (Opt.mod_common_roughness_DW == 1){
        Prov_Vars->common_DW_coefs = (double*)malloc(nCP*sizeof(double));
    }

    if (Opt.mod_common_roughness_HW == 1){
        Prov_Vars->common_HW_coefs = (double*)malloc(nCP*sizeof(double));
    }

    if (mod_MP == 1){
        Prov_Vars->roughness_coefficients = (double*)malloc(nMP*sizeof(double));
    } else {
        Prov_Vars->roughness_coefficients = (double*)malloc(nPI*sizeof(double));
    }

    if (Opt.mod_init_tank_levels == 1){
        Prov_Vars->init_tank_levels = (double*)malloc(nT*sizeof(double));
    }

    if (Opt.mod_tank_diameters == 1){
        Prov_Vars->tank_diameters = (double*)malloc(nT*sizeof(double));
    }
}







/**
 * @brief Sirve para poner como valores iniciales (semilla) de las variables de estado los
 * valores medidos
 * 
 * @note EE INSTANTÁNEA
 */
void prepare_ph_snapshot_SE(int nJ, int nR, int nT, int nJ_Dem, int* ind_J_Dem, double** dm_values, 
    double** wlm_values, double* min_tank_levels, double* max_tank_levels, EN_Project ph){
    
    //Asignamos a cada nodo una demanda base igual a la que hemos pseudomedido
    for (int i = 0; i < nJ_Dem; i++){
        EN_setnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, 0.999*dm_values[i][0]);
    }


    //Poner como nivel inicial de los tanques en este periodo el nivel FINAL
    //ESTIMADO en la EE del periodo anterior
    for (int i = 0; i < nT; i++){
        if (0.9999*wlm_values[i][0] < min_tank_levels[i]){
            EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, min_tank_levels[i]);
        } else if (1.0001*wlm_values[i][0] > max_tank_levels[i]){
            EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, max_tank_levels[i]);
        } else {
            EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, 0.999*wlm_values[i][0]);
        }
        //EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, wlm_values[i][t]);
    }

}




/**
 * @brief Sirve para inicializar valores antes de comenzar el ajuste de un nuevo
 * periodo y forzar el estado inicial de elementos de control. Prepara el ph
 * para el nuevo periodo.
 * Invoca funciones específicas: update_time_controls_inp,
 * edit_ph_init_valve_setting, edit_ph_initial_status
 * 
 * @note EE MULTIPERIODO
 */
void prepare_ph_for_new_period(int t, int nJ, int nR, int nT, int add_emitter, 
    int period_to_add_emitter, int emitter_index, double emitter_coef, int nCtrl_23, 
    int nR_pattern, long patternStep, int nJ_Dem, int* ind_J_Dem, double* last_demands,
    double* initial_tank_levels, double* min_tank_levels, double* max_tank_levels, 
    char* intermediate_File, char* rpt_File, char* binary_File,
    Reservoir_Pattern_Info* Reservoir_Patterns_Info, Ctrl_Links_Summary* Controls_Links_Summary,
    Ctrl_23* Controls_23, EN_Project ph, int lang){

    
    //Rescribir los controles temporales con setting 0.0 = CLOSED y con 1.0 = OPEN (para evitar fallos)
    update_time_controls_inp(nCtrl_23, t, patternStep, intermediate_File, Controls_23, ph, lang);

    EN_close(ph);
    EN_open(ph, intermediate_File, rpt_File, binary_File);

    //Editar setting de válvulas según estado inicial (para que no sea incompatible)
    edit_ph_init_valve_setting(Controls_Links_Summary, ph);

    //Añadir coef. emisor en nodo concreto si se ha configurado hacerlo en este periodo o antes
    if (period_to_add_emitter <= t && add_emitter == 1){
        EN_setnodevalue(ph, emitter_index, EN_EMITTER, emitter_coef);
        // printf("Coef. emisor de %.4f en nodo de indice %d \n\n", emitter_coef, emitter_index);
    }
       
    //Asignamos demanda base igual a la demanda ajustada en el último periodo (valor semilla)
    for (int i = 0; i < nJ_Dem; i++){
        EN_setnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, 1.000*last_demands[i]);
    }

    //Ponemos como valor semilla para nivel inicial de los tanques el nivel FINAL
    //ESTIMADO en la EE del periodo anterior
    for (int i = 0; i < nT; i++){
        if (1.000*initial_tank_levels[i] < min_tank_levels[i]){
            EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, min_tank_levels[i]);
        } else if (1.000*initial_tank_levels[i] > max_tank_levels[i]){
            EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, max_tank_levels[i]);
        } else {
            EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, 1.000*initial_tank_levels[i]);
        }
    }

    //Asignamos a aquellos embalses con patrón (head variable) el head de este periodo
    for (int i = 0; i < nR_pattern; i++){
        EN_setnodevalue(ph, Reservoir_Patterns_Info[i].reservoir_index, EN_ELEVATION, Reservoir_Patterns_Info[i].head[t]);
    }

    //Forzamos el estado inicial de los links afectados por reglas de control
    edit_ph_initial_status(Controls_Links_Summary, ph);

    // EN_saveinpfile(ph, "INIT_PERIOD.inp");
    // system("pause");

}





/**
 * @brief Accede al estado de los elementos de control susceptibles de cambio
 * (los asociados a reglas de control) al final de la simulación (para forzar 
 * ese estado al inicio del siguiente periodo)
 * 
 * @note EE MULTIPERIODO
 */
void get_status(Ctrl_01* Controls_01, Ctrl_23* Controls_23, 
    Ctrl_Links_Summary* Controls_Links_Summary, EN_Project ph){

    double status;
    int status_integer;

    //Almacenar estado (binario) para tuberías y bombas asociadas a reglas de control
    for (int i = 0; i < Controls_Links_Summary->nPipePump; i++){
        EN_getlinkvalue(ph, Controls_Links_Summary->pipe_pump_indices[i], EN_STATUS, &status);
        status_integer = (int)status;
        Controls_Links_Summary->pipe_pump_status[i] = status_integer;
    }


    //Almacenar estado (closed / open / active) para válvulas asociadas a reglas de control
    for (int i = 0; i < Controls_Links_Summary->nValves; i++){
        EN_getlinkvalue(ph, Controls_Links_Summary->valve_indices[i], EN_STATUS, &status);
        status_integer = (int)status;
        if (status == 0) Controls_Links_Summary->valve_status[i] = 0;
        else {
            continue; //no hacer nada (PENSAR)
        }
    }


}




/**
 * @brief Reescribe los controles temporales en el inp para ponerlos dentro del
 * rango de simulación en el instante deseado (si corresponden al periodo actual) 
 * o fuera (si son de otro periodo, para que no se ejecuten)
 * 
 * @note EE MULTIPERIODO
 */
void update_time_controls_inp(int nCtrl_23, int t, long patternStep, const char *intermediate_File, 
    Ctrl_23 *Controls_23, EN_Project ph, int lang) {

    FILE *file = fopen(intermediate_File, "r");
    if (!file) {
        printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "intermediate_File", intermediate_File);
        exit(EXIT_FAILURE);
    }

    char tmpFile[512];
    sprintf(tmpFile, "%s.tmp", intermediate_File);

    FILE *ftmp = fopen(tmpFile, "w");
    if (!ftmp) {
        printf(messages[MSG_ERR_GENERAL_FILE_OPEN][lang], "tmpFile", tmpFile);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char line[512];
    int inControlsSection = 0;
    int lineCountControls = 0;

    while (fgets(line, sizeof(line), file)) {

        // Detectar inicio / fin de la sección [CONTROLS]
        if (strncmp(line, "[CONTROLS]", 10) == 0) {
            inControlsSection = 1;
            fputs(line, ftmp);  // copia la cabecera
            continue;
        }
        if (line[0] == '[' && strncmp(line, "[CONTROLS]", 10) != 0) {
            inControlsSection = 0;
        }

        // Saltar comentarios
        char *trimmed = line;
        while (isspace((unsigned char)*trimmed)) trimmed++;
        if (*trimmed == ';' || *trimmed == '\0') {
            if (!inControlsSection) fputs(line, ftmp);
            continue;
        }

        if (inControlsSection) {
            lineCountControls++;

            int lineHandled = 0; // <-- para controlar si ya imprimimos la línea
            int matchFound = 0;

            for (int i = 0; i < nCtrl_23; i++) {
                if (lineCountControls == Controls_23[i].index) {
                    matchFound = 1;
                    lineHandled = 1;

                    double setting = Controls_23[i].setting;
                    double time = Controls_23[i].sec_in_period / 3600;
                    double time_out_of_period = ((double)patternStep + 100) / 3600;
                    int period = Controls_23[i].period;
                    char link_id[32];  
                    EN_getlinkid(ph, Controls_23[i].linkIndex, link_id);

                    if (setting == 0) {
                        if (period == t){
                            fprintf(ftmp, "LINK %s CLOSED AT TIME %.4f HOURS\n", link_id, time);
                        } else {
                            fprintf(ftmp, "LINK %s CLOSED AT TIME %.4f HOURS\n", link_id, time_out_of_period);
                        }

                        // printf("Control linea %d (%s) actualizado a CLOSED\n", lineCountControls, link_id);
                    } 
                    else if (setting == 1) {
                        if (period == t) {
                            fprintf(ftmp, "LINK %s OPEN AT TIME %.4f HOURS\n", link_id, time);
                        } else {
                            fprintf(ftmp, "LINK %s OPEN AT TIME %.4f HOURS\n", link_id, time_out_of_period);
                        }
                        
                        // printf("Control linea %d (%s) actualizado a OPEN\n", lineCountControls, link_id);
                    } 
                    else {
                        if (period == t) {
                            fprintf(ftmp, "LINK %s %.4f AT TIME %.4f HOURS\n", link_id, setting, time);
                        } else {
                            fprintf(ftmp, "LINK %s %.4f AT TIME %.4f HOURS\n", link_id, setting, time_out_of_period);
                        }

                        // printf("Control linea %d (%s) mantenido (valor %.3f)\n", lineCountControls, link_id, setting);
                    }

                    break; // salimos del for, ya tratamos la línea
                }
            }

            // Solo copiamos la línea original si no se trató
            if (!matchFound && !lineHandled) {
                fputs(line, ftmp);
            }
        } else {
            // Copiar las líneas fuera de [CONTROLS]
            fputs(line, ftmp);
        }
    }

    fclose(file);
    fclose(ftmp);

    // Sustituir archivo original
    remove(intermediate_File);
    rename(tmpFile, intermediate_File);

}





/**
 * @brief Obtiene en forma de vector el nivel de los tanques al inicio de la simulación
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
double* get_initial_tank_levels(int nJ, int nR, int nT, EN_Project ph){

    double* init_tank_levels = (double*)malloc(nT*sizeof(double));

    double init_tl;

    for (int i = 0; i < nT; i++){
        EN_getnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, &init_tl);
        init_tank_levels[i] = init_tl;
    }

    return init_tank_levels;

}




// FUNCIONES DE SIMULACIÓN (y relacionadas)
// ----------------------------------------


/**
 * @brief Lleva a cabo una simulación durante 1 periodo dadas las condiciones
 * iniciales y valores impuestos. 
 * 
 * @return Vector ordenado de tamaño "m_aug" (m + nT) con las demandas medias, niveles
 * iniciales de los tanques, presiones en junctions con PM, flujo (instantáneo
 * al principio y promediado) en links con FM y FM_avg (respectivamente) y
 * niveles en los tanques al final del periodo
 * 
 * @note EE MULTIPERIODO
 */
double* run_single_period_simulation(long patternStep, long hydStep, int nJ, int nR, int nT, 
    int nJ_Dem, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int m, int m_aug, 
    int* wlm_positions, int* pm_positions, int* fm_positions, int* fm_avg_positions, 
    int* ind_J_Dem, int nCtrl_01, int nCtrl_23, int nPI, int nPU, int period, int check_ctrl,
    Ctrl_01* Controls_01, Ctrl_23* Controls_23, Ctrl_Links_Summary* Controls_Links_Summary, 
    EN_Project ph, int lang) {

    EN_closeH(ph);   // Libera estado previo
    EN_openH(ph);    // Prepara nuevo estado
    EN_initH(ph, 1); //Iniciar motor hidráulico


    double value_to_store, tank_level;
    int element_index;
    char element_id[32];
    
    long time = 0;    // Tiempo acumulado
    long tstep = 1;   // Inicializamos en 1 seg para entrar al while
    int t = 0; // Índice (0 si time = 0 y 1 si time = patternStep)
    int next_t;

    double* gx = (double*)malloc(m_aug*sizeof(double));  //g(x) es la notación para los valores simulados que comparamos con z

    //Suma de los flujos ponderados por el paso hidráulico (en links con FM_avg)
    double* sum_weighted_flows = (double*)malloc(nFM_avg*sizeof(double));
    for (int i = 0; i < nFM_avg; i++){
        sum_weighted_flows[i] = 0;
    }

    int err; 
    
    while (tstep > 0) { 
        
        // Ejecutar el paso hidráulico
        err = EN_runH(ph, &time);  //Resolver ecuaciones en instante time
        if (err > 0) {
            char msg[256];
            EN_geterror(err, msg, 255);
            printf(messages[MSG_ERR_EN_RUNH][lang], time, msg);
            // system("pause");
        }
        //printf("Tiempo transcurrido: %ld segundos\n", time);

        if (check_ctrl == 1){
            // printf("\033[0;31mTIEMPO: %ld \n\033[0m", time);
            check_active_controls(nPI, nPU, nCtrl_01, nCtrl_23, period, time, 
                Controls_01, Controls_23, Controls_Links_Summary, ph);
            //Comprobar si la parada se debe a causas naturales de la simulación o ha sido
            //causada por algún evento asociado a regla de control (en ese caso,
            //detectar cuál y actualizar en Controls_Links_Summary el cambio de
            //estado para seguir la evolución del estado de las válvulas)
        }


        // Si estamos en un instante múltiplo de patternStep, almacenamos datos
        if (time % patternStep == 0) {
            t = time / patternStep;  // Calculamos el índice t
            //printf("\nInstante t = %d \n", t);

            if (t > 1) break; 

            if (t == 0){
                // EN_getnodeindex(ph, "J118", &element_index);
                // EN_getnodevalue(ph, element_index, EN_BASEDEMAND, &value_to_store);
                // double demand;
                // EN_getnodevalue(ph, element_index, EN_DEMAND, &demand);
                // printf("J118) Dem. registrada: %.4f | Q_fugado: %.4f \n", value_to_store, demand-value_to_store);

                for (int i = 0; i < nJ_Dem; i++){  //Demandas medias a lo largo del intervalo
                    //Almacenamos la demanda base (para no medir agua fugada en caso de que haya 
                    //emisor) (recordar que los factores multiplicadores de patrones son 1)
                    EN_getnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, &value_to_store);
                    gx[i] = value_to_store;
                }

                for (int i = 0; i < nWLM; i++){ //Niveles al principio del intervalo
                    EN_getnodevalue(ph, wlm_positions[i]+nJ+nR, EN_PRESSURE, &value_to_store);
                    gx[i+nDM] = value_to_store;
                }

                for (int i = 0; i < nPM; i++){  //Presiones al principio del intervalo
                    EN_getnodevalue(ph, pm_positions[i], EN_PRESSURE, &value_to_store);
                    gx[i+nDM+nWLM] = value_to_store;
                }

                for (int i = 0; i < nFM; i++){  //Flujos al principio del intervalo
                    EN_getlinkvalue(ph, fm_positions[i], EN_FLOW, &value_to_store);
                    if (fabs(value_to_store) < 0.05){  //Evitar pseudo-ceros (numéricos)
                        gx[i+nDM+nWLM+nPM] = 0;
                    } else {
                        gx[i+nDM+nWLM+nPM] = value_to_store;
                    }
                }
            }


            if (t == 1){ 
                for (int i = 0; i < nWLM; i++){  //Niveles al final del intervalo
                    EN_getnodevalue(ph, wlm_positions[i]+nJ+nR, EN_PRESSURE, &value_to_store);
                    gx[i+m] = value_to_store;
                }

                //printf("Flujos promediados: \n");
                for (int i = 0; i < nFM_avg; i++){ //Flujos promediados durante el intervalo
                    if (fabs(sum_weighted_flows[i])/patternStep < 0.05){
                        gx[i+nDM+nWLM+nPM+nFM] = 0;
                    } else {
                        gx[i+nDM+nWLM+nPM+nFM] = sum_weighted_flows[i]/patternStep;
                    }
                    //printf("%.4f \n", gx[i+nDM+nWLM+nPM+nFM]);
                }
                //printf("\n");
            }
        }

        long nextCheckpoint = ((time / hydStep) + 1) * hydStep; //Proximo Checkpoint 
        //(recordar que time/hydStep es una división entera, por ser long)
        long timeToNextCheckpoint = nextCheckpoint - time;  //Lo que queda para el proximo checkpoint
        //printf("Next Checkpoint: %ld, quedan: %ld \n", nextCheckpoint, timeToNextCheckpoint);

        EN_settimeparam(ph, EN_HYDSTEP, timeToNextCheckpoint);  //Forzamos parada en el próximo checkpoint

        // Avanzar al siguiente paso hidráulico
        err = EN_nextH(ph, &tstep);
        if (err > 0) {
            char msg[256];
            EN_geterror(err, msg, 255);
            printf(messages[MSG_ERR_EN_NEXTH][lang], time, msg);
            SENSEI_PAUSE();
        }
        //printf("Tamaño del siguiente paso: %ld segundos\n", tstep);

        for (int i = 0; i < nFM_avg; i++){  //En parada intermedia, agregar flujo ponderado por el tiempo transcurrido
            EN_getlinkvalue(ph, fm_avg_positions[i], EN_FLOW, &value_to_store);
            sum_weighted_flows[i] += value_to_store*tstep;
            //printf("Flujo %.4f durante %ld \n", value_to_store, tstep);
        }
        //printf("\n");

        // Salimos del bucle si no hay más pasos pendientes
        if (tstep <= 0) {
            break;
        }
    }

    return gx;
}






/**
 * @brief Lleva a cabo una simulación instantánea dadas las condiciones y
 * valores impuestos en el ph
 * 
 * @return Vector ordenado de tamaño "m" con las demandas, niveles de los
 * tanques, presiones en junctions con PM y flujo (instantáneo) en links con FM
 * 
 * @note EE INSTANTÁNEA
 */
double* run_snapshot_simulation(int nJ, int nR, int nT, int nJ_Dem, int nDM, int nWLM, 
    int nPM, int nFM, int m, int* wlm_positions, int* pm_positions, int* fm_positions, 
    int* ind_J_Dem, EN_Project ph) {


    double value_to_store;
    double tl;
    int element_index;
    char element_id[32];
  
    long time = 0;    // Tiempo acumulado

    double* gx = (double*)malloc(m*sizeof(double));  //g(x) es la notación para la red resulta para las variables x
    
    // Ejecutar el paso hidráulico
    EN_solveH(ph);
        
    for (int i = 0; i < nJ_Dem; i++){  //Demandas medias a lo largo del intervalo
        EN_getnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, &value_to_store);
        gx[i] = value_to_store;
    }

    for (int i = 0; i < nWLM; i++){ //Niveles al principio
        EN_getnodevalue(ph, wlm_positions[i]+nJ+nR, EN_PRESSURE, &value_to_store);
        gx[i+nDM] = value_to_store;
    }

    for (int i = 0; i < nPM; i++){  //Presiones al principio
        EN_getnodevalue(ph, pm_positions[i], EN_PRESSURE, &value_to_store);
        gx[i+nDM+nWLM] = value_to_store;
    }

    for (int i = 0; i < nFM; i++){  //Flujos al principio
        EN_getlinkvalue(ph, fm_positions[i], EN_FLOW, &value_to_store);
        if (fabs(value_to_store) < 0.05){
            gx[i+nDM+nWLM+nPM] = 0;
        } else {
            gx[i+nDM+nWLM+nPM] = value_to_store;
        }
    }

    return gx;

}





/**
 * @brief Lleva a cabo una simulación multiperiodo (la clásica de EPANET)
 * 
 * @return Vector de tamaño "m*T" en el que se almacenan, de manera ordenada
 * un periodo tras otro, las demandas medias, los niveles de los
 * tanques (al inicio de cada intervalo), presiones en junctions con PM (al
 * inicio de cada intervalo), flujo instantáneo en links con FM (al inicio de
 * cada intervalo) y flujo promediado en links con FM_avg (durante cada intervalo)
 * 
 * @warning SIN ARREGLAR MESSAGES (porque NO SE USA EN DEMO; solamente para
 * generar medidas)
 * 
 * @note ÚTIL PARA GENERAR MEDIDAS SINTÉTICAS
 */
double* run_whole_synthetic_simulation(long patternStep, long hydStep, int T, int nJ, int nR, 
    int nT, int nJ_Dem, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int nDemPat, int m, 
    int all_demands_nodes_rows, int* wlm_positions, int* pm_positions, int* fm_positions, 
    int* fm_avg_positions, int* ind_J_Dem, EN_Project ph) {

    // Inicializar simulación hidráulica
    EN_closeH(ph);   // Libera estado previo
    EN_openH(ph);    // Prepara nuevo estado
    EN_initH(ph, 1);

    int element_index;

    double value_to_store, pattern_index_double, base_demand, pattern_value;
    int pattern_index_int;

    double tl;
    char element_id[32]; 
    
    long time = 0;    // Tiempo acumulado
    long tstep = 1;   // Inicializamos en 1 seg para entrar al while

    //Especificar el ID del nodo en el que queremos aumentar repentinamente el coef. emisor
    EN_getnodeindex(ph, "J59", &element_index);  

    //Especificar el ID de la tubería en la que queremos reducir repentinamente
    //el coef. de rugosidad
    // EN_getlinkindex(ph, "P230", &element_index); 

    //g(x) es la notación para los valores simulados, dadas las variables x
    double* gx = (double*)malloc((m*T)*sizeof(double)); 
    for (int i = 0; i < m*T; i++){
        gx[i] = 0;
    }

    //Vector para ir agregando los flujos (ponderados por el tiempo de cada
    //subintervalo) en aquellas tuberías con FM_avg
    double* sum_weighted_flows = (double*)malloc(nFM_avg*sizeof(double));
    for (int i = 0; i < nFM_avg; i++){
        sum_weighted_flows[i] = 0;
    }

    int t = 0; // Índice de tiempo basado en patternStep (cada patternStep avanzamos 1 periodo)
    int next_t;
    int err;
    
    while (tstep > 0) { 
        
        // Ejecutar el paso hidráulico (corresponde a resolver las ecuaciones
        // que gobiernan la red en el instante "time")
        err = EN_runH(ph, &time);
        if (err > 0) {
            printf("\033[0;31mError en EN_runH %d (tiempo %ld) \033[0m\n", err, time);
            SENSEI_PAUSE();
        }


        //Esto de abajo es es simplemente para contar cuánta agua sale por el
        //nodo y cuánta sale en total (para saber cuánta agua se fuga con el
        //coef. emisor que hemos puesto)
        // printf("\033[0;31mTIME: %ld \033[0m \n", time);
        // double demand, bd, pv;
        // EN_getnodevalue(ph, element_index, EN_DEMAND, &demand);
        // EN_getnodevalue(ph, element_index, EN_BASEDEMAND, &bd);
        // EN_getpatternvalue(ph, 5, t+1, &pv);
        // printf("nodo J59 en t = %d | Demanda que realmente sale por el nodo: %.2f | Total: %.2f \n", t, bd*pv, demand);
        // for (int i = 0; i < nT; i++){
        //     EN_getnodeid(ph, i+1+nJ+nR, element_id);
        //     EN_getnodevalue(ph, i+1+nJ+nR, EN_PRESSURE, &tl);
        //     printf("Nivel %s: %.3f \n", element_id, tl);
        // }
        // printf("\n");

        // Si estamos en un instante múltiplo de patternStep, almacenamos datos
        // Ej: si patternStep = 3600 (cada hora), solamente almacenamos datos si
        // time = 0, 3600, 7200, etc. (no lo haremos si el motor de EPANET para
        // en puntos intermedios)
        if (time % patternStep == 0) {
            t = time / patternStep;  // Calculamos el índice t (índice del periodo)

            printf("t = %d \n--------------------\n", t);

            //Almacenamos nivel de agua en los tanques
            for (int i = 0; i < nWLM; i++){
                EN_getnodevalue(ph, wlm_positions[i]+nJ+nR, EN_PRESSURE, &value_to_store);
                gx[i+nDM+m*t] = value_to_store;
            }

            //Si estamos en un period t > 0, ya podemos calcular el flujo
            //promediado para cada link con sensor FM_avg y almacenarlo
            if (t > 0){
                //printf("Flujo promediado: \n");
                for (int i = 0; i < nFM_avg; i++){
                    gx[i+nDM+nWLM+nPM+nFM+m*(t-1)] = sum_weighted_flows[i]/patternStep;
                    //Lo indexamos en el periodo anterior
                }
                // printf("\n");
                // system("pause");
            }

            if (t >= T-1) break; //Está puesto después de los niveles para en el últ. instante solamente 
            // almacenar niveles y acumular flujo del último intervalo

            //Guardamos la demanda "contabilizada" para cada nodo de demanda 
            for (int i = 0; i < nJ_Dem; i++){
                //Al guardar base_demand*pattern, estamos ignorando las
                //fugas (porque no queremos contarlas como agua que sale de
                //manera "legal" por el nodo)
                //NO HACER ESTO: EN_getnodevalue(ph, ind_J_Dem[i],
                //EN_DEMAND, &demand) porque entonces incluye también el
                //agua que se fuga
                EN_getnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, &base_demand);
                EN_getnodevalue(ph, ind_J_Dem[i], EN_PATTERN, &pattern_index_double);
                pattern_index_int = (int)pattern_index_double;   // conversión explícita
                EN_getpatternvalue(ph, pattern_index_int, t+1, &pattern_value);
                value_to_store = pattern_value*base_demand;
                gx[i+m*t] = value_to_store;
                // if (ind_J_Dem[i] == element_index){
                //     double demand;
                //     EN_getnodevalue(ph, ind_J_Dem[i], EN_DEMAND, &demand);
                //     printf("nodo J59 en t = %d | Demanda contada: %.2f | Total: %.2f \n", t, value_to_store, demand);
                // }
            }

            //Guardamos la presión en aquellos nodos con piezómetro
            for (int i = 0; i < nPM; i++){
                EN_getnodevalue(ph, pm_positions[i], EN_PRESSURE, &value_to_store);
                gx[i+nDM+nWLM+m*t] = value_to_store;
            }

            //Guardamos el flujo (instantáneo) en aquellos nodos con FM
            for (int i = 0; i < nFM; i++){
                EN_getlinkvalue(ph, fm_positions[i], EN_FLOW, &value_to_store);
                gx[i+nDM+nWLM+nPM+m*t] = value_to_store;
            }

            //Como estamos al inicio de un intervalo, inicializamos
            //sum_weighted_flows en 0
            for (int i = 0; i < nFM_avg; i++){
                sum_weighted_flows[i] = 0;
            }

        }

        // Calcular el próximo múltiplo de hydStep
        long next_checkpoint = ((time / hydStep) + 1) * hydStep;
        //IMPORTANTE: la operación time/hydStep va a ser siempre el entero que
        //resulte de truncar la división time/hydStep (porque tanto time como
        //hydStep son ambos long)
        long time_to_checkpoint = next_checkpoint - time;
        //Establecemos el siguiente tiempo de avance (time_to_checkpoint) como 
        // "lo que falte desde el instante actual (time) para llegar al
        // siguiente checkpoint"

        // AQUÍ ES DONDE INDUCIMOS EL FALLO
        // IMPORTANTE que sea múltiplo de hydStep (si no, no está asegurado que
        // el motor pare en ese instante)
        if (next_checkpoint <= 25201 && next_checkpoint >= 25199){ //corresponde a las 7:00
            EN_setnodevalue(ph, element_index, EN_EMITTER, 1.0);
            printf("Acabamos de cambiar el coef. emisor !!!! (tiempo: %ld) \n", next_checkpoint);
            SENSEI_PAUSE();
        }

        // if (next_checkpoint <= 25201 && next_checkpoint >= 25199){ //corresponde a las 7:00
        //     EN_setlinkvalue(ph, element_index, EN_ROUGHNESS, 30.0);
        //     printf("Acabamos de cambiar la rugosidad!!!! (tiempo: %ld) \n", next_checkpoint);
        //     system("pause");
        // }
        
        //Esteblecemos EN_HYDSTEP = time_to_checkpoint (para tratar de avanzar
        //esa cantidad)
        EN_settimeparam(ph, EN_HYDSTEP, time_to_checkpoint);

        // Avanzar al siguiente punto, que puede ser:
        //  - o bien el siguiente múltiplo de hydStep
        //  - o bien un instante intermedio (si hay algún evento, como
        //    activación de reglas de control o similar)
        err = EN_nextH(ph, &tstep);
        if (err > 0) {
            printf("\033[0;31mError en EN_nextH %d (tiempo %ld) \033[0m\n", err, time);
            SENSEI_PAUSE();
        }
        // printf("Tamaño del siguiente paso: %ld segundos\n", tstep);

        // Una vez que sabemos cuánto vale el avance tstep, obtenemos los flujos
        // en el último instante (antes de resolver de nuevo con EN_runH al
        // principio del while) y los ponderamos por tstep
        for (int i = 0; i < nFM_avg; i++){
            EN_getlinkvalue(ph, fm_avg_positions[i], EN_FLOW, &value_to_store);
            sum_weighted_flows[i] += value_to_store*tstep;
        }
        // printf("\n");

        // Salimos del bucle si no hay más pasos pendientes
        if (tstep <= 0) {
            break;
        }
    }

    // printf("Salimos\n");

    return gx;
}







/**
 * @brief Lleva a cabo una simulación multiperiodo (la clásica de EPANET)
 * 
 * @return Vector de tamaño "m*T" en el que se almacenan, de manera ordenada
 * un periodo tras otro, las demandas medias, los niveles de los
 * tanques (al inicio de cada intervalo), presiones en junctions con PM (al
 * inicio de cada intervalo), flujo instantáneo en links con FM (al inicio de
 * cada intervalo) y flujo promediado en links con FM_avg (durante cada intervalo)
 * 
 * @note CALIBRACIÓN
 */
double* run_whole_simulation_calibration(long patternStep, long hydStep, int T, int nJ, int nR, 
    int nT, int nJ_Dem, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int nDemPat, int m, 
    int all_demands_nodes_rows, int* wlm_positions, int* pm_positions, int* fm_positions, 
    int* fm_avg_positions, int* ind_J_Dem, EN_Project ph, int lang) {

    // Inicializar simulación hidráulica
    EN_closeH(ph);   // Libera estado previo
    EN_openH(ph);    // Prepara nuevo estado
    EN_initH(ph, 0);

    int element_index, err;

    double value_to_store;
    
    long time = 0;    // Tiempo acumulado
    long tstep = 1;   // Inicializamos en 1 seg para entrar al while

    double* gx = (double*)malloc((m*T)*sizeof(double));  //g(x) es la notación para la red resulta para las variables x
    for (int i = 0; i < m*T; i++){
        gx[i] = 0;
    }

    double* sum_weighted_flows = (double*)malloc(nFM_avg*sizeof(double));
    for (int i = 0; i < nFM_avg; i++){
        sum_weighted_flows[i] = 0;
    }

    int t = 0; // Índice de tiempo basado en patternStep
    int next_t;
    
    while (tstep > 0) { 
        
        // Ejecutar el paso hidráulico
        // EN_runH(ph, &time);
        err = EN_runH(ph, &time);  //Resolver ecuaciones en instante time
        if (err > 0) {
            char msg[256];
            EN_geterror(err, msg, 255);
            // printf(messages[MSG_ERR_EN_RUNH][lang], time, msg);
            // system("pause");
        }
        //printf("Tiempo transcurrido: %ld segundos\n", time);

        // Si estamos en un instante múltiplo de patternStep, almacenamos datos
        if (time % patternStep == 0) {
            t = time / patternStep;  // Calculamos el índice t

            
            //printf("\nInstante t = %d \n", t);
            if (t > T) break; //Si estamos en calibración, ignoramos lo que pasa al final del último periodo

            if (t < T){
                if (all_demands_nodes_rows == 1){  //Si las filas para demanda son las demandas de los nodos
                    for (int i = 0; i < nJ_Dem; i++){
                        EN_getnodevalue(ph, ind_J_Dem[i], EN_DEMAND, &value_to_store);
                        gx[i+m*t] = value_to_store;
                    }
                } else {  //Si no, simplemente la componente de cada patrón en ese instante
                    for (int i = 0; i < nDemPat; i++){
                        EN_getpatternvalue(ph, i+1, t+1, &value_to_store);
                        gx[i+m*t] = value_to_store;
                    }
                }

                for (int i = 0; i < nWLM; i++){
                    EN_getnodevalue(ph, wlm_positions[i]+nJ+nR, EN_PRESSURE, &value_to_store);
                    gx[i+nDM+m*t] = value_to_store;
                }

                for (int i = 0; i < nPM; i++){
                    EN_getnodevalue(ph, pm_positions[i], EN_PRESSURE, &value_to_store);
                    gx[i+nDM+nWLM+m*t] = value_to_store;
                }

                for (int i = 0; i < nFM; i++){
                    EN_getlinkvalue(ph, fm_positions[i], EN_FLOW, &value_to_store);
                    gx[i+nDM+nWLM+nPM+m*t] = value_to_store;
                }

                // EN_getlinkindex(ph, "101", &element_index);
                // EN_getlinkvalue(ph, element_index, EN_FLOW, &value_to_store);
                // printf("\033[1;32mFlujo tubo 101 (t=%d): %.4f \033[0m\n", t, value_to_store);

                // EN_getlinkindex(ph, "329", &element_index);
                // EN_getlinkvalue(ph, element_index, EN_FLOW, &value_to_store);
                // printf("\033[1;32mFlujo tubo 329 (t=%d): %.4f \033[0m\n\n\n", t, value_to_store);
            }


            if (t > 0){
                // printf("Flujo promediado t = %d: \n", t);
                for (int i = 0; i < nFM_avg; i++){
                    gx[i+nDM+nWLM+nPM+nFM+m*(t-1)] = sum_weighted_flows[i]/patternStep;
                    // printf("%.4f \n", gx[i+nDM+nWLM+nPM+nFM+m*(t-1)]);
                }
                // printf("\n--------------------\n");
            }

            for (int i = 0; i < nFM_avg; i++){
                sum_weighted_flows[i] = 0;
            }

        }

        long nextCheckpoint = ((time / hydStep) + 1) * hydStep;
        long timeToNextCheckpoint = nextCheckpoint - time;

        //printf("Next Checkpoint: %ld, quedan: %ld \n", nextCheckpoint, timeToNextCheckpoint);

        EN_settimeparam(ph, EN_HYDSTEP, timeToNextCheckpoint);

        // Avanzar al siguiente paso hidráulico
        err = EN_nextH(ph, &tstep);
        if (err > 0) {
            char msg[256];
            EN_geterror(err, msg, 255);
            printf(messages[MSG_ERR_EN_NEXTH][lang], time, msg);
            SENSEI_PAUSE();
        }
        //printf("Tamaño del siguiente paso: %ld segundos\n", tstep);

        // printf("Next H: %ld \n", tstep);
        for (int i = 0; i < nFM_avg; i++){
            EN_getlinkvalue(ph, fm_avg_positions[i], EN_FLOW, &value_to_store);
            // printf("FLOW: %.4f \n", value_to_store);
            sum_weighted_flows[i] += value_to_store*tstep;
            //printf("Flujo %.4f durante %ld \n", value_to_store, tstep);
        }
        // printf("\n\n");

        // Salimos del bucle si no hay más pasos pendientes
        if (tstep <= 0) {
            break;
        }
    }

    return gx;
}



/**
 * @brief Sirve para detectar qué reglas de control han actuado cuando ha habido
 * un evento en una simulación hidráulica, y así hacer un seguimiento de la
 * evolución del estado de las válvulas
 * 
 * @note EE MULTIPERIODO
 */
void check_active_controls(int nPI, int nPU, int nCtrl_01, int nCtrl_23, int period, long time,
    Ctrl_01* Controls_01, Ctrl_23* Controls_23, Ctrl_Links_Summary* Controls_Links_Summary, EN_Project ph){

    double tank_level;

    //Solamente hacemos las comprobaciones de abajo para las válvulas
    //Comprobamos si la parada se debe a que algún tanque ha alcanzado
    //alguna cota (por tanto, se activaría alguna regla de control)
    for (int i = 0; i < nCtrl_01; i++){
        // printf("Control indice %d \n", i);
        EN_getnodevalue(ph, Controls_01[i].nodeIndex, EN_PRESSURE, &tank_level);

        if (Controls_01[i].control_type == 0){  //EN_LOWLEVEL
            if (tank_level*0.9995 < Controls_01[i].level){  //Detectamos si el nivel de tanque coincide con el nivel de la regla de control
                // printf("\033[0;33m[Tiempo %ld] %s (Tank level: %.4f) \033[0m\n", time, Controls_01[i].sentence, tank_level);
                if (Controls_01[i].linkIndex > nPI + nPU){  //Comprobamos si el link es válvula
                    int pos;
                    for (int j = 0; j < Controls_Links_Summary->nValves; j++){
                        if (Controls_Links_Summary->valve_indices[j] == Controls_01[i].linkIndex){
                            pos = j; //Posición de la válvula en Controls_Links_Summary
                        }
                    }
                    if (Controls_01[i].setting == 1) {  //Regla para OPEN
                        Controls_Links_Summary->valve_status[pos] = 1; //Estado OPEN
                        // printf("Valvula cambia a abierta \n");
                    } else if (Controls_01[i].setting > 0) {
                        Controls_Links_Summary->valve_status[pos] = 2; //Estado ACTIVE
                        Controls_Links_Summary->last_valve_setting[pos] = Controls_01[i].setting;
                        // printf("Valvula cambia a activa con setting %.2f \n", Controls_01[i].setting);
                    }
                }
            } 
        } else { //EN_HILEVEL
            if (tank_level*1.0005 > Controls_01[i].level){  //Detectamos si el nivel de tanque coincide con el nivel de la regla de control
                // printf("\033[0;33m[Tiempo %ld] %s (tank level: %.4f) \033[0m\n", time, Controls_01[i].sentence, tank_level);
                if (Controls_01[i].linkIndex > nPI + nPU){  //Comprobamos si el link es válvula
                    int pos;
                    for (int j = 0; j < Controls_Links_Summary->nValves; j++){
                        if (Controls_Links_Summary->valve_indices[j] == Controls_01[i].linkIndex){
                            pos = j; //Posición de la válvula en Controls_Links_Summary
                        }
                    }
                    if (Controls_01[i].setting == 0) {  //Regla para CLOSED
                        Controls_Links_Summary->valve_status[pos] = 0; //Estado CLOSED
                        // printf("Valvula cambia a cerrada \n");
                    } else if (Controls_01[i].setting > 0) {
                        Controls_Links_Summary->valve_status[pos] = 2; //Estado ACTIVE
                        Controls_Links_Summary->last_valve_setting[pos] = Controls_01[i].setting;
                        // printf("Valvula cambia a activa con setting %.2f \n", Controls_01[i].setting);
                    }
                }
            }
        }
    }


    for (int i = 0; i < nCtrl_23; i++){
        //Comprobar si la parada se debe a una regla de control temporal dentro de este periodo
        if (time>=(Controls_23[i].sec_in_period-1) && time<=(Controls_23[i].sec_in_period+1) && period == Controls_23[i].period){

            // printf("\033[0;33m[Tiempo %ld] %s \n\033[0m", time, Controls_23[i].sentence);

            if (Controls_23[i].linkIndex > nPI + nPU){  //Comprobamos si el link es válvula
                int pos;
                for (int j = 0; j < Controls_Links_Summary->nValves; j++){
                    if (Controls_Links_Summary->valve_indices[j] == Controls_23[i].linkIndex){
                        pos = j; //Posición de la válvula en Controls_Links_Summary
                    }
                }
                
                if (Controls_23[i].setting == 0) {  //Regla para CLOSED
                    Controls_Links_Summary->valve_status[pos] = 0; //Estado CLOSED
                    // printf("\033[0;32mValvula %d se cierra por control temporal \n\033[0m", Controls_23[i].linkIndex);
                } else if (Controls_23[i].setting == 1) { //REVISAR EL 1 (AL IMPONER REGLA TEMPORAL QUIZAS NO ES OPEN!)
                    Controls_Links_Summary->valve_status[pos] = 1; //Estado OPEN
                    // printf("\033[0;32mValvula %d se abre por control temporal \n\033[0m", Controls_23[i].linkIndex);
                } else if (Controls_23[i].setting > 0) {
                    Controls_Links_Summary->valve_status[pos] = 2; //Estado ACTIVE
                    Controls_Links_Summary->last_valve_setting[pos] = Controls_23[i].setting;
                    // printf("\033[0;32mValvula %d cambia a activa con setting %.4f por control temporal \n\033[0m", Controls_23[i].linkIndex, Controls_01[i].setting);
                }
            }
        }
    }

}






// FUNCIONES PARA EVALUAR LA FUNCIÓN OBJETIVO (y relacionadas)
// -----------------------------------------------------------


/**
 * @brief Vector cholmod dense de los residuos ponderados por la raíz cuadrada
 * de los pesos
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
cholmod_dense* compute_sqrt_weighted_residuals(int T, int m, double* z, double* gx,
    double* weights_vector, cholmod_common* c) {

    // Crear un cholmod_dense de tamaño adecuado 
    cholmod_dense* sqrt_weighted_residuals = cholmod_zeros(m*T, 1, CHOLMOD_REAL, c);

    double* sqrt_w_res_data = (double*) sqrt_weighted_residuals->x;  // Puntero a los datos

    // Primera parte: Cálculo de los residuos ponderados
    for (int i = 0; i < m*T; i++) {
        sqrt_w_res_data[i] = sqrt(weights_vector[i])*(z[i] - gx[i]);
    }


    return sqrt_weighted_residuals;
}





/**
 * @brief Calcula el valor de la función objetivo (Sum of Weighted Squared
 * Errors o SWSE) y muestra por print el valor desglosado por tipo de variables
 * 
 * @note EE MULTIPERIODO y EE INSTANTÁNEA
 */
double compute_SWSE_SE(cholmod_dense* sqrt_weighted_residuals, int nDM, int nWLM, 
    int nPM, int nFM, int nFM_avg, int final_nWLM, int lang){

    double* sqrt_w_res_data = (double*) sqrt_weighted_residuals->x;  // Puntero a los datos

    double SWSE_demands = 0, SWSE_levels_start = 0, SWSE_pressures = 0,
        SWSE_flows = 0, SWSE_avg_flows = 0, SWSE_levels_end = 0;

    for (int i = 0; i < nDM; i++){
        SWSE_demands += pow(sqrt_w_res_data[i],2);
    }
    if (lang == 0) printf("\033[1;36mSWSE (demandas) = %.4f \033[0m \n", SWSE_demands);
    else printf("\033[1;36mSWSE (demands) = %.4f \033[0m \n", SWSE_demands);
    for (int i = 0; i < nWLM; i++){
        SWSE_levels_start += pow(sqrt_w_res_data[i+nDM],2);
    }
    if (lang == 0) printf("\033[1;36mSWSE (niveles iniciales) = %.4f \033[0m \n", SWSE_levels_start);
    else printf("\033[1;36mSWSE (initial tank levels) = %.4f \033[0m \n", SWSE_levels_start);
    if (nPM > 0){
        for (int i = 0; i < nPM; i++){
            SWSE_pressures += pow(sqrt_w_res_data[i+nDM+nWLM],2);
        }
        if (lang == 0) printf("\033[1;36mSWSE (presiones) = %.4f \033[0m \n", SWSE_pressures);
        else printf("\033[1;36mSWSE (pressures) = %.4f \033[0m \n", SWSE_pressures);
    }
    if (nFM > 0){
        for (int i = 0; i < nFM; i++){
            SWSE_flows += pow(sqrt_w_res_data[i+nDM+nWLM+nPM],2);
        }
        if (lang == 0) printf("\033[1;36mSWSE (flujos instantáneos) = %.4f \033[0m \n", SWSE_flows);
        else printf("\033[1;36mSWSE (instantaneous flows) = %.4f \033[0m \n", SWSE_flows);
    }
    if (nFM_avg > 0){
        for (int i = 0; i < nFM_avg; i++){
            SWSE_flows += pow(sqrt_w_res_data[i+nDM+nWLM+nPM+nFM],2);
        }
        if (lang == 0) printf("\033[1;36mSWSE (flujos promediados) = %.4f \033[0m \n", SWSE_avg_flows);
        else printf("\033[1;36mSWSE (averaged flows) = %.4f \033[0m \n", SWSE_avg_flows);
    }
    for (int i = 0; i < final_nWLM; i++){
        SWSE_levels_end += pow(sqrt_w_res_data[i+nDM+nWLM+nPM+nFM+nFM_avg],2);
    }
    if (lang == 0) printf("\033[1;36mSWSE (niveles al final) = %.4f \033[0m \n\n", SWSE_levels_end);
    else printf("\033[1;36mSWSE (final tank levels) = %.4f \033[0m \n\n", SWSE_levels_end);
    

    double SWSE = SWSE_demands + SWSE_levels_start + SWSE_levels_end + SWSE_pressures + SWSE_flows;
    if (lang == 0) printf("\033[1;36mEn total, la suma de los residuos al cuadrado ponderados por los pesos es SWSE = %.4f \033[0m \n\n", SWSE);
    else printf("\033[1;36mIn total, the sum of weighted squared errores is SWSE = %.4f \033[0m \n\n", SWSE);

    return SWSE;
}





/**
 * @brief Calcula el valor de la función objetivo (Sum of Weighted Squared
 * Errors o SWSE) y muestra por print el valor desglosado por tipo de variables
 * 
 * @param print_by_period Poner 1 si se desea ver el desglose de la función
 * objetivo por periodos
 * 
 * @note CALIBRACIÓN
 */
double compute_SWSE_calibration(cholmod_dense* sqrt_weighted_residuals, int print_by_period, 
    int T, int m, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int lang){

    double* sqrt_w_res_data = (double*) sqrt_weighted_residuals->x;  // Puntero a los datos

    double* SWSE_by_period = (double*)malloc(T*sizeof(double));

    double SWSE_demands = 0, SWSE_levels = 0, SWSE_pressures = 0, SWSE_flows = 0, SWSE_avg_flows = 0;

    for (int t = 0; t < T; t++){
        double SWSE_t = 0;
        for (int i = 0; i < nDM; i++){
            SWSE_t += pow(sqrt_w_res_data[i+t*m],2);
            SWSE_demands += pow(sqrt_w_res_data[i+t*m],2);
        }
        for (int i = 0; i < nWLM; i++){
            SWSE_t += pow(sqrt_w_res_data[i+nDM+t*m],2);
            SWSE_levels += pow(sqrt_w_res_data[i+nDM+t*m],2);
        }
        for (int i = 0; i < nPM; i++){
            SWSE_t += pow(sqrt_w_res_data[i+nDM+nWLM+t*m],2);
            SWSE_pressures += pow(sqrt_w_res_data[i+nDM+nWLM+t*m],2);
        }
        for (int i = 0; i < nFM; i++){
            SWSE_t += pow(sqrt_w_res_data[i+nDM+nWLM+nPM+t*m],2);
            SWSE_flows += pow(sqrt_w_res_data[i+nDM+nWLM+nPM+t*m],2);
        }
        for (int i = 0; i < nFM_avg; i++){
            SWSE_t += pow(sqrt_w_res_data[i+nDM+nWLM+nPM+nFM+t*m],2);
            SWSE_avg_flows += pow(sqrt_w_res_data[i+nDM+nWLM+nPM+nFM+t*m],2);
        }
        SWSE_by_period[t] = SWSE_t;
    }

    double SWSE = SWSE_demands + SWSE_levels + SWSE_pressures + SWSE_flows + SWSE_avg_flows;

    if (lang == 0){
        printf("\033[1;36mSWSE (demandas) = %.2f \033[0m \n", SWSE_demands);
        printf("\033[1;36mSWSE (niveles) = %.2f \033[0m \n", SWSE_levels);
        if (nPM > 0) printf("\033[1;36mSWSE (presiones) = %.2f \033[0m \n", SWSE_pressures);
        if (nFM > 0) printf("\033[1;36mSWSE (flujos instantáneos) = %.2f \033[0m \n", SWSE_flows);
        if (nFM_avg > 0) printf("\033[1;36mSWSE (flujos promediados) = %.2f \033[0m \n\n", SWSE_avg_flows);

        printf("\033[1;36mEn total, la suma de los residuos al cuadrado ponderados por los pesos es SWSE = %.2f \033[0m \n\n", SWSE);
    } else {
        printf("\033[1;36mSWSE (demands) = %.2f \033[0m \n", SWSE_demands);
        printf("\033[1;36mSWSE (tank levels) = %.2f \033[0m \n", SWSE_levels);
        if (nPM > 0) printf("\033[1;36mSWSE (pressures) = %.2f \033[0m \n", SWSE_pressures);
        if (nFM > 0) printf("\033[1;36mSWSE (instantaneous flows) = %.2f \033[0m \n", SWSE_flows);
        if (nFM_avg > 0) printf("\033[1;36mSWSE (averaged flows) = %.2f \033[0m \n\n", SWSE_avg_flows);

        printf("\033[1;36mIn total, the sum of weighted squared errors is SWSE = %.2f \033[0m \n\n", SWSE);
    }

    if (print_by_period == 1){
        printf("SWSE por periodos: \n");
        for (int t = 0; t < T; t++){
            printf("%.2f  ", SWSE_by_period[t]);
        }
        printf("\n\n");
    }

    return SWSE;
}




/**
 * @brief Calcula la tasa de cambio entre el nuevo vector de variables g(x_new)
 * y el inmediatamente anterior g(x_old)
 * 
 * @note EE MULTIPERIODO y EE INSTANTÁNEA
 */
double compute_convergence_rate(int T, int m, double* gx_new, double* gx_old){

    double numerator_error = 0, denominator_error = 0;

    for (int i = 0; i < m*T; i++){
        numerator_error += fabs(gx_new[i]-gx_old[i]);
        denominator_error += fabs(gx_new[i]);
    } 

    double err_conv = numerator_error/denominator_error; 

    return err_conv; 
}






// FUNCIONES PARA EL CÁLCULO DEL JACOBIANO
// ---------------------------------------

/**
 * @brief Calcula el jacobiano de la función g(x) para las últimas variables de
 * estado calculadas
 * 
 * @return Matriz dispersa de tamaño (m_aug, nDM+nT)
 * 
 * @note EE MULTIPERIODO
 */
cholmod_sparse* compute_Jacobian_multiperiod_SE(int num_unknowns, long patternStep, long hydStep, 
    double epsilon, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int* wlm_positions, 
    int* pm_positions, int* fm_positions, int* fm_avg_positions, int nJ, int nR, int nT, int m, 
    int m_aug, int nJ_Dem, int* ind_J_Dem, double* min_tank_levels, double* max_tank_levels, 
    double* gx, cholmod_common* c, int nCtrl_01, int nCtrl_23, int nPI, int nPU, int t, 
    Ctrl_01* Controls_01, Ctrl_23* Controls_23, Ctrl_Links_Summary* Controls_Links_Summary, 
    EN_Project ph, int lang){ 

    // Creamos una matriz dispersa 
    int rows = m_aug;
    int columns = num_unknowns;

    size_t estimated_nnz = (size_t)(rows*columns);  //Siendo MUY generosos! Pero hay casos no muy dispersos (por si acaso!)
    cholmod_triplet* J_triplet = cholmod_allocate_triplet(rows, columns, estimated_nnz, 0, CHOLMOD_REAL, c);
    J_triplet->nnz = 0;  // Inicializar número de elementos no nulos

    double* gx_inc = NULL;

    // printf("Tenemos %d filas y %d columnas \n\n", rows, columns);

    // system("pause");

    double derivative, act, new_value, old_value;
    int error, col_index;


    // printf("\nModificando demandas base \n\n");

    for (int i = 0; i < nJ_Dem; i++){
        EN_getnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, &old_value);
        if (old_value == 0){
            act = 0.05;
        } else {
            act = epsilon/100*old_value;
        }
        new_value = old_value + act;


        EN_setnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, new_value);

        if (gx_inc != NULL){
            free(gx_inc);
            gx_inc = NULL;
        }
        gx_inc = run_single_period_simulation(patternStep, hydStep, nJ, nR, nT, nJ_Dem, 
            nDM, nWLM, nPM, nFM, nFM_avg, m, m_aug, wlm_positions, pm_positions, fm_positions, 
            fm_avg_positions, ind_J_Dem, nCtrl_01, nCtrl_23, nPI, nPU, t, 0, 
            Controls_01, Controls_23, Controls_Links_Summary, ph, lang);


        col_index = i;

        for (int k = 0; k < m_aug; k++) {
            derivative = (gx_inc[k]-gx[k])/act;
            if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
            }
        }

        EN_setnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, old_value);  //Reestablecer demanda base anterior
    }


    // printf("\nModificando los niveles iniciales de los tanques \n\n");

    for (int i = 0; i < nT; i++){
        EN_getnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, &old_value);
        if (old_value*(1+epsilon/100) >= max_tank_levels[i]){  //Si al perturbar nos pasamos, perturbamos hacia abajo
            act = -epsilon/100*old_value;
        } else if (old_value == 0){
            act = 0.1;
        } else {
            act = epsilon/100*old_value;
        }
        new_value = old_value + act;

        EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, new_value);

        if (gx_inc != NULL){
            free(gx_inc);
            gx_inc = NULL;
        }
        gx_inc = run_single_period_simulation(patternStep, hydStep, nJ, nR, nT, nJ_Dem, 
            nDM, nWLM, nPM, nFM, nFM_avg, m, m_aug, wlm_positions, pm_positions, fm_positions, 
            fm_avg_positions, ind_J_Dem, nCtrl_01, nCtrl_23, nPI, nPU, t, 0, 
            Controls_01, Controls_23, Controls_Links_Summary, ph, lang);

        col_index = nJ_Dem + i;

        for (int k = 0; k < m_aug; k++) {
            derivative = (gx_inc[k]-gx[k])/act;
            if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
            }
        }

        EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, old_value);  //Reestablecemos valor antiguo
    }


    free(gx_inc);

    // cholmod_print_triplet(J_triplet, "TRIPLET", c);

    cholmod_sparse* J_sparse = cholmod_triplet_to_sparse(J_triplet, J_triplet->nnz, c);

    cholmod_free_triplet(&J_triplet, c);

    return J_sparse;
}





/**
 * @brief Calcula el jacobiano de la función g(x) para las últimas variables de
 * estado calculadas
 * 
 * @return Matriz dispersa de tamaño (m, nDM+nT)
 * 
 * @note EE INSTANTÁNEA
 */
cholmod_sparse* compute_Jacobian_snapshot_SE(int num_unknowns, int add_emitter, int emitter_index,
    double epsilon, int nDM, int nWLM, int nPM, int nFM, int* wlm_positions, int* pm_positions, 
    int* fm_positions, int nJ, int nR, int nT, int m, int nJ_Dem, int* ind_J_Dem, 
    double* min_tank_levels, double* max_tank_levels, Variables_Snapshot_SE Current_Vars, 
    double* gx, cholmod_common* c, EN_Project ph){ 

    // Creamos una matriz dispersa 
    int rows = m;
    int columns = num_unknowns + add_emitter;

    size_t estimated_nnz = (size_t)(rows*columns);  //Siendo MUY generosos! Pero hay casos no muy dispersos (por si acaso!)
    cholmod_triplet* J_triplet = cholmod_allocate_triplet(rows, columns, estimated_nnz, 0, CHOLMOD_REAL, c);
    J_triplet->nnz = 0;  // Inicializar número de elementos no nulos

    double* gx_inc = NULL;


    double derivative, act, new_value, old_value;
    int error, col_index;


    for (int i = 0; i < nJ_Dem; i++){
        EN_getnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, &old_value);
        if (old_value == 0){
            act = 0.05;
        } else {
            act = epsilon/100*old_value;
        }
        new_value = old_value + act;
    

        EN_setnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, new_value);

        if (gx_inc != NULL){
            free(gx_inc);
            gx_inc = NULL;
        }
        gx_inc = run_snapshot_simulation(nJ, nR, nT, nJ_Dem, nDM, nWLM, nPM, nFM, 
            m, wlm_positions, pm_positions, fm_positions, ind_J_Dem, ph);


        col_index = i;

        for (int k = 0; k < m; k++) {
            derivative = (gx_inc[k]-gx[k])/act;
            if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
            }
        }

        EN_setnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, old_value);  //Reestablecer demanda base anterior
    }


    for (int i = 0; i < nT; i++){
        EN_getnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, &old_value);
        if (old_value*(1+epsilon/100) >= max_tank_levels[i]){  //Si al perturbar nos pasamos, perturbamos hacia abajo
            act = -epsilon/100*old_value;
        } else if (old_value == 0){
            act = 0.1;
        } else {
            act = epsilon/100*old_value;
        }
        new_value = old_value + act;

        EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, new_value);

        if (gx_inc != NULL){
            free(gx_inc);
            gx_inc = NULL;
        }
        gx_inc = run_snapshot_simulation(nJ, nR, nT, nJ_Dem, nDM, nWLM, nPM, nFM, 
            m, wlm_positions, pm_positions, fm_positions, ind_J_Dem, ph);

        col_index = nJ_Dem + i;

        for (int k = 0; k < m; k++) {
            derivative = (gx_inc[k]-gx[k])/act;
            if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
            }
        }

        EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, old_value);  //Reestablecemos valor antiguo
    }

    if (add_emitter){
       old_value = Current_Vars.emitter_coef;
       if (old_value == 0){
            act = 0.05;
        } else {
            act = epsilon/100*old_value;
        }
        new_value = old_value + act;

        EN_setnodevalue(ph, emitter_index, EN_EMITTER, new_value);

        if (gx_inc != NULL){
            free(gx_inc);
            gx_inc = NULL;
        }
        gx_inc = run_snapshot_simulation(nJ, nR, nT, nJ_Dem, nDM, nWLM, nPM, nFM, 
            m, wlm_positions, pm_positions, fm_positions, ind_J_Dem, ph);

        col_index = nJ_Dem + nT;

        for (int k = 0; k < m; k++) {
            derivative = (gx_inc[k]-gx[k])/act;
            if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
            }
        }

        EN_setnodevalue(ph, emitter_index, EN_EMITTER, old_value);;  //Reestablecemos valor antiguo
        
    }

    free(gx_inc);

    cholmod_sparse* J_sparse = cholmod_triplet_to_sparse(J_triplet, J_triplet->nnz, c);

    cholmod_free_triplet(&J_triplet, c);

    return J_sparse;
}



/**
 * @brief Calcula el jacobiano de la función g(x) para las variables que se
 * optimizan en la etapa en cuestión
 * 
 * @return Matriz dispersa de m*T filas y tantas columnas como variables se
 * optimicen en la etapa
 * 
 * @note CALIBRACIÓN
 */
cholmod_sparse* compute_Jacobian_calibration(int* columns, long patternStep, long hydStep, int T, 
    double epsilon, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int* wlm_positions, 
    int* pm_positions, int* fm_positions, int* fm_avg_positions, int nJ, int nR, int nT, int nPI, 
    int nPU, int nV, int m, int nDemPat, int nCP, int nMP, int stage, int** CM, int mod_MP, 
    int all_demands_nodes_rows, int nPumpCurves, int* pumpCurveIndices, int nJ_Dem, int* ind_J_Dem, 
    int nSetCtrls, Setting_Control_Info* Setting_Controls_Info, 
    Cluster_Pipes_Info* Clusters_Pipes_Info, int* indMP, double* min_tank_levels, 
    double* max_tank_levels, Calib_Variables Current_Vars, Options Opt, double* gx, 
    char* inp_file_name, char* rpt_File, char* binary_File, cholmod_common* c, EN_Project ph, int lang){

    // printf("Inicio todo bien \n");

    // Creamos una matriz dispersa 
    int rows = m*T;

    //Leemos la fila "stage" de la matriz de cronología, para saber qué
    //variables estamos soltando en esta etapa de la calibración

    int m_pat = CM[stage][0]; //Para modificar los patrones de demanda
    int m_bd = CM[stage][1]; //Para modificar demandas base
    int m_pcX = CM[stage][2]; //Para modificar las coord. X (Flujo) de las curvas de las bombas
    int m_pcY = CM[stage][3]; //Para modificar las coord. Y (Head) de las curvas de las bombas
    int m_set = CM[stage][4]; //Para modificar los settings de las valvulas
    int m_ir = CM[stage][5]; //Para modificar el coef. de rugosidad inidividual de cada tubería.
    int m_cr_DW = CM[stage][6]; //Para modificar coef. de rugosidad común (DW) para grupos de tuberías (luego se convierte a HW)
    int m_cr_HW = CM[stage][7]; //Para modificar coef. de rugosidad común (HW) para grupos de tuberías (luego se extiende a todas las tuberías)
    int m_itl = CM[stage][8]; //Para modificar los niveles iniciales de tanques (initial tank levels)
    int m_td = CM[stage][9]; //Para modificar los diámetros de tanques (tank diameters)


    *columns = m_pat*T*nDemPat + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves + 
    m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + m_cr_DW*nCP + m_cr_HW*nCP +
    m_itl*nT + m_td*nT;

    size_t estimated_nnz = (size_t)(1.00*rows*(*columns));  //Siendo MUY generosos! Pero hay casos no muy dispersos (por si acaso!)
    cholmod_triplet* J_triplet = cholmod_allocate_triplet(rows, *columns, estimated_nnz, 0, CHOLMOD_REAL, c);
    J_triplet->nnz = 0;  // Inicializar número de elementos no nulos

    double* gx_inc = NULL;

    if (lang == 0){
        printf("Cálculo del Jacobiano para la etapa %d \n", stage+1);
        printf("Matriz con %d filas y %d columnas \n\n", rows, *columns);
    } else {
        printf("Jacobian computation for stage %d \n", stage+1);
        printf("Matrix with %d rows and %d columns \n\n", rows, *columns);
    }

    double derivative, act, new_value, old_value;
    int error, col_index;

    if (m_pat == 1){
        if (lang == 0) printf("\nModificando patrones de demanda \n\n");
        else printf("\nModifying demand patterns \n\n");

        for (int t = 0; t < T; t++){
            if (t % 10 == 0) {
                if (lang == 0) printf("Periodo t = %d ... \n", t);
                else printf("Period t = %d ... \n", t);
            }
            for (int i = 0; i < nDemPat; i++){
                EN_getpatternvalue(ph, i+1, t+1, &old_value);
                if (old_value == 0){
                    act = 0.1;
                } else {
                    act = epsilon/100*old_value;
                }
                new_value = old_value + act;

                EN_setpatternvalue(ph, i+1, t+1, new_value);

                if (gx_inc != NULL){
                    free(gx_inc);
                    gx_inc = NULL;
                }
                gx_inc = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, 
                    nDM, nWLM,nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, 
                    pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);

                col_index = i+nDemPat*t;

                double suma = 0;
                for (int k = 0; k < m*T; k++) {
                    derivative = (gx_inc[k]-gx[k])/act;
                    if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                        suma += fabs(derivative);
                        ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                        ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                        ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                        J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                    }
                }

                if (suma < 0.1){  //Si toda la columna es nula (o casi), forzamos que el elemento (fila = col_index, columna = col_index) de J sea 0.0001 para evitar columnas nulas
                    //printf("Dentro de IF \n\n");
                    ((int*)J_triplet->i)[J_triplet->nnz] = col_index;  // Fila
                    ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                    ((double*)J_triplet->x)[J_triplet->nnz] = 0.0001;  // Valor
                    J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                }

                EN_setpatternvalue(ph, i+1, t+1, old_value);  //Reestablecer valor anterior del patrón
            }
        }
        printf("\n");
    }

    if (m_bd == 1){
        if (lang == 0) printf("\nModificando demandas base (para cada junction de demanda)\n\n");
        else printf("\nModifying base demands (for every demand junction)\n\n");

        for (int i = 0; i < nJ_Dem; i++){
            EN_getnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, &old_value);
            if (old_value == 0){
                act = 0.05;
            } else {
                act = epsilon/100*old_value;
            }
            new_value = old_value + act;

            EN_setnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, new_value);

            if (gx_inc != NULL){
                free(gx_inc);
                gx_inc = NULL;
            }
            gx_inc = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, 
                nDM, nWLM,nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, 
                pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);

            col_index = m_pat*nDemPat*T + i;

            double suma = 0;
            for (int k = 0; k < m*T; k++) {
                derivative = (gx_inc[k]-gx[k])/act;
                if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                    suma += fabs(derivative);
                    ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                    ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                    ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                    J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                }
            }

            if (suma < 0.1){  //Si toda la columna es nula (o casi), forzamos que el elemento (fila = col_index, columna = col_index) de J sea 0.0001 para evitar columnas nulas
                //printf("Dentro de IF \n\n");
                ((int*)J_triplet->i)[J_triplet->nnz] = col_index;  // Fila
                ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                ((double*)J_triplet->x)[J_triplet->nnz] = 0.0001;  // Valor
                J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
            }

            EN_setnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, old_value);  //Reestablecer demanda base anterior
        }
    }


    if (m_pcX == 1){
        if (lang == 0) printf("\nModificando coordenadas X (flujo) de las curvas de las bombas \n\n");
        else printf("\nModifying the X coordinates (flow) of the pump curves \n\n");

        //Creamos copias de las coordenadas X que definen las curvas (xPumpCurves) 
        double** xCurves_inc = (double**)malloc(nPumpCurves * sizeof(double*));
        for (int i = 0; i < nPumpCurves; i++) {
            xCurves_inc[i] = (double*)malloc(3*sizeof(double));
            for (int j = 0; j < 3; j++){
                xCurves_inc[i][j] = Current_Vars.xPumpCurves[i][j];
            }
        }


        for (int i = 0; i < nPumpCurves; i++){  //Después las x de cada curva (solo las 2 últimas, mantenemos el 0)
            for (int j = 0; j < 2; j++){
                old_value = xCurves_inc[i][j+1];
                act = epsilon/100*old_value;
                new_value = old_value + act;

                xCurves_inc[i][j+1] = new_value;

                //Editamos las curvas 
                //Como hay que cerrar y abrir el ph (hay que guardar el .inp)
                //volvemos a cargar toda la info para no perder precisión en los decimales
                edit_ph_all_current_variables(inp_file_name, rpt_File, binary_File, nPumpCurves, 
                    xCurves_inc, Current_Vars.yPumpCurves, pumpCurveIndices, nPI, nPU, nV, nMP, 
                    mod_MP, indMP, nJ, nR, nT, nJ_Dem, ind_J_Dem, nDemPat, T, nSetCtrls, 
                    Current_Vars, Opt, Setting_Controls_Info, ph, lang);

                if (gx_inc != NULL){
                    free(gx_inc);
                    gx_inc = NULL;
                }
                gx_inc = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, 
                    nDM, nWLM,nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, 
                    pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);

                col_index = m_pat*nDemPat*T + m_bd*nJ_Dem + i*2+j;

                double suma = 0;
                for (int k = 0; k < m*T; k++) {
                    derivative = (gx_inc[k]-gx[k])/act;
                    if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                        suma += fabs(derivative);
                        ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                        ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                        ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                        J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                    }
                }

                if (suma < 0.1){  //Si toda la columna es nula (o casi), forzamos que el elemento (fila = col_index, columna = col_index) de J sea 0.0001 para evitar columnas nulas
                    //printf("Dentro de IF \n\n");
                    ((int*)J_triplet->i)[J_triplet->nnz] = col_index;  // Fila
                    ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                    ((double*)J_triplet->x)[J_triplet->nnz] = 0.0001;  // Valor
                    J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                }

                xCurves_inc[i][j+1] = old_value;
            }           
        }

        //Cargar valores anteriores antes de pasar al siguiente grupo de variables
        edit_ph_all_current_variables(inp_file_name, rpt_File, binary_File, nPumpCurves, 
            Current_Vars.xPumpCurves, Current_Vars.yPumpCurves, pumpCurveIndices, nPI, nPU, 
            nV, nMP, mod_MP, indMP, nJ, nR, nT, nJ_Dem, ind_J_Dem, nDemPat, T, nSetCtrls, 
            Current_Vars, Opt, Setting_Controls_Info, ph, lang);
 
        for (int i = 0; i < nPumpCurves; i++) {
            free(xCurves_inc[i]);
        }
        free(xCurves_inc);

    }

    if (m_pcY == 1){
        if (lang == 0) printf("\nModificando coordenadas Y (head) de las curvas de las bombas \n\n");
        else printf("\nModifying the Y coordinates (head) of the pump curves \n\n");

        double** yCurves_inc = (double**)malloc(nPumpCurves * sizeof(double*));
        for (int i = 0; i < nPumpCurves; i++) {
            yCurves_inc[i] = (double*)malloc(3*sizeof(double));
            for (int j = 0; j < 3; j++){
                yCurves_inc[i][j] = Current_Vars.yPumpCurves[i][j];
            }
        }


        for (int i = 0; i < nPumpCurves; i++){  //Primero, las (3) coord. y de cada curva
            for (int j = 0; j < 3; j++){
                old_value = yCurves_inc[i][j];
                if (old_value == 0){
                    act = 0.1;
                } else {
                    act = epsilon/100*old_value;
                }
                new_value = old_value + act;

                yCurves_inc[i][j] = new_value;

                //Editamos las curvas 
                //Como hay que cerrar y abrir el ph (hay que guardar el .inp)
                //volvemos a cargar toda la info para no perder precisión en los decimales
                edit_ph_all_current_variables(inp_file_name, rpt_File, binary_File, nPumpCurves, 
                    Current_Vars.xPumpCurves, yCurves_inc, pumpCurveIndices, nPI, nPU, nV, nMP, 
                    mod_MP, indMP, nJ, nR, nT, nJ_Dem, ind_J_Dem, nDemPat, T, nSetCtrls, 
                    Current_Vars, Opt, Setting_Controls_Info, ph, lang);

                if (gx_inc != NULL){
                    free(gx_inc);
                    gx_inc = NULL;
                }
                gx_inc = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, 
                    nDM, nWLM,nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, 
                    pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);

                col_index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + i*3+j;

                double suma = 0;
                for (int k = 0; k < m*T; k++) {
                    derivative = (gx_inc[k]-gx[k])/act;
                    if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                        suma += fabs(derivative);
                        ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                        ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                        ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                        J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                    }
                }

                if (suma < 0.1){  //Si toda la columna es nula (o casi), forzamos que el elemento (fila = col_index, columna = col_index) de J sea 0.0001 para evitar columnas nulas
                    //printf("Dentro de IF \n\n");
                    ((int*)J_triplet->i)[J_triplet->nnz] = col_index;  // Fila
                    ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                    ((double*)J_triplet->x)[J_triplet->nnz] = 0.0001;  // Valor
                    J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                }
            }           
        }

        //Cargar valores anteriores antes de pasar al siguiente grupo de
        //variables
        edit_ph_all_current_variables(inp_file_name, rpt_File, binary_File, nPumpCurves, 
            Current_Vars.xPumpCurves, Current_Vars.yPumpCurves, pumpCurveIndices, nPI, nPU, 
            nV, nMP, mod_MP, indMP, nJ, nR, nT, nJ_Dem, ind_J_Dem, nDemPat, T, nSetCtrls, 
            Current_Vars, Opt, Setting_Controls_Info, ph, lang);

        for (int i = 0; i < nPumpCurves; i++) {
            free(yCurves_inc[i]);
        }
        free(yCurves_inc);

    }


    if (m_set == 1){
        if(lang == 0) printf("\nModificando los setting las válvulas \n\n");
        else printf("\nModifying valve settings \n\n");

        double* valves_settings_inc = (double*)malloc(nV*sizeof(double));
        for (int i = 0; i < nV; i++){
            valves_settings_inc[i] = Current_Vars.valves_settings[i];
        }

        for (int i = 0; i < nV; i++){
            old_value = Current_Vars.valves_settings[i];
            if (old_value == 0){  //No debería ser nunca el valor inicial de un setting 0 (por si acaso)
                act = 0.1;
            } else {
                act = epsilon/100*old_value;
            }
            new_value = old_value + act;

            valves_settings_inc[i] = new_value;

            edit_ph_valves_settings(nPI, nPU, nV, valves_settings_inc, ph);

            edit_ph_setting_controls(nSetCtrls, valves_settings_inc, Setting_Controls_Info, ph);

            if (gx_inc != NULL){
                free(gx_inc);
                gx_inc = NULL;
            }
            gx_inc = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, 
                nDM, nWLM,nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, 
                pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);

            col_index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + 
                m_pcY*3*nPumpCurves + i;

            double suma = 0;
            for (int k = 0; k < m*T; k++) {
                derivative = (gx_inc[k]-gx[k])/act;
                if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                    suma += fabs(derivative);
                    ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                    ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                    ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                    J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                }
            }

            if (suma < 0.1){  //Si toda la columna es nula (o casi), forzamos que el elemento (fila = col_index, columna = col_index) de J sea 0.0001 para evitar columnas nulas
                //printf("Dentro de IF \n\n");
                ((int*)J_triplet->i)[J_triplet->nnz] = col_index;  // Fila
                ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                ((double*)J_triplet->x)[J_triplet->nnz] = 0.0001;  // Valor
                J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
            }

            valves_settings_inc[i] = old_value;  //Reestablecemos el valor anterior
        }

        //Cargar valores anteriores de settings en controles antes de pasar al
        //siguiente grupo de variables
        edit_ph_valves_settings(nPI, nPU, nV, Current_Vars.valves_settings, ph);

        edit_ph_setting_controls(nSetCtrls, Current_Vars.valves_settings, Setting_Controls_Info, ph);

        free(valves_settings_inc);
    }


    if (m_ir == 1){
        if (mod_MP == 1){
            if (lang == 0) printf("Modificando el coeficiente de rugosidad de cada tubería principal (en total %d) \n\n", nMP);
            else printf("Modifying the roughness coefficient of each main pipe (in total %d) \n\n", nMP);

            for (int i = 0; i < nMP; i++){  
                EN_getlinkvalue(ph, indMP[i], EN_ROUGHNESS, &old_value);
                act = epsilon/100*old_value;
                new_value = old_value + act;

                EN_setlinkvalue(ph, indMP[i], EN_ROUGHNESS, new_value);

                if (gx_inc != NULL){
                    free(gx_inc);
                    gx_inc = NULL;
                }
                gx_inc = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, 
                    nDM, nWLM,nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, 
                    pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);

                col_index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves +
                    m_pcY*3*nPumpCurves + m_set*nV + i;

                double suma = 0;
                for (int k = 0; k < m*T; k++) {
                    derivative = (gx_inc[k]-gx[k])/act;
                    if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                        suma += fabs(derivative);
                        ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                        ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                        ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                        J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                    }
                }

                if (suma < 0.1){  //Si toda la columna es nula (o casi), forzamos que el elemento (fila = col_index, columna = col_index) de J sea 0.0001 para evitar columnas nulas
                    //printf("Dentro de IF \n\n");
                    ((int*)J_triplet->i)[J_triplet->nnz] = col_index;  // Fila
                    ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                    ((double*)J_triplet->x)[J_triplet->nnz] = 0.0001;  // Valor
                    J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                }
                EN_setlinkvalue(ph, indMP[i], EN_ROUGHNESS, old_value);  //Reestablecemos el valor anterior
            }

        } else {

            if (lang == 0) printf("Modificando el coeficiente de rugosidad de cada tubería de la red\n\n");
            else printf("Modifying the roughness cofficient of each pipe of the network\n\n");

            for (int i = 0; i < nPI; i++){
                EN_getlinkvalue(ph, i+1, EN_ROUGHNESS, &old_value);  
                act = epsilon/100*old_value;
                new_value = old_value + act;

                EN_setlinkvalue(ph, i+1, EN_ROUGHNESS, new_value);

                if (gx_inc != NULL){
                    free(gx_inc);
                    gx_inc = NULL;
                }
                gx_inc = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, 
                    nDM, nWLM,nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, 
                    pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);

                col_index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves +
                    m_pcY*3*nPumpCurves + m_set*nV + i;

                double suma = 0;
                for (int k = 0; k < m*T; k++) {
                    derivative = (gx_inc[k]-gx[k])/act;
                    if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                        suma += fabs(derivative);
                        ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                        ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                        ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                        J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                    }
                }

                if (suma < 0.1){  //Si toda la columna es nula (o casi), forzamos que el elemento (fila = col_index, columna = col_index) de J sea 0.0001 para evitar columnas nulas
                    //printf("Dentro de IF \n\n");
                    ((int*)J_triplet->i)[J_triplet->nnz] = col_index;  // Fila
                    ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                    ((double*)J_triplet->x)[J_triplet->nnz] = 0.0001;  // Valor
                    J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                }
                EN_setlinkvalue(ph, i+1, EN_ROUGHNESS, old_value);  //Reestablecemos el valor anterior
            }
        }
    }


    if (m_cr_DW == 1){
        if (lang == 0) printf("Modificando el coeficiente de rugosidad de D-W de cada grupo de tuberías\n\n");
        else printf("Modifying the D-W roughness coefficient of each cluster of pipes\n\n");

        double* common_DW_coefs_inc = (double*)malloc(nCP*sizeof(double));
        for (int i = 0; i < nCP; i++){
            common_DW_coefs_inc[i] = Current_Vars.common_DW_coefs[i];
        }
        // printf("common_DW_coefs_inc bien definido? \n");
        double* roughness_coefficients_inc = NULL;

        for (int i = 0; i < nCP; i++){
            // printf("i = %d \n", i);
            old_value = Current_Vars.common_DW_coefs[i];
            act = epsilon/100*old_value;
            new_value = old_value + act;

            common_DW_coefs_inc[i] = new_value;

            if (roughness_coefficients_inc != NULL){
                free(roughness_coefficients_inc);
                roughness_coefficients_inc = NULL;
            }

            roughness_coefficients_inc = convert_roughness_coefs_from_DW_to_HW(nPI, nCP, common_DW_coefs_inc, Clusters_Pipes_Info, ph);

            edit_ph_roughness_coefficients(nPI, nMP, mod_MP, indMP, roughness_coefficients_inc, ph);

            if (gx_inc != NULL){
                free(gx_inc);
                gx_inc = NULL;
            }

            gx_inc = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, 
                nDM, nWLM,nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, 
                pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);

            col_index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves +
                m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + i;

            double suma = 0;
            for (int k = 0; k < m*T; k++) {
                derivative = (gx_inc[k]-gx[k])/act;
                if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                    suma += fabs(derivative);
                    ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                    ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                    ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                    J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                }
            }

            if (suma < 0.1){  //Si toda la columna es nula (o casi), forzamos que el elemento (fila = col_index, columna = col_index) de J sea 0.0001 para evitar columnas nulas
                //printf("Dentro de IF \n\n");
                ((int*)J_triplet->i)[J_triplet->nnz] = col_index;  // Fila
                ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                ((double*)J_triplet->x)[J_triplet->nnz] = 0.0001;  // Valor
                J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
            }


            common_DW_coefs_inc[i] = old_value;  //Reestablecemos el valor anterior       
        }

        //Cargar rugosidades de actuales antes de pasar al siguiente grupo de variables
        edit_ph_roughness_coefficients(nPI, nMP, mod_MP, indMP, Current_Vars.roughness_coefficients, ph);

        free(common_DW_coefs_inc);
        free(roughness_coefficients_inc);  
    }

    // for (int kk = 0; kk < m*T; kk++){
    //     printf("%.6f  \n", gx[kk]);
    // }
    // printf("\n\n");
    // system("pause");


    if (m_cr_HW == 1){
        if (lang == 0) printf("Modificando el coeficiente de rugosidad de H-W de cada grupo de tuberías\n\n");
        else printf("Modifying the H-W roughness coefficient of each cluster of pipes\n\n");

        double* common_HW_coefs_inc = (double*)malloc(nCP*sizeof(double));
        for (int i = 0; i < nCP; i++){
            common_HW_coefs_inc[i] = Current_Vars.common_HW_coefs[i];
        }
        double* roughness_coefficients_inc = NULL;

        for (int i = 0; i < nCP; i++){
            // printf("i = %d \n", i);
            old_value = Current_Vars.common_HW_coefs[i];
            act = epsilon/100*old_value;
            new_value = old_value + act;

            common_HW_coefs_inc[i] = new_value;

            if (roughness_coefficients_inc != NULL){
                free(roughness_coefficients_inc);
                roughness_coefficients_inc = NULL;
            }

            roughness_coefficients_inc = extend_common_roughness_coefs_to_all_pipes(nPI, nCP, common_HW_coefs_inc, Clusters_Pipes_Info, ph);

            edit_ph_roughness_coefficients(nPI, nMP, mod_MP, indMP, roughness_coefficients_inc, ph);

            if (gx_inc != NULL){
                free(gx_inc);
                gx_inc = NULL;
            }

            gx_inc = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, 
                nDM, nWLM,nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, 
                pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);

            col_index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves +
                m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + m_cr_DW*nCP + i;
            
            // for (int kk = 0; kk < m*T; kk++){
            //     printf("%.6f  \n", gx_inc[kk]);
            // }
            // printf("\n\n");
            // system("pause");

            double suma = 0;
            for (int k = 0; k < m*T; k++) {
                derivative = (gx_inc[k]-gx[k])/act;
                if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                    suma += fabs(derivative);
                    ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                    ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                    ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                    J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                }
            }

            if (suma < 0.1){  //Si toda la columna es nula (o casi), forzamos que el elemento (fila = col_index, columna = col_index) de J sea 0.0001 para evitar columnas nulas
                //printf("Dentro de IF \n\n");
                ((int*)J_triplet->i)[J_triplet->nnz] = col_index;  // Fila
                ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                ((double*)J_triplet->x)[J_triplet->nnz] = 0.0001;  // Valor
                J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
            }

            common_HW_coefs_inc[i] = old_value;  //Reestablecemos el valor anterior      
            
        }

        //Cargar rugosidades de actuales antes de pasar al siguiente grupo de variables
        edit_ph_roughness_coefficients(nPI, nMP, mod_MP, indMP, Current_Vars.roughness_coefficients, ph);

        free(common_HW_coefs_inc);
        free(roughness_coefficients_inc);  
    }


    if (m_itl == 1){
        if (lang == 0) printf("Modificando los niveles iniciales de los tanques \n\n");
        else printf("Modifying initial tank levels \n\n");

        for (int i = 0; i < nT; i++){
            EN_getnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, &old_value);
            if (old_value*(1+epsilon/100) >= max_tank_levels[i]){  //Si al perturbar nos pasamos, perturbamos hacia abajo
                act = -epsilon/100*old_value;
            } else if (old_value == 0){
                act = 0.1;
            } else {
                act = epsilon/100*old_value;
            }
            new_value = old_value + act;

            EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, new_value);

            if (gx_inc != NULL){
                free(gx_inc);
                gx_inc = NULL;
            }
            gx_inc = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, 
                nDM, nWLM,nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, 
                pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);

            col_index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves +
                m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + m_cr_DW*nCP + m_cr_HW*nCP + i;

            double suma = 0;
            for (int k = 0; k < m*T; k++) {
                derivative = (gx_inc[k]-gx[k])/act;
                if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                    suma += fabs(derivative);
                    ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                    ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                    ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                    J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                }
            }

            if (suma < 0.1){  //Si toda la columna es nula (o casi), forzamos que el elemento (fila = col_index, columna = col_index) de J sea 0.0001 para evitar columnas nulas
                //printf("Dentro de IF \n\n");
                ((int*)J_triplet->i)[J_triplet->nnz] = col_index;  // Fila
                ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                ((double*)J_triplet->x)[J_triplet->nnz] = 0.0001;  // Valor
                J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
            }

            EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKLEVEL, old_value);  //Reestablecemos valor antiguo
        }

    }


    if (m_td == 1){
        if (lang == 0) printf("Modificando los diámetros de los tanques \n\n");
        else printf("Modifying tank diameters \n\n");

        for (int i = 0; i < nT; i++){

            EN_getnodevalue(ph, nJ+nR+i+1, EN_TANKDIAM, &old_value);
            if (old_value == 0){
                act = 0.1;
            } else {
                act = epsilon/100*old_value;
            }
            new_value = old_value + act;

            EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKDIAM, new_value);


            if (gx_inc != NULL){
                free(gx_inc);
                gx_inc = NULL;
            }
            gx_inc = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, 
                nDM, nWLM,nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, 
                pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);

            col_index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves +
                m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + m_cr_DW*nCP + 
                m_cr_HW*nCP + m_itl*nT + i;

            double suma = 0;
            for (int k = 0; k < m*T; k++) {
                derivative = (gx_inc[k]-gx[k])/act;
                if (fabs(derivative) > 0.0001) {  //Si la derivada no es cero, la ponemos como non-zero en la jacobiana
                    suma += fabs(derivative);
                    ((int*)J_triplet->i)[J_triplet->nnz] = k;  // Fila
                    ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                    ((double*)J_triplet->x)[J_triplet->nnz] = derivative;  // Valor
                    J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
                }
            }

            if (suma < 0.1){  //Si toda la columna es nula (o casi), forzamos que el elemento (fila = col_index, columna = col_index) de J sea 0.0001 para evitar columnas nulas
                //printf("Dentro de IF \n\n");
                ((int*)J_triplet->i)[J_triplet->nnz] = col_index;  // Fila
                ((int*)J_triplet->j)[J_triplet->nnz] = col_index;  // Columna
                ((double*)J_triplet->x)[J_triplet->nnz] = 0.0001;  // Valor
                J_triplet->nnz++;  // Incrementar el contador de elementos no nulos
            }

            EN_setnodevalue(ph, nJ+nR+i+1, EN_TANKDIAM, old_value);  //Reestablecemos valor antiguo
        }

    }


    free(gx_inc);


    // cholmod_print_triplet(J_triplet, "TRIPLET", c);

    cholmod_sparse* J_sparse = cholmod_triplet_to_sparse(J_triplet, J_triplet->nnz, c);

    cholmod_free_triplet(&J_triplet, c);

    return J_sparse;
}




// FUNCIONES PARA RESOLVER ECUACIONES / SISTEMAS formato cholmod
// -------------------------------------------------------------



/**
 * @brief Concentra la resolución de un sistema U*Delta_x = b (U matriz SDP)
 * mediante Descomposición de Cholesky
 * 
 * @return Vector Delta_x en forma cholmod_dense
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
cholmod_dense* solve_SDP_linear_system(cholmod_sparse* U, cholmod_dense* b, cholmod_common* c, int lang) {
    // printf("Verificando la matriz U antes de la factorizacion\n");

    // Revisar valores en la diagonal (una matriz SPD debería tener valores positivos en la diagonal)
    /*double* U_x = (double*) U->x;  // Puntero a los valores
    int* U_p = (int*) U->p;  // Puntero a los índices de columna 
    int* U_i = (int*) U->i;  // Puntero a los índices de fila 

    for (size_t i = 0; i < U->ncol; i++) {
        // Buscar la fila i en la columna i
        for (size_t j = U_p[i]; j < U_p[i + 1]; j++) {
            if (U_i[j] == i) {  // Si encontramos la diagonal (fila == columna)
                double diag_value = U_x[j];
                if (diag_value <= 0) {
                    printf("Elemento diagonal %ld de U es %f (deberia ser positivo)\n",
                        (long)i, diag_value);
                }
                break;  // Ya encontramos el elemento de la diagonal, podemos salir
            }
        }
    }*/

    //Descomentar bloque de arriba para comprobar que U es SDP

    // printf("Matriz U parece valida. Procediendo con la factorizacion \n");

    // Factorizar U usando Cholesky
    cholmod_factor* L = cholmod_analyze(U, c);

    if (cholmod_factorize(U, L, c) == 0) {
        if (lang == 0) printf("\033[0;31mERROR: Fallo en cholmod_factorize(). La matriz U puede ser singular \033[0m \n\n");
        else printf("\033[0;31mERROR: Failure in cholmod_factorize(). The matrix U may be singular \033[0m \n\n");
        SENSEI_PAUSE();
        cholmod_free_factor(&L, c);
        return NULL;
    }

    // printf("Rango estimado de U: %ld\n", (long)L->minor);

    // printf("Factorizacion exitosa. Resolviendo el sistema\n");

    // Resolver U * delta_x = b
    cholmod_dense* delta_x = cholmod_solve(CHOLMOD_A, L, b, c);
    if (delta_x == NULL) {
        if (lang == 0) printf("\033[0;31mERROR: Fallo en cholmod_solve(). Es posible que la matriz U esté mal condicionada\033[0m\n");
        else printf("\033[0;31mERROR: Failure in cholmod_solve(). The matrix U is likely ill-conditioned\033[0m\n");
        SENSEI_PAUSE();
        cholmod_free_factor(&L, c);
        return NULL;
    }

    // Liberar memoria de la factorización
    cholmod_free_factor(&L, c);

    return delta_x;
}





/**
 * @brief Resuelve mediante factorización de Cholesky un sistema matricial de
 * tipo AX = B, donde A es una matriz cuadrada SDP. Se usa en la propagación de
 * incertidumbre para calcular la matriz de sensibilidad de las variables de
 * estado respecto de las medidas
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
cholmod_sparse* solve_SDP_matricial_system(cholmod_sparse *A, cholmod_sparse *B, cholmod_common *c, int lang) {
    //Función para resolver sistemas matriciales cholmod sparse de tipo AX=B
    if (!A || A->nrow != A->ncol) {
        if (lang == 0) printf("\033[0;31mERROR: La matriz de coeficientes es NULL o no es cuadrada\033[0m\n");
        else printf("\033[0;31mERROR: The coefficient matrix is NULL or it is not square\033[0m\n");
        SENSEI_PAUSE();
        return NULL;
    }

    // Factorización de Cholesky
    cholmod_factor *L = cholmod_analyze(A, c);
    if (!L || !cholmod_factorize(A, L, c)) {
        if (lang == 0) printf("\033[0mERROR: La matriz de coeficientes no es SPD o es singular\033[0m\n");
        else printf("\033[0mERROR: The coefficiente matrix is not SDP or it is singular\033[0m\n");
        SENSEI_PAUSE();
        cholmod_free_factor(&L, c);
        return NULL;
    }

    // Convertir B de dispersa a densa antes de resolver
    cholmod_dense *B_dense = cholmod_sparse_to_dense(B, c);
    if (!B_dense) {
        if (lang == 0) printf("\033[0mERROR al convertir la matriz B a densa\033[0m\n");
        else printf("\033[0mERROR when converting matrix B to dense\033[0m\n");
        SENSEI_PAUSE();
        cholmod_free_factor(&L, c);
        return NULL;
    }

    // Resolver AX = B (ahora B es densa)
    cholmod_dense *X_dense = cholmod_solve(CHOLMOD_A, L, B_dense, c);
    if (!X_dense) {
        if (lang == 0) printf("\033[0mERROR en cholmod_solve\033[0m\n");
        else printf("\033[0mERROR in cholmod_solve\033[0m\n");
        SENSEI_PAUSE();
        cholmod_free_dense(&B_dense, c);
        cholmod_free_factor(&L, c);
        return NULL;
    }

    // Convertir X de denso a disperso para la salida
    cholmod_sparse *X_sparse = cholmod_dense_to_sparse(X_dense, 1, c);
    if (!X_sparse) {
        if (lang == 0) printf("\033[0mERROR al aplicar cholmod_dense_to_sparse() a la matriz incógnita\033[0m\n");
        else printf("\033[0mERROR when applying cholmod_dense_to_sparse() to the unknown matrix\033[0m\n");
        SENSEI_PAUSE();
    }

    // Obtenemos punteros a los datos necesarios 
    double* X_data = (double *)X_sparse->x;    // Valores de la matriz (suponiendo tipo double)
    int* X_p = (int *)X_sparse->p;           // Puntero a los inicios de columnas
    

    // Liberar memoria intermedia
    cholmod_free_dense(&B_dense, c);
    cholmod_free_dense(&X_dense, c);
    cholmod_free_factor(&L, c);

    return X_sparse;  // Devolver la solución en formato disperso
}






/**
 * @brief Resuelve la ecuación del método de Gauss-Newton, (J^t*W*J)*Delta_x = (J^t*W)*(z-g(x))
 * 
 * @return Vector Delta_x en forma cholmod_dense
 * 
 * @note EE INSTANTÁNEA
 */
cholmod_dense* solve_Gauss_Newton_equation(int num_unknowns, cholmod_sparse* J, 
    cholmod_sparse* sqrt_W_matrix, cholmod_dense* sqrt_weighted_residuals, cholmod_common *c, int lang){

    double alpha[2] = {1.0, 0.0}; // Multiplica por 1.0
    double beta[2] = {0.0, 0.0};  // No suma nada a b

    int nan_count, inf_count;

    cholmod_sparse* sqrt_weighted_J = cholmod_ssmult(sqrt_W_matrix, J, 0, 1, 1, c);  //sqrt(W)*J

    cholmod_sparse* sqrt_weighted_J_T = cholmod_transpose(sqrt_weighted_J, 1, c);  //J^t*sqrt(W)

    cholmod_sparse* U = cholmod_ssmult(sqrt_weighted_J_T, sqrt_weighted_J, 1, 1, 1, c);  //U = J^t*W*J
    
    //save_dense_matrix_to_txt(U, "U.txt", c);

    check_nan_inf_matrix(&nan_count, &inf_count, U); //Comprobación de NaN y +-inf

    if (nan_count > 0 || inf_count > 0){
        printf(messages[MSG_ERR_MATRIX_U_NAN_INF][lang], nan_count, inf_count);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    cholmod_dense* b = cholmod_zeros(num_unknowns, 1, CHOLMOD_REAL, c);  //Definimos b

    cholmod_sdmult(sqrt_weighted_J, 1, alpha, beta, sqrt_weighted_residuals, b, c); //b = J^t*W*(z-g(x))

    //system("pause");

    cholmod_dense* delta_x = solve_SDP_linear_system(U, b, c, lang);

    double* delta_x_data = (double*) delta_x->x;

    cholmod_free_sparse(&sqrt_weighted_J, c);
    cholmod_free_sparse(&sqrt_weighted_J_T, c);
    cholmod_free_sparse(&U, c);
    cholmod_free_dense(&b, c);

    return delta_x;

}





/**
 * @brief Resuelve la ecuación del método de Levenberg-Marquardt, 
 * (J^t*W*J+D^t*D)*Delta_x = (J^t*W)*(z-g(x))
 * 
 * @return Vector Delta_x en forma cholmod_dense
 * 
 * @note CALIBRACIÓN y EE MULTIPERIODO
 */
cholmod_dense* solve_Levenberg_Marquardt_equation(int num_unknowns, cholmod_sparse* J, cholmod_sparse* sqrt_W_matrix,
    cholmod_dense* sqrt_weighted_residuals, double lambda, int scale, cholmod_common *c, int lang){

    //Scale 0 = "none"  
    //Scale 1 = "jacobian"

    double alpha[2] = {1.0, 0.0}; // Multiplica por 1.0
    double beta[2] = {0.0, 0.0};  // No suma nada a b

    int nan_count, inf_count;

    cholmod_sparse* sqrt_weighted_J = cholmod_ssmult(sqrt_W_matrix, J, 0, 1, 1, c);  //sqrt(W)*J

    cholmod_sparse* sqrt_weighted_J_T = cholmod_transpose(sqrt_weighted_J, 1, c);  //J^t*sqrt(W)

    cholmod_sparse* U = cholmod_ssmult(sqrt_weighted_J_T, sqrt_weighted_J, 1, 1, 1, c);  //U = J^t*W*J
    
    //save_dense_matrix_to_txt(U, "U.txt", c);

    //double trace_U = compute_matrix_trace(num_unknowns, U);

    double* DtD_diag = (double*) calloc(num_unknowns, sizeof(double));  //Vector con la diagonal de D^t*D

    if (scale == 1){  //Si escalamos, guardamos la diagonal de U = J^t*W*J
        for (int col = 0; col < num_unknowns; col++) {
            int *U_col_p = (int *) U->p;
            int *U_row_i = (int *) U->i;
            double *Ux = (double *) U->x;

            for (int k = U_col_p[col]; k < U_col_p[col+1]; k++) {
                if (U_row_i[k] == col) {
                    DtD_diag[col] = Ux[k];  // Copiar diagonal de U = J^t*W*J
                    break;
                }
            }
        }

    } else { //Si no escalamos, estaríamos regularizando con la Identidad (diagonal de 1s)
        for (int i = 0; i < num_unknowns; i++){
            DtD_diag[i] = 1.0;
        }
    }

    
    //J^t*W*J + lambda*D^t*D
    cholmod_sparse* U_reg = add_regularization_term(lambda, num_unknowns, DtD_diag, U, c, lang); 

    // cholmod_print_sparse(U_reg, "Matriz U = J^t*W*J (regularizada si hace falta)", c);  //Vista rápida de U_reg
    check_nan_inf_matrix(&nan_count, &inf_count, U_reg); //Comprobación de NaN y +-inf
    // printf("En la matriz U (regularizada si necesario) hay %d NaN y %d +-inf \n\n", nan_count, inf_count);

    //save_dense_matrix_to_txt(U_reg, "U_reg.txt", c);

    SENSEI_PAUSE();

    if (nan_count > 0 || inf_count > 0){
        printf(messages[MSG_ERR_MATRIX_U_NAN_INF][lang], nan_count, inf_count);
        SENSEI_PAUSE();
    }

    cholmod_dense* b = cholmod_zeros(num_unknowns, 1, CHOLMOD_REAL, c);  //Definimos b

    cholmod_sdmult(sqrt_weighted_J, 1, alpha, beta, sqrt_weighted_residuals, b, c); //b = J^t*W*(z-g(x))

    //system("pause");

    cholmod_dense* delta_x = solve_SDP_linear_system(U_reg, b, c, lang);

    double* delta_x_data = (double*) delta_x->x;

    cholmod_free_sparse(&sqrt_weighted_J, c);
    cholmod_free_sparse(&sqrt_weighted_J_T, c);
    cholmod_free_sparse(&U, c);
    cholmod_free_sparse(&U_reg, c);
    cholmod_free_dense(&b, c);

    return delta_x;

}



/**
 * @brief Sirve para añadir el término regularizado lambda*D^t*D a la matriz
 * J^t*W*J, para la ecuación de Levenberg-Marquardt
 * 
 * @note CALIBRACIÓN
 */
cholmod_sparse* add_regularization_term(double lambda, int num_unknowns, double* DtD_diag,
    cholmod_sparse* U, cholmod_common* c, int lang) {
    
    // Convertir a triplet para modificar fácilmente
    cholmod_triplet* T = cholmod_sparse_to_triplet(U, c);

    int* Ti = (int*) T->i;
    int* Tj = (int*) T->j;
    double* Tx = (double*) T->x;
    size_t nz = T->nnz;
    size_t nzmax = T->nzmax;

    // Vector auxiliar para saber qué diagonales ya existen
    bool* diag_present = (bool*) calloc(num_unknowns, sizeof(bool));
    if (!diag_present) {
        if (lang == 0) printf("\033[0;31mError al reservar memoria para diag_present\033[0m\n");
        else printf("\033[0;31mError allocating memory for diag_present\033[0m\n");
        cholmod_free_triplet(&T, c);
        return NULL;
    }

    // Sumar lambda * DtD_diag[i] en las diagonales existentes
    for (size_t k = 0; k < nz; k++) {
        if (Ti[k] == Tj[k]) {
            int i = Ti[k];
            Tx[k] += lambda * DtD_diag[i];
            diag_present[i] = true;
        }
    }

    // Contar cuántas entradas diagonales faltan
    int missing_diags = 0;
    for (int i = 0; i < num_unknowns; i++) {
        if (!diag_present[i]) missing_diags++;
    }

    // Ampliar espacio si es necesario
    size_t required_nz = nz + missing_diags;
    if (required_nz > nzmax) {
        size_t new_nzmax = required_nz + 8;

        T->i = cholmod_realloc(new_nzmax, sizeof(int), T->i, &T->nzmax, c);
        T->j = cholmod_realloc(new_nzmax, sizeof(int), T->j, &T->nzmax, c);
        T->x = cholmod_realloc(new_nzmax, sizeof(double), T->x, &T->nzmax, c);

        if (!T->i || !T->j || !T->x) {
            if (lang == 0) printf("\033[0;31mError al reservar memoria para el triplet.\033[0m\n");
            else printf("\033[0;31mError allocating memory for the triplet.\033[0m\n");
            cholmod_free_triplet(&T, c);
            free(diag_present);
            return NULL;
        }

        Ti = (int*) T->i;
        Tj = (int*) T->j;
        Tx = (double*) T->x;
    }

    // Añadir entradas diagonales que no estaban presentes
    for (int i = 0; i < num_unknowns; i++) {
        if (!diag_present[i]) {
            Ti[T->nnz] = i;
            Tj[T->nnz] = i;
            Tx[T->nnz] = lambda * DtD_diag[i];
            T->nnz++;
        }
    }

    free(diag_present);

    // Convertir de vuelta a sparse
    cholmod_sparse* U_new = cholmod_triplet_to_sparse(T, T->nnz, c);
    cholmod_free_triplet(&T, c);

    return U_new;
}




// FUNCIONES PARA CALCULAR Y ACTUALIZAR VARIABLES 
// ----------------------------------------------


/**
 * @brief Almacena el incremento calculado para las variables (Delta_x) como variables
 * provisionales: desglosa el valor calculado en las estructuras de demandas y
 * niveles iniciales de los tanques
 * 
 * @note EE MULTIPERIODO
 */
void compute_provisional_variables_multiperiod_SE(cholmod_dense* delta_x, int nJ_Dem, int nT, 
    int* ind_J_Dem, double* min_tank_levels, double* max_tank_levels, 
    Variables_Multiperiod_SE Current_Vars, Variables_Multiperiod_SE *Prov_Vars, EN_Project ph){

    int index;

    double* delta_x_data = (double*) delta_x->x;

    for (int i = 0; i < nJ_Dem; i++){
        index = i;
        
        Prov_Vars->base_demands[i] = Current_Vars.base_demands[i] + delta_x_data[index];
        //CONTROL
        if (Prov_Vars->base_demands[i] <= 0){ //La demanda base no debe ser negativa (sería nodo de aportación)
            Prov_Vars->base_demands[i] = 0;
        }
    }

    // printf("Asi quedan las demandas de los 5 primeros nodos de demanda: \n");
    // for (int i = 0; i < 5; i++){
    //     printf("%.4f  ", Prov_Vars->base_demands[i]);
    // }
    // printf("\n\n");



    for (int i = 0; i < nT; i++){
        index = nJ_Dem + i;
        Prov_Vars->init_tank_levels[i] = Current_Vars.init_tank_levels[i] + delta_x_data[index];
        //CONTROL
        if (Prov_Vars->init_tank_levels[i] < min_tank_levels[i]){  //El nivel inicial no puede ser menor que el mínimo
            Prov_Vars->init_tank_levels[i] = min_tank_levels[i];
        } else if (Prov_Vars->init_tank_levels[i] > max_tank_levels[i]){  //El nivel inicial no puede ser mayor que el máximo
            Prov_Vars->init_tank_levels[i] = max_tank_levels[i];
        }
    }

    // printf("Asi quedan los niveles inciales de los tanques provisionales: \n");
    // for (int i = 0; i < nT; i++){
    //     printf("%.4f  ", Prov_Vars->init_tank_levels[i]);
    // }
    // printf("\n\n");

}



/**
 * @brief Almacena el incremento calculado para las variables (Delta_x) como variables
 * provisionales: desglosa el valor calculado en las estructuras de demandas y
 * niveles iniciales de los tanques (también coef. emisor, si se ha activado esa
 * opción)
 * 
 * @note EE INSTANTÁNEA
 */
void compute_provisional_variables_snapshot_SE(cholmod_dense* delta_x, int nJ_Dem, int nT, 
    int add_emitter, int* ind_J_Dem, double* min_tank_levels, double* max_tank_levels, 
    Variables_Snapshot_SE Current_Vars, Variables_Snapshot_SE *Prov_Vars, EN_Project ph, int lang){

    int index;

    double* delta_x_data = (double*) delta_x->x;

    for (int i = 0; i < nJ_Dem; i++){
        index = i;
        
        Prov_Vars->base_demands[i] = Current_Vars.base_demands[i] + delta_x_data[index];
        //CONTROL
        if (Prov_Vars->base_demands[i] <= 0){ //La demanda base no debe ser negativa (sería nodo de aportación)
            Prov_Vars->base_demands[i] = 0;
        }
    }

    // printf("Asi quedan las demandas de los 5 primeros nodos de demanda: \n");
    // for (int i = 0; i < 5; i++){
    //     printf("%.4f  ", Prov_Vars->base_demands[i]);
    // }
    // printf("\n\n");



    for (int i = 0; i < nT; i++){
        index = nJ_Dem + i;
        Prov_Vars->init_tank_levels[i] = Current_Vars.init_tank_levels[i] + delta_x_data[index];
        //CONTROL
        if (Prov_Vars->init_tank_levels[i] < min_tank_levels[i]){  //El nivel inicial no puede ser menor que el mínimo
            Prov_Vars->init_tank_levels[i] = min_tank_levels[i];
        } else if (Prov_Vars->init_tank_levels[i] > max_tank_levels[i]){  //El nivel inicial no puede ser mayor que el máximo
            Prov_Vars->init_tank_levels[i] = max_tank_levels[i];
        }
    }

    // printf("Asi quedan los niveles inciales de los tanques provisionales: \n");
    // for (int i = 0; i < nT; i++){
    //     printf("%.4f  ", Prov_Vars->init_tank_levels[i]);
    // }
    // printf("\n\n");


    if (add_emitter){
        index = nJ_Dem + nT;
        Prov_Vars->emitter_coef = Current_Vars.emitter_coef + delta_x_data[index];

        //CONTROL
        if (Prov_Vars->emitter_coef <= 0){ //El coef. emisor no puede ser menor que 0
            Prov_Vars->emitter_coef = 0;
        }

        if (lang == 0) printf("Coeficiente emisor calculado: %.4f \n\n\n", Prov_Vars->emitter_coef);
        else printf("Computed emitter coefficient: %.4f \n\n\n", Prov_Vars->emitter_coef);
    }

}





/**
 * @brief Almacena el incremento calculado para las variables (Delta_x) como variables
 * provisionales: desglosa el valor calculado en las estructuras adecuadas según
 * el tipo de variable
 * 
 * @note CALIBRACIÓN
 */
void compute_provisional_variables_calibration(double omega, int num_unknowns, 
    cholmod_dense* delta_x, int stage, int** CM, int T, int nDemPat, int nJ_Dem, int nPumpCurves, 
    int* pumpCurveIndices, int nSetCtrls, int nPI, int nV, int nCP, int nMP, int mod_MP, int nT, 
    int nJ, int nR, int* ind_J_Dem, double* min_tank_levels, double* max_tank_levels, 
    Setting_Control_Info* Setting_Controls_Info, Calib_Variables Current_Vars, 
    Calib_Variables *Prov_Vars, Cluster_Pipes_Info* Clusters_Pipes_Info, EN_Project ph, int lang){

    int index;

    int m_pat = CM[stage][0]; //Para modificar los patrones de demanda
    int m_bd = CM[stage][1]; //Para modificar demandas base
    int m_pcX = CM[stage][2]; //Para modificar las coord. X de las curvas de las bombas 
    int m_pcY = CM[stage][3]; //Para modificar las coord. Y de las curvas de las bombas
    int m_set = CM[stage][4]; //Para modificar los settings de las válvulas
    int m_ir = CM[stage][5]; //Para modificar el coef. de rugosidad inidividual de cada tubería.
    int m_cr_DW = CM[stage][6]; //Para modificar coef. de rugosidad común (DW) para grupos de tuberías (luego se convierte a HW)
    int m_cr_HW = CM[stage][7]; //Para modificar coef. de rugosidad común (HW) para grupos de tuberías (luego se extiende a todas las tuberías)
    int m_itl = CM[stage][8]; //Para modificar los niveles iniciales de tanques (initial tank levels)
    int m_td = CM[stage][9]; //Para modificar los niveles iniciales de tanques (initial tank levels)

    double* delta_x_data = (double*) delta_x->x;

    // printf("Delta_x: \n");
    // for (int i = 0; i < num_unknowns; i++){
    //     printf("%.2f  \n", delta_x_data[i]);
    // }
    // printf("\n\n");

    // system("pause");

    if (m_pcX == 1){

        for (int i = 0; i < nPumpCurves; i++){
            Prov_Vars->xPumpCurves[i][0] = 0;
            for (int j = 0; j < 2; j++){
                index = m_pat*nDemPat*T + m_bd*nJ_Dem + i*2+j;
                Prov_Vars->xPumpCurves[i][j+1] = Current_Vars.xPumpCurves[i][j+1] + omega*delta_x_data[index];
                //CONTROL  (la coord. x de un punto no puede ser menor que la del punto anterior)
                if (Prov_Vars->xPumpCurves[i][j+1] <= Prov_Vars->xPumpCurves[i][j]){  
                    Prov_Vars->xPumpCurves[i][j+1] = Prov_Vars->xPumpCurves[i][j]+10.0;
                }
            }
        }

        if (lang == 0) printf("Valores provisionales de las coordenadas X (flujo) de las curvas de las bombas: \n");
        else printf("Provisional values for the X coordinates (flow) of the pump curves: \n");
        for (int i = 0; i < nPumpCurves; i++){
            printf("%.2f  ", Prov_Vars->xPumpCurves[i][0]);
            printf("%.2f  ", Prov_Vars->xPumpCurves[i][1]);
            printf("%.2f  \n", Prov_Vars->xPumpCurves[i][2]);
        }
        printf("\n\n");

    }



    if (m_pcY == 1){
        for (int i = 0; i < nPumpCurves; i++){
            for (int j = 0; j < 3; j++){
                index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + i*3+j;
                Prov_Vars->yPumpCurves[i][j] = Current_Vars.yPumpCurves[i][j] + omega*delta_x_data[index];
                //CONTROL: 
                if (j == 0){  //El head a flujo 0 no puede ser "muy pequeño" (es el primer punto)
                    if (Prov_Vars->yPumpCurves[i][j] < 10){  //Si lo fuera, forzamos a que sea 10
                        Prov_Vars->yPumpCurves[i][j] = 10;
                    }
                } else { //Una coordenada "y" no puede ser mayor que la coordenada "y" del punto anterior
                    if (Prov_Vars->yPumpCurves[i][j] >= Prov_Vars->yPumpCurves[i][j-1]){  
                        Prov_Vars->yPumpCurves[i][j] = Prov_Vars->yPumpCurves[i][j-1]-5.0;
                    }

                    if (Prov_Vars->yPumpCurves[i][j] <= 0){
                        if (Prov_Vars->yPumpCurves[i][j-1] >= 2.0){
                            Prov_Vars->yPumpCurves[i][j] = Prov_Vars->yPumpCurves[i][j-1]-2.0;
                        } else {
                            Prov_Vars->yPumpCurves[i][j] = 0;
                        }
                    }
                }
            }
        }

        if (lang == 0) printf("Valores provisionales de las coordenadas Y (head) de las curvas de las bombas: \n");
        else printf("Provisional values for the Y coordinates (head) of the pump curves: \n");
        for (int i = 0; i < nPumpCurves; i++){
            printf("%.2f  ", Prov_Vars->yPumpCurves[i][0]);
            printf("%.2f  ", Prov_Vars->yPumpCurves[i][1]);
            printf("%.2f  \n", Prov_Vars->yPumpCurves[i][2]);
        }
        printf("\n\n");


    }


    if (m_pat == 1){
        for (int t = 0; t < T; t++){
            for (int i = 0; i < nDemPat; i++){
                Prov_Vars->demand_patterns[t][i] = Current_Vars.demand_patterns[t][i] + omega*delta_x_data[i+t*nDemPat];
                //CONTROL
                if (Prov_Vars->demand_patterns[t][i] <= 0){
                    Prov_Vars->demand_patterns[t][i] = 0;  //La componente de un patrón de demanda no puede ser negativa
                }
            }
        }
    }

    if (m_bd == 1){

        for (int i = 0; i < nJ_Dem; i++){

            index = m_pat*(nDemPat*T) + i;
            
            Prov_Vars->base_demands[i] = Current_Vars.base_demands[i] + omega*delta_x_data[index];
            //CONTROL
            if (Prov_Vars->base_demands[i] <= 0){ //La demanda base no debe ser negativa (sería nodo de aportación)
                Prov_Vars->base_demands[i] = 0;
            }
        }
        //system("pause");

        //edit_ph_base_demands(nJ_Dem, ind_J_Dem, base_demands_prov[0], ph);
    }




    if (m_set == 1){
        
        for (int i = 0; i < nV; i++){
            index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves + i;
            Prov_Vars->valves_settings[i] = Current_Vars.valves_settings[i] + omega*delta_x_data[index];
            //CONTROL
            if (Prov_Vars->valves_settings[i] < 0){  //No dejamos que el setting de sea 0 
                Prov_Vars->valves_settings[i] = 0; //(Si no, el control no tiene sentido)
            }
        }

        if (lang == 0) printf("Valores provisionales de los settings de las válvulas: \n");
        else printf("Provisional values for valves settings: \n");
        for (int i = 0; i < nV; i++){
            printf("%.4f  ", Prov_Vars->valves_settings[i]);
        }
        printf("\n\n");

    }


    if (m_ir == 1){
        if (mod_MP == 1){
            for (int i = 0; i < nMP; i++){
                index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + 
                    m_pcY*3*nPumpCurves + m_set*nV + i;
                Prov_Vars->roughness_coefficients[i] = Current_Vars.roughness_coefficients[i] + omega*delta_x_data[index];
                //CONTROL
                if (Prov_Vars->roughness_coefficients[i] < 50){ //No permitimos que las rugosidades sean menores que 50
                    Prov_Vars->roughness_coefficients[i] = 50;
                } else if (Prov_Vars->roughness_coefficients[i] > 220){ //No permitimos que las rugosidades sean mayores que 220
                    Prov_Vars->roughness_coefficients[i] = 220;
                }
                printf("%.3f  %.3f  \n", Prov_Vars->roughness_coefficients[i], Current_Vars.roughness_coefficients[i]);
                
            }
        } else {
            for (int i = 0; i < nPI; i++){
                index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + 
                    m_pcY*3*nPumpCurves + m_set*nV + i;
                Prov_Vars->roughness_coefficients[i] = Current_Vars.roughness_coefficients[i] + omega*delta_x_data[index];
                //CONTROL
                if (Prov_Vars->roughness_coefficients[i] < 50){ //No permitimos que las rugosidades sean menores que 50
                    Prov_Vars->roughness_coefficients[i] = 50;
                } else if (Prov_Vars->roughness_coefficients[i] > 220){ //No permitimos que las rugosidades sean mayores que 220
                    Prov_Vars->roughness_coefficients[i] = 220;
                }
            }
        }

        //edit_ph_roughness_coefficients(nPI, roughness_coefficients_prov[0], ph);
    }

    if (m_cr_DW == 1){
        for (int i = 0; i < nCP; i++){
            index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves + 
                m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + i;
            Prov_Vars->common_DW_coefs[i] = Current_Vars.common_DW_coefs[i] + omega*delta_x_data[index];
            //CONTROL 
            if (Prov_Vars->common_DW_coefs[i] < 0.3){ //No dejamos que sea 0.3 (MUY POCO RUGOSAS)
                Prov_Vars->common_DW_coefs[i] = 0.3;
            } else if (Prov_Vars->common_DW_coefs[i] > 2.5){ //No dejamos que sea 2.5 (MUY RUGOSAS)
                Prov_Vars->common_DW_coefs[i] = 2.5;
            }
        }

        if (lang == 0) printf("Valores provisionales de los coeficientes de rugosidad de D-W para cada grupo de tuberías: \n");
        else printf("Provisional values for the D-W roughness coefficients of each cluster of pipes: \n");
        for (int i = 0; i < nCP; i++){
            printf("%.4f  ", Prov_Vars->common_DW_coefs[i]);
        }
        printf("\n\n");

        double* common_DW_coefs_prov_to_vector = (double*)malloc(nCP*sizeof(double));

        for (int i = 0; i < nCP; i++){
            common_DW_coefs_prov_to_vector[i] = Prov_Vars->common_DW_coefs[i];
        }

        Prov_Vars->roughness_coefficients = convert_roughness_coefs_from_DW_to_HW(nPI, nCP, common_DW_coefs_prov_to_vector, Clusters_Pipes_Info, ph);
        //edit_ph_roughness_coefficients(nPI, roughness_coefficients_prov[0], ph);
    }


    if (m_cr_HW == 1){
        for (int i = 0; i < nCP; i++){
            index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves + 
                m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + m_cr_DW*nCP + i;
            Prov_Vars->common_HW_coefs[i] = Current_Vars.common_HW_coefs[i] + omega*delta_x_data[index];
            //CONTROL 
            if (Prov_Vars->common_HW_coefs[i] < 50){ //No permitimos que las rugosidades sean menores que 50
                Prov_Vars->common_HW_coefs[i] = 50;
            } else if (Prov_Vars->common_HW_coefs[i] > 220){ //No permitimos que las rugosidades sean mayores que 220
                Prov_Vars->common_HW_coefs[i] = 220;
            }
        }

        if (lang == 0) printf("Valores provisionales de los coeficientes de rugosidad de H-W para cada grupo de tuberías: \n");
        else printf("Provisional values for the H-W roughness coefficients of each cluster of pipes: \n");
        for (int i = 0; i < nCP; i++){
            printf("%.4f  ", Prov_Vars->common_HW_coefs[i]);
        }
        printf("\n\n");

        double* common_HW_coefs_prov_to_vector = (double*)malloc(nCP*sizeof(double));

        for (int i = 0; i < nCP; i++){
            common_HW_coefs_prov_to_vector[i] = Prov_Vars->common_HW_coefs[i];
        }

        Prov_Vars->roughness_coefficients = extend_common_roughness_coefs_to_all_pipes(nPI, nCP, 
            common_HW_coefs_prov_to_vector, Clusters_Pipes_Info, ph);
        //edit_ph_roughness_coefficients(nPI, roughness_coefficients_prov[0], ph); 
    }


    if (m_itl == 1){
        for (int i = 0; i < nT; i++){
            index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves + 
                m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + m_cr_DW*nCP + m_cr_HW*nCP + i;
            Prov_Vars->init_tank_levels[i] = Current_Vars.init_tank_levels[i] + omega*delta_x_data[index];
            //CONTROL
            if (Prov_Vars->init_tank_levels[i] < min_tank_levels[i]){  //El nivel inicial no puede ser menor que el mínimo
                Prov_Vars->init_tank_levels[i] = min_tank_levels[i];
            } else if (Prov_Vars->init_tank_levels[i] > max_tank_levels[i]){  //El nivel inicial no puede ser mayor que el máximo
                Prov_Vars->init_tank_levels[i] = max_tank_levels[i];
            }
        }

        if (lang == 0) printf("Valores provisionales de los niveles iniciales de los tanques: \n");
        else printf("Provisional values for the initial tank levels: \n");
        for (int i = 0; i < nT; i++){
            printf("%.4f  ", Prov_Vars->init_tank_levels[i]);
        }
        printf("\n\n");

        //edit_ph_initial_tank_levels(nJ, nR, nT, ph, init_tank_levels_prov[0]);
    }



    if (m_td == 1){

        for (int i = 0; i < nT; i++){
            index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves + 
                m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + m_cr_DW*nCP + m_cr_HW*nCP + m_itl*nT + i;
            Prov_Vars->tank_diameters[i] = Current_Vars.tank_diameters[i] + omega*delta_x_data[index];
            
            //CONTROL
            if (Prov_Vars->tank_diameters[i] < 0.1){  //El diámetro no puede ser 0 ni negativo
                Prov_Vars->tank_diameters[i] = 0.1;
            } 
        }

        if (lang == 0) printf("Valores provisionales de los diámetros de los tanques: \n");
        else printf("Provisional values for the tank diameters: \n");
        for (int i = 0; i < nT; i++){
            printf("%.4f  ", Prov_Vars->tank_diameters[i]);
        }
        printf("\n\n");

    }

}




/**
 * @brief Actualiza (consolida) las variables de estado con el valor
 * provisional calculado
 * 
 * @note EE MULTIPERIODO
 */
void update_current_variables_with_provisional_variables_multiperiod_SE(int nJ_Dem, int nT, 
    Variables_Multiperiod_SE* Current_Vars, Variables_Multiperiod_SE Prov_Vars, EN_Project ph){

    for (int i = 0; i < nJ_Dem; i++){
        Current_Vars->base_demands[i] = Prov_Vars.base_demands[i];
    }

    for (int i = 0; i < nT; i++){
        Current_Vars->init_tank_levels[i] = Prov_Vars.init_tank_levels[i];
    }

}



/**
 * @brief Actualiza (consolida) las variables de estado con el valor
 * provisional calculado (también coef. emisor, si se ha activado)
 * 
 * @note EE INSTANTÁNEA
 */
void update_current_variables_with_provisional_variables_snapshot_SE(int nJ_Dem, int nT, 
    int add_emitter, Variables_Snapshot_SE* Current_Vars, 
    Variables_Snapshot_SE Prov_Vars, EN_Project ph){

    for (int i = 0; i < nJ_Dem; i++){
        Current_Vars->base_demands[i] = Prov_Vars.base_demands[i];
    }

    for (int i = 0; i < nT; i++){
        Current_Vars->init_tank_levels[i] = Prov_Vars.init_tank_levels[i];
    }

    if (add_emitter){
        Current_Vars->emitter_coef = Prov_Vars.emitter_coef;
    }

}




/**
 * @brief Actualiza (consolida) las variables que optimizamos en esta etapa con
 * el valor provisional calculado
 * 
 * @note CALIBRACIÓN
 */
void update_current_variables_with_provisional_variables_calibration(int stage, int** CM, int T, 
    int nDemPat, int nJ_Dem, int nPumpCurves, int nSetCtrls, int nPI, int nV, int nCP, int nMP, 
    int mod_MP, int nT, Cluster_Pipes_Info* Clusters_Pipes_Info, Calib_Variables* Current_Vars, 
    Calib_Variables Prov_Vars, EN_Project ph){


    int m_pat = CM[stage][0]; //Para modificar los patrones de demanda
    int m_bd = CM[stage][1]; //Para modificar demandas base
    int m_pcX = CM[stage][2]; //Para modificar las coord. X de las curvas de las bombas
    int m_pcY = CM[stage][3]; //Para modificar las coord. Y de las curvas de las bombas
    int m_set = CM[stage][4]; //Para modificar los settings de las válvulas
    int m_ir = CM[stage][5]; //Para modificar el coef. de rugosidad inidividual de cada tubería.
    int m_cr_DW = CM[stage][6]; //Para modificar coef. de rugosidad común (DW) para grupos de tuberías (luego se convierte a HW)
    int m_cr_HW = CM[stage][7]; //Para modificar coef. de rugosidad común (HW) para grupos de tuberías (luego se extiende a todas las tuberías)
    int m_itl = CM[stage][8]; //Para modificar los niveles iniciales de tanques (initial tank levels)
    int m_td = CM[stage][9]; //Para modificar los diámetros de tanques (tank diameters)


    if (m_pcX == 1){
        for (int i = 0; i < nPumpCurves; i++){
            for (int j = 0; j < 3; j++){
                Current_Vars->xPumpCurves[i][j] = Prov_Vars.xPumpCurves[i][j];
            }
        }
    }

    if (m_pcY == 1){
        for (int i = 0; i < nPumpCurves; i++){
            for (int j = 0; j < 3; j++){
                Current_Vars->yPumpCurves[i][j] = Prov_Vars.yPumpCurves[i][j];
            }
        }
    }

    if (m_pat == 1){
        for (int t = 0; t < T; t++){
            for (int i = 0; i < nDemPat; i++){
                Current_Vars->demand_patterns[t][i] = Prov_Vars.demand_patterns[t][i];
            }
        }
    }

    if (m_bd == 1){
        for (int i = 0; i < nJ_Dem; i++){
            Current_Vars->base_demands[i] = Prov_Vars.base_demands[i];
        }
    }


    if (m_set == 1){
        for (int i = 0; i < nV; i++){
            Current_Vars->valves_settings[i] = Prov_Vars.valves_settings[i];
        }
    }

    if (m_ir == 1){
        if (mod_MP == 1){
            for (int i = 0; i < nMP; i++){
                Current_Vars->roughness_coefficients[i] = Prov_Vars.roughness_coefficients[i];
            }
        } else {
            for (int i = 0; i < nPI; i++){
                Current_Vars->roughness_coefficients[i] = Prov_Vars.roughness_coefficients[i];
            }
        }
    }

    if (m_cr_DW == 1){
        for (int i = 0; i < nCP; i++){
            Current_Vars->common_DW_coefs[i] = Prov_Vars.common_DW_coefs[i];
        }

        Current_Vars->roughness_coefficients = convert_roughness_coefs_from_DW_to_HW(nPI, nCP, Prov_Vars.common_DW_coefs, Clusters_Pipes_Info, ph);       
    }

    if (m_cr_HW == 1){
        for (int i = 0; i < nCP; i++){
            Current_Vars->common_HW_coefs[i] = Prov_Vars.common_HW_coefs[i];
        }

        Current_Vars->roughness_coefficients = extend_common_roughness_coefs_to_all_pipes(nPI, nCP, Prov_Vars.common_HW_coefs, Clusters_Pipes_Info, ph);       
    }

    if (m_itl == 1){
        for (int i = 0; i < nT; i++){
            Current_Vars->init_tank_levels[i] = Prov_Vars.init_tank_levels[i];
        }
    }

    if (m_td == 1){
        for (int i = 0; i < nT; i++){
            Current_Vars->tank_diameters[i] = Prov_Vars.tank_diameters[i];
        }
    }

}




// FUNCIONES PARA EDITAR EN EL PH EL VALOR DE VARIABLES
// ----------------------------------------------------



/**
 * @brief Sirve para cargar en el ph los valores actuales de todas las variables que se
 * quieren modificar desde las estructuras específicas. Se hace para no perder
 * precisión tras abrir un .inp o al comenzar una nueva etapa
 * 
 * @warning Cada vez que se hace un EN_setcurvevalue se debe llamar a esta
 * función, porque si no (por problemas de la toolkit) las simulaciones fallan
 * 
 * @note CALIBRACIÓN
 */
void edit_ph_all_current_variables(char* inp_file_name, char* rpt_File, char* binary_File, 
    int nPumpCurves, double** xPumpCurves, double** yPumpCurves, int* pumpCurveIndices, int nPI, 
    int nPU, int nV, int nMP, int mod_MP, int* indMP, int nJ, int nR, int nT, int nJ_Dem, 
    int* ind_J_Dem, int nDemPat, int T, int nSetCtrls, Calib_Variables Current_Vars, Options Opt, 
    Setting_Control_Info* Setting_Controls_Info, EN_Project ph, int lang){

    if (Opt.mod_curves_X == 1 || Opt.mod_curves_Y == 1){
        //Ponemos las curvas (lo primero de todo, porque hay que cerrar y abrir ph!!)
        edit_ph_pump_curves(inp_file_name, rpt_File, binary_File, nPumpCurves, 
            xPumpCurves, yPumpCurves, pumpCurveIndices, ph, lang);
    } 

    if (Opt.mod_individual_roughness == 1 || Opt.mod_common_roughness_DW == 1 || Opt.mod_common_roughness_HW == 1){
        //Ponemos las rugosidades (o bien leídas del .inp, o bien calculadas desde
        //material+edad y convertidas desde DW a HW a través del diámetro, o
        //bien extendidas desde coefs. comunes de HW por grupo a todas las tuberías)
        edit_ph_roughness_coefficients(nPI, nMP, mod_MP, indMP, Current_Vars.roughness_coefficients, ph);
    }
    if (Opt.mod_init_tank_levels == 1){
        //Ponemos los niveles iniciales de los tanques
        edit_ph_initial_tank_levels(nJ, nR, nT, ph, Current_Vars.init_tank_levels);
    }
    if (Opt.mod_base_demands == 1){
        //Ponemos las demandas base 
        edit_ph_base_demands(nJ_Dem, ind_J_Dem, Current_Vars.base_demands, ph);
    }
    if (Opt.mod_patterns == 1){
        //Ponemos los patrones de demanda
        edit_ph_demand_patterns(nDemPat, T, Current_Vars.demand_patterns, ph, lang);
    } if (Opt.mod_valves_settings == 1){
        //Editamos el setting "general" de las válvulas (EN_INITSETTING)
        edit_ph_valves_settings(nPI, nPU, nV, Current_Vars.valves_settings, ph);
        //Ponemos los settings en los controles que haya que editar
        edit_ph_setting_controls(nSetCtrls, Current_Vars.valves_settings, Setting_Controls_Info, ph);
    } if (Opt.mod_tank_diameters == 1){
        //Ponemos los niveles de los tanques
        edit_ph_tank_diameters(nJ, nR, nT, ph, Current_Vars.tank_diameters);
    }
}





/**
 * @brief Fuerza un estado (en elementos de control asociados a controles) al inicio de una simulación
 * 
 * @note EE MULTIPERIODO
 */
void edit_ph_initial_status(Ctrl_Links_Summary* Controls_Links_Summary, EN_Project ph){
    
    for (int i = 0; i < Controls_Links_Summary->nPipePump; i++){
        EN_setlinkvalue(ph, Controls_Links_Summary->pipe_pump_indices[i], 
            EN_INITSTATUS, Controls_Links_Summary->pipe_pump_status[i]);
        // printf("Indice %d, estado %d \n", Controls_Links_Summary->pipe_pump_indices[i], Controls_Links_Summary->pipe_pump_status[i]);
    }

    for (int i = 0; i < Controls_Links_Summary->nValves; i++){
        if (Controls_Links_Summary->valve_status[i] == 0){
            EN_setlinkvalue(ph, Controls_Links_Summary->valve_indices[i], EN_INITSTATUS, EN_CLOSED);
        } else if (Controls_Links_Summary->valve_status[i] == 1){
            EN_setlinkvalue(ph, Controls_Links_Summary->valve_indices[i], EN_INITSTATUS, EN_OPEN);
        } else {
            //Si es 2, es activo -> no hacemos nada!
        }
    } 

}





/**
 * @brief Impone como setting de la válvula el setting necesario (según estado inicial)
 * 
 * @note EE MULTIPERIODO
 */
void edit_ph_init_valve_setting(Ctrl_Links_Summary* Controls_Links_Summary, EN_Project ph){

    for (int i = 0; i < Controls_Links_Summary->nValves; i++){
        double last_setting = Controls_Links_Summary->last_valve_setting[i];
        double current_setting;
        EN_getlinkvalue(ph, Controls_Links_Summary->valve_indices[i], EN_SETTING, &current_setting);
        if (last_setting != -1 && fabs(last_setting-current_setting) > 0){
            EN_setlinkvalue(ph, Controls_Links_Summary->valve_indices[i], EN_INITSETTING, last_setting);
        }
    }

}




/**
 * @brief Impone el último valor calculado de las variables de estado en el ph
 * 
 * @note EE MULTIPERIODO
 */
void edit_ph_provisional_variables_multiperiod_SE(int nJ_Dem, int nT, int nJ, int nR, 
    int* ind_J_Dem, Variables_Multiperiod_SE Prov_Vars, EN_Project ph){

    edit_ph_base_demands(nJ_Dem, ind_J_Dem, Prov_Vars.base_demands, ph);

    edit_ph_initial_tank_levels(nJ, nR, nT, ph, Prov_Vars.init_tank_levels);

}




/**
 * @brief Impone el último valor calculado de las variables de estado en el ph
 * (también coef. emisor, si se ha activado)
 * 
 * @note EE INSTANTÁNEA
 */
void edit_ph_provisional_variables_snapshot_SE(int nJ_Dem, int nT, int nJ, int nR, int add_emitter, 
    int emitter_index, int* ind_J_Dem, Variables_Snapshot_SE Prov_Vars, EN_Project ph){

    edit_ph_base_demands(nJ_Dem, ind_J_Dem, Prov_Vars.base_demands, ph);

    edit_ph_initial_tank_levels(nJ, nR, nT, ph, Prov_Vars.init_tank_levels);

    if (add_emitter){
        EN_setnodevalue(ph, emitter_index, EN_EMITTER, Prov_Vars.emitter_coef);
    }

}





/**
 * @brief Impone el último valor calculado de las variables en el ph. Invoca
 * funciones más pequeñas para imponer cada tipo de variable
 * 
 * @note CALIBRACIÓN
 */
void edit_ph_provisional_variables_calibration(int stage, int** CM, int T, int nDemPat, int nJ_Dem, 
    int nPumpCurves, int* pumpCurveIndices, int nSetCtrls, int nPI, int nPU, int nV, int nCP, 
    int nMP, int mod_MP, int* indMP, int nT, int nJ, int nR, char* inp_file_name, char* rpt_File, 
    char* binary_File, int* ind_J_Dem, Setting_Control_Info* Setting_Controls_Info, 
    Cluster_Pipes_Info* Clusters_Pipes_Info, Calib_Variables Current_Vars, 
    Calib_Variables Prov_Vars, Options Opt, EN_Project ph, int lang){

    int m_pat = CM[stage][0]; //Para modificar los patrones de demanda
    int m_bd = CM[stage][1]; //Para modificar demandas base
    int m_pcX = CM[stage][2]; //Para modificar las coord. X de las curvas de las bombas 
    int m_pcY = CM[stage][3]; //Para modificar las coord. Y de las curvas de las bombas 
    int m_set = CM[stage][4]; //Para modificar los settings de las válvulas
    int m_ir = CM[stage][5]; //Para modificar el coef. de rugosidad inidividual de cada tubería.
    int m_cr_DW = CM[stage][6]; //Para modificar coef. de rugosidad común (DW) para grupos de tuberías (luego se convierte a HW)
    int m_cr_HW = CM[stage][7]; //Para modificar coef. de rugosidad común (HW) para grupos de tuberías (luego se extiende a todas las tuberias)
    int m_itl = CM[stage][8]; //Para modificar los niveles iniciales de tanques (initial tank levels)
    int m_td = CM[stage][9]; //Para modificar los diámetros de tanques (tank diameters)

    double **xPumpCurves, **yPumpCurves;

    if (m_pcX == 1 && m_pcY == 1){
        xPumpCurves = Prov_Vars.xPumpCurves;
        yPumpCurves = Prov_Vars.yPumpCurves;
    } else if (m_pcX == 1 && m_pcY == 0){
        xPumpCurves = Prov_Vars.xPumpCurves;
        yPumpCurves = Current_Vars.yPumpCurves;
    } else if (m_pcX == 0 && m_pcY == 1){
        xPumpCurves = Current_Vars.xPumpCurves;
        yPumpCurves = Prov_Vars.yPumpCurves;
    } 

    if (m_pcX == 1 || m_pcY == 1){
        edit_ph_all_current_variables(inp_file_name, rpt_File, binary_File, nPumpCurves, xPumpCurves, yPumpCurves, 
            pumpCurveIndices, nPI, nPU, nV, nMP, mod_MP, indMP, nJ, nR, nT, nJ_Dem, ind_J_Dem, nDemPat, 
            T, nSetCtrls, Current_Vars, Opt, Setting_Controls_Info, ph, lang);
    }

    if (m_pat == 1){
        edit_ph_demand_patterns(nDemPat, T, Prov_Vars.demand_patterns, ph, lang);
    }

    if (m_bd == 1){
        edit_ph_base_demands(nJ_Dem, ind_J_Dem, Prov_Vars.base_demands, ph);
    }

    if (m_set == 1){
        edit_ph_valves_settings(nPI, nPU, nV, Prov_Vars.valves_settings, ph);

        edit_ph_setting_controls(nSetCtrls, Prov_Vars.valves_settings, Setting_Controls_Info, ph);
    }

    if (m_ir == 1 || m_cr_DW == 1 || m_cr_HW == 1){
        edit_ph_roughness_coefficients(nPI, nMP, mod_MP, indMP, Prov_Vars.roughness_coefficients, ph);
    }

    if (m_itl == 1){
        edit_ph_initial_tank_levels(nJ, nR, nT, ph, Prov_Vars.init_tank_levels);
    }

    if (m_td == 1){
        edit_ph_tank_diameters(nJ, nR, nT, ph, Prov_Vars.tank_diameters);
    }

}





/**
 * @brief Modifica en el ph las demandas base de los nodos de demanda,
 * estableciendo los valores dados por vector
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
void edit_ph_base_demands(int nJ_Dem, int* ind_J_Dem, double* base_demands_vector, EN_Project ph){

    for (int i = 0; i < nJ_Dem; i++){
        EN_setnodevalue(ph, ind_J_Dem[i], EN_BASEDEMAND, base_demands_vector[i]);
    }

}


/**
 * @brief Modifica en el ph los niveles iniciales de los tanques, estableciendo
 * los valores dados por vector
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
void edit_ph_initial_tank_levels(int nJ, int nR, int nT, EN_Project ph, double* init_tank_levels){
    
    for (int i = 0; i < nT; i++) {   
        EN_setnodevalue(ph, i+nJ+nR+1, EN_TANKLEVEL, init_tank_levels[i]);
    }
    
}





/**
 * @brief Impone en el ph el valor calculado para las curvas de las bombas
 * 
 * @note CALIBRACIÓN
 */
void edit_ph_pump_curves(char* inp_file_name, char* rpt_File, char* binary_File, int nPumpCurves, 
    double** xPumpCurves, double** yPumpCurves, int* pumpCurveIndices, EN_Project ph, int lang) {

    int error;

    for (int i = 0; i < nPumpCurves; i++){
        for (int j = 0; j < 3; j++){
            EN_setcurvevalue(ph, pumpCurveIndices[i], j+1, xPumpCurves[i][j], yPumpCurves[i][j]);
        }
    } 

    error = EN_saveinpfile(ph, inp_file_name);
    if (error){
        char msg[256];
        EN_geterror(error, msg, 255);
        if (lang == 0) printf("\n\033[0;31mError al guardar .inp tras modificar las curvas de las bombas (%s) \033[0m \n\n", msg); 
        else printf("\n\033[0;31mError saving .inp after modifying pump curves (%s) \033[0m \n\n", msg); 
    }

    EN_closeH(ph);
    EN_close(ph);
    EN_open(ph, inp_file_name, rpt_File, binary_File);
    EN_openH(ph);

}



/**
 * @brief Impone en el ph el valor calculado para los coefs. de rugosidad calculados
 * 
 * @note CALIBRACIÓN
 */
void edit_ph_roughness_coefficients(int nPI, int nMP, int mod_MP, int* indMP,
    double* roughness_coefficients, EN_Project ph){

    if (mod_MP == 0){
        for (int i = 0; i < nPI; i++){
        EN_setlinkvalue(ph, i+1, EN_ROUGHNESS, roughness_coefficients[i]);
        }
    } else {
        for (int i = 0; i < nMP; i++){
        EN_setlinkvalue(ph, indMP[i], EN_ROUGHNESS, roughness_coefficients[i]);
        }
    }

}



/**
 * @brief Impone en el ph el valor calculado para los patrones de demanda
 * 
 * @note CALIBRACIÓN
 */
void edit_ph_demand_patterns(int nDemPat, int T, double** demand_patterns, EN_Project ph, int lang){
    
    double pattern_values[T];
    int error;

    for (int i = 0; i < nDemPat; i++){
        for (int t = 0; t < T; t++) {
            pattern_values[t] = demand_patterns[t][i];
        }

        // Establecer todos los valores del patrón de una vez
        error = EN_setpattern(ph, i+1, pattern_values, T);
        if (error){
            char pattern_id[60], msg[256];
            EN_geterror(error, msg, 255);
            EN_getpatternid(ph, i+1, pattern_id);
            if (lang == 0) printf("\033[0;31mError al establecer valores del patrón %s (%s)\033[0m\n", pattern_id, msg);
            else printf("\033[0;31mError setting pattern %s values (%s)\033[0m\n", pattern_id, msg);
        }
    }

}




/**
 * @brief Modifica en el ph aquellos controles que usen un setting para las
 * válvulas, con el nuevo valor calculado del setting
 * 
 * @note CALIBRACIÓN
 */
void edit_ph_setting_controls(int nSetCtrls, double* valves_settings, 
    Setting_Control_Info* Setting_Controls_Info, EN_Project ph){

    for (int i = 0; i < nSetCtrls; i++){
        EN_setcontrol(ph, Setting_Controls_Info[i].index, Setting_Controls_Info[i].control_type, Setting_Controls_Info[i].linkIndex, 
           valves_settings[Setting_Controls_Info[i].valve_index], Setting_Controls_Info[i].nodeIndex, Setting_Controls_Info[i].level);
    }

}



/**
 * @brief Impone en el ph el valor calculado para los setting de las válvulas
 * 
 * @note CALIBRACIÓN
 */
void edit_ph_valves_settings(int nPI, int nPU, int nV, double* valves_settings, EN_Project ph){

    for (int i = 0; i < nV; i++){
        int err = EN_setlinkvalue(ph, nPI+nPU+i+1, EN_INITSETTING, valves_settings[i]);
    }

}



/**
 * @brief Impone en el ph el valor calculado para los diámetros de los tanques
 * 
 * @note CALIBRACIÓN
 */
void edit_ph_tank_diameters(int nJ, int nR, int nT, EN_Project ph, double* tank_diameters){
    
    for (int i = 0; i < nT; i++) {   
        EN_setnodevalue(ph, i+nJ+nR+1, EN_TANKDIAM, tank_diameters[i]);
    }
    
}




// FUNCIONES PARA LA PROPAGACIÓN DE INCERTIDUMBRE (y relacionado)
// --------------------------------------------------------------


/**
 * @brief Realiza la progagación de incertidumbre y calcula los residuos
 * normalizados tras el ajuste
 * 
 * @note EE MULTIPERIODO
 */
void propagate_uncertainty_compute_normalized_residuals_multiperiod_SE(int t, int num_unknowns, 
    int nFM, int nFM_avg, int nT, int m, int m_aug, double* gx, double* z_t, double** posterior_CV, 
    double** normalized_residuals, double* sd_final_tank_level, double* weights_vector_t, 
    cholmod_sparse* sqrt_W_matrix, cholmod_sparse* R_matrix, cholmod_sparse* J, cholmod_common *c, int lang){

    int inf_count, nan_count;

    //Limpiar los "pseudo-ceros" del flujo (para no desproporcionar los residuos)
    for (int i = 0; i < nFM+nFM_avg; i++){
        if (fabs(gx[m-(nFM+nFM_avg)+i]) < 0.1){  
            gx[m-(nFM+nFM_avg)+i] = 0;
        }
    }

    //sqrt(W)*J
    cholmod_sparse* sqrt_weighted_J = cholmod_ssmult(sqrt_W_matrix, J, 0, 1, 1, c); 
    // cholmod_print_sparse(sqrt_weighted_J, "sqrt_weighted_J", c);
    // printf("sqrt_weighted_J BIEN! \n");

    //J^t*sqrt(W)
    cholmod_sparse* sqrt_weighted_J_T = cholmod_transpose(sqrt_weighted_J, 1, c);
    // cholmod_print_sparse(sqrt_weighted_J_T, "sqrt_weighted_J_T", c); 
    // printf("sqrt_weighted_J_T BIEN! \n");

    //U = J^t * W * J
    cholmod_sparse *U = cholmod_ssmult(sqrt_weighted_J_T, sqrt_weighted_J, 1, 1, 1, c);  
    // cholmod_print_sparse(U, "matriz U", c);
    // printf("U BIEN! \n");
            
    check_nan_inf_matrix(&nan_count, &inf_count, U);

    if (inf_count > 0 || nan_count > 0){
        printf("%s", messages[MSG_WARN_U_NAN_INF][lang]);
        SENSEI_PAUSE();
    }


    //J^t*W = (J^t*sqrt(W))*sqrt(W)
    cholmod_sparse *J_T_W = cholmod_ssmult(sqrt_weighted_J_T, sqrt_W_matrix, 0, 1, 1, c);
    // printf("J^t*W BIEN! \n");
    // cholmod_print_sparse(J_T_W, "J^t*W", c);

    // U * S_xz = J^t*W  ==> S_xz es la matriz incógnita (X) del sistema
    cholmod_sparse* Sxz = solve_SDP_matricial_system(U, J_T_W, c, lang);
    // printf("S_xz BIEN! \n");
    // cholmod_print_sparse(Sxz, "S_xz", c);

    // S_xz^t
    cholmod_sparse* Sxz_T = cholmod_transpose(Sxz, 1, c);
    // printf("S_xz^t BIEN! \n");
    // cholmod_print_sparse(Sxz_T, "S_xz^t", c);

    //S_xz * R
    cholmod_sparse* Sxz_R = cholmod_ssmult(Sxz, R_matrix, 0, 1, 1, c);
    // printf("S_xz*R BIEN! \n");
    // cholmod_print_sparse(Sxz_R, "S_xz*R", c);

    //C_x = S_xz * R * S_xz^T
    cholmod_sparse* Cx = cholmod_ssmult(Sxz_R, Sxz_T, 1, 1, 1, c);
    // printf("C_x BIEN! \n");
    // cholmod_print_sparse(Cx, "C_x", c);

    // system("pause");


    double *Cx_data = (double *) Cx ->x;   // Valores de la matriz.
    int *Cx_i = (int *) Cx->i;   // Índices de fila para cada nonzero.
    int *Cx_p = (int *) Cx->p;   // Índices de columna para cada nonzero.


    //printf("PROPAGACION DE INCERTIDUMBRE EN LAS VARIABLES DE ESTADO: \n");
    for (int j = 0; j < num_unknowns; j++) {

        if (weights_vector_t[j] != 0){
            int diag_index = -1;  // Índice donde se encuentra el elemento diagonal en la columna j.
            // Recorrer los nonzeros de la columna j.
            for (int k = Cx_p[j]; k < Cx_p[j+1]; k++) {
                if (Cx_i[k] == j) { // Si el índice de fila es igual a j, es el elemento diagonal.
                    diag_index = k;
                    break;
                }
            }
            if (diag_index != -1) {
                //Orden: sd(estimacion)  |  CV (estimacion)  |  valor variable de estado
                //printf("%d) %.6f  %.6f  %.6f \n", j, sqrt(Cx_data[diag_index]), sqrt(Cx_data[diag_index])/fabs(gx[j]), gx[j]);
                double post_CV = sqrt(Cx_data[diag_index])/fabs(gx[j]);
                if (isnan(post_CV) || isinf(post_CV)){
                    posterior_CV[j][t] = 0;
                } else {
                    posterior_CV[j][t] = post_CV;
                }
            } else {
                // Si no se almacena explícitamente, se asume 0 (o según el modelo de la matriz).
                // printf("%d) -  -  %.6f  \n", j, gx[j]);
                posterior_CV[j][t] = 0.0;
            }
        } else {
            posterior_CV[j][t] = -1.0;
        }
    }
    //printf("\n\n\n\n");

    // printf("Posterior CV BIEN! \n");
    // system("pause");


    // Crear una nueva matriz dispersa para almacenar el bloque inferior de J
    cholmod_sparse *Us = cholmod_allocate_sparse(m_aug-num_unknowns, num_unknowns, 
        J->nzmax, J->sorted, J->packed, J->stype, J->xtype, c);
    // cholmod_print_sparse(Us, "Matriz U_s", c);


    // Acceder a los punteros CSC de J
    int *J_p = (int *)J->p;  // Índices de columna
    int *J_i = (int *)J->i;  // Índices de fila
    double *J_data = (double *)J->x;  // Valores

    int *Us_p = (int *)Us->p;
    int *Us_i = (int *)Us->i;
    double *Us_data = (double *)Us->x;

    int nnz = 0;  // Contador de elementos no nulos en la nueva matriz

    // Recorremos cada columna de J
    for (int col = 0; col < num_unknowns; col++) {
        Us_p[col] = nnz;  // Guardar el inicio de la columna en la nueva matriz

        // Recorrer elementos de la columna actual
        for (int k = J_p[col]; k < J_p[col+1]; k++) {
            int row = J_i[k];
            
            // Si la fila está en el bloque inferior, copiarla
            if (row >= num_unknowns) {
                Us_i[nnz] = row - num_unknowns;  // Ajustar fila en la nueva matriz
                Us_data[nnz] = J_data[k];  // Copiar valor
                nnz++;
            }
        }
    }

    // Último índice de la última columna
    Us_p[num_unknowns] = nnz;

    // Ajustar "nzmax" al número real de elementos copiados
    Us->nzmax = nnz;


    cholmod_sparse* Us_Cx = cholmod_ssmult(Us, Cx, 0, 1, 1, c);
    // printf("Us_Cx bien! \n");
    // cholmod_print_sparse(Us_Cx, "Us_Cx", c);

    cholmod_sparse* Us_T = cholmod_transpose(Us, 1, c);
    // printf("Us_T! \n");
    // cholmod_print_sparse(Us_T, "Us^t", c);

    cholmod_sparse* Cz = cholmod_ssmult(Us_Cx, Us_T, 1, 1, 1, c);
    // printf("Cz bien! \n");
    // cholmod_print_sparse(Cz, "C_z", c);

    // system("pause");

    
    double *Cz_data = (double *) Cz->x;   // Valores de la matriz.
    int *Cz_p = (int *) Cz->p;     // Puntero a los inicios de columna.
    int *Cz_i = (int *) Cz->i;     // Índices de fila.


    //printf("PROPAGACION DE INCERTIDUMBRE EN VARIABLES QUE NO SON DE ESTADO \n");
    for (int j = 0; j < m_aug-num_unknowns; j++) {

        if (weights_vector_t[j+num_unknowns] != 0){
            int diag_index = -1;  
            for (int k = Cz_p[j]; k < Cz_p[j+1]; k++) {
                if (Cz_i[k] == j) { // Si el índice de fila es igual a j, es el elemento diagonal.
                    diag_index = k;
                    break;
                }
            }


            if (diag_index != -1) {
                //Orden: sd(estimacion)  |  CV (estimacion)  |  valor estimación
                //printf("%d) %.6f  %.6f  %.6f \n", j+num_unknowns, sqrt(Cz_data[diag_index]), sqrt(Cz_data[diag_index])/fabs(gx[j+num_unknowns]), gx[j+num_unknowns]);
                double post_CV = sqrt(fabs(Cz_data[diag_index]))/fabs(gx[j+num_unknowns]);
                if (isnan(post_CV) || isinf(post_CV)){
                    posterior_CV[j+num_unknowns][t] = 0.0;
                } else {
                    posterior_CV[j+num_unknowns][t] = post_CV;
                }
            } else {
                // Si no se almacena explícitamente, se asume 0 (o según el modelo de la matriz).
                //printf("%d) -  -  %.6f  \n", j+num_unknowns, gx[j+num_unknowns]);
                posterior_CV[j+num_unknowns][t] = 0.0;
            }
        } else {
            posterior_CV[j+num_unknowns][t] = -1.0;
        }
    }
    //printf("\n\n\n\n");

    for (int j = (m_aug-num_unknowns)-nT; j < m_aug-num_unknowns; j++) {
        int diag_index = -1;  
        for (int k = Cz_p[j]; k < Cz_p[j+1]; k++) {
            if (Cz_i[k] == j) { // Si el índice de fila es igual a j, es el elemento diagonal.
                diag_index = k;
                break;
            }
        }

        sd_final_tank_level[j-(m_aug-num_unknowns-nT)] = sqrt(Cz_data[diag_index]);
    }




    // RESIDUOS NORMALIZADOS 

    //J*S_xz
    cholmod_sparse* J_Sxz = cholmod_ssmult(J, Sxz, 0, 1, 1, c);  

    cholmod_sparse *I = cholmod_speye(m_aug, m_aug, CHOLMOD_REAL, c);  //I (Identidad)

    double alpha_cholmod[2] = {1.0, 0.0};
    double beta[2] = {-1.0, 0.0};

    //I - J*S_xz
    cholmod_sparse *I_minus_J_Sxz = cholmod_add(I, J_Sxz, alpha_cholmod, beta, 1, 1, c); 
    // printf("I_minus_J_Sxz bien! \n");
    // cholmod_print_sparse(I_minus_J_Sxz, "I-J*S_xz", c);

    //(I-J*S_xz)*R
    cholmod_sparse *I_minus_J_Sxz_R = cholmod_ssmult(I_minus_J_Sxz, R_matrix, 0, 1, 1, c); 
    // printf("I_minus_J_Sxz_R bien! \n");
    // cholmod_print_sparse(I_minus_J_Sxz_R, "(I-J*S_xz)*R", c);

    // (I-J*S_xz)^t
    cholmod_sparse *I_minus_J_Sxz_T = cholmod_transpose(I_minus_J_Sxz, 1, c);  
    // printf("I_minus_J_Sxz_T bien! \n");
    // cholmod_print_sparse(I_minus_J_Sxz_T, "(I-J*S_xz)^t", c);

    //Omega = (I-J*S_xz)*R*(I-J*S_xz)^t
    cholmod_sparse* Omega = cholmod_ssmult(I_minus_J_Sxz_R, I_minus_J_Sxz_T, 1, 1, 1, c); 
    // printf("Omega bien! \n");
    // cholmod_print_sparse(Omega, "Omega", c);
    

    double* Omega_data = (double *)Omega->x;    
    int* Omega_i = (int *)Omega->i;  
    int* Omega_p = (int *)Omega->p;    

    //printf("RESIDUOS NORMALIZADOS PARA CADA VARIABLE: \n");
    for (int j = 0; j < m_aug; j++) {
        if (weights_vector_t[j] != 0){
            int diag_index = -1;  
            for (int k = Omega_p[j]; k < Omega_p[j+1]; k++) {
                if (Omega_i[k] == j) { 
                    diag_index = k;
                    break;
                }
            }
            if (diag_index != -1) {
                // Orden: sd(residuo) | Residuo studentizado | Residuo (tal cual)
                // printf("%d) %.6f  %.6f  %.6f \n", j, sqrt(Omega_data[diag_index]), (z_t[j] - gx[j])/sqrt(Omega_data[diag_index]), z_t[j] - gx[j]);
                double norm_res = (z_t[j] - gx[j])/sqrt(Omega_data[diag_index]);
                if (isnan(norm_res) || isinf(norm_res)){
                    normalized_residuals[j][t] = 0.0;
                } else {
                    normalized_residuals[j][t] = norm_res;
                }
            } else {
                // Solamente imprimimos el residuo
                // printf("%d) -  -  %.6f  \n", j, z_t[j] - gx[j]);
                // printf("-   " );
                normalized_residuals[j][t] = -1.0;
            }
        } else {
            normalized_residuals[j][t] = 0.0;
        }
    }
    // printf("\n\n");

    // printf("TODO BIEN! \n");

    // system("pause");


    cholmod_free_sparse(&sqrt_weighted_J, c);
    cholmod_free_sparse(&sqrt_weighted_J_T, c);
    cholmod_free_sparse(&U, c);
    cholmod_free_sparse(&J_T_W, c);
    cholmod_free_sparse(&Sxz, c);
    cholmod_free_sparse(&Sxz_T, c);
    cholmod_free_sparse(&Sxz_R, c);
    cholmod_free_sparse(&Cx, c);
    cholmod_free_sparse(&Us, c);
    cholmod_free_sparse(&Us_Cx, c);
    cholmod_free_sparse(&Us_T, c);
    cholmod_free_sparse(&Cz, c);
    cholmod_free_sparse(&J_Sxz, c);
    cholmod_free_sparse(&I, c);
    cholmod_free_sparse(&I_minus_J_Sxz, c);
    cholmod_free_sparse(&I_minus_J_Sxz_R, c);
    cholmod_free_sparse(&I_minus_J_Sxz_T, c);
    cholmod_free_sparse(&Omega, c);

}







/**
 * @brief Realiza la progagación de incertidumbre y calcula los residuos
 * normalizados de la EE instantánea
 * 
 * @note EE INSTANTÁNEA
 */
void propagate_uncertainty_compute_normalized_residuals_snapshot_SE(int num_unknowns, int nFM, 
    int m, double* gx, double* z, double* posterior_CV, double* normalized_residuals, 
    double* weights_vector, cholmod_sparse* sqrt_W_matrix, cholmod_sparse* R_matrix, 
    cholmod_sparse* J, cholmod_common *c, int lang){

    int inf_count, nan_count;

    //Limpiar los "pseudo-ceros" del flujo (para no desproporcionar los residuos)
    for (int i = 0; i < nFM; i++){
        if (fabs(gx[m-nFM+i]) < 0.1){  
            gx[m-nFM+i] = 0;
        }
    }

    //sqrt(W)*J
    cholmod_sparse* sqrt_weighted_J = cholmod_ssmult(sqrt_W_matrix, J, 0, 1, 1, c); 
    //printf("sqrt_weighted_J BIEN! \n");

    //J^t*sqrt(W)
    cholmod_sparse* sqrt_weighted_J_T = cholmod_transpose(sqrt_weighted_J, 1, c); 
    //printf("sqrt_weighted_J_T BIEN! \n");

    //U = J^t * W * J
    cholmod_sparse *U = cholmod_ssmult(sqrt_weighted_J_T, sqrt_weighted_J, 1, 1, 1, c);  
    //printf("U BIEN! \n");
            
    check_nan_inf_matrix(&nan_count, &inf_count, U);

    if (inf_count > 0 || nan_count > 0){
        printf("%s", messages[MSG_WARN_U_NAN_INF][lang]);
        SENSEI_PAUSE();
    }


    //J^t*W = (J^t*sqrt(W))*sqrt(W)
    cholmod_sparse *J_T_W = cholmod_ssmult(sqrt_weighted_J_T, sqrt_W_matrix, 0, 1, 1, c);
    //printf("J^t*W BIEN! \n");

    // U * S_xz = J^t*W  ==> S_xz es la matriz incógnita (X) del sistema
    cholmod_sparse* Sxz = solve_SDP_matricial_system(U, J_T_W, c, lang);
    //printf("S_xz BIEN! \n");

    // S_xz^t
    cholmod_sparse* Sxz_T = cholmod_transpose(Sxz, 1, c);
    //printf("S_xz^t BIEN! \n");

    //S_xz * R
    cholmod_sparse* Sxz_R = cholmod_ssmult(Sxz, R_matrix, 0, 1, 1, c);
    //printf("S_xz*R BIEN! \n");

    //C_x = S_xz * R * S_xz^T
    cholmod_sparse* Cx = cholmod_ssmult(Sxz_R, Sxz_T, 1, 1, 1, c);
    //printf("C_x BIEN! \n");


    double *Cx_data = (double *) Cx ->x;   // Valores de la matriz.
    int *Cx_i = (int *) Cx->i;   // Índices de fila para cada nonzero.
    int *Cx_p = (int *) Cx->p;   // Índices de columna para cada nonzero.


    //printf("PROPAGACION DE INCERTIDUMBRE EN LAS VARIABLES DE ESTADO: \n");
    for (int j = 0; j < num_unknowns; j++) {

        if (weights_vector[j] != 0){
            int diag_index = -1;  // Índice donde se encuentra el elemento diagonal en la columna j.
            // Recorrer los nonzeros de la columna j.
            for (int k = Cx_p[j]; k < Cx_p[j+1]; k++) {
                if (Cx_i[k] == j) { // Si el índice de fila es igual a j, es el elemento diagonal.
                    diag_index = k;
                    break;
                }
            }
            if (diag_index != -1) {
                //Orden: sd(estimacion)  |  CV (estimacion)  |  valor variable de estado
                //printf("%d) %.6f  %.6f  %.6f \n", j, sqrt(Cx_data[diag_index]), sqrt(Cx_data[diag_index])/fabs(gx[j]), gx[j]);
                double post_CV = sqrt(Cx_data[diag_index])/fabs(gx[j]);
                if (isnan(post_CV) || isinf(post_CV)){
                    posterior_CV[j] = 0;
                } else {
                    posterior_CV[j] = post_CV;
                }
            } else {
                // Si no se almacena explícitamente, se asume 0 (o según el modelo de la matriz).
                // printf("%d) -  -  %.6f  \n", j, gx[j]);
                posterior_CV[j] = 0.0;
            }
        } else {
            posterior_CV[j] = -1.0;
        }
    }
    //printf("\n\n\n\n");


    // Crear una nueva matriz dispersa para almacenar el bloque inferior de J
    cholmod_sparse *Us = cholmod_allocate_sparse(m-num_unknowns, num_unknowns, 
        J->nzmax, J->sorted, J->packed, J->stype, J->xtype, c);


    // Acceder a los punteros CSC de J
    int *J_p = (int *)J->p;  // Índices de columna
    int *J_i = (int *)J->i;  // Índices de fila
    double *J_data = (double *)J->x;  // Valores

    int *Us_p = (int *)Us->p;
    int *Us_i = (int *)Us->i;
    double *Us_data = (double *)Us->x;

    int nnz = 0;  // Contador de elementos no nulos en la nueva matriz

    // Recorremos cada columna de J
    for (int col = 0; col < num_unknowns; col++) {
        Us_p[col] = nnz;  // Guardar el inicio de la columna en la nueva matriz

        // Recorrer elementos de la columna actual
        for (int k = J_p[col]; k < J_p[col+1]; k++) {
            int row = J_i[k];
            
            // Si la fila está en el bloque inferior, copiarla
            if (row >= num_unknowns) {
                Us_i[nnz] = row - num_unknowns;  // Ajustar fila en la nueva matriz
                Us_data[nnz] = J_data[k];  // Copiar valor
                nnz++;
            }
        }
    }

    // Último índice de la última columna
    Us_p[num_unknowns] = nnz;

    // Ajustar "nzmax" al número real de elementos copiados
    Us->nzmax = nnz;

    cholmod_sparse* Us_Cx = cholmod_ssmult(Us, Cx, 0, 1, 1, c);

    cholmod_sparse* Us_T = cholmod_transpose(Us, 1, c);

    cholmod_sparse* Cz = cholmod_ssmult(Us_Cx, Us_T, 1, 1, 1, c);

    
    double *Cz_data = (double *) Cz->x;   // Valores de la matriz.
    int *Cz_p = (int *) Cz->p;     // Puntero a los inicios de columna.
    int *Cz_i = (int *) Cz->i;     // Índices de fila.


    //printf("PROPAGACION DE INCERTIDUMBRE EN VARIABLES QUE NO SON DE ESTADO \n");
    for (int j = 0; j < m-num_unknowns; j++) {

        if (weights_vector[j+num_unknowns] != 0){
            int diag_index = -1;  
            for (int k = Cz_p[j]; k < Cz_p[j+1]; k++) {
                if (Cz_i[k] == j) { // Si el índice de fila es igual a j, es el elemento diagonal.
                    diag_index = k;
                    break;
                }
            }


            if (diag_index != -1) {
                //Orden: sd(estimacion)  |  CV (estimacion)  |  valor estimación
                //printf("%d) %.6f  %.6f  %.6f \n", j+num_unknowns, sqrt(Cz_data[diag_index]), sqrt(Cz_data[diag_index])/fabs(gx[j+num_unknowns]), gx[j+num_unknowns]);
                double post_CV = sqrt(fabs(Cz_data[diag_index]))/fabs(gx[j+num_unknowns]);
                if (isnan(post_CV) || isinf(post_CV)){
                    posterior_CV[j+num_unknowns] = 0.0;
                } else {
                    posterior_CV[j+num_unknowns] = post_CV;
                }
            } else {
                // Si no se almacena explícitamente, se asume 0 (o según el modelo de la matriz).
                //printf("%d) -  -  %.6f  \n", j+num_unknowns, gx[j+num_unknowns]);
                posterior_CV[j+num_unknowns] = 0.0;
            }
        } else {
            posterior_CV[j+num_unknowns] = -1.0;
        }
    }
    //printf("\n\n\n\n");



    // RESIDUOS STUDENTIZADOS 

    //J*S_xz
    cholmod_sparse* J_Sxz = cholmod_ssmult(J, Sxz, 0, 1, 1, c);  

    cholmod_sparse *I = cholmod_speye(m, m, CHOLMOD_REAL, c);  //I (Identidad)

    double alpha_cholmod[2] = {1.0, 0.0};
    double beta[2] = {-1.0, 0.0};

    //I - J*S_xz
    cholmod_sparse *I_minus_J_Sxz = cholmod_add(I, J_Sxz, alpha_cholmod, beta, 1, 1, c); 

    //(I-J*S_xz)*R
    cholmod_sparse *I_minus_J_Sxz_R = cholmod_ssmult(I_minus_J_Sxz, R_matrix, 0, 1, 1, c); 

    // (I-J*S_xz)^t
    cholmod_sparse *I_minus_J_Sxz_T = cholmod_transpose(I_minus_J_Sxz, 1, c);  

    //Omega = (I-J*S_xz)*R*(I-J*S_xz)^t
    cholmod_sparse* Omega = cholmod_ssmult(I_minus_J_Sxz_R, I_minus_J_Sxz_T, 1, 1, 1, c); 
    

    double* Omega_data = (double *)Omega->x;    
    int* Omega_i = (int *)Omega->i;  
    int* Omega_p = (int *)Omega->p;    

    //printf("RESIDUOS NORMALIZADOS PARA CADA VARIABLE: \n");
    for (int j = 0; j < m; j++) {
        if (weights_vector[j] != 0){
            int diag_index = -1;  
            for (int k = Omega_p[j]; k < Omega_p[j+1]; k++) {
                if (Omega_i[k] == j) { 
                    diag_index = k;
                    break;
                }
            }
            if (diag_index != -1) {
                // Orden: sd(residuo) | Residuo studentizado | Residuo (tal cual)
                // printf("%d) %.6f  %.6f  %.6f \n", j, sqrt(Omega_data[diag_index]), (z_t[j] - gx[j])/sqrt(Omega_data[diag_index]), z_t[j] - gx[j]);
                double norm_res = (z[j] - gx[j])/sqrt(Omega_data[diag_index]);
                if (isnan(norm_res) || isinf(norm_res)){
                    normalized_residuals[j] = 0.0;
                } else {
                    normalized_residuals[j] = norm_res;
                }
            } else {
                // Solamente imprimimos el residuo
                // printf("%d) -  -  %.6f  \n", j, z_t[j] - gx[j]);
                // printf("-   " );
                normalized_residuals[j] = -1.0;
            }
        } else {
            normalized_residuals[j] = 0.0;
        }
    }
    printf("\n\n");


    cholmod_free_sparse(&sqrt_weighted_J, c);
    cholmod_free_sparse(&sqrt_weighted_J_T, c);
    cholmod_free_sparse(&U, c);
    cholmod_free_sparse(&J_T_W, c);
    cholmod_free_sparse(&Sxz, c);
    cholmod_free_sparse(&Sxz_T, c);
    cholmod_free_sparse(&Sxz_R, c);
    cholmod_free_sparse(&Cx, c);
    cholmod_free_sparse(&Us, c);
    cholmod_free_sparse(&Us_Cx, c);
    cholmod_free_sparse(&Us_T, c);
    cholmod_free_sparse(&Cz, c);
    cholmod_free_sparse(&J_Sxz, c);
    cholmod_free_sparse(&I, c);
    cholmod_free_sparse(&I_minus_J_Sxz, c);
    cholmod_free_sparse(&I_minus_J_Sxz_R, c);
    cholmod_free_sparse(&I_minus_J_Sxz_T, c);
    cholmod_free_sparse(&Omega, c);

}





/**
 * @brief Realiza la progagación de incertidumbre para todas las variables que
 * se han tratado de optimizar durante el proceso de calibración
 * 
 * @note CALIBRACIÓN
 */
void uncertainty_propagation_calibration(int num_unknowns, int T, int nFM, int nFM_avg, 
    int m, double* gx, double* z, double* posterior_sd, double* weights_vector, 
    cholmod_sparse* sqrt_W_matrix, cholmod_sparse* R_matrix, cholmod_sparse* J, 
    cholmod_common *c, int lang){

    if (lang == 0) printf("PROPAGACIÓN DE INCERTIDUMBRE EN PROCESO... \n");
    else printf("UNCERTAINTY PROPAGATION IN PROGRESS... \n");

    int inf_count, nan_count;

    //Limpiar los "pseudo-ceros" del flujo (para no desproporcionar los residuos)
    for (int i = 0; i < nFM+nFM_avg; i++){
        for (int t = 0; t < T; t++){
            if (fabs(gx[m*t-(nFM+nFM_avg)+i]) < 0.1){  
                gx[m*t-(nFM+nFM_avg)+i] = 0;
            }
        }
    }

    //sqrt(W)*J
    cholmod_sparse* sqrt_weighted_J = cholmod_ssmult(sqrt_W_matrix, J, 0, 1, 1, c); 
    // save_dense_matrix_to_txt(sqrt_weighted_J, "sqrt_weighted_J.txt", c);
    // printf("sqrt_weighted_J BIEN! \n\n");

    //J^t*sqrt(W)
    cholmod_sparse* sqrt_weighted_J_T = cholmod_transpose(sqrt_weighted_J, 1, c); 
    // save_dense_matrix_to_txt(sqrt_weighted_J_T, "sqrt_weighted_J_T.txt", c);
    // printf("sqrt_weighted_J_T BIEN! \n\n");

    //U = J^t * W * J
    cholmod_sparse *U = cholmod_ssmult(sqrt_weighted_J_T, sqrt_weighted_J, 1, 1, 1, c);  
    // save_dense_matrix_to_txt(U, "U.txt", c);
    // printf("U BIEN! \n\n");
            
    check_nan_inf_matrix(&nan_count, &inf_count, U);

    if (inf_count > 0 || nan_count > 0){
        printf("%s", messages[MSG_WARN_U_NAN_INF][lang]);
        SENSEI_PAUSE();
    }


    //J^t*W = (J^t*sqrt(W))*sqrt(W)
    cholmod_sparse *J_T_W = cholmod_ssmult(sqrt_weighted_J_T, sqrt_W_matrix, 0, 1, 1, c);
    // save_dense_matrix_to_txt(J_T_W, "J_T_W.txt", c);
    // printf("J^t*W BIEN! \n\n");


    // U * S_xz = J^t*W  ==> S_xz es la matriz incógnita (X) del sistema
    cholmod_sparse* Sxz = solve_SDP_matricial_system(U, J_T_W, c, lang);
    // save_dense_matrix_to_txt(Sxz, "S_xz.txt", c);
    // printf("S_xz BIEN! \n\n");

    // S_xz^t
    cholmod_sparse* Sxz_T = cholmod_transpose(Sxz, 1, c);
    // save_dense_matrix_to_txt(Sxz_T, "S_xz_T.txt", c);
    // printf("S_xz^t BIEN! \n");

    //S_xz * R
    cholmod_sparse* Sxz_R = cholmod_ssmult(Sxz, R_matrix, 0, 1, 1, c);
    // save_dense_matrix_to_txt(Sxz_R, "S_xz_R.txt", c);
    // printf("S_xz*R BIEN! \n");

    if (lang == 0) printf("(El proceso podría tardar unos minutos) \n\n");
    else (printf("(The process may take a few minutes)"));

    //C_x = S_xz * R * S_xz^T
    cholmod_sparse* Cx = cholmod_ssmult(Sxz_R, Sxz_T, 1, 1, 1, c);
    // save_dense_matrix_to_txt(Cx, "C_x.txt", c);
    // printf("C_x BIEN! \n");


    double *Cx_data = (double *) Cx ->x;   // Valores de la matriz.
    int *Cx_i = (int *) Cx->i;   // Índices de fila para cada nonzero.
    int *Cx_p = (int *) Cx->p;   // Índices de columna para cada nonzero.


    //printf("PROPAGACION DE INCERTIDUMBRE EN LAS VARIABLES DE ESTADO: \n");
    for (int j = 0; j < num_unknowns; j++) {

        int diag_index = -1;  // Índice donde se encuentra el elemento diagonal en la columna j.
        // Recorrer los nonzeros de la columna j.
        for (int k = Cx_p[j]; k < Cx_p[j+1]; k++) {
            if (Cx_i[k] == j) { // Si el índice de fila es igual a j, es el elemento diagonal.
                diag_index = k;
                break;
            }
        }
        if (diag_index != -1) {
            //Orden: sd(estimacion)  |  CV (estimacion)  |  valor variable de estado
            //printf("%d) %.6f  %.6f  %.6f \n", j, sqrt(Cx_data[diag_index]), sqrt(Cx_data[diag_index])/fabs(gx[j]), gx[j]);
            // double post_CV = sqrt(Cx_data[diag_index])/fabs(gx[j]);
            double post_sd = sqrt(Cx_data[diag_index]);
            if (isnan(post_sd) || isinf(post_sd)){
                posterior_sd[j] = 0;
            } else {
                posterior_sd[j] = post_sd;
            }
        } else {
            // Si no se almacena explícitamente, se asume 0 (o según el modelo de la matriz).
            // printf("%d) -  -  %.6f  \n", j, gx[j]);
            posterior_sd[j] = 0.0;
        }
    }
    //printf("\n\n\n\n");

    cholmod_free_sparse(&sqrt_weighted_J, c);
    cholmod_free_sparse(&sqrt_weighted_J_T, c);
    cholmod_free_sparse(&U, c);
    cholmod_free_sparse(&J_T_W, c);
    cholmod_free_sparse(&Sxz, c);
    cholmod_free_sparse(&Sxz_T, c);
    cholmod_free_sparse(&Sxz_R, c);
    cholmod_free_sparse(&Cx, c);

}




/**
 * @brief Se propaga la incertidumbre, se guarda la información del ajuste del
 * periodo y se imprimen resultados relevantes sobre el ajuste
 * 
 * @note EE MULTIPERIODO
 */
void end_iteration_and_print_results(int t, int nJ, int nR, int nT, int nDM, int nPM, int nFM, 
    int nFM_avg, int num_unknowns, int m, int m_aug, int* ind_J_Dem, int* pm_positions, 
    int* fm_positions, int* fm_avg_positions, double* z_t, double SWSE, double* weights_vector_t, 
    double* last_demands, double* init_tank_levels_next_period, double* gx, double** estimated_values, 
    double** posterior_CV, double** normalized_residuals, double* sd_final_tank_level, 
    cholmod_sparse* sqrt_W_matrix, cholmod_sparse* R_matrix, cholmod_sparse* J, cholmod_common *c, 
    Ctrl_Links_Summary* Controls_Links_Summary, EN_Project ph, int lang){
        
    char element_id[32];  


    //Guardamos las demandas de este intervalo
    for (int i = 0; i < nDM; i++){
        last_demands[i] = gx[i];
    }

    
    //Ponemos como iniciales del periodo siguiente los calculados (simulados) al
    //final de este periodo
    for (int i = 0; i < nT; i++){
        init_tank_levels_next_period[i] = gx[i+m];
    }

    // Almacenar los valores ajustados
    for (int i = 0; i < m_aug; i++) {
        estimated_values[i][t] = gx[i];
    }

    // Propagación incertidumbre + residuos estudentizados
    propagate_uncertainty_compute_normalized_residuals_multiperiod_SE(t, num_unknowns, nFM, 
        nFM_avg, nT, m, m_aug, gx, z_t, posterior_CV, normalized_residuals, sd_final_tank_level, 
        weights_vector_t, sqrt_W_matrix, R_matrix, J, c, lang);

    if (lang == 0){
        printf("\033[4;36mRESUMEN DE LA EE DEL PERIODO t = %d\033[0m\n", t);
        printf("\033[0;34mValor de la función objetivo (SWSE): %.4f \033[0m\n\n", SWSE);
        printf("\033[0;34mEstimaciones del nivel final en los tanques y CV de la estimación: \033[0m\n");
    } else {
        printf("\033[4;36mSUMMARY OF THE SE FOR PERIOD t = %d\033[0m\n", t);
        printf("\033[0;34mObjective function value (SWSE): %.4f \033[0m\n\n", SWSE);
        printf("\033[0;34mEstimates of the final tank levels and CV of the estimation: \033[0m\n");
    }



    for (int i = 0; i < nT; i++){
        if (isnan(sd_final_tank_level[i]) || sd_final_tank_level[i] < 1e-8){
            sd_final_tank_level[i] = 0.001*gx[i+m];
        }
    }

    for (int i = 0; i < nT; i++) {
        EN_getnodeid(ph, nJ+nR+i+1, element_id);
        if (lang == 0){
            printf("\033[0;34m%s) Nivel final estimado:%.12f; CV: %.12f \033[0m\n", 
                element_id, gx[i+m], sd_final_tank_level[i] / gx[i+m]);
        } else {
            printf("\033[0;34m%s) Estimated final tank level:%.12f; CV: %.12f \033[0m\n", 
                element_id, gx[i+m], sd_final_tank_level[i] / gx[i+m]);
        }
    }

    printf("\n\n");

    if (lang == 0) printf("\033[4;36mESTADO FINAL DE LOS LINKS AFECTADOS POR REGLAS DE CONTROL:\033[0m\n");
    else printf("\033[4;36mFINAL STATUS OF THE LINKS AFFECTED BY CONTROL RULES:\033[0m\n");

    if (Controls_Links_Summary->nPipePump > 0){
        if (lang == 0) printf("\033[1;36mTUBERÍAS y BOMBAS:\033[0m\n");
        else printf("\033[1;36mPIPES and PUMPS:\033[0m\n");
        for (int i = 0; i < Controls_Links_Summary->nPipePump; i++){
            EN_getlinkid(ph, Controls_Links_Summary->pipe_pump_indices[i], element_id);
            printf("%s) ", element_id);
            if (Controls_Links_Summary->pipe_pump_status[i] == 0) printf("CLOSED \n");
            else printf("OPEN \n");
        }
        printf("\n");
    }

    if (Controls_Links_Summary->nValves > 0){
        if (lang == 0) printf("\033[1;36mVÁLVULAS:\033[0m\n");
        else printf("\033[1;36mVALVES:\033[0m\n");
        for (int i = 0; i < Controls_Links_Summary->nValves; i++){
            EN_getlinkid(ph, Controls_Links_Summary->valve_indices[i], element_id);
            printf("%s) ", element_id);
            if (Controls_Links_Summary->valve_status[i] == 0) printf("CLOSED \n");
            else if (Controls_Links_Summary->valve_status[i] == 1) printf("OPEN \n");
            else if (Controls_Links_Summary->valve_status[i] == 2) printf("ACTIVE \n");
        }
        printf("\n");
    }
    

    SENSEI_PAUSE();

    if (lang == 0) printf("\033[4;34mVALORES ESTIMADOS g(x*) | MEDIDAS / PSEUDOMEDIDAS z\033[0m\n");
    else printf("\033[4;34mESTIMATED VALUES g(x*) | MEASUREMENTS / PSEUDO-MEASUREMENTS z\033[0m\n");
    for (int i = 0; i < nDM; i++) {
        EN_getnodeid(ph, ind_J_Dem[i], element_id);
        printf("\033[0;34mDem(%s)  %.4f  |  %.4f  \033[0m\n", element_id, gx[i], z_t[i]);
    }
    printf("\033[0;34m ............... \033[0m\n");

    for (int i = 0; i < nT; i++) {
        EN_getnodeid(ph, nJ+nR+i+1, element_id);
        if (lang == 0) printf("\033[0;34mNiv.ini(%s)  %.4f  |  %.4f  \033[0m\n", element_id, gx[i+nDM], z_t[i+nDM]);
        else printf("\033[0;34mInit. lev.(%s)  %.4f  |  %.4f  \033[0m\n", element_id, gx[i+nDM], z_t[i+nDM]);
    }
    printf("\033[0;34m ............... \033[0m\n");

    if (nPM > 0){
        for (int i = 0; i < nPM; i++) {
            EN_getnodeid(ph, pm_positions[i], element_id);
            printf("\033[0;34mP(%s)  %.4f  |  %.4f  \033[0m\n", element_id, gx[i+nDM+nT], z_t[i+nDM+nT]);
        }
        printf("\033[0;34m ............... \033[0m\n");
    }

    if (nFM > 0){
        for (int i = 0; i < nFM; i++) {
            EN_getlinkid(ph, fm_positions[i], element_id);
            printf("\033[0;34mQ(%s)  %.4f  |  %.4f  \033[0m\n", element_id, gx[i+nDM+nT+nPM], z_t[i+nDM+nT+nPM]);
        }
        printf("\033[0;34m ............... \033[0m\n");
    }

    if (nFM_avg){
        for (int i = 0; i < nFM_avg; i++) {
            EN_getlinkid(ph, fm_avg_positions[i], element_id);
            printf("\033[0;34mQ_avg(%s)  %.4f  |  %.4f  \033[0m\n", element_id, gx[i+nDM+nT+nPM+nFM], z_t[i+nDM+nT+nPM+nFM]);
        }
        printf("\033[0;34m ............... \033[0m\n");
    }

    for (int i = 0; i < nT; i++) {
        EN_getnodeid(ph, nJ+nR+i+1, element_id);
        if (lang == 0) printf("\033[0;34mNiv.fin(%s)  %.4f  |  %.4f  \033[0m\n", element_id, gx[i+m], z_t[i+m]);
        else printf("\033[0;34mFinal lev.(%s)  %.4f  |  %.4f  \033[0m\n", element_id, gx[i+m], z_t[i+m]);
    }
    printf("\n\n\n\n");

}







// FUNCIONES PARA GUARDAR MATRICES / RESULTADOS EN ARCHIVOS .csv 
// -------------------------------------------------------------


/**
 * @brief Guarda en un .csv una matriz de resultados de la EE. Cada fila es una
 * variable y cada columna corresponde a un periodo
 * 
 * @param results_matrix Puede ser de valores ajustados, residuos normalizados o
 * CV posterior
 * 
 * @note EE MULTIPERIODO
 */
void save_SE_results_matrix_to_csv(char* file_name, int nJ, int nR, int nDM, int nWLM, int nPM, 
    int nFM, int nFM_avg, int m, int T, int* ind_J_Dem, int* wlm_positions, int* pm_positions, 
    int* fm_positions, int* fm_avg_positions, double** results_matrix, EN_Project ph, int lang){

    FILE *file = fopen(file_name, "w");
    
    if (!file) {
        if (lang == 0) printf("\033[1;31mERROR al crear el archivo %s \033[0m \n", file_name);
        else printf("\033[1;31mERROR creating the file %s \033[0m \n", file_name);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char element_id[32];  

    for (int i = 0; i < nDM; i++) {
        //En la primera columna poner que es la demanda de un nodo (ej: Dem(J1))
        EN_getnodeid(ph, ind_J_Dem[i], element_id);
        fprintf(file, "Dem; %s;", element_id);
        
        for (int t = 0; t < T; t++) {  //En el resto poner los resultados en results_matrix
            fprintf(file, "%.6f", results_matrix[i][t]);

            if (t < T) fprintf(file, ";"); // espacio entre columnas
        }
        fprintf(file, "\n"); // salto de línea al final de cada fila
    }


    for (int i = 0; i < nWLM; i++) {
        //En la primera columna poner que es el nivel inicial de un tanque (ej: Niv.ini(T1))
        EN_getnodeid(ph, wlm_positions[i]+nJ+nR, element_id);
        if (lang == 0) fprintf(file, "Niv.ini; %s;", element_id);
        else fprintf(file, "Init. lev.; %s;", element_id);
        
        for (int t = 0; t < T; t++) {  //En el resto poner los resultados de results_matrix
            fprintf(file, "%.6f", results_matrix[i+nDM][t]);

            if (t < T) fprintf(file, ";"); // espacio entre columnas
        }
        fprintf(file, "\n"); // salto de línea al final de cada fila
    }


    for (int i = 0; i < nPM; i++) {
        //En la primera columna poner que es la presión de un junction (ej: Pr(J1))
        EN_getnodeid(ph, pm_positions[i], element_id);
        fprintf(file, "Pr; %s;", element_id);
        
        for (int t = 0; t < T; t++) {  //En el resto poner los resultados de results_matrix
            fprintf(file, "%.6f", results_matrix[i+nDM+nWLM][t]);

            if (t < T) fprintf(file, ";"); // espacio entre columnas
        }
        fprintf(file, "\n"); // salto de línea al final de cada fila
    }


    for (int i = 0; i < nFM; i++) {
        //En la primera columna poner que es el flujo al inicio en una tubería (ej: Q_ini(PI1))
        EN_getlinkid(ph, fm_positions[i], element_id);
        fprintf(file, "Q; %s;", element_id);
        
        for (int t = 0; t < T; t++) {  //En el resto poner los resultados de results_matrix
            fprintf(file, "%.6f", results_matrix[i+nDM+nWLM+nPM][t]);

            if (t < T) fprintf(file, ";"); // espacio entre columnas
        }
        fprintf(file, "\n"); // salto de línea al final de cada fila
    }


    for (int i = 0; i < nFM_avg; i++) {
        //En la primera columna poner que es el flujo en una tubería promediado
        //durante un intervalo (ej: Q_avg(PI1))
        EN_getlinkid(ph, fm_avg_positions[i], element_id);
        fprintf(file, "Q_avg; %s;", element_id);
        
        for (int t = 0; t < T; t++) {  //En el resto poner los resultados de results_matrix
            fprintf(file, "%.6f", results_matrix[i+nDM+nWLM+nPM+nFM][t]);

            if (t < T) fprintf(file, ";"); // espacio entre columnas
        }
        fprintf(file, "\n"); // salto de línea al final de cada fila
    }


    for (int i = 0; i < nWLM; i++) {
        //En la primera columna poner que es el nivel final de un tanque (ej: Niv.fin(T1))
        EN_getnodeid(ph, wlm_positions[i]+nJ+nR, element_id);
        if (lang == 0) fprintf(file, "Niv.fin.; %s;", element_id);
        else fprintf(file, "Final lev.; %s;", element_id);
        
        for (int t = 0; t < T; t++) {  //En el resto poner los resultados de results_matrix
            fprintf(file, "%.6f", results_matrix[i+m][t]);

            if (t < T) fprintf(file, ";"); // espacio entre columnas
        }
        fprintf(file, "\n"); // salto de línea al final de cada fila
    }

    fclose(file);

    //printf("\033[0;32mArchivo %s guardado \033[0m \n", file_name);

}





/**
 * @brief Guarda las desv. típicas y CVs de los parámetros / variables ajustados
 * durante la calibracion
 * 
 * @note CALIBRACIÓN
 */
void save_posterior_sd_CV_calib_parameters(char* file_name, int T, int nJ_Dem, int nJ, int nR, 
    int nT, int nPI, int nPU, int nV, int mod_MP, int nMP, int nCP, int nPumpCurves, int nDemPat, 
    int* indMP, int* ind_J_Dem, int* pumpCurveIndices, double* posterior_sd, int** CM,
    char **demand_patterns_id, Calib_Variables Current_Vars, 
    Cluster_Pipes_Info *Clusters_Pipes_Info, EN_Project ph, int lang){

    FILE *file = fopen(file_name, "w");
    
    if (!file) {
        if (lang == 0) printf("\033[1;31mERROR al crear el archivo %s \033[0m \n", file_name);
        else printf("\033[1;31mERROR creating the file %s \033[0m \n", file_name);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char element_id[32];

    double fitted, sd, cv;
    int index;

    int m_pat = CM[0][0]; //Indica si se han modificado los patrones de demanda
    int m_bd = CM[0][1]; //Indica si se han modificado demandas base
    int m_pcX = CM[0][2]; //Indica si se han modificado las coord. X (Flujo) de las curvas de las bombas
    int m_pcY = CM[0][3]; //Indica si se han modificado las coord. Y (Head) de las curvas de las bombas
    int m_set = CM[0][4]; //Indica si se han modificado los settings de las valvulas
    int m_ir = CM[0][5]; //Indica si se han modificado el coef. de rugosidad inidividual de cada tubería.
    int m_cr_DW = CM[0][6]; //Indica si se han modificado coef. de rugosidad común (DW) para grupos de tuberías (luego se convierte a HW)
    int m_cr_HW = CM[0][7]; //Indica si se han modificado coef. de rugosidad común (HW) para grupos de tuberías (luego se extiende a todas las tuberías)
    int m_itl = CM[0][8]; //Indica si se han modificado los niveles iniciales de tanques (initial tank levels)
    int m_td = CM[0][9]; //Indica si se han modificado los diámetros
    
    //Primera línea (títulos)
    if (lang == 0) fprintf(file, "Variable; ; Valor estimado; Desv. típica; CV posterior \n");
    else fprintf(file, "Variable; ; Estimated value; Std. deviation; Posterior CV \n");


    if (m_pat){ //Patrones de demanda
        for (int t = 0; t < T; t++){
            for (int i = 0; i < nDemPat; i++){
                index = i+nDemPat*t;
                fitted = Current_Vars.demand_patterns[t][i];
                sd = posterior_sd[index];
                if (lang == 0) fprintf(file, "Patrón; %s_t%d; %.4f; %.4f; ", demand_patterns_id[i], t, fitted, sd);
                else fprintf(file, "Pattern; %s_t%d; %.4f; %.4f; ", demand_patterns_id[i], t, fitted, sd);
                if (fitted == 0){
                    fprintf(file, "- \n");
                } else {
                    fprintf(file, "%.4f \n", sd/fitted);
                }
            }
        }
        fprintf(file, "\n\n");
    } 


    if (m_bd){ //Demandas base
        for (int i = 0; i < nJ_Dem; i++){
            index = m_pat*nDemPat*T + i;
            fitted = Current_Vars.base_demands[i];
            sd = posterior_sd[index];
            EN_getnodeid(ph, ind_J_Dem[i], element_id);
            if (lang == 0) fprintf(file, "Dem. base; %s; %.4f; %.4f; ", element_id, fitted, sd);
            else fprintf(file, "Base dem.; %s; %.4f; %.4f; ", element_id, fitted, sd);
            if (fitted == 0){
                fprintf(file, "- \n");
            } else {
                fprintf(file, "%.4f \n", sd/fitted);
            }
        }
        fprintf(file, "\n\n");
    } 


    if (m_pcX){ //Coord. X de las curvas de las bombas
        for (int i = 0; i < nPumpCurves; i++){
            EN_getcurveid(ph, pumpCurveIndices[i], element_id);
            fprintf(file, "Coord. x0; %s; 0; -; - \n", element_id, fitted, sd);
            for (int j = 0; j < 2; j++){
                index = m_pat*nDemPat*T + m_bd*nJ_Dem + 2*i+j;
                fitted = Current_Vars.xPumpCurves[i][j+1];
                sd = posterior_sd[index];
                fprintf(file, "Coord. x%d; %s; %.4f; %.4f; ", j+1, element_id, fitted, sd);
                if (fitted == 0){
                    fprintf(file, "- \n");
                } else {
                    fprintf(file, "%.4f \n", sd/fitted);
                }
            }
        }
        fprintf(file, "\n\n");
    } 


    if (m_pcY){ //Coord. Y de las curvas de las bombas
        for (int i = 0; i < nPumpCurves; i++){
            EN_getcurveid(ph, pumpCurveIndices[i], element_id);
            for (int j = 0; j < 3; j++){
                index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + 3*i+j;
                fitted = Current_Vars.yPumpCurves[i][j];
                sd = posterior_sd[index];
                fprintf(file, "Coord. y%d; %s; %.4f; %.4f; ", j, element_id, fitted, sd);
                if (fitted == 0){
                    fprintf(file, "- \n");
                } else {
                    fprintf(file, "%.4f \n", sd/fitted);
                }
            }
        }
        fprintf(file, "\n\n");
    } 



    if (m_set){ //Setting de valvulas
        for (int i = 0; i < nV; i++){
            index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves + i;
            fitted = Current_Vars.valves_settings[i];
            sd = posterior_sd[index];
            EN_getlinkid(ph, nPI+nPU+i+1, element_id);
            fprintf(file, "Setting; %s; %.4f; %.4f; ", element_id, fitted, sd);
            if (fitted == 0){
                fprintf(file, "- \n");
            } else {
                fprintf(file, "%.4f \n", sd/fitted);
            }
        }
        fprintf(file, "\n\n");
    } 
    



    if (m_ir){ //Rugosidades individuales de las tuberías

        if (mod_MP == 0){  //Todas las tuberías
            for (int i = 0; i < nPI; i++){
                EN_getlinkid(ph, i+1, element_id);
                index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves + 
                    nV + i;
                fitted = Current_Vars.roughness_coefficients[i];
                sd = posterior_sd[index];
                if (lang == 0) fprintf(file, "Rug. H-W; %s; %.4f; %.4f; ", element_id, fitted, sd);
                else fprintf(file, "H-W rough.; %s; %.4f; %.4f; ", element_id, fitted, sd);
                if (fitted == 0){
                    fprintf(file, "- \n");
                } else {
                    fprintf(file, "%.4f \n", sd/fitted);
                }
            }
        } else { //Solo las "principales"
            for (int i = 0; i < nMP; i++){
                EN_getlinkid(ph, indMP[i], element_id);
                index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves +
                    nV + i;
                fitted = Current_Vars.roughness_coefficients[i];
                sd = posterior_sd[index];
                if (lang == 0) fprintf(file, "Rug. H-W; %s; %.4f; %.4f; ", element_id, fitted, sd);
                else fprintf(file, "H-W rough.; %s; %.4f; %.4f; ", element_id, fitted, sd);
                if (fitted == 0){
                    fprintf(file, "- \n");
                } else {
                    fprintf(file, "%.4f \n", sd/fitted);
                }
            }
        }

        fprintf(file, "\n\n");
    } 


    if (m_cr_DW){ //Coef. de rugosidad común DW para grupos
        for (int i = 0; i < nCP; i++){
            index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves +
                m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + i;
            fitted = Current_Vars.common_DW_coefs[i];
            sd = posterior_sd[index];
            if (lang == 0) fprintf(file, "Rug. D-W; %s; %.4f; %.4f; ", Clusters_Pipes_Info[i].cluster_id, fitted, sd);
            else fprintf(file, "D-W rough.; %s; %.4f; %.4f; ", Clusters_Pipes_Info[i].cluster_id, fitted, sd);
            if (fitted == 0){
                fprintf(file, "- \n");
            } else {
                fprintf(file, "%.4f \n", sd/fitted);
            }
        }
        fprintf(file, "\n\n");
    } 



    if (m_cr_HW){ //Coef. de rugosidad común HW para grupos
        for (int i = 0; i < nCP; i++){
            index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves +
                m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + m_cr_DW*nCP + i;
            fitted = Current_Vars.common_HW_coefs[i];
            sd = posterior_sd[index];
            if (lang == 0) fprintf(file, "Rug. H-W; %s; %.4f; %.4f; ", Clusters_Pipes_Info[i].cluster_id, fitted, sd);
            else fprintf(file, "H-W rough.; %s; %.4f; %.4f; ", Clusters_Pipes_Info[i].cluster_id, fitted, sd);
            if (fitted == 0){
                fprintf(file, "- \n");
            } else {
                fprintf(file, "%.4f \n", sd/fitted);
            }
        }
        fprintf(file, "\n\n");
    } 



    if (m_itl){ //Nivel inicial de los tanques
        for (int i = 0; i < nT; i++){
            index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves +
                m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + m_cr_DW*nCP + m_cr_HW*nCP + i;
            fitted = Current_Vars.init_tank_levels[i];
            sd = posterior_sd[index];
            EN_getnodeid(ph, nJ+nR+i+1, element_id);
            if (lang == 0) fprintf(file, "Nivel ini.; %s; %.4f; %.4f; ", element_id, fitted, sd);
            else fprintf(file, "Init. lev.; %s; %.4f; %.4f; ", element_id, fitted, sd);
            if (fitted == 0){
                fprintf(file, "- \n");
            } else {
                fprintf(file, "%.4f \n", sd/fitted);
            }
        }
        fprintf(file, "\n\n");
    } 




    if (m_td){ //Diámetro de los tanques
        for (int i = 0; i < nT; i++){
            index = m_pat*nDemPat*T + m_bd*nJ_Dem + m_pcX*2*nPumpCurves + m_pcY*3*nPumpCurves +
            m_set*nV + m_ir*((1-mod_MP)*nPI + mod_MP*nMP) + m_cr_DW*nCP + m_cr_HW*nCP + m_itl*nT + i;
            fitted = Current_Vars.tank_diameters[i];
            sd = posterior_sd[index];
            EN_getnodeid(ph, nJ+nR+i+1, element_id);
            fprintf(file, "Diam.; %s; %.4f; %.4f; ", element_id, fitted, sd);
            if (fitted == 0){
                fprintf(file, "- \n");
            } else {
                fprintf(file, "%.4f \n", sd/fitted);
            }
        }
        fprintf(file, "\n\n");
    } 



    fclose(file);

    //printf("\033[0;32mArchivo %s guardado \033[0m \n", file_name);

}






/**
 * @brief Guarda la información de la EE instantánea en un .csv
 * 
 * @note EE INSTANTÁNEA
 */
void save_snapshot_SE_results_to_csv(char* file_name, int add_emitter, int emitter_index, int nJ, 
    int nR, int nDM, int nWLM, int nPM, int nFM, int m, int* ind_J_Dem, int* wlm_positions, 
    int* pm_positions, int* fm_positions, double* z, double* estimated_values, 
    double* posterior_CV, double* normalized_residuals, double** CV_matrix, 
    Variables_Snapshot_SE Current_Vars, EN_Project ph, int lang){

    FILE *file = fopen(file_name, "w");
    
    if (!file) {
        if (lang == 0) printf("\033[1;31mERROR al crear el archivo %s \033[0m \n", file_name);
        else printf("\033[1;31mERROR creating the file %s \033[0m \n", file_name);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    char element_id[32];  

    if (lang == 0) fprintf(file, "Variable; Elemento; Valor medido; CV medida; Valor estimado; CV posterior; Res. normalizado \n");
    else fprintf(file, "Variable; Element; Measured value; CV measurement; Estimated value; Posterior CV; Normalized residual \n");

    for (int i = 0; i < nDM; i++) {
        EN_getnodeid(ph, ind_J_Dem[i], element_id);
        
        fprintf(file, "Dem; %s; %.6f; %.6f; %.6f; %.6f; %6f \n", element_id, z[i], CV_matrix[i][0],
            estimated_values[i], posterior_CV[i], normalized_residuals[i]);
    }


    for (int i = 0; i < nWLM; i++) {
        EN_getnodeid(ph, wlm_positions[i]+nJ+nR, element_id);
        
        if (lang == 0) {
            fprintf(file, "Nivel; %s; %.6f; %.6f; %.6f; %.6f; %6f \n", element_id, z[i+nDM],
            CV_matrix[i+nDM][0], estimated_values[i+nDM], 
            posterior_CV[i+nDM], normalized_residuals[i+nDM]);
        } else {
            fprintf(file, "Level; %s; %.6f; %.6f; %.6f; %.6f; %6f \n", element_id, z[i+nDM],
            CV_matrix[i+nDM][0], estimated_values[i+nDM], 
            posterior_CV[i+nDM], normalized_residuals[i+nDM]);
        }   
    }


    for (int i = 0; i < nPM; i++) {
        EN_getnodeid(ph, pm_positions[i], element_id);
        
        fprintf(file, "Pr; %s; %.6f; %.6f; %.6f; %.6f; %6f \n", element_id, z[i+nDM+nWLM],
            CV_matrix[i+nDM+nWLM][0], estimated_values[i+nDM+nWLM], 
            posterior_CV[i+nDM+nWLM], normalized_residuals[i+nDM+nWLM]);
    }


    for (int i = 0; i < nFM; i++) {
        EN_getlinkid(ph, fm_positions[i], element_id);
        
        fprintf(file, "Q; %s; %.6f; %.6f; %.6f; %.6f; %6f \n", element_id, z[i+nDM+nWLM+nPM],
            CV_matrix[i+nDM+nWLM+nPM][0], estimated_values[i+nDM+nWLM+nPM], 
            posterior_CV[i+nDM+nWLM+nPM], normalized_residuals[i+nDM+nWLM+nPM]);
    }

    if (add_emitter){
        char element_id[50];
        EN_getnodeid(ph, emitter_index, element_id);
        if (lang == 0) fprintf(file, "Coef.emisor; %s; ; ; %.4f \n", element_id, Current_Vars.emitter_coef);
        else fprintf(file, "Emitter coef.; %s; ; ; %.4f \n", element_id, Current_Vars.emitter_coef);
    }


    fclose(file);

    //printf("\033[0;32mArchivo %s guardado \033[0m \n", file_name);

}







/**
 * @brief Sirve para guardar las demandas, niveles, presiones y/o flujos
 * (instántaneos y/o promediados) generados sintéticamente en archivos .csv
 * compatibles con el formato de lectura
 */
void save_synthetic_measurements_to_csv(char* dm_File, char* wlm_File, char* pm_File, char* fm_File, 
    char* fm_avg_File, double cv_DM, double cv_WLM, double cv_PM, double cv_FM, double cv_FM_avg, 
    int nJ, int nR, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int m, int T, int* ind_J_Dem, 
    int* wlm_positions, int* pm_positions, int* fm_positions, int* fm_avg_positions, 
    double* measurements_vector, EN_Project ph){

    char element_id[32];  


    //DEMANDAS 
    FILE *file = fopen(dm_File, "w");
    if (!file) {
        printf("\033[1;31mERROR al crear %s \033[0m \n", dm_File);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nDM; i++) {
        EN_getnodeid(ph, ind_J_Dem[i], element_id);
        fprintf(file, "%s;", element_id); //Primera celda para el ID del elemento sobre el que está el sensor
        fprintf(file, "%.4f;", cv_DM); //Segunda celda para el CV de los demandmeters
        for (int t = 0; t < T; t++) {  //En el resto de celdas, poner los resultados de measurements_vector
            fprintf(file, "%.6f", measurements_vector[i+t*m]);
            if (t < T-1) fprintf(file, ";"); // espacio entre columnas
        }
        fprintf(file, "\n"); // salto de línea al final de cada fila
    }

    fclose(file);

    printf("\033[0;32mMedidas sinteticas de demanda guardadas como %s \033[0m \n", dm_File);  


    //NIVEL DE AGUA  
    file = fopen(wlm_File, "w");
    if (!file) {
        printf("\033[1;31mERROR al crear %s \033[0m \n", wlm_File);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nWLM; i++) {
        EN_getnodeid(ph, wlm_positions[i]+nJ+nR, element_id);
        fprintf(file, "%s;", element_id); //Primera celda para el ID del elemento sobre el que está el sensor
        fprintf(file, "%.4f;", cv_WLM); //Segunda celda para el CV de los waterlevelmeters
        for (int t = 0; t < T; t++) {  //En el resto de celdas, poner los resultados de measurements_vector
            fprintf(file, "%.6f", measurements_vector[i+nDM+t*m]);
            if (t < T-1) fprintf(file, ";"); // espacio entre columnas
        }
        fprintf(file, "\n"); // salto de línea al final de cada fila
    }

    fclose(file);

    printf("\033[0;32mMedidas sinteticas de nivel en tanques guardadas como %s \033[0m \n", wlm_File);


    //PRESIÓN
    if (nPM > 0){
        file = fopen(pm_File, "w");
        if (!file) {
            printf("\033[1;31mERROR al crear %s \033[0m \n", pm_File);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nPM; i++) {
            EN_getnodeid(ph, pm_positions[i], element_id);
            fprintf(file, "%s;", element_id); //Primera celda para el ID del elemento sobre el que está el sensor
            fprintf(file, "%.4f;", cv_PM); //Segunda celda para el CV de los pressuremeters
            for (int t = 0; t < T; t++) {  //En el resto de celdas, poner los resultados de measurements_vector
                fprintf(file, "%.6f", measurements_vector[i+nDM+nWLM+t*m]);
                if (t < T-1) fprintf(file, ";"); // espacio entre columnas
            }
            fprintf(file, "\n"); // salto de línea al final de cada fila
        }

        fclose(file);
    
        printf("\033[0;32mMedidas sinteticas de presion en junctions tanques guardadas como %s \033[0m \n", pm_File);
    }


    //FLUJO INSTANTÁNEO
    if (nFM > 0){
        file = fopen(fm_File, "w");
        if (!file) {
            printf("\033[1;31mERROR al crear %s \033[0m \n", fm_File);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nFM; i++) {
            EN_getlinkid(ph, fm_positions[i], element_id);
            fprintf(file, "%s;", element_id); //Primera celda para el ID del elemento sobre el que está el sensor
            fprintf(file, "%.4f;", cv_FM); //Segunda celda para el CV de los flowmeters
            for (int t = 0; t < T; t++) {  //En el resto de celdas, poner los resultados de measurements_vector
                fprintf(file, "%.6f", measurements_vector[i+nDM+nWLM+nPM+t*m]);
                if (t < T-1) fprintf(file, ";"); // espacio entre columnas
            }
            fprintf(file, "\n"); // salto de línea al final de cada fila
        }

        fclose(file);

        printf("\033[0;32mMedidas sinteticas de flujo instantaneo en tuberias guardadas como %s \033[0m \n", fm_File);
    }


    //FLUJO PROMEDIADO
    if (nFM_avg > 0){
        file = fopen(fm_avg_File, "w");
        if (!file) {
            printf("\033[1;31mERROR al crear %s \033[0m \n", fm_avg_File);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nFM_avg; i++) {
            EN_getlinkid(ph, fm_avg_positions[i], element_id);
            fprintf(file, "%s;", element_id); //Primera celda para el ID del elemento sobre el que está el sensor
            fprintf(file, "%.4f;", cv_FM_avg); //Segunda celda para el CV de los flowmeters (AVG)
            for (int t = 0; t < T; t++) {  //En el resto de celdas, poner los resultados de measurements_vector
                fprintf(file, "%.6f", measurements_vector[i+nDM+nWLM+nPM+nFM+t*m]);
                if (t < T-1) fprintf(file, ";"); // espacio entre columnas
            }
            fprintf(file, "\n"); // salto de línea al final de cada fila
        }

        fclose(file);

        printf("\033[0;32mMedidas sinteticas de flujo promediado en tuberias guardadas como %s \033[0m \n", fm_avg_File);
    }

}





// DEMÁS FUNCIONES VARIADAS 
// ------------------------


/**
 * @brief Función para ordenar los n primeros elementos de un array en orden ascendente
 * 
 * @note Auxiliar para 'count_demand_patterns_from_inp' (Calibración)
 */
void sort_first_n(int *arr, int n) {
    qsort(arr, n, sizeof(int), cmpfunc);
}



/**
 * @brief Función auxiliar para sort_first_n
 *
 * @note Auxiliar a su vez para 'count_demand_patterns_from_inp' (Calibración)
 */
int cmpfunc(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}




/**
 * @brief Guarda en formato .txt la cualquier matriz dispersa en formato cholmod_sparse
 * 
 * @note Se puede usar en CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA
 */
void save_dense_matrix_to_txt(cholmod_sparse *A, const char *file_name, cholmod_common *c) {

    // Convertir matriz dispersa a densa
    cholmod_dense *A_dense = cholmod_sparse_to_dense(A, c);
    if (!A_dense) {
        printf("\033[0;31mError: No se ha podido convertir a densa\033[0m \n");
        return;
    }

    // Abrir archivo para escritura
    FILE *file = fopen(file_name, "w");
    if (!file) {
        printf("\033[0;31mError: No se ha podido abrir el archivo\033[0m \n");
        cholmod_free_dense(&A_dense, c);
        return;
    }

    // Obtener puntero a los datos de la matriz densa
    double *Ax = (double *)A_dense->x;
    int rows = A->nrow;
    int cols = A->ncol;

    // Escribir la matriz en el archivo con la indexación correcta (columna mayor)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%.10f ", Ax[j * rows + i]);  // Acceso correcto en columna mayor
        }
        fprintf(file, "\n");  // Nueva línea al final de cada fila
    }

    // Cerrar archivo y liberar memoria
    fclose(file);
    cholmod_free_dense(&A_dense, c);
    printf("Matriz guardada en %s\n", file_name);
}



/**
 * @brief Comprueba si hay elementos NaN o infinitos en una matriz cholmod_sparse
 * 
 * @note Se usa como comprobación intermedia en algunos puntos concretos de los algoritmos
 */
void check_nan_inf_matrix(int* nan_count, int* inf_count, cholmod_sparse* matrix){
    double* matrix_x = (double*)matrix->x;  //puntero a los datos de la matriz

    *nan_count = 0;
    *inf_count = 0;

    for (size_t i = 0; i < matrix->nzmax; i++) {
        if (isnan(matrix_x[i])) {
            (*nan_count)++;
        } else if (isinf(matrix_x[i])) {
            (*inf_count)++;
        }
    }

}



/**
 * @brief Muestra la memoria usada hasta el momento
 * 
 * @note CALIBRACIÓN, EE MULTIPERIODO y EE INSTANTÁNEA (como información para el
 * usuario)
 */
void print_memory_usage(int lang) {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    if (lang == 0) printf("MEMORIA USADA HASTA AHORA: \n %lu KB\n", memInfo.WorkingSetSize / 1024);
    else printf("MEMORY USED SO FAR: \n %lu KB\n", memInfo.WorkingSetSize / 1024);
}




/**
 * @brief Separa una línea en fragmentos, delimitados por el delimitador que se
 * elija (por ejemplo, ";")
 * 
 * @note Se usa en la lectura de los archivos .csv de medidas / pseudomedidas
 */
char* strsep(char **stringp, const char *delim) {
    if (*stringp == NULL) {
        return NULL;
    }
    char *token = *stringp;
    *stringp = strpbrk(token, delim); // Busca el primer delimitador en token
    if (*stringp) {
        *(*stringp) = '\0'; // Reemplaza el delimitador con NULL para terminar el token
        (*stringp)++;       // Mueve el puntero al siguiente carácter después del delimitador
    }
    return token;
}



/**
 * @brief Elimina los espacios de delante y detrás de un caracter
 * 
 * @note Se usa en la lectura de los archivos .csv de medidas / pseudomedidas
 */
char* trim(char *str) {
    char *end;

    // Trim leading space
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r')
        str++;

    if (*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
        end--;

    *(end + 1) = '\0';

    return str;
}














/**
 * @brief Realización de distribución normal de media 0 y desv. típica sd.
 * Lo hace a través de dos realizaciones de una uniforme (0,1)
 * 
 * @note Se usa para añadir a los valores generados sintéticamente mediante simulación ruido 
 * acorde a la desv. típica de una medida (en base a su CV)
 */
double normal_sample(double sd) {
    double pi = 3.14159265358979323846;

    double u1 = rand()/(double)RAND_MAX;
    double u2 = rand()/(double)RAND_MAX;

    double z0 = sqrt(-2.0*log(u1)) * cos(2.0*pi* u2); // z0 ~ N(0,1)

    return z0 * sd; // Conversión a N(0,sd^2)
}







/**
 * @brief Calcula un coef. de rugosidad de Darcy-Weisbach para cada grupo de
 * tuberías en base al material y la edad, tras leer el .csv age_materials_pipes_File
 * 
 * @warning Revisar lectura y añadir expresiones para más materiales
 * 
 * @note CALIBRACIÓN
 */
// void classify_age_materials_pipes_info(int nCP, const char *age_materials_pipes_File, 
//     EN_Project ph, Cluster_Pipes_Info *Clusters_Pipes_Info) {

    
//     FILE *file = fopen(age_materials_pipes_File, "r");
//     if (!file) {
//         printf("\033[0;31mError abriendo archivo age_materials_pipes_File = %s \033[0m \n", age_materials_pipes_File);
//         system("pause");
//         exit(EXIT_FAILURE);
//     }

//     char line[256];

//     double common_DW_coef;

//     while (fgets(line, sizeof(line), file)) {
//         // Eliminar salto de línea
//         line[strcspn(line, "\n")] = 0;

//         // Manejar BOM si existe
//         if (strncmp(line, "\xEF\xBB\xBF", 3) == 0) {
//             memmove(line, line + 3, strlen(line) - 2);
//         }

//         // Ignorar líneas vacías
//         if (strlen(line) == 0) continue;

//         // Parsear la línea
//         char *cluster_id = strtok(line, ";");
//         char *material = strtok(NULL, ";");
//         double avg_age = atof(strtok(NULL, "\n"));

//         if (!cluster_id || !material || !avg_age) {
//             printf("Línea inválida en el archivo .csv: %s\n", line);
//             continue;
//         }


//         for (int i = 0; i < nCP; i++) {
//             if (strcmp(Clusters_Pipes_Info[i].cluster_id, cluster_id) == 0) {
//                 strcpy(Clusters_Pipes_Info[i].material, material); //Leemos el material (puede ser un material como tal o "None")

//                 Clusters_Pipes_Info[i].avg_age = avg_age;  

//                 if (strcmp(Clusters_Pipes_Info[i].material, "Cast_Iron_cement-mortar-lined_pipe") == 0){
//                     common_DW_coef = 0.5 + 0.01*Clusters_Pipes_Info[i].avg_age;
//                     Clusters_Pipes_Info[i].common_DW_coef = common_DW_coef;
//                 }  //ir rellenando más casos de materiales (preguntar a Fernando)
//                 else if (strcmp(Clusters_Pipes_Info[i].material, "Carbon_Steel_pipe") == 0){
//                     printf("\033[0;31mMATERIAL %s VALIDO PERO OPCION AUN NO IMPLEMENTADA\n\n\033[0m", material);
//                     system("pause");
//                     exit(EXIT_FAILURE);
//                 } else if (strcmp(Clusters_Pipes_Info[i].material, "Stainless_Steel_pipe") == 0){
//                     printf("\033[0;31mMATERIAL %s VALIDO PERO OPCION AUN NO IMPLEMENTADA\n\n\033[0m", material);
//                     system("pause");
//                     exit(EXIT_FAILURE);
//                 } else if (strcmp(Clusters_Pipes_Info[i].material, "Ductile_Iron_pipe") == 0){
//                     printf("\033[0;31mMATERIAL %s VALIDO PERO OPCION AUN NO IMPLEMENTADA\n\n\033[0m", material);
//                     system("pause");
//                     exit(EXIT_FAILURE);
//                 } else if (strcmp(Clusters_Pipes_Info[i].material, "Copper_Steel_pipe") == 0){
//                     printf("\033[0;31mMATERIAL %s VALIDO PERO OPCION AUN NO IMPLEMENTADA\n\n\033[0m", material);
//                     system("pause");
//                     exit(EXIT_FAILURE);
//                 } else if (strcmp(Clusters_Pipes_Info[i].material, "PVC_Steel_pipe") == 0){
//                     printf("\033[0;31mMATERIAL %s VALIDO PERO OPCION AUN NO IMPLEMENTADA\n\n\033[0m", material);
//                     system("pause");
//                     exit(EXIT_FAILURE);
//                 } else if (strcmp(Clusters_Pipes_Info[i].material, "CPVC_Steel_pipe") == 0){
//                     printf("\033[0;31mMATERIAL %s VALIDO PERO OPCION AUN NO IMPLEMENTADA\n\n\033[0m", material);
//                     system("pause");
//                     exit(EXIT_FAILURE);
//                 } else if (strcmp(Clusters_Pipes_Info[i].material, "HDPE_Steel_pipe") == 0){
//                     printf("\033[0;31mMATERIAL %s VALIDO PERO OPCION AUN NO IMPLEMENTADA\n\n\033[0m", material);
//                     system("pause");
//                     exit(EXIT_FAILURE);
//                 } 
//                 else {
//                     printf("\033[0;31mEl material %s no esta reconocido como opcion \033[0m \n", material);
//                     printf("\033[0;31mPARA Y REVISA o CAMBIA LA OPCION DE RUGOSIDAD  \033[0m \n");
//                     system("pause");
//                     exit(EXIT_FAILURE);
//                     // common_DW_coef = 0.5 + 0.01*Clusters_Pipes_Info[i].avg_age;
//                     // Clusters_Pipes_Info[i].common_DW_coef = common_DW_coef;
//                 }

//             }
        
//         }

//     }

//     fclose(file);
// }










/**
 * @brief Ordena enteros en orden ascendente
 * 
 * @note Útil para generación sintética de medidas
 */
int compare_ints(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}
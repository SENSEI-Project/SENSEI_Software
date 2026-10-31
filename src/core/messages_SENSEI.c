#include "messages_SENSEI.h"  // Incluir el archivo de cabecera 


// Reset
#define RESET       "\033[0m"

// === COLORES NORMALES (0;3x) ===
#define BLACK       "\033[0;30m"
#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define YELLOW      "\033[0;33m"
#define BLUE        "\033[0;34m"
#define MAGENTA     "\033[0;35m"
#define CYAN        "\033[0;36m"
#define WHITE       "\033[0;37m"

// === COLORES BRILLANTES (1;3x) ===
#define BLACK_B     "\033[1;30m"
#define RED_B       "\033[1;31m"
#define GREEN_B     "\033[1;32m"
#define YELLOW_B    "\033[1;33m"
#define BLUE_B      "\033[1;34m"
#define MAGENTA_B   "\033[1;35m"
#define CYAN_B      "\033[1;36m"
#define WHITE_B     "\033[1;37m"

// === COLORES SUBRAYADOS (4;3x) ===
#define BLACK_U     "\033[4;30m"
#define RED_U       "\033[4;31m"
#define GREEN_U     "\033[4;32m"
#define YELLOW_U    "\033[4;33m"
#define BLUE_U      "\033[4;34m"
#define MAGENTA_U   "\033[4;35m"
#define CYAN_U      "\033[4;36m"
#define WHITE_U     "\033[4;37m"


// FUNCIÓN PARA IMPRIMIR MENSAJES
// ------------------------------

const char *messages[MSG_COUNT][2] = {

    [MSG_WELCOME_CALIBRATION] = {
        BLUE_B "*********************************************\n"
               "******* BIENVENIDO AL SOFTWARE SENSEI *******\n"
               "**** Este es el archivo para CALIBRACIÓN ****\n"
               "*********************************************" RESET "\n\n",
        BLUE_B "******************************************\n"
               "******* WELCOME TO SENSEI SOFTWARE *******\n"
               "**** This is the file for CALIBRATION ****\n" 
               "******************************************" RESET "\n\n"
    },


    [MSG_WELCOME_SNAPSHOT_SE] = {
        BLUE_B "************************************************\n"
               "******** BIENVENIDO AL SOFTWARE SENSEI *********\n"
               "**** Este es el archivo para EE INSTANTÁNEA ****\n"
               "************************************************" RESET "\n\n",
        BLUE_B "******************************************\n"
               "******* WELCOME TO SENSEI SOFTWARE *******\n"
               "**** This is the file for SNAPSHOT SE ****\n" 
               "******************************************" RESET "\n\n"
    },


    [MSG_WELCOME_MULTIPERIOD_SE] = {
        BLUE_B "*************************************************\n"
               "******** BIENVENIDO AL SOFTWARE SENSEI **********\n"
               "**** Este es el archivo para EE MULTIPERIODO ****\n"
               "*************************************************" RESET "\n\n",
        BLUE_B "************************************************\n"
               "********** WELCOME TO SENSEI SOFTWARE **********\n"
               "***** This is the file for MULTIPERIOD SE ******\n" 
               "************************************************" RESET "\n\n"
    },


    [MSG_SWITCH_LANG] = {
        MAGENTA "You can switch to English by writing 'ENG' in the 'LANGUAGE' field of the file %s" RESET "\n\n",
        MAGENTA "Puedes cambiar a español escribiendo 'ESP' en el campo 'LANGUAGE' del archivo %s" RESET "\n\n"
    },

    [MSG_HEADER_PATH_FILES] = {
        WHITE_U "RUTA Y ARCHIVOS DECLARADOS:" RESET "\n",
        WHITE_U "PATH AND DECLARED FILES:" RESET "\n"
    },

    [MSG_PATH_LABEL] = {
        "RUTA (subcarpeta): ",
        "PATH (subfolder): "
    },

    [MSG_PATH_NULL] = {
        RED "NULL. Debes declarar una subcarpeta (PATH en %s) "
        "y todos los archivos .csv e .inp deben estar dentro de esa carpeta." RESET "\n",
        RED "NULL. You must declare a subfolder (PATH in %s) "
        "and all .csv and .inp files must be inside that folder." RESET "\n"
    },

    [MSG_PATH_OK] = {
        GREEN "%s" RESET "\n",
        GREEN "%s" RESET "\n"
    },

    [MSG_LABEL_INP_INFO] = {
        "Archivo inp de informacion: ",
        "Information .inp file: "
    },

    [MSG_LABEL_DM] = {
        "Archivo csv medidas de demanda en junctions: ",
        "CSV file with demand measurements at junctions: "
    },

    [MSG_LABEL_WLM] = {
        "Archivo csv medidas de nivel de agua en tanques: ",
        "CSV file with water level measurements in tanks: "
    },

    [MSG_LABEL_PM] = {
        "Archivo csv medidas de presion en junctions: ",
        "CSV file with pressure measurements at junctions: "
    },

    [MSG_LABEL_FM] = {
        "Archivo csv medidas de flujo instantaneo en links: ",
        "CSV file with instantaneous flow measurements in link: "
    },

    [MSG_LABEL_FMAVG] = {
        "Archivo csv medidas de flujo promediado en links: ",
        "CSV file with averaged flow measurements in links: "
    },

    [MSG_LABEL_CLUSTERS] = {
        "Archivo csv informacion de clusters de links: ",
        "CSV file with link cluster information: "
    },

    [MSG_LABEL_HW] = {
        "Archivo csv coef. de rugosidad comun (H-W) para clusters: ",
        "CSV file with common roughness coefficient (H-W) for clusters: "
    },

    [MSG_LABEL_DW] = {
        "Archivo csv coef. de rugosidad comun (D-W) para clusters: ",
        "CSV file with common roughness coefficient (D-W) for clusters: "
    },

    [MSG_LABEL_MAIN_PIPES] = {
        "Archivo csv informacion de tuberias principales: ",
        "CSV file with main pipes information: "
    },

    [MSG_LABEL_OPTIONS] = {
        "Archivo csv OPCIONES DE CALIBRACION: ",
        "CSV file with CALIBRATION OPTIONS: "
    },

    [MSG_FILE_NULL] = {
        YELLOW "NULL" RESET "\n",
        YELLOW "NULL" RESET "\n"
    },

    [MSG_FILE_OK] = {
        GREEN "%s" RESET "\n",
        GREEN "%s" RESET "\n"
    },

    // --- PARÁMETROS DEL ALGORITMO ---
    [MSG_HEADER_ALGO_PARAMS] = {
        WHITE_U "PARÁMETROS DEL ALGORITMO:" RESET "\n",
        WHITE_U "ALGORITHM PARAMETERS:" RESET "\n"
    },

    [MSG_LABEL_EPSILON] = {
        "Tasa de perturbación para el jacobiano (epsilon): " GREEN_B "%.4f" RESET "\n",
        "Jacobian perturbation rate (epsilon): "            GREEN_B "%.4f" RESET "\n"
    },

    [MSG_LABEL_LAMBDA_INIT_CALIB] = {
        "Valor inicial del parámetro de regularización (lambda): " GREEN_B "%.4f" RESET "\n",
        "Initial value of the regularization parameter (lambda): " GREEN_B "%.4f" RESET "\n"
    },

    [MSG_LABEL_MAX_LOOPS] = {
        "Número máximo de veces que se recorre el bucle (max_loops): " GREEN_B "%d" RESET "\n",
        "Maximum number of loop passes (max_loops): "                 GREEN_B "%d" RESET "\n"
    },

    [MSG_LABEL_MAX_IT_CALIB] = {
        "Número máximo de iteraciones por etapa (max_it): " GREEN_B "%d" RESET "\n",
        "Maximum iterations per stage (max_it): "           GREEN_B "%d" RESET "\n"
    },

    [MSG_LABEL_MAX_ATTEMPTS_CALIB] = {
        "Número máximo de intentos de regularización (max_attempts): " GREEN_B "%d" RESET "\n",
        "Maximum number of regularization attempts (max_attempts): "   GREEN_B "%d" RESET "\n"
    },

    [MSG_LABEL_TOL] = {
        "Tolerancia (tol): " GREEN_B "%e" RESET "\n\n",
        "Tolerance (tol): "  GREEN_B "%e" RESET "\n\n"
    },

    [MSG_LABEL_MAX_IT_MULTIPERIOD] = {
        "Número máximo de iteraciones por periodo (max_it): " GREEN_B "%d" RESET "\n",
        "Maximum number of iterations per period (max_it): "  GREEN_B "%d" RESET "\n"
    },

    [MSG_LABEL_LAMBDA_INIT_MULTIPERIOD] = {
        "Valor inicial del parámetro de regularización (lambda) en cada periodo: "
            GREEN_B "%.4f" RESET "\n",
        "Initial value of the regularization parameter (lambda) for each period: "
            GREEN_B "%.4f" RESET "\n"
    },


    [MSG_LABEL_MAX_ATTEMPTS_MULTIPERIOD] = {
        "Número máximo de intentos de regularización antes de pasar al siguiente periodo (max_attempts): "
            GREEN_B "%d" RESET "\n\n",
        "Maximum number of regularization attempts before moving to the next period (max_attempts): "
            GREEN_B "%d" RESET "\n\n"
    },


    [MSG_LABEL_MAX_IT_SIMPLE] = {
        "Número máximo de iteraciones (max_it): " GREEN_B "%d" RESET "\n",
        "Maximum number of iterations (max_it): "  GREEN_B "%d" RESET "\n"
    },



    // INTRO
    [MSG_CALIB_INTRO] = {
        GREEN_B "Vamos a hacer la calibración / EE en bucles de %d etapas." RESET,
        GREEN_B "We will perform calibration / EE in loops of %d stages." RESET
    },

    [MSG_CALIB_STAGE_INTRO] = {
        GREEN_B "En la %da etapa, retocamos:" RESET,
        GREEN_B "In stage %d, we adjust:" RESET
    },

    [MSG_CALIB_STAGE_ACTIONS] = {
        "En cada etapa, modificaremos las siguientes variables:\n",
        "In each stage, we will modify the following variables:\n"
    },


    // VARIABLES QUE SE RETOCAN
    [MSG_VAR_DEMAND_PATTERNS] = {
        "  -> Patrones de demanda \n",
        "  -> Demand patterns \n"
    },

    [MSG_VAR_BASE_DEMAND] = {
        "  -> Demandas base (cada nodo de demanda) \n",
        "  -> Base demands (each demand node) \n"
    },

    [MSG_VAR_PUMP_CURVE_X] = {
        "  -> Coord. X (Flujo) de las curvas de las bombas \n",
        "  -> Pump curve X-coordinate (Flow) \n"
    },

    [MSG_VAR_PUMP_CURVE_Y] = {
        "  -> Coord. Y (Head) de las curvas de las bombas \n",
        "  -> Pump curve Y-coordinate (Head) \n"
    },

    [MSG_VAR_VALVE_SETTINGS] = {
        "  -> Setting de válvulas \n",
        "  -> Valve settings \n"
    },

    [MSG_VAR_HW_MAIN] = {
        "  -> Coef. de rugosidad HW para cada tubería PRINCIPAL (dadas en .csv) \n",
        "  -> HW roughness coefficient for each MAIN pipe (from .csv) \n"
    },

    [MSG_VAR_HW_ALL] = {
        "  -> Coef. de rugosidad HW para cada tubería de la red \n",
        "  -> HW roughness coefficient for all pipes in the network \n"
    },

    [MSG_VAR_DW_GROUP] = {
        "  -> Coef. de rugosidad DW común para cada grupo de tuberías \n",
        "  -> Common DW roughness coefficient for each pipe group \n"
    },

    [MSG_VAR_HW_GROUP] = {
        "  -> Coef. de rugosidad HW común para cada grupo de tuberías \n",
        "  -> Common HW roughness coefficient for each pipe group \n"
    },

    [MSG_VAR_TANK_LEVEL] = {
        "  -> Nivel inicial de los tanques \n",
        "  -> Initial tank level \n"
    },

    [MSG_VAR_TANK_DIAM] = {
        "  -> Diámetro de los tanques \n",
        "  -> Tank diameter \n"
    },

    [MSG_CALIB_CONFIG_DONE] = {
        GREEN "CONFIGURACIÓN DE LA CALIBRACIÓN REALIZADA\n"
        "-----------------------------------------"
        RESET "\n",

        GREEN "CALIBRATION CONFIGURATION COMPLETED\n"
        "-----------------------------------------"
        RESET "\n"
    },


    [MSG_ERROR_INFO_FILE] = {
        RED "ERROR al abrir info_File = %s (%s)\n"
        "Revisa el nombre o el contenido del archivo\n\n"
        RESET,

        RED "ERROR opening info_File = %s (%s)\n"
        "Check the file name or its contents\n\n"
        RESET
    },  


    [MSG_CALIB_PERIODS] = {
        "SE REALIZARÁ UNA CALIBRACIÓN DE " GREEN_B "%d PERIODOS" RESET "\n",
        "A CALIBRATION OF " GREEN_B "%d PERIODS" RESET " WILL BE PERFORMED\n"
    },

    [MSG_CALIB_DURATION] = {
        "La simulación (calibración) dura " GREEN_B "%.2f h" RESET "\n",
        "The simulation (calibration) lasts " GREEN_B "%.2f h" RESET "\n"
    },

    [MSG_PERIOD_LENGTH] = {
        "Cada periodo dura " GREEN_B "%.2f minutos" RESET " (patternStep)\n",
        "Each period lasts " GREEN_B "%.2f minutes" RESET " (patternStep)\n"
    },

    [MSG_HYD_STEP] = {
        "Hydraulic step: " GREEN_B "%.2f minutos" RESET "\n\n",
        "Hydraulic step: " GREEN_B "%.2f minutes" RESET "\n\n"
    },


    [MSG_NET_HEADER] = {
        WHITE_U "NUMERO DE ELEMENTOS EN LA RED" RESET "\n",
        WHITE_U "NUMBER OF ELEMENTS IN THE NETWORK" RESET "\n"
    },

    [MSG_NET_NODES] = {
        GREEN_B "%d" RESET " NODOS, que corresponden a " 
        GREEN_B "%d" RESET " JUNCTIONS, "
        GREEN_B "%d" RESET " EMBALSES y "
        GREEN_B "%d" RESET " TANQUES\n",

        GREEN_B "%d" RESET " NODES, corresponding to "
        GREEN_B "%d" RESET " JUNCTIONS, "
        GREEN_B "%d" RESET " RESERVOIRS and "
        GREEN_B "%d" RESET " TANKS\n"
    },

    [MSG_NET_LINKS] = {
        GREEN_B "%d" RESET " LINKS, que corresponden a "
        GREEN_B "%d" RESET " TUBERIAS, "
        GREEN_B "%d" RESET " BOMBAS y "
        GREEN_B "%d" RESET " VALVULAS\n\n",

        GREEN_B "%d" RESET " LINKS, corresponding to "
        GREEN_B "%d" RESET " PIPES, "
        GREEN_B "%d" RESET " PUMPS and "
        GREEN_B "%d" RESET " VALVES\n\n"
    },


    [MSG_JUNCTIONS_SPLIT] = {
        "Los junctions se reparten en %d junctions de TRANSITO y %d de DEMANDA\n\n",
        "The junctions are divided into %d TRANSIT junctions and %d DEMAND junctions\n\n"
    },


    [MSG_DEMAND_PATTERNS_HEADER] = {
        "Hay %d patrones de demanda, con nombres: ",
        "There are %d demand patterns, with names: "
    },


    [MSG_NET_COUNT_DONE] = {
        GREEN "CONTEO DE LOS ELEMENTOS DE LA RED Y LECTURA INICIAL COMPLETADA\n"
        "--------------------------------------------------------------" RESET "\n",

        GREEN "NETWORK ELEMENT COUNT AND INITIAL READING COMPLETED\n"
        "---------------------------------------------------" RESET "\n"
    },

    
    [MSG_DEMAND_PATTERNS_READ] = {
        "Patrones de demanda leídos correctamente desde %s\n\n",
        "Demand patterns successfully read from %s\n\n"
    },


    [MSG_HW_MAIN_MODIFY] = {
        "Vamos a modificar el coef. de rugosidad de %d tuberías (las \"principales\")\n",
        "We will modify the roughness coefficient of %d pipes (the \"main\" ones)\n"
    },


    [MSG_HW_OTHER_FIXED] = {
        "Dejaremos el coef. de rugosidad del resto de tuberías fijo (con el valor de %s)\n\n",
        "The roughness coefficient of the remaining pipes will be kept fixed (using the value from %s)\n\n"
    },


    [MSG_HW_ALL_MODIFY] = {
        "Vamos a modificar el coef. de rugosidad de todas las tuberías (%d)\n",
        "We will modify the roughness coefficient of all pipes (%d)\n"
    },


    [MSG_HW_READ_OK] = {
        "Coeficientes de rugosidad leídos correctamente desde %s\n",
        "Roughness coefficients successfully read from %s\n"
    },


    [MSG_PIPE_GROUPS_READ] = {
        "Se han leído %d grupos de tuberías\n\n",
        "Read %d pipe groups\n\n"
    },


    [MSG_CLUSTER_ID] = {
        "Cluster de tuberías: %s | ",
        "Pipe cluster: %s | "
    },


    [MSG_CLUSTER_DW_COEF] = {
        "Coeficiente inicial de rugosidad de Darcy-Weisbach: %.3f\n",
        "Initial Darcy-Weisbach roughness coefficient: %.3f\n"
    },


    [MSG_COMMON_COEFS_OK] = {
        "Coeficientes de rugosidad comunes correctamente inicializados y coeficientes individuales calculados adecuadamente\n\n",
        "Common roughness coefficients correctly initialized and individual coefficients correctly computed\n\n"
    },


    [MSG_CLUSTER_HW_COEF] = {
        "Coeficiente inicial de rugosidad de Hazen-Williams: %.3f\n",
        "Initial Hazen-Williams roughness coefficient: %.3f\n"
    },


    [MSG_COMMON_COEFS_EXTENDED] = {
        "\nCoeficientes de rugosidad comunes correctamente inicializados y aplicados a las tuberías de cada grupo\n\n",
        "\nCommon roughness coefficients correctly initialized and propagated to the pipes of each group\n\n"
    },


    [MSG_BASE_DEMANDS_READ_OK] = {
        "Demandas base de los %d nodos de demanda leídas correctamente desde %s\n\n",
        "Base demands of the %d demand nodes successfully read from %s\n\n"
    },


    [MSG_PUMP_CURVES_READ_OK] = {
        "Curvas de las %d bombas leídas correctamente desde %s\n",
        "Reading of the %d pump curves successfully completed from %s\n"
    },


    [MSG_PUMP_CURVE_HEADER] = {
        "ID: %s | Puntos: ",
        "ID: %s | Points: "
    },


    [MSG_TANK_LEVELS_READ_OK] = {
        "Niveles iniciales, mínimos y máximos de los tanques leídos correctamente desde %s\n",
        "Initial, minimum, and maximum tank levels successfully read from %s\n"
    },


    [MSG_TANK_LEVEL_LINE] = {
        "%s) Nivel inicial: %.2f | Nivel mínimo: %.2f | Nivel máximo: %.2f\n",
        "%s) Initial level: %.2f | Minimum level: %.2f | Maximum level: %.2f\n"
    },


    [MSG_TANK_DIAMETERS_READ_OK] = {
        "Diámetros de los tanques leídos correctamente desde %s\n",
        "Tank diameters successfully read from %s\n"
    },


    [MSG_TANK_DIAM_LINE] = {
        "%s) Diámetro: %.2f\n",
        "%s) Diameter: %.2f\n"
    },


    [MSG_TOTAL_CONTROLS] = {
        "Número total de reglas de control: %d\n",
        "Total number of control rules: %d\n"
    },


    [MSG_VALVE_SETTINGS_HEADER] = {
        "Setting de las válvulas:\n",
        "Valve settings:\n"
    },


    [MSG_VALVE_SETTING_LINE] = {
        "%s) %.4f\n",
        "%s) %.4f\n"
    },


    [MSG_VALVE_SETTINGS_CONTROLS_COUNT] = {
        "Hay %d controles afectados por los settings de las válvulas\n\n",
        "There are %d controls affected by the valve settings\n\n"
    },


    [MSG_CALIB_OPTIONS_READ_OK] = {
        GREEN "LECTURA DE VARIABLES ASOCIADAS A OPCIONES DE CALIBRACIÓN REALIZADA\n"
        "------------------------------------------------------------------" RESET "\n\n",

        GREEN "READING OF VARIABLES ASSOCIATED WITH CALIBRATION OPTIONS COMPLETED\n"
        "------------------------------------------------------------------" RESET "\n\n"
    },

    [MSG_DM_ROWS] = {
        GREEN "Hay datos de demanda en %d junctions" RESET "\n",
        GREEN "There are demand data in %d junctions" RESET "\n"
    },

    [MSG_DM_MEASURE_COUNT] = {
        GREEN "(Se han recopilado %d medidas / pseudomedidas de demanda)" RESET "\n\n",
        GREEN "(A total of %d demand measurements / pseudo-measurements have been collected)" RESET "\n\n"
    },


    [MSG_WLM_ROWS_ID] = {
        GREEN "Hay datos de nivel de agua en %d embalses/tanques, que son: " RESET "\n",
        GREEN "There are water level data in %d reservoirs/tanks, which are: " RESET "\n"
    },

    [MSG_WLM_MEASURE_COUNT] = {
        GREEN "(Se han recopilado %d medidas de nivel)" RESET "\n\n",
        GREEN "(A total of %d water-level measurements have been collected)" RESET "\n\n"
    },


    [MSG_PM_ROWS_ID] = {
        GREEN "Hay datos de medidores de presión en %d nodos, que son: " RESET "\n",
        GREEN "There are pressure meter data at %d nodes, which are: " RESET "\n"
    },

    [MSG_PM_MEASURE_COUNT] = {
        GREEN "(Se han recopilado %d medidas de presión)" RESET "\n\n",
        GREEN "(A total of %d pressure measurements have been collected)" RESET "\n\n"
    },


    [MSG_FM_ROWS_ID] = {
        GREEN "Hay datos de medidores instantáneos de flujo en %d links, que son: " RESET "\n",
        GREEN "There are instantaneous flow meter data at %d links, which are: " RESET "\n"
    },

    [MSG_FM_MEASURE_COUNT] = {
        GREEN "(Se han recopilado %d medidas de flujo instantáneo)" RESET "\n\n",
        GREEN "(A total of %d instantaneous-flow measurements have been collected)" RESET "\n\n"
    },


    [MSG_FMAVG_ROWS_ID] = {
        GREEN "Hay datos de medidores de flujo promediado en %d links, que son: " RESET "\n",
        GREEN "There are averaged flow meter data at %d links, which are: " RESET "\n"
    },

    [MSG_FMAVG_MEASURE_COUNT] = {
        GREEN "(Se han recopilado %d medidas de flujo promediado)" RESET "\n\n",
        GREEN "(A total of %d averaged-flow measurements have been collected)" RESET "\n\n"
    },


    [MSG_MEASUREMENTS_READ_OK] = {
        GREEN "LECTURA y ALMACENAMIENTO de MEDIDAS / PSEUDOMEDIDAS desde archivos .csv COMPLETADO\n"
        "----------------------------------------------------------------------------------" RESET "\n\n",

        GREEN "READING and STORAGE of MEASUREMENTS / PSEUDO-MEASUREMENTS from .csv files COMPLETED\n"
        "----------------------------------------------------------------------------------" RESET "\n\n"
    },


    [MSG_MEASURES_PER_PERIOD] = {
        "En cada periodo contamos con m = %d medidas/pseudomedidas (algunas pueden ser nulas)\n\n",
        "In each period we have m = %d measurements/pseudo-measurements (some may be null)\n\n"
    },


    [MSG_MEASURES] = {
        "Contamos con m = %d medidas/pseudomedidas\n\n",
        "We have m = %d measurements/pseudo-measurements\n\n"
    },


    [MSG_GROUPING_CVS_DONE] = {
        GREEN "AGRUPACIÓN DE CVs, MEDIDAS y PESOS REALIZADA\n"
        "--------------------------------------------" RESET "\n\n",

        GREEN "GROUPING OF CVs, MEASUREMENTS and WEIGHTS COMPLETED\n"
        "--------------------------------------------" RESET "\n\n"
    },

    [MSG_SIM_START_INFO] = {
        "Simulamos con la información de arranque que tenemos sobre los parámetros y variables\n\n",
        "We simulate using the initial information we have about the parameters and variables\n\n"
    },


    [MSG_OBJFUNC_INITIAL_HEADER] = {
        "Valor inicial de la función objetivo:\n",
        "Initial value of the objective function:\n"
    },


    [MSG_LOOP_HEADER] = {
        MAGENTA "=================================\n"
        "=================================\n"
        "RECORREMOS EL BUCLE POR %dª VEZ\n"
        "=================================\n"
        "=================================" RESET "\n\n\n",

        MAGENTA "=================================\n"
        "=================================\n"
        "WE ARE RUNNING THROUGH THE LOOP FOR THE %dth TIME\n"
        "=================================\n"
        "=================================" RESET "\n\n\n"
    },


    [MSG_NO_IMPROVEMENT_STOP] = {
        YELLOW "Finalizamos el proceso porque no ha habido mejora en la última iteración "
        "(nos hemos estancado)" RESET "\n",

        YELLOW "We stop the process because there was no improvement in the last iteration "
        "(we have stagnated)" RESET "\n"
    },


    [MSG_CONV_RATE] = {
        "Tasa de cambio (o convergencia) entre el inicio del bucle anterior y este: %.6f\n\n",
        "Rate of change (or convergence) between the start of the previous loop and this one: %.6f\n\n"
    },


    [MSG_CONVERGENCE_REACHED] = {
        YELLOW_B 
        "NO HA HABIDO CAMBIO SUFICIENTE EN LA ÚLTIMA VEZ QUE HEMOS RECORRIDO EL BUCLE\n"
        "CONSIDERAMOS QUE EL ALGORITMO HA CONVERGIDO. DAMOS EL PROCESO POR FINALIZADO\n"
        "(Si lo deseas, puedes usar el nuevo archivo .inp generado como punto de partida y ajustar la configuración)\n"
        RESET "\n",

        YELLOW_B
        "THERE HAS NOT BEEN SUFFICIENT CHANGE IN THE LAST LOOP ITERATION\n"
        "WE CONSIDER THAT THE ALGORITHM HAS CONVERGED. THE PROCEDURE IS NOW FINISHED\n"
        "(If you wish, you may use the newly generated .inp file as a starting point and adjust the configuration)\n"
        RESET "\n"
    },



    [MSG_TIME_EVOLUTION_TANK_WLM] = {
        GREEN_B "EVOLUCIÓN TEMPORAL DEL NIVEL SIMULADO PARA LOS TANQUES CON SENSOR:\n" RESET,
        GREEN_B "TEMPORAL EVOLUTION OF SIMULATED WATER LEVEL FOR SENSOR-EQUIPPED TANKS:\n" RESET
    },

    [MSG_TIME_EVOLUTION_JUNCTION_PM] = {
        GREEN_B "EVOLUCIÓN TEMPORAL DE LA PRESIÓN SIMULADA PARA LOS JUNCTIONS CON SENSOR:\n" RESET,
        GREEN_B "TEMPORAL EVOLUTION OF SIMULATED PRESSURE FOR SENSOR-EQUIPPED JUNCTIONS:\n" RESET
    },

    [MSG_TIME_EVOLUTION_LINK_FM] = {
        GREEN_B "EVOLUCIÓN TEMPORAL DEL FLUJO (INSTANTÁNEO) SIMULADO PARA LOS ENLACES CON SENSOR:\n" RESET,
        GREEN_B "TEMPORAL EVOLUTION OF SIMULATED (INSTANTANEOUS) FLOW FOR SENSOR-EQUIPPED LINKS:\n" RESET
    },

    [MSG_TIME_EVOLUTION_LINK_FMAVG] = {
        GREEN_B "EVOLUCIÓN TEMPORAL DEL FLUJO (PROMEDIADO) SIMULADO PARA LOS ENLACES CON SENSOR:\n" RESET,
        GREEN_B "TEMPORAL EVOLUTION OF SIMULATED (AVERAGED) FLOW FOR SENSOR-EQUIPPED LINKS:\n" RESET
    },


    [MSG_STAGE_START] = {
        MAGENTA "=========================================\n"
        "=========================================\n"
        "INICIAMOS LA ETAPA %d DE LA CALIBRACIÓN\n"
        "=========================================\n"
        "=========================================" RESET "\n\n\n",

        MAGENTA "=========================================\n"
        "=========================================\n"
        "WE BEGIN STAGE %d OF THE CALIBRATION\n"
        "=========================================\n"
        "=========================================" RESET "\n\n\n"
    },


    [MSG_STAGE_OPTIMIZATION_HEADER] = {
        MAGENTA "En esta etapa optimizamos:" RESET "\n",
        MAGENTA "In this stage we optimize:" RESET "\n"
    },


    [MSG_ITERATION_HEADER] = {
        MAGENTA_B "===================\n"
        "ITERACIÓN %d\n"
        "===================" RESET "\n\n\n",

        MAGENTA_B "===================\n"
        "ITERATION %d\n"
        "===================" RESET "\n\n\n"
    },


    [MSG_SIM_DONE_CURRENT_VARS] = {
        "Simulación realizada con los valores actuales de las variables\n",
        "Simulation performed using the current values of the variables\n"
    },


    [MSG_NUM_UNKNOWNS] = {
        "Hay %d variables (columnas del jacobiano)\n\n",
        "There are %d variables (columns of the Jacobian)\n\n"
    },


    [MSG_REGULARIZATION_ATTEMPT] = {
        BLUE_B "Intento de mejora (regularizando): %d/%d" RESET "\n",
        BLUE_B "Improvement attempt (regularization): %d/%d" RESET "\n"
    },


    [MSG_DESCENT_DIRECTION_UPDATE] = {
        GREEN_U "DIRECCIÓN DE DESCENSO. Las variables provisionales pasan a ser las actuales" RESET "\n\n",
        GREEN_U "DESCENT DIRECTION. The provisional variables become the current variables" RESET "\n\n"
    },


    [MSG_CONV_RATE_SIMPLE] = {
        "Tasa de cambio (or convergencia): %.6f\n\n",
        "Rate of change (or convergence): %.6f\n\n"
    },



    [MSG_STAGE_MAX_IT_REACHED] = {
        YELLOW_U "Máximo número de iteraciones alcanzado. Pasamos a la siguiente etapa" RESET "\n\n\n",
        YELLOW_U "Maximum number of iterations reached. Proceeding to the next stage" RESET "\n\n\n"
    },
    
    [MSG_STAGE_CONVERGED] = {
        GREEN_U "Hemos convergido en esta etapa. Pasamos a la siguiente etapa" RESET "\n\n\n",
        GREEN_U "Convergence achieved in this stage. Proceeding to the next stage" RESET "\n\n\n"
    },

        
    [MSG_PERIOD_MAX_IT_REACHED] = {
        YELLOW_U "MAXIMO DE ITERACIONES ALCANZADO. Pasamos a la EE del siguiente periodo" RESET "\n\n",
        YELLOW_U "MAXIMUM NUMBER OF ITERATIONS REACHED. Moving to the SE of the next period" RESET "\n\n"
    },

    [MSG_PERIOD_CONVERGED] = {
        GREEN_U "EL ALGORITMO HA CONVERGIDO. Pasamos a la EE del siguiente periodo" RESET "\n\n",
        GREEN_U "THE ALGORITHM HAS CONVERGED. Moving to the SE of the next period" RESET "\n\n"
    },


    [MSG_MAX_IT_REACHED] = {
        YELLOW_U "MAXIMO DE ITERACIONES ALCANZADO" RESET "\n\n",
        YELLOW_U "MAXIMUM NUMBER OF ITERATIONS REACHED" RESET "\n\n"
    },

    [MSG_CONVERGED] = {
        GREEN_U "EL ALGORITMO HA CONVERGIDO" RESET "\n\n",
        GREEN_U "THE ALGORITHM HAS CONVERGED" RESET "\n\n"
    },


    [MSG_LAMBDA_REDUCE] = {
        "Reducimos lambda a la décima parte de su valor\n",
        "We reduce lambda to one tenth of its value\n"
    },

    [MSG_LAMBDA_INCREASE] = {
        "Aumentamos lambda (x10) y pasamos al siguiente intento:\n\n",
        "We increase lambda (x10) and move on to the next attempt:\n\n"
    },


    [MSG_NO_IMPROVEMENT] = {
        YELLOW_U "NO HA HABIDO MEJORA" RESET "\n",
        YELLOW_U "NO IMPROVEMENT" RESET "\n"
    },


    [MSG_NO_IMPROVEMENT_EXIT] = {
        YELLOW_U "NO HA HABIDO MEJORA. DAMOS POR FINALIZADO EL PROCESO" RESET "\n",
        YELLOW_U "NO IMPROVEMENT. PROCESS HAS FINISHED" RESET "\n"
    },


    [MSG_ATTEMPTS_EXHAUSTED_CALIB] = {
        YELLOW_U "Hemos agotado los intentos. PASAMOS A LA SIGUIENTE ETAPA" RESET "\n\n",
        YELLOW_U "All attempts have been exhausted. WE MOVE TO THE NEXT STAGE" RESET "\n\n"
    },

    [MSG_ATTEMPTS_EXHAUSTED_MULTIPERIOD] = {
        YELLOW_U "Hemos agotado los intentos. PASAMOS A LA EE DEL SIGUIENTE PERIODO" RESET "\n\n",
        YELLOW_U "All attempts have been exhausted. WE MOVE TO THE SE OF THE NEXT PERIOD" RESET "\n\n"
    },



    [MSG_SOLUTION_SAVED] = {
        GREEN_U "MODELO CALIBRADO GUARDADO COMO %s" RESET "\n\n\n",
        GREEN_U "CALIBRATED MODEL SAVED AS %s" RESET "\n\n\n"
    },


    [MSG_UNCERTAINTY_PROPAGATION_HEADER] = {
        "PROPAGACIÓN DE INCERTIDUMBRE\n\n\n",
        "UNCERTAINTY PROPAGATION\n\n\n"
    },


    [MSG_TOTAL_PARAMS_OPTIMIZED] = {
        "Durante el proceso de calibración, se ha optimizado un total de %d parámetros\n\n",
        "During the calibration process, a total of %d parameters have been optimized\n\n"
    },


    [MSG_UNCERTAINTY_PROPAGATION_DONE] = {
        GREEN_U "Propagación de incertidumbre completada exitosamente" RESET "\n\n",
        GREEN_U "Uncertainty propagation completed successfully" RESET "\n\n"
    },


    [MSG_UNCERTAINTY_FILE_SAVED] = {
        GREEN_U "Archivo con la información de la propagación de incertidumbre guardado en %s" RESET "\n",
        GREEN_U "File with uncertainty propagation information saved to %s" RESET "\n"
    },


    [MSG_PROJECT_DELETING] = {
        "Eliminando proyecto...\n",
        "Deleting project...\n"
    },

    [MSG_PROJECT_DELETED] = {
        "Proyecto eliminado correctamente.\n\n",
        "Project deleted successfully.\n\n"
    },


    [MSG_INTERMEDIATE_DELETING] = {
        "Eliminando archivos intermedios...\n\n",
        "Deleting intermediate files...\n\n"
    },

    [MSG_INTERMEDIATE_DELETED] = {
        "%s eliminado\n\n",
        "%s deleted\n\n"
    },

    [MSG_INTERMEDIATE_DELETE_FAILED] = {
        "No se ha podido eliminar %s\n\n",
        "Unable to delete %s\n\n"
    },


    [MSG_LEAK_EXTRA_INFO_HEADER] = {
        WHITE_U "INFORMACIÓN EXTRA RESPECTO A FUGAS:" RESET "\n",
        WHITE_U "ADDITIONAL INFORMATION REGARDING LEAKS:" RESET "\n"
    },


    [MSG_LEAK_NO_NODE_SPECIFIED] = {
        "No se ha especificado ningún nodo en el que activar el coeficiente emisor\n\n",
        "No node has been specified for activating the emitter coefficient\n\n"
    },


    [MSG_LEAK_EMITTER_ASSUMED] = {
        "Se va a considerar un coeficiente emisor de " GREEN_B "%.8f" RESET
        " en el nodo " GREEN_B "%s" RESET
        " a partir del comienzo del intervalo " GREEN_B "%d" RESET "\n\n",

        "An emitter coefficient of " GREEN_B "%.8f" RESET
        " will be applied to node " GREEN_B "%s" RESET
        " starting at the beginning of interval " GREEN_B "%d" RESET "\n\n"
    },


    [MSG_ERR_MISSING_DM_SENSORS] = {
        RED 
        "TIENES QUE INCLUIR UN ARCHIVO CON INFORMACIÓN DE SENSORES SOBRE LAS DEMANDAS EN CADA JUNCTION DE DEMANDA "
        "(EN SU DEFECTO, USA PSEUDOMEDIDAS).\n"
        "CIERRA Y ARRÉGLALO" 
        RESET "\n\n",

        RED
        "YOU MUST INCLUDE A SENSOR DATA FILE FOR THE DEMANDS AT EACH DEMAND JUNCTION "
        "(OTHERWISE, USE PSEUDO-MEASUREMENTS).\n"
        "CLOSE THE PROGRAM AND FIX IT"
        RESET "\n\n"
    },

    [MSG_ERR_MISSING_WLM_SENSORS] = {
        RED 
        "TIENES QUE INCLUIR UN ARCHIVO CON INFORMACIÓN DE SENSORES SOBRE LOS NIVELES EN CADA TANQUE "
        "(EN SU DEFECTO, USA PSEUDOMEDIDAS).\n"
        "CIERRA Y ARRÉGLALO"
        RESET "\n\n",

        RED
        "YOU MUST INCLUDE A SENSOR DATA FILE FOR THE WATER LEVELS IN EACH TANK "
        "(OTHERWISE, USE PSEUDO-MEASUREMENTS).\n"
        "CLOSE THE PROGRAM AND FIX IT"
        RESET "\n\n"
    },


    [MSG_ERR_EMITTER_ID] = {
        RED "ERROR con el ID del nodo en el que quieres colocar el emisor (%s): %s\n"
            "Revisa %s\n\n"
        RESET,

        RED "ERROR with the ID of the node where you want to place the emitter (%s): %s\n"
            "Check %s\n\n"
        RESET
    },


    [MSG_MULTIPERIOD_PERIODS] = {
        "SE REALIZARÁ UNA ESTIMACIÓN DE ESTADO DE " GREEN_B "%d PERIODOS" RESET "\n",
        "A STATE ESTIMATION OF " GREEN_B "%d PERIODS" RESET " WILL BE PERFORMED\n"
    },


    [MSG_CONTROL_TYPES_COUNT] = {
        "Hay %d reglas de control de tipo EN_LOWLEVEL o EN_HILEVEL y %d de tipo EN_TIMER o EN_TIMEOFDAY\n\n",
        "There are %d control rules of type EN_LOWLEVEL or EN_HILEVEL and %d of type EN_TIMER or EN_TIMEOFDAY\n\n"
    },


    [MSG_CTRL_PIPEPUMP_AFFECTED] = {
        "Hay %d tuberías y bombas afectadas por controles:\n",
        "There are %d pipes and pumps affected by controls:\n"
    },

    [MSG_CTRL_VALVES_AFFECTED] = {
        "Hay %d válvulas afectadas por controles:\n",
        "There are %d valves affected by controls:\n"
    },

    [MSG_INITIAL_STATUS_LABEL] = {
        "%s) Estado inicial: ",
        "%s) Initial status: "
    },


    [MSG_RESERVOIRS_WITH_PATTERN] = {
        "Hay %d embalses con patrón asociado\n",
        "There are %d reservoirs with an associated pattern\n"
    },

    [MSG_RESERVOIR_HEADS_LABEL] = {
        "Embalse %s | Heads: ",
        "Reservoir %s | Heads: "
    },


    [MSG_ERR_WLM_COUNT_MISMATCH] = {
        RED
        "ERROR: Se esperaban %d series de medidas/pseudomedidas de nivel en tanques, "
        "pero se han leído %d líneas de datos\n"
        "Revisa el contenido de wlm_File (%s)\n\n"
        RESET,

        RED
        "ERROR: %d series of tank level measurements/pseudo-measurements were expected, "
        "but %d data lines were read\n"
        "Check the contents of wlm_File (%s)\n\n"
        RESET
    },


    [MSG_ERR_DM_MEASURE_MISMATCH] = {
        RED
        "ERROR: Hay %d medidas/pseudomedidas pero debería haber T*nDM = %d*%d = %d\n"
        "Revisa el contenido del %s\n\n"
        RESET,

        RED
        "ERROR: There are %d measurements/pseudo-measurements but there should be T*nDM = %d*%d = %d\n"
        "Check the contents of %s\n\n"
        RESET
    },


    [MSG_WLM_MEASURE_SUMMARY] = {
        "Se han recogido %d medidas / pseudomedidas de nivel en tanques "
        "(%d para cada uno de los %d tanques)\n\n",

        "A total of %d tank level measurements / pseudo-measurements have been collected "
        "(%d for each of the %d tanks)\n\n"
    },

    [MSG_DM_MEASURE_SUMMARY] = {
        "Se han recogido %d medidas / pseudomedidas de demanda "
        "(%d para cada uno de los %d junctions de demanda)\n\n",

        "A total of %d demand measurements / pseudo-measurements have been collected "
        "(%d for each of the %d demand junctions)\n\n"
    },


    [MSG_SE_PRECONFIG_DONE] = {
        GREEN "CONFIGURACIONES PREVIAS PARA LA ESTIMACIÓN DE ESTADO REALIZADAS\n"
        "---------------------------------------------------------------" RESET "\n\n",

        GREEN "PRELIMINARY CONFIGURATION FOR STATE ESTIMATION COMPLETED\n"
        "--------------------------------------------------------" RESET "\n\n"
    },



    [MSG_PERIOD_BANNER] = {
        MAGENTA_B "==================\n"
                "==================\n"
                "PERIODO t = %d\n"
                "==================\n"
                "==================" RESET "\n\n",

        MAGENTA_B "==================\n"
                "==================\n"
                "PERIOD t = %d\n"
                "==================\n"
                "==================" RESET "\n\n"
    },


    [MSG_FILE_SAVED_ESTIMATED_VALUES] = {
        GREEN "Archivo de VALORES ESTIMADOS guardado como %s" RESET "\n",
        GREEN "File with ESTIMATED VALUES saved as %s" RESET "\n"
    },


    [MSG_FILE_SAVED_POSTERIOR_CV] = {
        GREEN "Archivo con el COEFICIENTE DE VARIACIÓN de las estimaciones guardado como %s" RESET "\n",
        GREEN "File with the COEFFICIENT OF VARIATION of the estimates saved as %s" RESET "\n"
    },


    [MSG_FILE_SAVED_NORMALIZED_RESIDUALS] = {
        GREEN "Archivo de RESIDUOS NORMALIZADOS guardado como %s" RESET "\n",
        GREEN "File with NORMALIZED RESIDUALS saved as %s" RESET "\n"
    },


    [MSG_SE_SNAP_NO_NODE_SPECIFIED] = {
        "No se ha especificado ningún nodo en el que optimizar el coeficiente emisor\n\n",
        "No node has been specified for optimizing the emitter coefficient\n\n"
    },


    [MSG_SE_SNAP_SEARCH_EMITTER] = {
        "Vamos a optimizar también un coeficiente emisor en el nodo " GREEN_B "%s" RESET "\n\n",
        "We will also optimize an emitter coefficient at node " GREEN_B "%s" RESET "\n\n"
    },



    [MSG_SE_ALGO_START] = {
        GREEN "INICIAMOS EL ALGORITMO DE ESTIMACIÓN DE ESTADO\n"
        "----------------------------------------------" RESET "\n\n",

        GREEN "WE START THE STATE ESTIMATION ALGORITHM\n"
        "---------------------------------------" RESET "\n\n"
    },



    [MSG_EMITTER_COEF_ADJUSTED] = {
        "COEFICIENTE EMISOR ajustado para %s: " GREEN_B "%.6f" RESET "\n",
        "Adjusted EMITTER COEFFICIENT for %s: " GREEN_B "%.6f" RESET "\n"
    },



    [MSG_EMITTER_LEAK_FLOW] = {
        "CAUDAL FUGADO por %s: " GREEN_B "%.6f" RESET "\n\n\n",
        "LEAK FLOW at %s: " GREEN_B "%.6f" RESET "\n\n\n"
    },


    [MSG_FILE_SAVED_SE_RESULTS] = {
        GREEN "Archivo de resultados de la estimación de estado guardado como %s" RESET "\n",
        GREEN "State Estimation results file saved as %s" RESET "\n"
    },



    [MSG_ERR_DM_AND_PATTERNS_CONFLICT] = {
        RED
        "NO puedes incluir a la vez archivos .csv de demanda en todos los nodos de demanda y de patrones\n\n"
        "De todas maneras, sólo consideraremos como filas de demanda los patrones\n"
        "Si quieres, puedes cerrar y cambiar de opción\n\n"
        RESET,

        RED
        "You CANNOT include both .csv demand files for all demand nodes AND pattern files at the same time\n\n"
        "In any case, only the patterns will be considered as demand rows\n"
        "If you wish, you may close the program and change the option\n\n"
        RESET
    },


    [MSG_ERR_MISSING_DM_FILE] = {
        RED
        "Debes incluir un archivo con información de demandas en los nodos de demanda\n"
        "CIERRA Y ARREGLALO!\n"
        "(Si no tienes información certera, crea un .csv con la información dada en el .inp semilla)\n\n"
        RESET,

        RED
        "You must include a file with demand information for the demand nodes\n"
        "CLOSE THE PROGRAM AND FIX IT!\n"
        "(If you do not have reliable information, create a .csv using the data from the seed .inp)\n\n"
        RESET
    },



    [MSG_ERR_GENERAL_FILE_OPEN] = {
        RED "Error al abrir el archivo %s (%s)\n" RESET,
        RED "Error opening file %s (%s)\n" RESET
    },


    [MSG_ERR_INCOMPATIBLE_ROUGHNESS_OPTIONS] = {
        RED "Has activado opciones de rugosidad INCOMPATIBLES\n"
            "REVISA Y VUELVE A EJECUTAR\n\n"
        RESET,

        RED "You have activated INCOMPATIBLE roughness options\n"
            "CHECK AND RUN AGAIN\n\n"
        RESET
    },


    [MSG_WARN_MAIN_PIPES_IGNORED_DW] = {
        RED_B
        "El archivo main_pipes_File será ignorado "
        "porque has activado la opción de optimizar un coef. de D-W para cada grupo\n\n"
        RESET,

        RED_B
        "The file main_pipes_File will be ignored "
        "because you have enabled the option to optimize a D-W coefficient for each group\n\n"
        RESET
    },


    [MSG_WARN_MAIN_PIPES_IGNORED_HW] = {
        RED_B
        "El archivo main_pipes_File será ignorado "
        "porque has activado la opción de optimizar un coef. de H-W para cada grupo\n\n"
        RESET,

        RED_B
        "The file main_pipes_File will be ignored "
        "because you have enabled the option to optimize an H-W coefficient for each group\n\n"
        RESET
    },




    [MSG_ERR_DW_GROUPS_MISSING_BOTH] = {
        RED
        "Has activado la opción de optimizar un coef. de D-W para cada grupo de tuberías "
        "PERO no has definido los archivos clusters_pipes_File ni common_DW_File\n"
        "REVISA Y VUELVE A EJECUTAR\n\n"
        RESET,

        RED
        "You enabled the option to optimize a D-W coefficient for each pipe group "
        "BUT you did not define clusters_pipes_File nor common_DW_File\n"
        "CHECK AND RUN AGAIN\n\n"
        RESET
    },



    [MSG_ERR_DW_GROUPS_MISSING_CLUSTERS_PIPES_FILE] = {
        RED
        "Has activado la opción de optimizar un coef. de D-W para cada grupo de tuberías "
        "PERO no has definido el archivo clusters_pipes_File\n"
        "REVISA Y VUELVE A EJECUTAR\n\n"
        RESET,

        RED
        "You enabled the option to optimize a D-W coefficient for each pipe group "
        "BUT you did not define the clusters_pipes_File\n"
        "CHECK AND RUN AGAIN\n\n"
        RESET
    },



    [MSG_ERR_DW_GROUPS_MISSING_DW_FILE] = {
        RED
        "Has activado la opción de optimizar un coef. de D-W para cada grupo de tuberías "
        "PERO no has definido el archivo common_DW_File\n"
        "REVISA Y VUELVE A EJECUTAR\n\n"
        RESET,

        RED
        "You enabled the option to optimize a D-W coefficient for each pipe group "
        "BUT you did not define the common_DW_File\n"
        "CHECK AND RUN AGAIN\n\n"
        RESET
    },




    [MSG_ERR_HW_GROUPS_MISSING_BOTH] = {
        RED
        "Has activado la opción de optimizar un coef. de H-W para cada grupo de tuberías "
        "PERO no has definido los archivos clusters_pipes_File ni common_HW_File\n"
        "REVISA Y VUELVE A EJECUTAR\n\n"
        RESET,

        RED
        "You enabled the option to optimize a H-W coefficient for each pipe group "
        "BUT you did not define clusters_pipes_File nor common_HW_File\n"
        "CHECK AND RUN AGAIN\n\n"
        RESET
    },



    [MSG_ERR_HW_GROUPS_MISSING_CLUSTERS_PIPES_FILE] = {
        RED
        "Has activado la opción de optimizar un coef. de H-W para cada grupo de tuberías "
        "PERO no has definido el archivo clusters_pipes_File\n"
        "REVISA Y VUELVE A EJECUTAR\n\n"
        RESET,

        RED
        "You enabled the option to optimize a H-W coefficient for each pipe group "
        "BUT you did not define the clusters_pipes_File\n"
        "CHECK AND RUN AGAIN\n\n"
        RESET
    },



    [MSG_ERR_HW_GROUPS_MISSING_HW_FILE] = {
        RED
        "Has activado la opción de optimizar un coef. de H-W para cada grupo de tuberías "
        "PERO no has definido el archivo common_HW_File\n"
        "REVISA Y VUELVE A EJECUTAR\n\n"
        RESET,

        RED
        "You enabled the option to optimize a H-W coefficient for each pipe group "
        "BUT you did not define the common_HW_File\n"
        "CHECK AND RUN AGAIN\n\n"
        RESET
    },



    [MSG_WARN_GENERAL_FILE_IGNORED] = {
        RED_B
        "El archivo %s será ignorado porque no has activado la opción asociada de calibración\n\n"
        RESET,

        RED_B
        "The file %s will be ignored because the associated calibration option has not been enabled\n\n"
        RESET
    },


    [MSG_ERR_PATTERN_TOO_SHORT] = {
        RED
        "El patrón %s tiene longitud %d (y hay %d periodos de tiempo)\n"
        "Alarga el patrón en info_File (puedes repetir el patrón que ya tienes hasta que alcance la longitud requerida)\n\n"
        RESET,

        RED
        "Pattern %s has length %d (and there are %d time periods)\n"
        "Extend the pattern in info_File (you can repeat the existing pattern until it reaches the required length)\n\n"
        RESET
    },



    [MSG_ERR_PIPE_NOT_FOUND] = {
        RED "Tubería '%s' no encontrada en info_File\n" RESET,
        RED "Pipe '%s' not found in info_File\n" RESET
    },


    [MSG_ERR_INVALID_CSV_LINE] = {
        "Línea inválida en archivo .csv %s: '%s'\n",
        "Invalid line in .csv file %s: '%s'\n"
    },


    [MSG_ERR_PUMP_CURVE_INCOMPLETE] = {
        RED
        "Hay alguna curva de bomba que no tiene 3 puntos\n"
        "CIERRA Y REVISA el .inp info_File\n\n"
        RESET,

        RED
        "There is at least one pump curve that does not have 3 points\n"
        "CLOSE AND CHECK the .inp info_File\n\n"
        RESET
    },


    [MSG_WARN_NO_WLM_FILE] = {
        YELLOW
        "No se ha encontrado ningún archivo wlm_File\n"
        "Se interpreta que no hay información de sensores para niveles de agua en tanques\n\n"
        RESET,

        YELLOW
        "No wlm_File has been found\n"
        "We assume that there is no sensor information for tank water levels\n\n"
        RESET
    },



    [MSG_WARN_NO_PM_FILE] = {
        YELLOW
        "No se ha encontrado ningún archivo pm_File\n"
        "Se interpreta que no hay información de sensores para presión en junctions\n\n"
        RESET,

        YELLOW
        "No pm_File has been found\n"
        "We assume that there is no sensor information for pressure at junctions\n\n"
        RESET
    },



    [MSG_WARN_NO_FM_FILE] = {
        YELLOW
        "No se ha encontrado ningún archivo fm_File\n"
        "Se interpreta que no hay información de sensores para flujo instantáneo en links\n\n"
        RESET,

        YELLOW
        "No fm_File has been found\n"
        "We assume that there is no sensor information for instantaneous flow at links\n\n"
        RESET
    },


    [MSG_WARN_NO_FMAVG_FILE] = {
        YELLOW
        "No se ha encontrado ningún archivo fm_avg_File\n"
        "Se interpreta que no hay información de sensores para flujo promediado en links\n\n"
        RESET,

        YELLOW
        "No fm_avg_File has been found\n"
        "We assume that there is no sensor information for averaged flow at links\n\n"
        RESET
    },


    [MSG_ERR_NODE_NOT_FOUND] = {
        RED
        "ERROR: Nodo '%s' no encontrado en info_File\n"
        "Revisa el contenido de %s (%s)\n\n"
        RESET,

        RED
        "ERROR: Node '%s' not found in info_File\n"
        "Check the contents of %s (%s)\n\n"
        RESET
    },


    [MSG_ERR_LINK_NOT_FOUND] = {
        RED
        "ERROR: Link '%s' no encontrado en info_File\n"
        "Revisa el contenido de %s (%s)\n\n"
        RESET,

        RED
        "ERROR: Link '%s' not found in info_File\n"
        "Check the contents of %s (%s)\n\n"
        RESET
    },



    [MSG_ERR_NODE_NOT_DEMAND_JUNCTION] = {
        RED
        "ERROR: Nodo '%s' no es un junction de demanda\n"
        "Revisa el contenido del dm_File (%s)\n\n"
        RESET,

        RED
        "ERROR: Node '%s' is not a demand junction\n"
        "Check the contents of dm_File (%s)\n\n"
        RESET
    },


    [MSG_ERR_NODE_NOT_JUNCTION] = {
        RED
        "ERROR: Nodo '%s' no es un junction\n"
        "Revisa el contenido del pm_File (%s)\n\n"
        RESET,

        RED
        "ERROR: Node '%s' is not a junction\n"
        "Check the contents of pm_File (%s)\n\n"
        RESET
    },



    [MSG_ERR_DM_COUNT_MISMATCH] = {
        RED
        "ERROR: Se esperaban %d series de medidas/pseudomedidas de demanda, "
        "pero se han leído %d líneas de datos\n"
        "Revisa el contenido de %s\n\n"
        RESET,

        RED
        "ERROR: %d series of demand measurements/pseudo-measurements were expected, "
        "but %d data lines were read\n"
        "Check the contents of %s\n\n"
        RESET
    },



    [MSG_ERR_EN_RUNH] = {
        RED "Error en EN_runH (tiempo %ld): %s\n" RESET,
        RED "Error in EN_runH (time %ld): %s\n" RESET
    },


    [MSG_ERR_EN_NEXTH] = {
        RED "Error en EN_nextH (tiempo %ld): %s\n" RESET,
        RED "Error in EN_nextH (time %ld): %s\n" RESET
    },



    [MSG_ERR_MATRIX_U_NAN_INF] = {
        RED
        "En la matriz U hay %d NaN y %d +-inf\n"
        "NO PODEMOS SEGUIR SI HAY NaN O INFINITOS EN LA MATRIZ U\n"
        "PARA Y REVISA\n\n"
        RESET,

        RED
        "Matrix U contains %d NaN values and %d +-inf values\n"
        "WE CANNOT CONTINUE IF MATRIX U CONTAINS NaN OR INFINITE VALUES\n"
        "STOP AND CHECK\n\n"
        RESET
    },



    [MSG_WARN_U_NAN_INF] = {
        RED
        "La matriz U contiene Inf o NaN. Es posible obtener valores extraños.\n"
        RESET,

        RED
        "The matrix U contains Inf or NaN values. This may lead to invalid results.\n"
        RESET
    },



    [MSG_INFO_LANGUAGE_SELECTED] = {
        GREEN "LA INFORMACIÓN SE MOSTRARÁ EN ESPAÑOL\n\n" RESET,
        GREEN "THE INFORMATION WILL BE DISPLAYED IN ENGLISH\n\n" RESET
    },







};


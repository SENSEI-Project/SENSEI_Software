#include "functions_SENSEI.h"  
#include "messages_SENSEI.h"

// CALIBRACIÓN

int main() {

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // salida en UTF-8
        SetConsoleCP(CP_UTF8);       // entrada en UTF-8
        system("chcp 65001 > nul");  // asegura UTF-8 en consola
    #endif

    /*******************************************************************
    ********************************************************************
    * CONFIGURACIÓN de la CALIBRACIÓN según "config_calibration.txt"
    ********************************************************************
    ********************************************************************/

    const char* config_File = "config_calibration.txt";
   
    //Hiperparámetros del algoritmo
    double epsilon, tol, lambda_init; 
    int max_it, max_attempts, max_loops, lang; 

    char* base_Path = NULL; //Subcarpeta
    char* info_File = NULL; //Modelo hidráulico (.inp)
    //Archivos (.csv) de medidas / pseudomedidas
    char* dm_File = NULL, *patterns_File = NULL, *wlm_File = NULL, *pm_File = NULL, *fm_File = NULL, *fm_avg_File = NULL;

    //Archivos (.csv) para opciones extra de calibración
    char* clusters_pipes_File = NULL, *common_HW_File = NULL, *common_DW_File = NULL, *main_pipes_File = NULL;

    //Archivos (.csv) para establecer las opciones de calibración y formar
    //grupos de variables (configurar un descenso por coordenadas)
    char* options_File = NULL;

    //Lectura de "config_calibration.txt" e inicialización de variables
    read_config_file_calibration(config_File, &base_Path, &info_File, &dm_File, 
        &patterns_File, &wlm_File, &pm_File, &fm_File, &fm_avg_File, &clusters_pipes_File, 
        &common_HW_File, &common_DW_File, &main_pipes_File, &options_File, &epsilon, &lambda_init, 
        &max_loops, &max_it, &max_attempts, &tol, &lang);

    printf("%s", messages[MSG_WELCOME_CALIBRATION][lang]);  // Mensaje de bienvenida a la terminal
    
    // IDIOMA
    printf("%s", messages[MSG_INFO_LANGUAGE_SELECTED][lang]);
    printf(messages[MSG_SWITCH_LANG][lang], config_File);

    SENSEI_PAUSE();
    

    //RUTA Y ARCHIVOS DECLARADOS 
    // Encabezado
    printf("%s", messages[MSG_HEADER_PATH_FILES][lang]);

    // --- PATH base ---
    printf("%s", messages[MSG_PATH_LABEL][lang]);

    if (base_Path == NULL) {
        printf(messages[MSG_PATH_NULL][lang], config_File);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    } else {
        printf(messages[MSG_PATH_OK][lang], base_Path);
    }

    // ===== ARCHIVOS =====

    // INP info
    printf("%s", messages[MSG_LABEL_INP_INFO][lang]);
    if (info_File == NULL)
        printf("%s", messages[MSG_FILE_NULL][lang]);
    else
        printf(messages[MSG_FILE_OK][lang], info_File);

    // DM file
    printf("%s", messages[MSG_LABEL_DM][lang]);
    if (dm_File == NULL)
        printf("%s", messages[MSG_FILE_NULL][lang]);
    else
        printf(messages[MSG_FILE_OK][lang], dm_File);

    // WLM file
    printf("%s", messages[MSG_LABEL_WLM][lang]);
    if (wlm_File == NULL)
        printf("%s", messages[MSG_FILE_NULL][lang]);
    else
        printf(messages[MSG_FILE_OK][lang], wlm_File);

    // PM file
    printf("%s", messages[MSG_LABEL_PM][lang]);
    if (pm_File == NULL)
        printf("%s", messages[MSG_FILE_NULL][lang]);
    else
        printf(messages[MSG_FILE_OK][lang], pm_File);

    // FM file (flow instant)
    printf("%s", messages[MSG_LABEL_FM][lang]);
    if (fm_File == NULL)
        printf("%s", messages[MSG_FILE_NULL][lang]);
    else
        printf(messages[MSG_FILE_OK][lang], fm_File);

    // FM AVG file
    printf("%s", messages[MSG_LABEL_FMAVG][lang]);
    if (fm_avg_File == NULL)
        printf("%s", messages[MSG_FILE_NULL][lang]);
    else
        printf(messages[MSG_FILE_OK][lang], fm_avg_File);

    // Clusters
    printf("%s", messages[MSG_LABEL_CLUSTERS][lang]);
    if (clusters_pipes_File == NULL)
        printf("%s", messages[MSG_FILE_NULL][lang]);
    else
        printf(messages[MSG_FILE_OK][lang], clusters_pipes_File);

    // HW roughness
    printf("%s", messages[MSG_LABEL_HW][lang]);
    if (common_HW_File == NULL)
        printf("%s", messages[MSG_FILE_NULL][lang]);
    else
        printf(messages[MSG_FILE_OK][lang], common_HW_File);

    // DW roughness
    printf("%s", messages[MSG_LABEL_DW][lang]);
    if (common_DW_File == NULL)
        printf("%s", messages[MSG_FILE_NULL][lang]);
    else
        printf(messages[MSG_FILE_OK][lang], common_DW_File);

    // Main pipes
    printf("%s", messages[MSG_LABEL_MAIN_PIPES][lang]);
    if (main_pipes_File == NULL)
        printf("%s", messages[MSG_FILE_NULL][lang]);
    else
        printf(messages[MSG_FILE_OK][lang], main_pipes_File);

    // Options file
    printf("%s", messages[MSG_LABEL_OPTIONS][lang]);
    if (options_File == NULL)
        printf("%s", messages[MSG_FILE_NULL][lang]);
    else
        printf(messages[MSG_FILE_OK][lang], options_File);


    // PARÁMETROS DEL ALGORITMO
    printf("\n%s", messages[MSG_HEADER_ALGO_PARAMS][lang]);

    printf(messages[MSG_LABEL_EPSILON][lang], epsilon);
    printf(messages[MSG_LABEL_LAMBDA_INIT_CALIB][lang], lambda_init);
    printf(messages[MSG_LABEL_MAX_LOOPS][lang], max_loops);
    printf(messages[MSG_LABEL_MAX_IT_CALIB][lang], max_it);
    printf(messages[MSG_LABEL_MAX_ATTEMPTS_CALIB][lang], max_attempts);
    printf(messages[MSG_LABEL_TOL][lang], tol);


    int scale = 1; //1 para regularizar J^t*W*J con lambda*D^t*D; 0 para hacerlo con lambda*I (DEJAR 1)

    //ESTRUCTURAS NECESARIAS
    Options Opt;  //Estructura para activar las variables que deseamos calibrar
    Calib_Variables Current_Vars; //Estructura para guardar el valor de las vairables en cada iteración
    Calib_Variables Prov_Vars; //Estructura para guardar el valor provisional de las variables (tras resolver la ec. de Lev-Marq.)
    Cluster_Pipes_Info Clusters_Pipes_Info[50];  //Estrucutura para clasificar las tuberías por grupos
    Setting_Control_Info Setting_Controls_Info[40]; //Estructura para clasificar la información de los controles que impongan un setting a válvulas

    //Indica si para las demandas trabajamos con demanda por nodo o con factor
    //multiplicador por patron 
    int all_demands_nodes_rows = set_all_demands_nodes_rows(dm_File, patterns_File, lang); 

    char* rpt_File = "report.rpt";  //nombre del report (necesario para EN_open(ph,...))
    char* binary_File = "salida.bin"; //nombre de la salida binaria (necesario para EN_open(ph,...))

    int** CM = NULL; //Matriz binaria con la información de qué variables optimizamos en cada etapa de la calibración
    
    int num_stages = read_options_variables(&Opt, options_File, &CM, lang); //número de etapas del proceso de calibración
    //(Número de grupos de variables definidos)

    int mod_MP = check_roughness_options_set_mod_MP(Opt, clusters_pipes_File, 
        common_DW_File, common_HW_File, main_pipes_File, lang); 
    //Indica si se optimiza el coef. de rugosidad solamente de las tuberías principales o de todas
    //La función sirve también para revisar si la configuración de archivos y opciones es compatible

    // INTRO
    printf(messages[MSG_CALIB_INTRO][lang], num_stages);
    printf("\n%s\n", messages[MSG_CALIB_STAGE_ACTIONS][lang]);   // ya definido antes

    // LOOP DE ETAPAS
    for (int i = 0; i < num_stages; i++) {

        // introducción colorida de la etapa
        printf(messages[MSG_CALIB_STAGE_INTRO][lang], i + 1);
        printf("\n");

        // variables activas en esta etapa
        if (CM[i][0] == 1) printf("%s", messages[MSG_VAR_DEMAND_PATTERNS][lang]);
        if (CM[i][1] == 1) printf("%s", messages[MSG_VAR_BASE_DEMAND][lang]);
        if (CM[i][2] == 1) printf("%s", messages[MSG_VAR_PUMP_CURVE_X][lang]);
        if (CM[i][3] == 1) printf("%s", messages[MSG_VAR_PUMP_CURVE_Y][lang]);
        if (CM[i][4] == 1) printf("%s", messages[MSG_VAR_VALVE_SETTINGS][lang]);

        if (CM[i][5] == 1) {
            if (mod_MP == 1)
                printf("%s", messages[MSG_VAR_HW_MAIN][lang]);
            else
                printf("%s", messages[MSG_VAR_HW_ALL][lang]);
        }

        if (CM[i][6] == 1) printf("%s", messages[MSG_VAR_DW_GROUP][lang]);
        if (CM[i][7] == 1) printf("%s", messages[MSG_VAR_HW_GROUP][lang]);
        if (CM[i][8] == 1) printf("%s", messages[MSG_VAR_TANK_LEVEL][lang]);
        if (CM[i][9] == 1) printf("%s", messages[MSG_VAR_TANK_DIAM][lang]);

        printf("\n");
    }


    printf("%s", messages[MSG_CALIB_CONFIG_DONE][lang]);

    SENSEI_PAUSE();

    
    char element_id[60];  //Variable para acceder a los ID de los elementos de la red
    int element_index;  //Variable para acceder al índice de los elementos de la red 

    EN_Project ph;  // Declarar un manejador de proyecto
    EN_createproject(&ph);  // Crear un nuevo proyecto

    int error = EN_open(ph, info_File, rpt_File, binary_File);
    if (error){
        char msg[256];
        EN_geterror(error, msg, 255);
        printf(messages[MSG_ERROR_INFO_FILE][lang], info_File, msg);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

   
    long duration, patternStep, hydStep;
    EN_gettimeparam(ph, EN_DURATION, &duration);    //Duración de la simulación
    EN_gettimeparam(ph, EN_PATTERNSTEP, &patternStep); //Frecuencia de los datos 
    EN_gettimeparam(ph, EN_HYDSTEP, &hydStep); //Paso hidráulico en la simulación

    int T = duration / patternStep;  //Número de periodos de calibración

    printf(messages[MSG_CALIB_PERIODS][lang], T);
    printf(messages[MSG_CALIB_DURATION][lang], duration / 3600.0);
    printf(messages[MSG_PERIOD_LENGTH][lang], patternStep / 60.0);
    printf(messages[MSG_HYD_STEP][lang], hydStep / 60.0);


    SENSEI_PAUSE();

    //Conteo de nodos y enlaces de la red y desglose por tipos
    int nN, nL, nJ, nR, nT, nPI, nPU, nV;
    count_elements_in_network(&nN, &nL, &nJ, &nR, &nT, &nPI, &nPU, &nV, ph);

    printf("%s", messages[MSG_NET_HEADER][lang]);
    printf(messages[MSG_NET_NODES][lang], nN, nJ, nR, nT);
    printf(messages[MSG_NET_LINKS][lang], nL, nPI, nPU, nV);

    int nJ_Trans; //Número de jucnctions de tránsito (demanda base nula)
    int nJ_Dem; //Número de junctions de demanda (que no son de tránsito)
    int *ind_J_Trans = NULL, *ind_J_Dem = NULL;

    //Clasifica los junctions según tránsito o demanda y almacena sus índices
    classify_transit_demand_junctions(nJ, &nJ_Trans, &nJ_Dem, &ind_J_Trans, &ind_J_Dem, ph);
    printf(messages[MSG_JUNCTIONS_SPLIT][lang], nJ_Trans, nJ_Dem);


    int* unique_demand_patterns = NULL;
    char** demand_patterns_id = NULL;
    int nDemPat = count_demand_patterns_from_inp(nJ, &unique_demand_patterns, &demand_patterns_id, ph); //Número de patrones de demanda
    printf(messages[MSG_DEMAND_PATTERNS_HEADER][lang], nDemPat);
    for (int i = 0; i < nDemPat; i++){
        printf("%s  ", demand_patterns_id[i]);
    }
    printf("\n\n");


    printf("%s", messages[MSG_NET_COUNT_DONE][lang]);

    SENSEI_PAUSE();

    /*************************************************************************
    *************************************************************************
    * LECTURA de valores semilla (de arranque) de variables para CALIBRACIÓN
    *************************************************************************
    *************************************************************************/


    //LECTURA DE LOS PATRONES DE DEMANDA DESDE infoFile
    if (Opt.mod_patterns == 1){
        
        Current_Vars.demand_patterns = get_demand_patterns_from_inp(nDemPat, T, demand_patterns_id, unique_demand_patterns, ph, lang);

        printf(messages[MSG_DEMAND_PATTERNS_READ][lang], info_File);
        // printf("En los primeros 5 instantes, los valores de los %d patrones de demanda son: \n", nDemPat);
        // for (int i = 0; i < nDemPat; i++){
        //     for (int t = 0; t < 5; t++){
        //         printf("%.4f  ", Current_Vars.demand_patterns[t*nDemPat+i]);
        //     }
        //     printf("\n");
        // }
        // printf("\n\n");

        SENSEI_PAUSE();
    }

    free(unique_demand_patterns);


    int nCP;  //Número de clusters de tuberias 
    int nMP;  //Número de tuberías principales
    int* indMP = NULL; //Si mod_MP = 1; almacenamos aquí los índices de las tuberías prinicpales
  
    //LECTURA DEL COEFICIENTE DE RUGOSIDAD DE LAS TUBERÍAS DESDE infoFile
    if (Opt.mod_individual_roughness == 1){

        //Caso de que solamente se optimicen las tuberías "principales"
        if (mod_MP == 1){
            //Lectura de tuberías "principales" desde .csv
            nMP = store_main_pipes(nL, &indMP, main_pipes_File, ph, lang);

            printf(messages[MSG_HW_MAIN_MODIFY][lang], nMP);
            // for (int i = 0; i < nMP; i++){
            //     printf("%d  ", indMP[i]);
            // }
            // printf("\n\n");
            printf(messages[MSG_HW_OTHER_FIXED][lang], info_File);
        } else {
            printf(messages[MSG_HW_ALL_MODIFY][lang], nPI);
        }

        //La función distingue internamente los casos mod_MP = {0,1} y crea el vector de rugosidades de longitud adecuada
        Current_Vars.roughness_coefficients = get_roughness_coefficients_from_inp(nPI, nMP, mod_MP, indMP, ph);

        printf(messages[MSG_HW_READ_OK][lang], info_File);

        SENSEI_PAUSE();
    }


    //LECTURA DE LA INFORMACIÓN DE LOS GRUPOS DE TUBERÍAS DESDE LOS ARCHIVOS .csv 
    if (Opt.mod_common_roughness_DW == 1 || Opt.mod_common_roughness_HW == 1){  

        nCP = classify_pipes_by_cluster(clusters_pipes_File, ph, Clusters_Pipes_Info, lang);

        printf(messages[MSG_PIPE_GROUPS_READ][lang], nCP);
        for (int i = 0; i < nCP; i++){
            // printf("Cluster de tuberias: %s \n", Clusters_Pipes_Info[i].cluster_id);
            // printf("Numero de tuberias: %d \n", Clusters_Pipes_Info[i].pipe_count);
            // printf("Indices: ");
            // for (int j = 0; j < Clusters_Pipes_Info[i].pipe_count; j++){
            //     printf("%d  ", Clusters_Pipes_Info[i].pipe_index[j]);
            // }
            // printf("\n\n\n");
        }

        //Si se ha elegido optimizar un coef. de DW para cada grupo
        if (Opt.mod_common_roughness_DW == 1){

            //Leer el coef. DW de cada grupo desde common_DW_coef y asociarlo a
            //algún grupo de Clusters_Pipes_Info
            classify_common_group_coef(nCP, 0, common_DW_File, Clusters_Pipes_Info, ph, lang);
            
            for (int i = 0; i < nCP; i++){
                printf(messages[MSG_CLUSTER_ID][lang], Clusters_Pipes_Info[i].cluster_id);
                printf(messages[MSG_CLUSTER_DW_COEF][lang], Clusters_Pipes_Info[i].common_DW_coef);
            }

            //Inicializar coeficiente
            Current_Vars.common_DW_coefs = (double*)malloc(nCP*sizeof(double));
            for (int i = 0; i < nCP; i++){
                Current_Vars.common_DW_coefs[i] = Clusters_Pipes_Info[i].common_DW_coef;
            }

            //Conversión de coeficiente DW de cada grupo a coeficiente HW para cada tubería
            Current_Vars.roughness_coefficients = convert_roughness_coefs_from_DW_to_HW(nPI, nCP, Current_Vars.common_DW_coefs, Clusters_Pipes_Info, ph);
            
            printf("%s", messages[MSG_COMMON_COEFS_OK][lang]);
        
        //Si se ha elegido optimizar un coef. de HW para cada grupo
        } else {

            //Leer el coef. de cada grupo desde common_HW_coef y asociarlo a
            //algún grupo de Clusters_Pipes_Info
            classify_common_group_coef(nCP, 1, common_HW_File, Clusters_Pipes_Info, ph, lang);

            for (int i = 0; i < nCP; i++){
                printf(messages[MSG_CLUSTER_ID][lang], Clusters_Pipes_Info[i].cluster_id);
                printf(messages[MSG_CLUSTER_HW_COEF][lang], Clusters_Pipes_Info[i].common_HW_coef);
            }

            Current_Vars.common_HW_coefs = (double*)malloc(nCP*sizeof(double));
            for (int i = 0; i < nCP; i++){
                Current_Vars.common_HW_coefs[i] = Clusters_Pipes_Info[i].common_HW_coef;
            }

            Current_Vars.roughness_coefficients = extend_common_roughness_coefs_to_all_pipes(nPI, nCP, Current_Vars.common_HW_coefs, Clusters_Pipes_Info, ph);

            printf("%s", messages[MSG_COMMON_COEFS_EXTENDED][lang]);
        
        }

        SENSEI_PAUSE();
    } 


    //LECTURA DE LAS DEMANDAS BASE (solamente para NODOS DE DEMANDA) DESDE infoFile
    if (Opt.mod_base_demands == 1){
        
        Current_Vars.base_demands = get_base_demands_vector_from_inp(nJ_Dem, ind_J_Dem, ph);

        printf(messages[MSG_BASE_DEMANDS_READ_OK][lang], nJ_Dem, info_File);
        // for (int i = 0; i < nJ_Dem; i++){
        //     printf("%.4f  ", Current_Vars.base_demands[i]);
        // }
        // printf("\n\n\n");

        SENSEI_PAUSE();
    }

    
    int nPumpCurves;
    int* pumpCurveIndices = NULL;

    //LECTURA DE CURVAS DE LAS BOMBAS DESDE infoFile
    if (Opt.mod_curves_Y == 1 || Opt.mod_curves_X == 1){

        nPumpCurves = store_pump_curves_info(&Current_Vars.xPumpCurves, &Current_Vars.yPumpCurves, 
            &pumpCurveIndices, ph, lang);

        //Comprobacion de que hemos almacenado bien las curvas
        printf(messages[MSG_PUMP_CURVES_READ_OK][lang], nPumpCurves, info_File);

        for (int i = 0; i < nPumpCurves; i++){
            EN_getcurveid(ph, pumpCurveIndices[i], element_id);
            printf(messages[MSG_PUMP_CURVE_HEADER][lang], element_id);
            for (int j = 0; j < 3; j++){
                printf("(%.2f, %.2f) \n", Current_Vars.xPumpCurves[i][j], Current_Vars.yPumpCurves[i][j]);
            }
            printf("\n");
        }
        printf("\n\n");

        SENSEI_PAUSE();
    }

    double* min_tank_levels = (double*)malloc(nJ*sizeof(double)); //Niveles mínimos permitidos para los tanques
    double* max_tank_levels = (double*)malloc(nJ*sizeof(double)); //Niveles máximos permitidos para los tanques

    //LECTURA DE NIVELES INICIALES DE LOS TANQUES DESDE infoFile 
    //(también de niveles mínimos y máximos permitidos)
    if (Opt.mod_init_tank_levels == 1){
        
        get_min_max_tank_levels(nJ, nR, nT, &min_tank_levels, &max_tank_levels, ph); 

        //Nivel inicial de los tanques (del .inp)
        Current_Vars.init_tank_levels = get_initial_tank_levels(nJ, nR, nT, ph);

        printf(messages[MSG_TANK_LEVELS_READ_OK][lang], info_File);
        for (int i = 0; i < nT; i++){
            EN_getnodeid(ph, nJ+nR+i+1, element_id);
            printf(messages[MSG_TANK_LEVEL_LINE][lang], element_id, 
                Current_Vars.init_tank_levels[i], min_tank_levels[i], max_tank_levels[i]);
        }
        printf("\n\n");

        SENSEI_PAUSE();
    }


    //LECTURA DEL DIÁMETRO DE LOS TANQUES DESDE infoFile
    if (Opt.mod_tank_diameters == 1){
        Current_Vars.tank_diameters = get_tank_diameters(nJ, nR, nT, ph);

        printf(messages[MSG_TANK_DIAMETERS_READ_OK][lang], info_File);
        for (int i = 0; i < nT; i++){
            EN_getnodeid(ph, nJ+nR+i+1, element_id);
            printf(messages[MSG_TANK_DIAM_LINE][lang], element_id, Current_Vars.tank_diameters[i]);
        }
        printf("\n\n");

        SENSEI_PAUSE();
    }


    int nControls, nSetCtrls;

    //LECTURA DE LOS SETTINGS DE LAS VÁLVULAS (desde info_File)
    if (Opt.mod_valves_settings == 1){

        EN_getcount(ph, EN_CONTROLCOUNT, &nControls);  //Número total de controles

        printf(messages[MSG_TOTAL_CONTROLS][lang], nControls);

        //Settings de las válvulas (declarados en sección [VALVES] de info_File)
        get_valves_settings(nPI, nPU, nV, &Current_Vars.valves_settings, ph);

        printf("%s", messages[MSG_VALVE_SETTINGS_HEADER][lang]);
        for (int i = 0; i < nV; i++){
            EN_getlinkid(ph, nPI+nPU+i+1, element_id);
            printf(messages[MSG_VALVE_SETTING_LINE][lang], element_id, Current_Vars.valves_settings[i]);
        }
        printf("\n\n");

        
        //Número de controles cuyo setting vamos a cambiar (porque afectan
        //directamente al setting de las válvulas) y almacenamiento en Setting_Controls_Info
        nSetCtrls = classify_control_settings_to_edit(nControls, nPI, nPU, Setting_Controls_Info, ph);
        
        printf(messages[MSG_VALVE_SETTINGS_CONTROLS_COUNT][lang], nSetCtrls);
        // for (int i = 0; i < nSetCtrls; i++){
        //     printf("Indice: %d | Tipo: %d | Link: %d | Setting: %.4f | Nodo: %d | Nivel / tiempo: %.4f \n", 
        //         Setting_Controls_Info[i].index, Setting_Controls_Info[i].control_type, Setting_Controls_Info[i].linkIndex,
        //         Setting_Controls_Info[i].setting, Setting_Controls_Info[i].nodeIndex, Setting_Controls_Info[i].level);
        //     printf("valve index: %d \n", Setting_Controls_Info[i].valve_index);
        // }
        // printf("\n\n");

        SENSEI_PAUSE();
    }

    printf("%s", messages[MSG_CALIB_OPTIONS_READ_OK][lang]);

    SENSEI_PAUSE();

    /*************************************************************************
    *************************************************************************
    * LECTURA y ALMACENAMIENTO de MEDIDAS / PSEUDOMEDIDAS desde archivos .csv
    *************************************************************************
    *************************************************************************/

    //Definimos e inicializamos los vectores y matrices necesarios 

    //Para medidores de nivel
    int* wlm_logic = NULL;
    double** wlm_values = NULL, **CV_wlm = NULL;
    define_initialize_vectors_matrices_for_sensors_info(nT, T, &wlm_logic, &wlm_values, &CV_wlm);

    //Para demanda
    int nDM; //Número de datos (en cada intervalo) para las demandas
    if (all_demands_nodes_rows == 1){
        nDM = nJ_Dem; //tantas medidas para demanda (filas) como nodos de demanda haya
    } else {
        nDM = nDemPat; //tantas medidas para demanda (filas) como patrones haya
    }
    int* dm_logic = NULL;
    double** dm_values = NULL, **CV_dm = NULL;
    define_initialize_vectors_matrices_for_sensors_info(nDM, T, &dm_logic, &dm_values, &CV_dm);

    //Para medidores de presión    
    int* pm_logic = NULL; //Vector lógico para saber si hay medida o no en cada elemento
    double** pm_values = NULL, **CV_pm = NULL; 
    define_initialize_vectors_matrices_for_sensors_info(nJ, T, &pm_logic, &pm_values, &CV_pm);

    //Para medidores de flujo instantáneo
    int* fm_logic = NULL; 
    double** fm_info = NULL, **CV_fm = NULL; 
    define_initialize_vectors_matrices_for_sensors_info(nL, T, &fm_logic, &fm_info, &CV_fm);

    //Para medidores de flujo promediado
    int* fm_avg_logic = NULL; 
    double** fm_avg_info = NULL, **CV_fm_avg = NULL; 
    define_initialize_vectors_matrices_for_sensors_info(nL, T, &fm_avg_logic, &fm_avg_info, &CV_fm_avg);


    //Leemos .csv y almacenamos información en los vectores y matrices
    int nWLM = store_wlm_info(T, nR, nT, nJ, wlm_File, wlm_logic, wlm_values, CV_wlm, ph, lang); //Número de sensores medidores de nivel
    if (all_demands_nodes_rows == 1){  //Demandas con enfoque de demanda por junction de demanda y por instante
        store_dm_info(T, nDM, nJ_Dem, ind_J_Dem, dm_File, dm_values, CV_dm, ph, lang);
    } else { //Demandas con enfoque de factor multiplicar por patrón y por instante
        store_patterns_info(T, nDemPat, patterns_File, dm_values, CV_dm, ph);
    }
    int nPM = store_pm_info(T, nJ, pm_File, pm_logic, pm_values, CV_pm, ph, lang); //Número de sensores medidores de presión
    int nFM = store_fm_info(T, nL, fm_File, fm_logic, fm_info, CV_fm, 0, ph, lang); //Número de sensores medidores instantáneos de flujo
    int nFM_avg = store_fm_info(T, nL, fm_avg_File, fm_avg_logic, fm_avg_info, CV_fm_avg, 1, ph, lang); //Número de sensores medidores de flujo promediado


    int measurements_count; //Número de medidas de cada tipo
    // DEMANDA
    printf(messages[MSG_DM_ROWS][lang], nDM);
    measurements_count = count_measurements(nDM, T, CV_dm);
    printf(messages[MSG_DM_MEASURE_COUNT][lang], measurements_count);


    //Almacenamos posiciones útiles para elementos con medida
    int* wlm_positions = get_sensor_positions_vector(nWLM, nT, wlm_logic); //Vector con la posición de los elementos en los que hay sensor de nivel (nJ+nR+1-based)
    int* pm_positions = get_sensor_positions_vector(nPM, nJ, pm_logic); //Vector con la posición de los elementos en los que hay sensor de presión (1-based)
    int* fm_positions = get_sensor_positions_vector(nFM, nL, fm_logic); //Vector con la posición de los elementos en los que hay sensor instantáneo de flujo (1-based)
    int* fm_avg_positions = get_sensor_positions_vector(nFM_avg, nL, fm_avg_logic); //Vector con la posición de los elementos en los que hay sensor promediado de flujo (1-based)

    // NIVEL DE AGUA
    if (nWLM > 0) {
        printf(messages[MSG_WLM_ROWS_ID][lang], nWLM);
        for (int i = 0; i < nWLM; i++){
            EN_getnodeid(ph, wlm_positions[i]+nJ+nR, element_id);  //sumamos nJ+nR porque debemos indexar para conseguir el id del tanque
            printf("%s ", element_id);
        }
        printf("\n");
        measurements_count = count_measurements(nT, T, CV_wlm);
        printf(messages[MSG_WLM_MEASURE_COUNT][lang], measurements_count);
    }

    // PRESIÓN
    if (nPM > 0) {
        printf(messages[MSG_PM_ROWS_ID][lang], nPM);
        for (int i = 0; i < nPM; i++){
            EN_getnodeid(ph, pm_positions[i], element_id);
            printf("%s ", element_id);
        }
        printf("\n");
        measurements_count = count_measurements(nJ, T, CV_pm);
        printf(messages[MSG_PM_MEASURE_COUNT][lang], measurements_count);
    }

    // FLUJO INSTANTÁNEO
    if (nFM > 0) {
        printf(messages[MSG_FM_ROWS_ID][lang], nFM);
        for (int i = 0; i < nFM; i++){
            EN_getlinkid(ph, fm_positions[i], element_id);
            printf("%s ", element_id);
        }
        printf("\n");
        measurements_count = count_measurements(nL, T, CV_fm);
        printf(messages[MSG_FM_MEASURE_COUNT][lang], measurements_count);
    }

    // FLUJO PROMEDIADO
    if (nFM_avg > 0) {
        printf(messages[MSG_FMAVG_ROWS_ID][lang], nFM_avg);
        for (int i = 0; i < nFM_avg; i++){
            EN_getlinkid(ph, fm_avg_positions[i], element_id);
            printf("%s ", element_id);
        }
        printf("\n");
        measurements_count = count_measurements(nL, T, CV_fm_avg);
        printf(messages[MSG_FMAVG_MEASURE_COUNT][lang], measurements_count);
    }

    printf("%s", messages[MSG_MEASUREMENTS_READ_OK][lang]);

    SENSEI_PAUSE();


    /*************************************************************************
    *************************************************************************
    * VECTOR de MEDIDAS / PSEUDOMEDIDAS y PESOS
    *************************************************************************
    *************************************************************************/

    int m = nDM + nWLM + nPM + nFM + nFM_avg; //Número de medidas/pseudomedidas por periodo
    printf(messages[MSG_MEASURES_PER_PERIOD][lang], m);


    //Matriz de tamaño (m,T) con el CV de cada medida/pseudomedida (fila) en cada momento (columna) 
    double** CV_matrix = get_CV_matrix(T, m, nDM, nWLM, nPM, nFM, nFM_avg, wlm_positions,
        pm_positions, fm_positions, fm_avg_positions, CV_dm, CV_wlm, CV_pm, 
        CV_fm, CV_fm_avg);

    // printf("Matriz de CV de medidas/pseudomedidas: \n\n");
    // for (int i = 0; i < m; i++){
    //     for (int t = 0; t < T; t++){
    //         printf("%.3f ", CV_matrix[i][t]);
    //     }
    //     printf("\n");
    // }
    // printf("\n\n");


    //Vector de tamaño m*T con los valores medidos/pseudomedidos cada intervalo
    double* z = get_measurements_vector(T, m, nDM, nWLM, nPM, nFM, nFM_avg, wlm_positions,
        pm_positions, fm_positions, fm_avg_positions, dm_values, wlm_values, pm_values, fm_info, fm_avg_info);

    // printf("Medidas / pseudomedidas en todos los periodos: \n\n");
    // for (int t = 0; t < T; t++){
    //     for (int i = 0; i < m; i++){
    //         printf("%.2f ", z[i+m*t]);
    //     }
    //     printf("\n");
    // }
    // printf("\n\n");



    //Vector de tamaño m*T con los pesos de las medidas / pseudomedidas de cada intervalo
    double* weights_vector = compute_weights_vector(T, m, z, CV_matrix);


    // printf("Pesos en todos los periodos: \n\n");
    // for (int t = 0; t < T; t++){
    //     for (int i = 0; i < m; i++){
    //         printf("%.2f ", weights_vector[t*m+i]);
    //     }
    //     printf("\n");
    // }
    // printf("\n\n");
        
    
    //Inicialización de cholmod
    cholmod_common c;  
    cholmod_start(&c); 
    
    //Matrices diagonales cholmod sparse con distintas opciones:
    //0) Diagonal con pesos 1) Diagonal con sqrt(pesos) 2) Diagonal con inversa de pesos
    cholmod_sparse* W_matrix = diagonal_matrix_from_vector(T, m, weights_vector, 0, &c);
    cholmod_sparse* sqrt_W_matrix = diagonal_matrix_from_vector(T, m, weights_vector, 1, &c);
    cholmod_sparse* R_matrix = diagonal_matrix_from_vector(T, m, weights_vector, 2, &c);


    //Liberamos matrices de medidas y de incertidumbre:
    for (int i = 0; i < nT; i++) {
        free(wlm_values[i]);  free(CV_wlm[i]);  
    }
    free(wlm_values); free(CV_wlm);  
    for (int i = 0; i < nDM; i++) {
        free(dm_values[i]);  free(CV_dm[i]);  
    }
    free(dm_values); free(CV_dm);
    for (int i = 0; i < nJ; i++) {
        free(pm_values[i]);  free(CV_pm[i]);  
    }
    free(pm_values);   free(CV_pm); 
    for (int i = 0; i < nL; i++) {
        free(fm_info[i]);  free(CV_fm[i]);  
    }
    free(fm_info);   free(CV_fm);  
    for (int i = 0; i < nL; i++) {
        free(fm_avg_info[i]);  free(CV_fm_avg[i]);  
    }
    free(fm_avg_info);   free(CV_fm_avg);  

    printf("%s", messages[MSG_GROUPING_CVS_DONE][lang]);

    print_memory_usage(lang);

    SENSEI_PAUSE();

    /*************************************************************************
    *************************************************************************
    * PREPARACIÓN PARA EL ALGORITMO DE CALIBRACIÓN
    *************************************************************************
    *************************************************************************/

    cholmod_sparse *J = NULL;  //Matriz Jacobiana

    double *gx, *gx_prov = NULL;  //Valores a comparar con las medidas (tras simulación)
    double *gx_init_loop = (double*)malloc(m*T*sizeof(double));

    cholmod_dense *sqrt_weighted_residuals = NULL; //Residuos ponderados por la raíz cuadrada de los pesos
    cholmod_dense *delta_x = NULL; //Incremento calculado para las variables 

    double omega = 1.0; //Factor de subrelajación (DEFAULT = 1. Equivale a NO hacer subrelajación)
    double lambda; //Factor de regularización de LEVENBERG-MARQUARDT

    int nan_count; //Contador de elementos NaN (en matrices que calculemos)
    int inf_count; //Contador de elementos infinitos (en matrices que calculemos)
    int num_unknowns;  //Número de variables en cada etapa (número de columnas en el jacobiano)

    double conv_rate;  //Tasa de cambio entre aproximaciones (valor que compararemos con la tolerancia)

    int convergence_in_stage; //Variable para indicar que hay que hemos convergido (y forzar un break DESPUÉS de actualizar valores)


    double SWSE, SWSE_prov, SWSE_beginning_loop; //Función objetivo (Sum of Weighted Squared Errors)

    //Preparamos la estructura Prov_Vars para almacenar las variables
    //provisionales tras resolver la ecuación de Levenberg-Marquardt
    define_provisional_variables_calibration(&Prov_Vars, Opt, nDemPat, T, nJ_Dem, nPumpCurves, 
        nV, nCP, mod_MP, nMP, nPI, nT);

    if (num_stages == 1){ //Si toda la calibración se reduce a 1 etapa, no hay que recorrer el "bucle" max_loops veces
        max_it = max_loops; //Forzamos tantas iteraciones como veces íbamos a recorrer el bucle
        max_loops = 1; 
    }


    const char* suffix = "_intermediate.inp";
    size_t base_len = strlen(info_File) - 4;
    size_t total_len = base_len + strlen(suffix) + 1;
    char* intermediate_File = (char*)malloc(total_len); //nombre del .inp intermedio para simular y actualizar valores
    snprintf(intermediate_File, total_len, "%.*s%s", (int)base_len, info_File, suffix);


    const char *preffix = "LATEST_IMPROVEMENT_";
    const char *slash = strrchr(info_File, '/');
    size_t path_len = strlen(base_Path);
    total_len = path_len + 1 + strlen(preffix) + strlen(slash + 1) + 1;
    char *latest_improv_File = malloc(total_len); //nombre del .inp en el que guardaremos la última mejora
    snprintf(latest_improv_File, total_len, "%s/%s%s", base_Path, preffix, slash + 1);


    //Creamos copia del info_File, y será éste el que usemos para cambiar valores y simular durante todo el proceso
    EN_saveinpfile(ph, intermediate_File); 

    printf("%s", messages[MSG_SIM_START_INFO][lang]);
    
    edit_ph_all_current_variables(intermediate_File, rpt_File, binary_File, nPumpCurves, Current_Vars.xPumpCurves, 
        Current_Vars.yPumpCurves, pumpCurveIndices, nPI, nPU, nV, nMP, mod_MP, indMP, nJ, nR, nT, 
        nJ_Dem, ind_J_Dem, nDemPat, T, nSetCtrls, Current_Vars, Opt, Setting_Controls_Info, ph, lang);

    //Guardar .inp semilla de la calibración
    // EN_saveinpfile(ph, "INICIO_CALIBRACION.inp"); 
    
    //Simulación hidráulica (multiperiodo) para la configuración de arranque
    gx = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, nDM, nWLM, 
        nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, pm_positions, 
        fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);
 
    //Residuos ponderados por la raíz cuadrada de los pesos
    sqrt_weighted_residuals = compute_sqrt_weighted_residuals(T, m, z, gx, weights_vector, &c);

    //Print de valores simulados al inicio
    // for (int i = 0; i < nDM; i++){
    //     printf("Demanda %d) ", i+1);
    //     for (int t = 0; t < T; t++){
    //         printf("%.3f  ", gx[i+m*t]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // for (int i = 0; i < nWLM; i++){
    //     printf("nivel %d) ", i+1);
    //     for (int t = 0; t < T; t++){
    //         printf("%.3f  ", gx[i+nDM+m*t]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // for (int i = 0; i < nPM; i++){
    //     EN_getnodeid(ph, pm_positions[i], element_id);
    //     printf("Pr(%s) ", element_id);
    //     for (int t = 0; t < T; t++){
    //         printf("%.3f  ", gx[i+nDM+nWLM+m*t]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // for (int i = 0; i < nFM; i++){
    //     EN_getlinkid(ph, fm_positions[i], element_id);
    //     printf("Q(%s) ", element_id);
    //     for (int t = 0; t < T; t++){
    //         printf("%.3f  ", gx[i+nDM+nWLM+nPM+m*t]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");


    // for (int i = 0; i < nFM_avg; i++){
    //     EN_getlinkid(ph, fm_avg_positions[i], element_id);
    //     printf("Q_avg(%s) ", element_id);
    //     for (int t = 0; t < T; t++){
    //         printf("%.3f  ", gx[i+nDM+nWLM+nPM+nFM+m*t]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    // system("pause");


    printf("%s", messages[MSG_OBJFUNC_INITIAL_HEADER][lang]);
    SWSE = compute_SWSE_calibration(sqrt_weighted_residuals, 0, T, m, nDM, nWLM, nPM, nFM, nFM_avg, lang);

    SENSEI_PAUSE();

    //Entramos en el bucle grande (que recoge una y otra vez los grupos de
    //variables en el orden definido en options_File)
    for (int loop = 0; loop < max_loops; loop++){

        if (max_loops > 1){
            printf(messages[MSG_LOOP_HEADER][lang], loop + 1);
        }

        if (loop > 0 && SWSE == SWSE_beginning_loop){
            printf("%s", messages[MSG_NO_IMPROVEMENT_STOP][lang]);
            SENSEI_PAUSE();
            break;
        } else {
            SWSE_beginning_loop = SWSE;  //Actualizamos el SWSE al principio del bucle como el actual
        }


        if (loop > 0){
            conv_rate = compute_convergence_rate(T, m, gx, gx_init_loop); 
            //Tasa de cambio entre g(x) al inicio del bucle y del anterior (para comparar cambio de un bucle a otro)
            printf(messages[MSG_CONV_RATE][lang], conv_rate);
            
            if (conv_rate < tol){
                printf("%s", messages[MSG_CONVERGENCE_REACHED][lang]);
                SENSEI_PAUSE();
                break;
            }
        }

        for (int i = 0; i < m*T; i++){
            gx_init_loop[i] = gx[i];
        }

        //print de la serie simulada para el nivel de los tanques con sensor
        if (nWLM > 0){
            printf("%s", messages[MSG_TIME_EVOLUTION_TANK_WLM][lang]);
            for (int i = 0; i < nWLM; i++){
                EN_getnodeid(ph, wlm_positions[i]+nJ+nR, element_id);
                printf("\033[1;32m%s)\033[0m ", element_id);
                for (int t = 0; t < T; t++){
                    printf("%.3f  ", gx[i+nDM+m*t]);
                }
                printf("\n");
            }
            printf("\n\n\n");
        }


        //print de la serie simulada para la presión en los junctions con sensor
        if (nPM > 0){
            printf("%s", messages[MSG_TIME_EVOLUTION_JUNCTION_PM][lang]);
            for (int i = 0; i < nPM; i++){
                EN_getnodeid(ph, pm_positions[i], element_id);
                printf("\033[1;32m%s)\033[0m ", element_id);
                for (int t = 0; t < T; t++){
                    printf("%.3f  ", gx[i+nDM+nWLM+m*t]);
                }
                printf("\n");
            }
            printf("\n\n\n");
        }


        //print de la serie simulada para el flujo (instantáneo) los enlaces con sensor
        if (nFM > 0){
            printf("%s", messages[MSG_TIME_EVOLUTION_LINK_FM][lang]);
            for (int i = 0; i < nFM; i++){
                EN_getlinkid(ph, fm_positions[i], element_id);
                printf("\033[1;32m%s)\033[0m ", element_id);
                for (int t = 0; t < T; t++){
                    printf("%.3f  ", gx[i+nDM+nWLM+nPM+m*t]);
                }
                printf("\n");
            }
            printf("\n\n\n");
        }

        //print de la serie simulada para el flujo (promediado) los enlaces con sensor
        if (nFM_avg > 0){ 
            printf("%s", messages[MSG_TIME_EVOLUTION_LINK_FMAVG][lang]);
            for (int i = 0; i < nFM_avg; i++){
                EN_getlinkid(ph, fm_avg_positions[i], element_id);
                printf("\033[1;32m%s)\033[0m ", element_id);
                for (int t = 0; t < T; t++){
                    printf("%.3f  ", gx[i+nDM+nWLM+nPM+nFM+m*t]);
                }
                printf("\n");
            }
            printf("\n\n\n");
        }

        SENSEI_PAUSE();

        //Entramos en cada etapa definida (optimizamos un grupo concreto de variables)
        for (int stage = 0; stage < num_stages; stage++){

            int it = 0;  //Contador de iteraciones para cada etapa
            lambda = lambda_init; //Parámetro de regularización
            convergence_in_stage = 0;
            
            printf(messages[MSG_STAGE_START][lang], stage + 1);


            printf("%s", messages[MSG_STAGE_OPTIMIZATION_HEADER][lang]);

            // Patrones de demanda
            if (CM[stage][0] == 1)
                printf("%s", messages[MSG_VAR_DEMAND_PATTERNS][lang]);
            // Demandas base
            if (CM[stage][1] == 1)
                printf("%s", messages[MSG_VAR_BASE_DEMAND][lang]);
            // Pump curve X
            if (CM[stage][2] == 1)
                printf("%s", messages[MSG_VAR_PUMP_CURVE_X][lang]);
            // Pump curve Y
            if (CM[stage][3] == 1)
                printf("%s", messages[MSG_VAR_PUMP_CURVE_Y][lang]);
            // Valve settings
            if (CM[stage][4] == 1)
                printf("%s", messages[MSG_VAR_VALVE_SETTINGS][lang]);
            // HW rugosidad MAIN o ALL
            if (CM[stage][5] == 1) {
                if (mod_MP == 1)
                    printf("%s", messages[MSG_VAR_HW_MAIN][lang]);
                else
                    printf("%s", messages[MSG_VAR_HW_ALL][lang]);
            }
            // DW común
            if (CM[stage][6] == 1)
                printf("%s", messages[MSG_VAR_DW_GROUP][lang]);
            // HW común
            if (CM[stage][7] == 1)
                printf("%s", messages[MSG_VAR_HW_GROUP][lang]);
            // Nivel inicial tanques
            if (CM[stage][8] == 1)
                printf("%s", messages[MSG_VAR_TANK_LEVEL][lang]);
            // Diámetro tanques
            if (CM[stage][9] == 1)
                printf("%s", messages[MSG_VAR_TANK_DIAM][lang]);

            printf("\n\n");

            print_memory_usage(lang);

            SENSEI_PAUSE();

            while (it < max_it){

                printf(messages[MSG_ITERATION_HEADER][lang], it + 1);

                //Antes de comenzar la iteración, actualizamos en el ph los valores
                //actuales de todas las variables que hemos ido modificando
                edit_ph_all_current_variables(intermediate_File, rpt_File, binary_File, 
                    nPumpCurves, Current_Vars.xPumpCurves, Current_Vars.yPumpCurves, 
                    pumpCurveIndices, nPI, nPU, nV, nMP, mod_MP, indMP, nJ, nR, nT, nJ_Dem, 
                    ind_J_Dem, nDemPat, T, nSetCtrls, Current_Vars, Opt, Setting_Controls_Info, ph, lang);

                //Guardamos la última mejora (por si hubiera algún colapso en este paso)
                EN_saveinpfile(ph, latest_improv_File);


                if (loop > 0 || stage > 0 || it > 0){                    
                    //Simulamos para los valores actuales de las variables
                    gx = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, nJ_Dem, 
                        nDM, nWLM, nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, wlm_positions, 
                        pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);
                    printf("%s", messages[MSG_SIM_DONE_CURRENT_VARS][lang]);
                } 

                if (sqrt_weighted_residuals != NULL) {
                    cholmod_free_dense(&sqrt_weighted_residuals, &c);
                    sqrt_weighted_residuals = NULL;
                }
                //Residuos ponderados por la raíz cuadrada de los pesos 
                //(se puede quitar, es solo para asegurarnos)
                sqrt_weighted_residuals = compute_sqrt_weighted_residuals(T, m, z, gx, weights_vector, &c);

                //Evaluamos función objetivo
                //(se puede quitar, es solo para asegurarnos)
                SWSE = compute_SWSE_calibration(sqrt_weighted_residuals, 0, T, m, nDM, nWLM, nPM, 
                    nFM, nFM_avg, lang);

                //Calculamos matriz jacobiana
                if (J != NULL) {
                    cholmod_free_sparse(&J, &c);
                    J = NULL;  // buena práctica para evitar punteros colgantes
                }
                J = compute_Jacobian_calibration(&num_unknowns, patternStep, hydStep, T, epsilon, 
                    nDM, nWLM, nPM, nFM, nFM_avg, wlm_positions, pm_positions, fm_positions, 
                    fm_avg_positions, nJ, nR, nT, nPI, nPU, nV, m, nDemPat, nCP, nMP, stage, CM, 
                    mod_MP, all_demands_nodes_rows, nPumpCurves, pumpCurveIndices, nJ_Dem, 
                    ind_J_Dem, nSetCtrls, Setting_Controls_Info, Clusters_Pipes_Info, indMP, 
                    min_tank_levels, max_tank_levels, Current_Vars, Opt, gx, intermediate_File, 
                    rpt_File, binary_File, &c, ph, lang);  
                        
                cholmod_print_sparse(J, "JACOBIAN", &c);  //Vista rápida de sus características
                // check_nan_inf_matrix(&nan_count, &inf_count, J); //Comprobación de NaN y +-inf
                // printf("En la matriz JACOBIANA hay %d NaN y %d +-inf \n\n", nan_count, inf_count);

                printf(messages[MSG_NUM_UNKNOWNS][lang], num_unknowns);
                // save_dense_matrix_to_txt(J, "J.txt", &c);

                int break_while = 0; //Para forzar salir del while (al haber muchos bucles anidados, el break no hace efecto en el while)

                for (int attempt = 0; attempt < max_attempts; attempt++){  //Máximo 3 intentos aumentando lambda

                    printf(messages[MSG_REGULARIZATION_ATTEMPT][lang], attempt + 1, max_attempts);

                    printf("Lambda = %e\n\n", lambda);  // Lambda en notación cientfíca

                    if (delta_x != NULL){
                        cholmod_free_dense(&delta_x, &c);
                        delta_x = NULL;
                    }
                    
                    //Resolvemos (J^t*W*J + lambda*D^t*D)*Delta_x = (J^t*W)*(z-g(x))
                    delta_x = solve_Levenberg_Marquardt_equation(num_unknowns, J, sqrt_W_matrix, 
                        sqrt_weighted_residuals, lambda, scale, &c, lang);       

                    //Trasladamos el incremento Delta_x a la estructura de variables provisionales
                    compute_provisional_variables_calibration(omega, num_unknowns, delta_x, stage, 
                        CM, T, nDemPat, nJ_Dem, nPumpCurves, pumpCurveIndices, nSetCtrls, nPI, nV, 
                        nCP, nMP, mod_MP, nT, nJ, nR, ind_J_Dem, min_tank_levels, max_tank_levels, 
                        Setting_Controls_Info, Current_Vars, &Prov_Vars, Clusters_Pipes_Info, ph, lang);

                    //Imponemos los valores provisionales (los modificamos en el ph)
                    edit_ph_provisional_variables_calibration(stage, CM, T, nDemPat, nJ_Dem, 
                        nPumpCurves, pumpCurveIndices, nSetCtrls, nPI, nPU, nV, nCP, nMP, mod_MP, 
                        indMP, nT, nJ, nR, intermediate_File, rpt_File, binary_File, ind_J_Dem, 
                        Setting_Controls_Info, Clusters_Pipes_Info, Current_Vars, Prov_Vars, Opt, ph, lang);

                    // Guardar .inp provisional 
                    // EN_saveinpfile(ph, "PROVISIONAL.inp");

                    //Simulamos para las variables provisionales
                    gx_prov = run_whole_simulation_calibration(patternStep, hydStep, T, nJ, nR, nT, 
                        nJ_Dem, nDM, nWLM, nPM, nFM, nFM_avg, nDemPat, m, all_demands_nodes_rows, 
                        wlm_positions, pm_positions, fm_positions, fm_avg_positions, ind_J_Dem, ph, lang);


                    //Residuos ponderados por la raíz cuadrada de los pesos
                    sqrt_weighted_residuals = compute_sqrt_weighted_residuals(T, m, z, gx_prov, weights_vector, &c);

                    //Valor PROVISIONAL de la función objetivo
                    SWSE_prov = compute_SWSE_calibration(sqrt_weighted_residuals, 0, T, m, nDM, 
                        nWLM, nPM, nFM, nFM_avg, lang);

                    
                    if (SWSE_prov < SWSE){ //Si mejoramos...
                        printf("%s", messages[MSG_DESCENT_DIRECTION_UPDATE][lang]);
                        
                        //Consolidamos los valores (hasta ahora provisionales) en Current_Vars
                        update_current_variables_with_provisional_variables_calibration(stage, CM, 
                            T, nDemPat, nJ_Dem, nPumpCurves, nSetCtrls, nPI, nV, nCP, nMP, 
                            mod_MP, nT, Clusters_Pipes_Info, &Current_Vars, Prov_Vars, ph);

                        //Calculamos tasa de cambio entre g(x_prov) (nueva) y g(x) (anterior)
                        conv_rate = compute_convergence_rate(T, m, gx_prov, gx);
                        printf(messages[MSG_CONV_RATE_SIMPLE][lang], conv_rate);
                        if (conv_rate < tol){ //Comparación con la tolerancia
                            convergence_in_stage = 1;
                        }
                        
                        //Actualizamos g(x) como g(x_prov)
                        for (int i = 0; i < m*T; i++){
                            gx[i] = gx_prov[i];
                        }

                        //Actualizamos el valor de la función objetivo (SWSE)
                        SWSE = SWSE_prov;
                        
                        //Avanzamos 1 iteración
                        it += 1;

                        if (gx_prov != NULL){
                            free(gx_prov);
                        }

                        if (it == max_it){ //Si agotamos las iteraciones, finalizamos este ajuste y pasamos a la siguiente etapa
                            printf("%s", messages[MSG_STAGE_MAX_IT_REACHED][lang]);
                        } else if (convergence_in_stage == 1){ //Si ha convergido, finalizamos este ajuste y pasamos a la siguiente etapa
                            printf("%s", messages[MSG_STAGE_CONVERGED][lang]);
                            break_while = 1;
                            break;
                        }
                        
                        //Como hemos mejorado, reducimos la regularización de J^t*W*J
                        printf("%s", messages[MSG_LAMBDA_REDUCE][lang]);
                        lambda = lambda/10;

                        SENSEI_PAUSE();
                        break;

                    } else { //Si no mejoramos...
                        printf("%s", messages[MSG_NO_IMPROVEMENT][lang]);
                        if (gx_prov != NULL){
                            free(gx_prov);
                        }

                        if (attempt < max_attempts-1){
                            printf("%s", messages[MSG_LAMBDA_INCREASE][lang]);
                            lambda = 10*lambda;
                        } else {
                            printf("%s", messages[MSG_ATTEMPTS_EXHAUSTED_CALIB][lang]);
                            break_while = 1;
                            SENSEI_PAUSE();
                            break;
                        }
                    }
                }

                if (break_while) {
                    break; // salir del while
                }

                print_memory_usage(lang);
                SENSEI_PAUSE();
            }
        }
    }

    SENSEI_PAUSE();

    //Guardamos el .inp del modelo calibrado añadiendo el sufijo "YYYYMMDD_hhmmss.csv"
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S.inp", t);

    total_len = base_len + strlen("_calibrated_model_") + strlen(timestamp) + 1; // +1 para '\0'
    char* solution_File = (char*)malloc(total_len);  //Nombre del .inp donde guardamos el modelo calibrado
    snprintf(solution_File, total_len, "%.*s_calibrated_model_%s", (int)base_len, info_File, timestamp);

    EN_saveinpfile(ph, solution_File);

    printf(messages[MSG_SOLUTION_SAVED][lang], solution_File);

    SENSEI_PAUSE();


    /***************************************************************************
    ****************************************************************************
    * PROPAGACIÓN DE INCERTIDUMBRE (cálculo del CV de las variables ajustadas)
    ****************************************************************************
    ****************************************************************************/

    printf("%s", messages[MSG_UNCERTAINTY_PROPAGATION_HEADER][lang]);

    if (Opt.mod_patterns) CM[0][0] = 1;
    if (Opt.mod_base_demands) CM[0][1] = 1;
    if (Opt.mod_curves_X) CM[0][2] = 1;
    if (Opt.mod_curves_Y) CM[0][3] = 1;
    if (Opt.mod_valves_settings) CM[0][4] = 1;
    if (Opt.mod_individual_roughness) CM[0][5] = 1;
    if (Opt.mod_common_roughness_DW) CM[0][6] = 1;
    if (Opt.mod_common_roughness_HW) CM[0][7] = 1;
    if (Opt.mod_init_tank_levels) CM[0][8] = 1;
    if (Opt.mod_tank_diameters) CM[0][9] = 1;

    // printf("CM[0][0]: %d \n", CM[0][0]);
    // printf("CM[0][1]: %d \n", CM[0][1]);
    // printf("CM[0][2]: %d \n", CM[0][2]);
    // printf("CM[0][3]: %d \n", CM[0][3]);
    // printf("CM[0][4]: %d \n", CM[0][4]);
    // printf("CM[0][5]: %d \n", CM[0][5]);
    // printf("CM[0][6]: %d \n", CM[0][6]);
    // printf("CM[0][7]: %d \n", CM[0][7]);
    // printf("CM[0][8]: %d \n", CM[0][8]);
    // printf("CM[0][9]: %d \n", CM[0][9]);


    // int num_params = Opt.mod_patterns*T*nDemPat + Opt.mod_base_demands*nJ_Dem + 
    //     Opt.mod_curves_X*2*nPumpCurves + Opt.mod_curves_Y*3*nPumpCurves + 
    //     Opt.mod_ctrl_levels*nControls + Opt.mod_valves_settings*nV + 
    //     Opt.mod_individual_roughness*((1-mod_MP)*nPI + mod_MP*nMP) + Opt.mod_common_roughness_DW*nCP + 
    //     Opt.mod_common_roughness_HW*nCP + Opt.mod_init_tank_levels*nT + Opt.mod_tank_diameters*nT;
    // printf("Tenemos %d parametros \n\n", num_params);

    if (J != NULL) {
        cholmod_free_sparse(&J, &c);
        J = NULL;  // buena práctica para evitar punteros colgantes
    }
    J = compute_Jacobian_calibration(&num_unknowns, patternStep, hydStep, T, epsilon, nDM, nWLM, 
        nPM, nFM, nFM_avg, wlm_positions, pm_positions, fm_positions, fm_avg_positions, 
        nJ, nR, nT, nPI, nPU, nV, m, nDemPat, nCP, nMP, 0, CM, mod_MP, all_demands_nodes_rows, 
        nPumpCurves, pumpCurveIndices, nJ_Dem, ind_J_Dem, nSetCtrls, 
        Setting_Controls_Info, Clusters_Pipes_Info, indMP, 
        min_tank_levels, max_tank_levels, Current_Vars, Opt, gx, 
        intermediate_File, rpt_File, binary_File, &c, ph, lang);

    printf(messages[MSG_TOTAL_PARAMS_OPTIMIZED][lang], num_unknowns);

    // save_dense_matrix_to_txt(J, "final_J.txt", &c);

    cholmod_print_sparse(J, "JACOBIAN", &c);  //Vista rápida de sus características
    check_nan_inf_matrix(&nan_count, &inf_count, J); //Comprobación de NaN y +-inf
    // printf("En la matriz JACOBIANA hay %d NaN y %d +-inf \n\n", nan_count, inf_count);

    //Vector para almacenar la desv. típica de todos los parámetros / variables
    //que hemos modificado a lo largo de la calibración
    double* posterior_sd = (double*)malloc(num_unknowns*sizeof(double)); 

    uncertainty_propagation_calibration(num_unknowns, T, nFM, nFM_avg, m, gx, z, 
        posterior_sd, weights_vector, sqrt_W_matrix, R_matrix, J, &c, lang);

    printf("%s", messages[MSG_UNCERTAINTY_PROPAGATION_DONE][lang]);

    // for (int i = 0; i < num_unknowns; i++){
    //     printf("%.4f \n", posterior_sd[i]);
    // }
    // printf("\n\n");

    // system("pause");


    //Guardamos el .csv con los resultados de la EE añadiendo el sufijo "_YYYYMMDD_hhmmss.csv"
    now = time(NULL);
    t = localtime(&now);

    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S.csv", t);

    total_len = base_len + strlen("_propagation_uncertainty_") + strlen(timestamp) + 1;

    char* propag_uncert_File = (char*)malloc(total_len); // nombre del .csv 
    snprintf(propag_uncert_File, total_len, "%.*s_propagation_uncertainty_%s", (int)base_len, info_File, timestamp);


    save_posterior_sd_CV_calib_parameters(propag_uncert_File, T, nJ_Dem, nJ, nR, nT, nPI,
        nPU, nV, mod_MP, nMP, nCP, nPumpCurves, nDemPat, indMP, ind_J_Dem, pumpCurveIndices,
        posterior_sd, CM, demand_patterns_id, Current_Vars, Clusters_Pipes_Info, ph, lang);

    printf(messages[MSG_UNCERTAINTY_FILE_SAVED][lang], propag_uncert_File);


    SENSEI_PAUSE();

    printf("----------------------------------------------- \n");
    printf("----------------------------------------------- \n\n");


    EN_close(ph);

    printf("%s", messages[MSG_PROJECT_DELETING][lang]);
    EN_deleteproject(ph);  // Eliminar el proyecto
    Sleep(1000);   // duerme 1000 ms = 1 segundo
    printf("%s", messages[MSG_PROJECT_DELETED][lang]);


    printf("%s", messages[MSG_INTERMEDIATE_DELETING][lang]);
    Sleep(1000);   // duerme 1000 ms = 1 segundo
    if (remove(intermediate_File) == 0) {
        printf(messages[MSG_INTERMEDIATE_DELETED][lang], intermediate_File);
    } else {
        printf(messages[MSG_INTERMEDIATE_DELETE_FAILED][lang], intermediate_File);
    }
    if (remove(latest_improv_File) == 0) {
        printf(messages[MSG_INTERMEDIATE_DELETED][lang], latest_improv_File);
    } else {
        printf(messages[MSG_INTERMEDIATE_DELETE_FAILED][lang], latest_improv_File);
    }
    if (remove(rpt_File) == 0) {
        printf(messages[MSG_INTERMEDIATE_DELETED][lang], rpt_File);
    } else {
        printf(messages[MSG_INTERMEDIATE_DELETE_FAILED][lang], rpt_File);
    }
    // if (remove(binary_File) == 0) {
    //     printf("%s eliminado\n\n", binary_File);
    // } else {
    //     printf("No se ha podido %s \n\n", binary_File);
    // }


    SENSEI_PAUSE();

    return 0;

}
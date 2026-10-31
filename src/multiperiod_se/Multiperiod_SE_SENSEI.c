#include "functions_SENSEI.h"  
#include "messages_SENSEI.h"

// ESTIMACIÓN DE ESTADO MULTIPERIODO (SECUENCIAL Y PSEUDO-DINÁMICA)

int main() {   

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // salida en UTF-8
        SetConsoleCP(CP_UTF8);       // entrada en UTF-8
        system("chcp 65001 > nul");  // asegura UTF-8 en consola
    #endif

    /************************************************************
    ************************************************************
    * CONFIGURACIÓN de la EE según "config_multiperiod_SE.txt"
    ************************************************************
    ************************************************************/

    const char* config_File = "config_multiperiod_SE.txt";

    //Hiperparámetros del algoritmo
    double epsilon, lambda_init, tol;
    int max_it, max_attempts, lang;

    char* base_Path = NULL; //Subcarpeta
    char* info_File = NULL; //Modelo hidráulico (.inp)
    //Archivos (.csv) de medidas / pseudomedidas
    char *dm_File = NULL, *wlm_File = NULL, *pm_File = NULL, *fm_File = NULL, *fm_avg_File = NULL;

    //Información necesaria para añadir un emisor en un nodo a partir de un cierto momento
    char* emitter_ID = NULL;
    double emitter_coef;
    int period_to_add_emitter;

    //Lectura de "config_multiperiod_SE.txt" e inicialización de variables
    read_config_file_multiperiod_SE(config_File, &base_Path, &info_File, &dm_File, 
        &wlm_File, &pm_File, &fm_File, &fm_avg_File, &epsilon, &max_it, &tol, &lambda_init, 
        &max_attempts, &emitter_ID, &emitter_coef, &period_to_add_emitter, &lang);

    
    printf("%s", messages[MSG_WELCOME_MULTIPERIOD_SE][lang]);  // Mensaje de bienvenida a la terminal

    // IDIOMA
    printf("%s", messages[MSG_INFO_LANGUAGE_SELECTED][lang]);
    printf(messages[MSG_SWITCH_LANG][lang], config_File);

    SENSEI_PAUSE();


    //RUTA Y ARCHIVOS DECLARADOS 
    printf("%s", messages[MSG_HEADER_PATH_FILES][lang]);
    printf("%s", messages[MSG_PATH_LABEL][lang]);
    if (base_Path == NULL){
        printf(messages[MSG_PATH_NULL][lang], config_File);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }
    else {printf(messages[MSG_PATH_OK][lang], base_Path);}

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
   
 
    //PARÁMETROS DEL ALGORITMO
    printf("\n%s", messages[MSG_HEADER_ALGO_PARAMS][lang]);
    printf(messages[MSG_LABEL_EPSILON][lang], epsilon);
    printf(messages[MSG_LABEL_MAX_IT_MULTIPERIOD][lang], max_it);
    printf(messages[MSG_LABEL_TOL][lang], tol);
    printf(messages[MSG_LABEL_LAMBDA_INIT_MULTIPERIOD][lang], lambda_init);
    printf(messages[MSG_LABEL_MAX_ATTEMPTS_MULTIPERIOD][lang], max_attempts);



    //INFORMACIÓN EXTRA RESPECTO A FUGAS (MEDIANTE EMISORES)
    int add_emitter = 0, emitter_index;
    printf("%s", messages[MSG_LEAK_EXTRA_INFO_HEADER][lang]);
    if (emitter_ID == NULL){
        printf("%s", messages[MSG_LEAK_NO_NODE_SPECIFIED][lang]);
    } else {    
        printf(messages[MSG_LEAK_EMITTER_ASSUMED][lang], emitter_coef, emitter_ID, period_to_add_emitter);

        add_emitter = 1; //Activamos flag (se pasa como argumento a funciones)        
    }
    
    int scale = 1;  //1 para regularizar J^t*W*J con lambda*D^t*D; 0 para hacerlo con lambda*I (DEJAR 1)

    //Verificar que dm_File ha sido declarado
    if (dm_File == NULL){
        printf("%s", messages[MSG_ERR_MISSING_DM_SENSORS][lang]);
        SENSEI_PAUSE();
    } 

    //Verificar que wlm_File ha sido declarado
    if (wlm_File == NULL){
        printf("%s", messages[MSG_ERR_MISSING_WLM_SENSORS][lang]);
        SENSEI_PAUSE();
    } 

    SENSEI_PAUSE();
    
    /************************************************************
    ************************************************************
    * LECTURA INICIAL y ALMACENAMIENTO desde info_File
    *************************************************************
    ************************************************************/
    
    Variables_Multiperiod_SE Current_Vars; //Estructura para almacenar el valor de las variables de estado en cada momento
    Variables_Multiperiod_SE Prov_Vars; //Estructura para almacenar el valor provisional de las variables de estado tras resolver ec. Lev-Marq.

    Ctrl_01 Controls_01[100]; //Estructura para almacenar la información de controles asociados a tanques declarados en info_File
    Ctrl_23 Controls_23[100]; //Estructura para almacenar la información de controles temporales (asociados a horas) declarados en info_File

    Ctrl_Links_Summary Controls_Links_Summary; //Estructura para seguir la evolución de los estados de los elementos de control

    Reservoir_Pattern_Info Reservoir_Patterns_Info[50]; //Estructura para almacenar el nivel piezométrico de los tanques en cada periodo (si se les ha asociado patrón)
    
    char* rpt_File = "report.rpt";
    char* binary_File = "salida.bin";
  
    char element_id[60];  
    int element_index;

    EN_Project ph;  // Project handler
    EN_createproject(&ph);  // Crear nuevo proyecto

    int error = EN_open(ph, info_File, rpt_File, binary_File);  //Cargar el .inp info_File en el ph
    if (error){
        char msg[256];
        EN_geterror(error, msg, 255);
        printf(messages[MSG_ERROR_INFO_FILE][lang], info_File, msg);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    if (add_emitter){
        // Asociar índice al ID del nodo con el emisor
        // (si se ha añadido la opción de activar emisor en un instante dado)
        error = EN_getnodeindex(ph, emitter_ID, &emitter_index);
        if (error){
            char msg[256];
            EN_geterror(error, msg, 255);
            printf(messages[MSG_ERR_EMITTER_ID][lang], emitter_ID, msg, config_File);
            SENSEI_PAUSE();
            exit(EXIT_FAILURE);
        }
    }


    //LECTURA DE PARÁMETROS TEMPORALES 
    long int duration, patternStep, hydStep;
    EN_gettimeparam(ph, EN_DURATION, &duration); //Duración de la serie temporal
    EN_gettimeparam(ph, EN_PATTERNSTEP, &patternStep); //Frecuencia de los datos 
    EN_gettimeparam(ph, EN_HYDSTEP, &hydStep); //Hydraulic Step (frecuencia de parada del motor de EPANET en simulación)

    int nPeriods = duration/patternStep;  //Número de periodos de la EE
    int nSteps = nPeriods+1;  //Número de instantes (se considera también el final del último periodo)

    printf(messages[MSG_MULTIPERIOD_PERIODS][lang], nPeriods);
    printf(messages[MSG_PERIOD_LENGTH][lang], patternStep/60.0);
    printf(messages[MSG_HYD_STEP][lang], hydStep/60.0);



    //Establecemos la duración de las simulaciones como la duración de cada periodo (patternStep)
    EN_settimeparam(ph, EN_DURATION, patternStep); 

    SENSEI_PAUSE();


    const char* suffix = "_single_period.inp";
    size_t base_len = strlen(info_File) - 4; 
    size_t total_len = base_len + strlen(suffix) + 1; 
    char* intermediate_File = (char*)malloc(total_len); //nombre del .inp para hacer simulaciones y actualizar valores
    snprintf(intermediate_File, total_len, "%.*s%s", (int)base_len, info_File, suffix);
    EN_saveinpfile(ph, intermediate_File); //Copia 


    //Conteo de nodos y enlaces de la red y desglose por tipos
    int nN, nL, nJ, nR, nT, nPI, nPU, nV;
    count_elements_in_network(&nN, &nL, &nJ, &nR, &nT, &nPI, &nPU, &nV, ph);

    printf("%s", messages[MSG_NET_HEADER][lang]);
    printf(messages[MSG_NET_NODES][lang], nN, nJ, nR, nT);
    printf(messages[MSG_NET_LINKS][lang], nL, nPI, nPU, nV);

    int nJ_Trans; //Número de junctions de tránsito (demanda base nula)
    int nJ_Dem; //Número de junctions de demanda (que no son de tránsito)
    int *ind_J_Trans = NULL, *ind_J_Dem = NULL;

    //Clasifica los junctions según tránsito o demanda y almacena sus índices
    classify_transit_demand_junctions(nJ, &nJ_Trans, &nJ_Dem, &ind_J_Trans, &ind_J_Dem, ph);
    printf(messages[MSG_JUNCTIONS_SPLIT][lang], nJ_Trans, nJ_Dem);

    //Lectura de niveles mínimos y máximos de los tanques 
    double* min_tank_levels = (double*)malloc(nJ*sizeof(double));
    double* max_tank_levels = (double*)malloc(nJ*sizeof(double));

    get_min_max_tank_levels(nJ, nR, nT, &min_tank_levels, &max_tank_levels, ph); 

    // for (int i = 0; i < nT; i++){
    //     EN_getnodeid(ph, nJ+nR+i+1, element_id);
    //     printf("%s) Nivel min: %.2f | Nivel max: %.2f \n", element_id, min_tank_levels[i], max_tank_levels[i]);
    // }
    // printf("\n\n");


    //LECTURA DEL ESTADO INICIAL DE TUBERÍAS, BOMBAS y VÁLVULAS AFECTADAS POR LEYES DE CONTROL

    int nCtrl_01;  //Número de controles de tipo 0 (EN_LOWLEVEL) ó 1 (EN_HILEVEL)
    int nCtrl_23;  //Número de controles de tipo 2 (EN_TIMER) ó 3 (EN_TIMEOFDAY)

    //Almacenar los controles de cada tipo en las estructuras específicas
    store_controls(nPI, nPU, &nCtrl_01, &nCtrl_23, patternStep, Controls_01, Controls_23, ph);

    printf(messages[MSG_CONTROL_TYPES_COUNT][lang], nCtrl_01, nCtrl_23);

    // printf("Controles de tipo 01: \n");
    // for (int i = 0; i < nCtrl_01; i++){
    //     printf("Indice: %d \n", Controls_01[i].index);
    //     printf("Tipo: %d \n", Controls_01[i].control_type);
    //     printf("Valvula? %d \n", Controls_01[i].is_valve);
    //     printf("Indice link: %d \n", Controls_01[i].linkIndex);
    //     printf("Indice nodo: %d \n", Controls_01[i].nodeIndex);
    //     printf("Setting: %.4f \n", Controls_01[i].setting);
    //     printf("SENTENCE: %s \n\n\n", Controls_01[i].sentence);
    // }

    // printf("Controles de tipo 23: \n");
    // for (int i = 0; i < nCtrl_23; i++){
    //     printf("Indice: %d \n", Controls_23[i].index);
    //     printf("Tipo: %d \n", Controls_23[i].control_type);
    //     printf("Valvula? %d \n", Controls_23[i].is_valve);
    //     printf("Indice link: %d \n", Controls_23[i].linkIndex);
    //     printf("Periodo: %d \n", Controls_23[i].period);
    //     printf("Sec in period: %.2f \n", Controls_23[i].sec_in_period);
    //     printf("Setting: %.4f \n", Controls_23[i].setting);
    //     printf("SENTENCE: %s \n\n\n", Controls_23[i].sentence);
    // }


    //Almacena en Controls_Links_Summary los elementos de control asociados a
    //alguna regla de control
    summarize_links_affected_by_controls(nPI, nPU, nCtrl_01, nCtrl_23, Controls_01, Controls_23, &Controls_Links_Summary, ph);

    //Lee el estado inicial (declarado en info_File) de cada elemento de control
    //susceptible de cambiar de estado
    get_initial_status_clean_section_intermediate_file(&Controls_Links_Summary, intermediate_File, ph, lang);

    //Print informativo sobre estado inicial
    if (Controls_Links_Summary.nPipePump > 0){
        printf(messages[MSG_CTRL_PIPEPUMP_AFFECTED][lang], Controls_Links_Summary.nPipePump);
        for (int i = 0; i < Controls_Links_Summary.nPipePump ; i++){
            EN_getlinkid(ph, Controls_Links_Summary.pipe_pump_indices[i], element_id);
            printf(messages[MSG_INITIAL_STATUS_LABEL][lang], element_id);
            if (Controls_Links_Summary.pipe_pump_status[i] == 0) printf("CLOSED \n");
            else printf("OPEN \n");
        }
        printf("\n");
    }

    if (Controls_Links_Summary.nValves > 0){
        printf(messages[MSG_CTRL_VALVES_AFFECTED][lang], Controls_Links_Summary.nValves);
        for (int i = 0; i < Controls_Links_Summary.nValves; i++){
            EN_getlinkid(ph, Controls_Links_Summary.valve_indices[i], element_id);
            printf(messages[MSG_INITIAL_STATUS_LABEL][lang], element_id);
            if (Controls_Links_Summary.valve_status[i] == 0) printf("CLOSED \n");
            else if (Controls_Links_Summary.valve_status[i] == 1) printf("OPEN \n");
            else if (Controls_Links_Summary.valve_status[i] == 2) printf("ACTIVE \n");
        }
        printf("\n");
    }


    //LECTURA DE PATRONES DE HEAD ASOCIADOS A EMBALSES (y almacentamiento en estructura específica)
    
    //Núm. de embalses con patrón asociado de nivel piezométrico
    int nR_pattern = store_reservoir_patterns_info(nPeriods, nJ, nR, Reservoir_Patterns_Info, ph);


    if (nR_pattern > 0){
        printf(messages[MSG_RESERVOIRS_WITH_PATTERN][lang], nR_pattern);
        for (int i = 0; i < nR_pattern; i++){
            EN_getnodeid(ph, Reservoir_Patterns_Info[i].reservoir_index, element_id);
            printf(messages[MSG_RESERVOIR_HEADS_LABEL][lang], element_id);
            for (int t = 0; t < nPeriods; t++){
                printf("%.2f  ", Reservoir_Patterns_Info[i].head[t]);
            }
            printf("\n");
        }
        printf("\n\n");
    }


    printf("%s", messages[MSG_NET_COUNT_DONE][lang]);

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
    define_initialize_vectors_matrices_for_sensors_info(nT, nSteps, &wlm_logic, &wlm_values, &CV_wlm);

    //Para demandas
    int nDM = nJ_Dem; //El número de medidores de demanda debe coincidir con el número de nodos de demanda
    int* dm_logic = NULL;
    double** dm_values = NULL, **CV_dm = NULL;
    define_initialize_vectors_matrices_for_sensors_info(nDM, nSteps, &dm_logic, &dm_values, &CV_dm);

    //Para medidores de presión    
    int* pm_logic = NULL; //Vector lógico para saber si hay medida o no en cada elemento
    double** pm_values = NULL, **CV_pm = NULL; 
    define_initialize_vectors_matrices_for_sensors_info(nJ, nSteps, &pm_logic, &pm_values, &CV_pm);

    //Para medidores de flujo instantáneos 
    int* fm_logic = NULL; 
    double** fm_values = NULL, **CV_fm = NULL; 
    define_initialize_vectors_matrices_for_sensors_info(nL, nSteps, &fm_logic, &fm_values, &CV_fm);

    //Para medidores de flujo promediados
    int* fm_avg_logic = NULL; 
    double** fm_avg_values = NULL, **CV_fm_avg = NULL; 
    define_initialize_vectors_matrices_for_sensors_info(nL, nSteps, &fm_avg_logic, &fm_avg_values, &CV_fm_avg);


    int nWLM = store_wlm_info(nSteps, nR, nT, nJ, wlm_File, wlm_logic, wlm_values, CV_wlm, ph, lang);
    if (nWLM != nT) { //Controlamos que haya tantas series como tanques
        printf(messages[MSG_ERR_WLM_COUNT_MISMATCH][lang], nT, nWLM, wlm_File);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }
    store_dm_info(nPeriods, nDM, nJ_Dem, ind_J_Dem, dm_File, dm_values, CV_dm, ph, lang); 
    //La condición de nDM = nJ_Dem se comprueba implicitamente en store_dm_info 
    int nPM = store_pm_info(nPeriods, nJ, pm_File, pm_logic, pm_values, CV_pm, ph, lang);
    int nFM = store_fm_info(nPeriods, nL, fm_File, fm_logic, fm_values, CV_fm, 0, ph, lang);
    int nFM_avg = store_fm_info(nPeriods, nL, fm_avg_File, fm_avg_logic, fm_avg_values, CV_fm_avg, 1, ph, lang);


    //Contamos que para WLM y para DM no haya celdas vacías
    int measurements_count = count_measurements(nT, nSteps, CV_wlm);
    if (measurements_count != nSteps*nT){
        printf(messages[MSG_ERR_WLM_MEASURE_MISMATCH][lang], measurements_count, nSteps, nT, nSteps*nT, wlm_File);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    } else {
        printf(messages[MSG_WLM_MEASURE_SUMMARY][lang], measurements_count, nSteps, nT);
    }

    measurements_count = count_measurements(nDM, nSteps, CV_dm);
    if (measurements_count != nPeriods*nDM){
        printf(messages[MSG_ERR_DM_MEASURE_MISMATCH][lang], measurements_count, nPeriods, nDM, nPeriods*nDM, dm_File);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    } else {
        printf(messages[MSG_DM_MEASURE_SUMMARY][lang], measurements_count, nPeriods, nDM);
    }

    //Almacenamos posiciones útiles para elementos con medida
    int* wlm_positions = get_sensor_positions_vector(nWLM, nT, wlm_logic); //Vector con la posición de los elementos en los que hay sensor de nivel (nJ+nR+1-based)
    int* pm_positions = get_sensor_positions_vector(nPM, nJ, pm_logic); //Vector con la posición de los elementos en los que hay sensor de presión (1-based)
    int* fm_positions = get_sensor_positions_vector(nFM, nL, fm_logic); //Vector con la posición de los elementos en los que hay sensor de flujo instantáneo (1-based)
    int* fm_avg_positions = get_sensor_positions_vector(nFM_avg, nL, fm_avg_logic); //Vector con la posición de los elementos en los que hay sensor de flujo promediado (1-based)

    // PRESIÓN
    if (nPM > 0) {
        printf(messages[MSG_PM_ROWS_ID][lang], nPM);
        for (int i = 0; i < nPM; i++){
            EN_getnodeid(ph, pm_positions[i], element_id);
            printf("%s ", element_id);
        }
        printf("\n\n");
    }

    // FLUJO INSTANTÁNEO
    if (nFM > 0) {
        printf(messages[MSG_FM_ROWS_ID][lang], nFM);
        for (int i = 0; i < nFM; i++){
            EN_getlinkid(ph, fm_positions[i], element_id);
            printf("%s ", element_id);
        }
        printf("\n\n");
    }

    // FLUJO PROMEDIADO
    if (nFM_avg > 0) {
        printf(messages[MSG_FMAVG_ROWS_ID][lang], nFM_avg);
        for (int i = 0; i < nFM_avg; i++){
            EN_getlinkid(ph, fm_avg_positions[i], element_id);
            printf("%s ", element_id);
        }
        printf("\n\n");
    }
    

    //Inicializamos el vector de niveles iniciales de los tanques para el siguiente periodo
    double* init_tank_levels_new_period = (double*)malloc(nT*sizeof(double));
    for (int i = 0; i < nT; i++){
        init_tank_levels_new_period[i] = wlm_values[i][0];
    }

    //Inicializamos el vector de demandas ajustadas en el último periodo (servirá como semilla)
    double* demands_fitted_last_period = (double*)malloc(nJ_Dem*sizeof(double));
    for (int i = 0; i < nJ_Dem; i++){
        demands_fitted_last_period[i] = dm_values[i][0];
    }

    printf("%s", messages[MSG_MEASUREMENTS_READ_OK][lang]);

    SENSEI_PAUSE();


    /*************************************************************************
    *************************************************************************
    * VECTOR de MEDIDAS / PSEUDOMEDIDAS y PESOS
    *************************************************************************
    *************************************************************************/

    int m = nDM + nWLM + nPM + nFM + nFM_avg;  //Número de medidas/pseudomedidas por periodo
    printf(messages[MSG_MEASURES_PER_PERIOD][lang], m);
    int m_aug = m + nWLM; //Número de variables en cada EE: m + nT
    int num_unknowns = nJ_Dem + nT;  //Número de variables de estado (número de columnas en el jacobiano)

    //Definimos estructuras de la dimensión adecuada para la propagación de
    //incertidumbre, residuos normalizados y para guardar los resultados de las estimaciones
    double** estimated_values = (double**) malloc(m_aug * sizeof(double*)); //Matriz para guardar los valores ajustados en cada EE
    double** posterior_CV = (double**) malloc(m_aug * sizeof(double*));  //Matriz de CVs de las variables estimadas
    double** normalized_residuals = (double**) malloc(m_aug * sizeof(double*)); //Matriz de CVs de residuos normalizados
    
    for (int i = 0; i < m_aug; i++) {
        estimated_values[i] = (double*) malloc(nPeriods * sizeof(double));
        posterior_CV[i] = (double*) malloc(nPeriods * sizeof(double));
        normalized_residuals[i] = (double*) malloc(nPeriods * sizeof(double));
    }


    //Matriz de tamaño (m,T+1) con el CV de cada medida/pseudomedida (fila) en cada momento (columna)
    double** CV_matrix = get_CV_matrix(nSteps, m, nDM, nWLM, nPM, nFM, nFM_avg, wlm_positions,
        pm_positions, fm_positions, fm_avg_positions, CV_dm, CV_wlm, CV_pm, CV_fm, CV_fm_avg);

    // printf("Matriz de CV de medidas/pseudomedidas: \n\n");
    // for (int i = 0; i < m; i++){
    //     for (int t = 0; t < nSteps; t++){
    //         printf("%.3f ", CV_matrix[i][t]);
    //     }
    //     printf("\n");
    // }
    // printf("\n\n");

    // system("pause");

    //Vector de tamaño m*(T+1) con los valores medidos/pseudomedidos cada intervalo
    double* z = get_measurements_vector(nSteps, m, nDM, nWLM, nPM, nFM, nFM_avg, wlm_positions,
        pm_positions, fm_positions, fm_avg_positions, dm_values, wlm_values, pm_values, fm_values, fm_avg_values);

    // printf("Medidas / pseudomedidas en todos los periodos: \n\n");
    // for (int i = 0; i < m; i++){
    //     for (int t = 0; t < nSteps; t++){
    //         printf("%.4f ", z[i+m*t]);
    //     }
    //     printf("\n");
    // }
    // printf("\n\n");

    // system("pause"); 

    //Vector de tamaño m*(T+1) con los pesos de las medidas / pseudomedidas de cada intervalo
    double* weights_vector = compute_weights_vector(nSteps, m, z, CV_matrix);

    // printf("Pesos en todos los periodos (excluyendo del print las demandas): \n\n");
    // for (int i = 0; i < m; i++){
    //     for (int t = 0; t < nSteps; t++){
    //         printf("%.4f ", weights_vector[t*m+i]);
    //     }
    //     printf("\n");
    // }
    // printf("\n\n");

    // system("pause");

    printf("%s", messages[MSG_GROUPING_CVS_DONE][lang]);

    SENSEI_PAUSE();

    /*************************************************************************
    *************************************************************************
    * PREPARACIÓN PARA EL ALGORITMO DE ESTIMACIÓN DE ESTADO
    *************************************************************************
    *************************************************************************/


    // Establecemos todos los patrones de longitud 1 y con el (único) factor multiplicador 
    // como 1 (para evitar problemas, porque solo vamos a trabajar en intervalos sueltos) 
    int patternCount; 

    double* new_pattern = (double*)malloc(1*sizeof(double));
    new_pattern[0] = 1.0;
    
    EN_getcount(ph, EN_PATCOUNT, &patternCount);
    if (patternCount > 0){
        for (int i = 1; i <= patternCount; i++) {
            EN_setpattern(ph, i, new_pattern, 1);
        }
    }


    EN_saveinpfile(ph, intermediate_File); //Copia de la topología de la red, sin patrones, para 1 periodo
    EN_closeH(ph);  //Cerramos motor hidráulico
    EN_close(ph);  //Cerramos el ph asociado a info_File
    EN_open(ph, intermediate_File, rpt_File, binary_File); //Cargamos intermediate_File (1 periodo) al ph


    //Definimos las variables provisionales (con vectores de la dimensión requerida)
    Prov_Vars.base_demands = (double*) malloc(nJ_Dem*sizeof(double));
    Prov_Vars.init_tank_levels = (double*)malloc(nT*sizeof(double));
   

    double* weights_vector_t = NULL; //Vector de pesos de las medidas para cada periodo
    double *z_t = NULL; //Vector de medidas / pseudomedidas para cada periodo

    //Inicialización de cholmod 
    cholmod_common c;
    cholmod_start(&c); 

    cholmod_sparse* W_matrix = NULL, *sqrt_W_matrix = NULL, *R_matrix = NULL;  //Matrices diagonales relacionadas con la incertidumbre de medidas
    cholmod_sparse *J = NULL;  //Matriz Jacobiana

    double *gx, *gx_prov = NULL; //Valores a comparar con las medidas (tras simulación)

    cholmod_dense *sqrt_weighted_residuals = NULL; //Residuos ponderados por la raíz cuadrada de los pesos
    cholmod_dense *delta_x = NULL; //Incremento calculado para las variables de estado

    int nan_count; //Contador de elementos NaN (en matrices que calculemos)
    int inf_count; //Contador de elementos infinitos (en matrices que calculemos)

    double conv_rate;  //Tasa de cambio entre aproximaciones (valor que compararemos con la tolerancia)

    double* sd_final_tank_level = (double*)malloc(nT*sizeof(double)); //Vector de desv. típicas de la estimaciones de los niveles al final del periodo

    printf("%s", messages[MSG_SE_PRECONFIG_DONE][lang]);
    SENSEI_PAUSE();

    for (int t = 0; t < nPeriods; t++){

        printf(messages[MSG_PERIOD_BANNER][lang], t);

        // print_memory_usage();

        if (weights_vector_t != NULL){
            free(weights_vector_t);
            weights_vector_t = NULL;
        }
        if (z_t != NULL){
            free (z_t);
            z_t = NULL;
        }
        weights_vector_t = (double*)malloc(m_aug*sizeof(double));  //Pesos del ajuste del periodo en cuestión
        z_t = (double*)malloc(m_aug*sizeof(double)); //Medidas/pseudomedidas del ajuste del periodo en cuestión
        for (int i = 0; i < nDM; i++){  //Demandas
            weights_vector_t[i] = weights_vector[i+t*m];
            z_t[i] = z[i+t*m];
        } 
        for (int i = 0; i < nWLM; i++){ //Niveles iniciales
            if (t == 0){
                weights_vector_t[i+nDM] = weights_vector[i+nDM+t*m];  
            } else {
                if (sd_final_tank_level[i]/init_tank_levels_new_period[i] < 1e-3){ //Si la desv.típica es "muy pequeña", forzamos CV = 0.001
                    sd_final_tank_level[i] = 0.001*init_tank_levels_new_period[i]; //Para evitar pesos muy desproporcionados
                }
                weights_vector_t[i+nDM] = 1/pow(sd_final_tank_level[i],2);  //Peso asociado a la incertidumbre de la estimación de la EE anterior
            }
            z_t[i+nDM] = init_tank_levels_new_period[i];  //Los niveles al principio son las estimaciones al final de la EE anterior
        } 
        for (int i = 0; i < nPM+nFM+nFM_avg; i++){ //Presiones, flujos iniciales y flujos intermedios
            weights_vector_t[i+nDM+nWLM] = weights_vector[i+nDM+nWLM+t*m];
            z_t[i+nDM+nWLM] = z[i+nDM+nWLM+t*m];
        }
        for (int i = 0; i < nWLM; i++){ //Niveles al final del intervalo
            weights_vector_t[i+m] = weights_vector[i+nDM+(t+1)*m];
            z_t[i+m] = z[i+nDM+(t+1)*m];
        }

        // printf("Vector de pesos para este periodo: \n");
        // for (int i = 0; i < m_aug; i++){
        //     printf("%.6f \n", weights_vector_t[i]);
        // }
        // printf("\n\n");


        if (W_matrix != NULL){
            cholmod_free_sparse(&W_matrix, &c);
            W_matrix = NULL;
        }
        if (sqrt_W_matrix != NULL){
            cholmod_free_sparse(&sqrt_W_matrix, &c);
            sqrt_W_matrix = NULL;
        }
        if (R_matrix != NULL){
            cholmod_free_sparse(&R_matrix, &c);
            R_matrix = NULL;
        }
        //Matrices diagonales cholmod sparse con distintas opciones:
        //0) Diagonal con pesos 1) Diagonal con sqrt(pesos) 2) Diagonal con inversa de pesos
        W_matrix = diagonal_matrix_from_vector(1, m_aug, weights_vector_t, 0, &c);
        sqrt_W_matrix = diagonal_matrix_from_vector(1, m_aug, weights_vector_t, 1, &c);
        R_matrix = diagonal_matrix_from_vector(1, m_aug, weights_vector_t, 2, &c);

        int convergence = 0; //Variable para indicar que hay que hemos convergido (y forzar un break tras actualizar valores)

        int it = 0; //Contador de iteraciones

        double lambda = lambda_init;  //Parámetro de regularización
        
        double SWSE, SWSE_prov;  //Función objetivo (Sum of Weighted Squared Errors)

        
        //Preparamos el ph con:
        //  * Valores semilla (de arranque) para demandas y niveles iniciales
        //  * Forzamos estados iniciales de elementos de control estimados al
        //    final del periodo anterior
        //  * Niv. piezométrico para embalses correspondiente (si tenían patrón asociado)
        //  * Añadimos coef. emisor (si corresponde, según configuración)
        prepare_ph_for_new_period(t, nJ, nR, nT, add_emitter, period_to_add_emitter, emitter_index, 
            emitter_coef, nCtrl_23, nR_pattern, patternStep, nJ_Dem, ind_J_Dem, 
            demands_fitted_last_period, init_tank_levels_new_period, min_tank_levels, 
            max_tank_levels, intermediate_File, rpt_File, binary_File, Reservoir_Patterns_Info, 
            &Controls_Links_Summary, Controls_23, ph, lang);


        //Print informativo sobre los estados iniciales de elementos de control
        if (Controls_Links_Summary.nPipePump > 0){
            for (int i = 0; i < Controls_Links_Summary.nPipePump ; i++){
                EN_getlinkid(ph, Controls_Links_Summary.pipe_pump_indices[i], element_id);
                printf(messages[MSG_INITIAL_STATUS_LABEL][lang], element_id);
                if (Controls_Links_Summary.pipe_pump_status[i] == 0) printf("CLOSED \n");
                else printf("OPEN \n");
            }
            printf("\n");
        }

        if (Controls_Links_Summary.nValves > 0){
            for (int i = 0; i < Controls_Links_Summary.nValves; i++){
                EN_getlinkid(ph, Controls_Links_Summary.valve_indices[i], element_id);
                printf(messages[MSG_INITIAL_STATUS_LABEL][lang], element_id);
                if (Controls_Links_Summary.valve_status[i] == 0) printf("CLOSED \n");
                else if (Controls_Links_Summary.valve_status[i] == 1) printf("OPEN \n");
                else printf("ACTIVE \n");
            }
            printf("\n");
        }


        //Guardamos el inp de arranque como "INICIO_PERIODO.inp"
        // EN_saveinpfile(ph, "INICIO_PERIODO.inp"); 

        //Cargamos como variables el nivel inicial de los tanques
        Current_Vars.init_tank_levels = get_initial_tank_levels(nJ, nR, nT, ph);
        // printf("\033[1;36mNiveles iniciales de los tanques (VALORES DE ARRANQUE): \033[0m");
        // for (int i = 0; i < nT; i++){
        //     printf("\033[1;36m%.4f  \033[0m", Current_Vars.init_tank_levels[i]);
        // }
        // printf("\n\n");

        //Cargamos como variables la demanda de arranque en los nodos de demanda
        Current_Vars.base_demands = get_base_demands_vector_from_inp(nJ_Dem, ind_J_Dem, ph);
        // printf("Demandas de los junctions de demanda (VALORES DE ARRANQUE): ");
        // for (int i = 0; i < nJ_Dem; i++){
        //     printf("%.4f  ", Current_Vars.base_demands[i]);
        // }
        // printf("\n\n");

        
        //Simulación hidráulica (prolongada durante un periodo) para la configuración de arranque
        gx = run_single_period_simulation(patternStep, hydStep, nJ, nR, nT, nJ_Dem, nDM, nWLM, nPM, 
            nFM, nFM_avg, m, m_aug, wlm_positions, pm_positions, fm_positions, fm_avg_positions, 
            ind_J_Dem, nCtrl_01, nCtrl_23, nPI, nPU, t, 1, Controls_01, Controls_23, 
            &Controls_Links_Summary, ph, lang);


        //Almacenamos estado simulado de los elementos de control al final del periodo
        get_status(Controls_01, Controls_23, &Controls_Links_Summary, ph);
    
        //Residuos ponderados por la raíz cuadrada de los pesos
        sqrt_weighted_residuals = compute_sqrt_weighted_residuals(1, m_aug, z_t, gx, weights_vector_t, &c);

        printf("%s", messages[MSG_OBJFUNC_INITIAL_HEADER][lang]);
        SWSE = compute_SWSE_SE(sqrt_weighted_residuals, nDM, nWLM, nPM, nFM, nFM_avg, nWLM, lang);
        
        SENSEI_PAUSE();
        
        while (it < max_it){

            printf(messages[MSG_ITERATION_HEADER][lang], it + 1);

            //Calculamos matriz jacobiana
            if (J != NULL) {
                cholmod_free_sparse(&J, &c);
                J = NULL;  
            }
            J = compute_Jacobian_multiperiod_SE(num_unknowns, patternStep, hydStep, epsilon, 
                nDM, nWLM, nPM, nFM, nFM_avg, wlm_positions, pm_positions, fm_positions, 
                fm_avg_positions, nJ, nR, nT, m, m_aug, nJ_Dem, ind_J_Dem, min_tank_levels, 
                max_tank_levels, gx, &c, nCtrl_01, nCtrl_23, 
                nPI, nPU, t, Controls_01, Controls_23, &Controls_Links_Summary, ph, lang);

            // cholmod_print_sparse(J, "MATRIZ JACOBIANA", &c);  
            // check_nan_inf_matrix(&nan_count, &inf_count, J); //Comprobación de NaN y +-inf en J
            // printf("En la matriz JACOBIANA hay %d NaN y %d +-inf \n\n", nan_count, inf_count);

            // save_dense_matrix_to_txt(J, "J.txt", &c);

            // system("pause");

            int break_while = 0;

            //Inicializamos bucle de intentos (regularizando) para ver si mejoramos el valor
            //actual de la función objetivo
            for (int attempt = 0; attempt < max_attempts; attempt ++){

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
                compute_provisional_variables_multiperiod_SE(delta_x, nJ_Dem, nT, ind_J_Dem, 
                    min_tank_levels, max_tank_levels, Current_Vars, &Prov_Vars, ph);

                //Imponemos los valores provisionales (los modificamos en el ph)
                edit_ph_provisional_variables_multiperiod_SE(nJ_Dem, nT, nJ, nR, ind_J_Dem, Prov_Vars, ph);

                // Guardar .inp provisional 
                // EN_saveinpfile(ph, "PROVISIONAL.inp");

                //Simulamos para las variables provisionales
                gx_prov = run_single_period_simulation(patternStep, hydStep, nJ, nR, nT, nJ_Dem, 
                nDM, nWLM, nPM, nFM, nFM_avg, m, m_aug, wlm_positions, pm_positions, fm_positions, 
                fm_avg_positions, ind_J_Dem, nCtrl_01, nCtrl_23, nPI, nPU, t, 1, 
                Controls_01, Controls_23, &Controls_Links_Summary, ph, lang);

                //Residuos ponderados por la raíz cuadrada de los pesos
                sqrt_weighted_residuals = compute_sqrt_weighted_residuals(1, m_aug, z_t, gx_prov, weights_vector_t, &c);

                //Valor PROVISIONAL de la función objetivo
                SWSE_prov = compute_SWSE_SE(sqrt_weighted_residuals, nDM, nWLM, nPM, nFM, nFM_avg, nWLM, lang);


                if (SWSE_prov < SWSE){ //Si mejoramos...
                    printf("%s", messages[MSG_DESCENT_DIRECTION_UPDATE][lang]);
        
                    //Consolidamos los valores (hasta ahora provisionales) en Current_Vars
                    update_current_variables_with_provisional_variables_multiperiod_SE(nJ_Dem, nT, 
                        &Current_Vars, Prov_Vars, ph);

                    //Calculamos tasa de cambio entre g(x_prov) (nueva) y g(x) (anterior)
                    conv_rate = compute_convergence_rate(1, m_aug, gx_prov, gx);
                    printf(messages[MSG_CONV_RATE_SIMPLE][lang], conv_rate);
                    if (conv_rate < tol){ //Comparación con la tolerancia
                        convergence = 1;
                    }
                    
                    //Actualizamos g(x) como g(x_prov)
                    for (int i = 0; i < m_aug; i++){
                        gx[i] = gx_prov[i];
                    }

                    //Actualizamos el valor de la función objetivo (SWSE)
                    SWSE = SWSE_prov;
                    
                    //Avanzamos 1 iteración
                    it += 1;

                    if (gx_prov != NULL){
                        free(gx_prov);
                    }

                    if (it == max_it){  //Si agotamos las iteraciones, finalizamos ajuste y pasamos al siguiente periodo
                        printf("%s", messages[MSG_PERIOD_MAX_IT_REACHED][lang]);
                        SENSEI_PAUSE();
                    } else if (convergence == 1){  //Si ha convergido, finalizamos ajuste y pasamos al siguiente periodo
                        printf("%s", messages[MSG_PERIOD_CONVERGED][lang]);
                        break_while = 1;
                        SENSEI_PAUSE();
                        break;
                    }

                    //Como hemos mejorado, reducimos la regularización de J^t*W*J
                    printf("%s", messages[MSG_LAMBDA_REDUCE][lang]);
                    lambda = lambda/10;

                    SENSEI_PAUSE();
                    break;

                } else {  //Si no mejoramos...
                    printf("%s", messages[MSG_NO_IMPROVEMENT][lang]);
                    if (gx_prov != NULL){
                        free(gx_prov);
                    }

                    if (attempt < max_attempts-1){
                        printf("%s", messages[MSG_LAMBDA_INCREASE][lang]);
                        lambda = 10*lambda;
                    } else {
                        printf("%s", messages[MSG_ATTEMPTS_EXHAUSTED_MULTIPERIOD][lang]);
                        break_while = 1;
                        SENSEI_PAUSE();
                        break;
                    }
                }

            }
                if (break_while) {
                    break; // salir del while
                }
                // printf("Memoria al acabar la iteracion: \n");
                // print_memory_usage();
        }

        //Estado de los elementos de control al final de la iteración
        get_status(Controls_01, Controls_23, &Controls_Links_Summary, ph);
        //Propagamos la incertidumbre, guardamos la información del ajuste del periodo e
        //imprimimos resultados sobre el ajuste
        end_iteration_and_print_results(t, nJ, nR, nT, nDM, nPM, nFM, nFM_avg, num_unknowns, 
            m, m_aug, ind_J_Dem, pm_positions, fm_positions, fm_avg_positions, z_t, SWSE, weights_vector_t, demands_fitted_last_period,
            init_tank_levels_new_period, gx, estimated_values, posterior_CV, normalized_residuals, 
            sd_final_tank_level, sqrt_W_matrix, R_matrix, J, &c, &Controls_Links_Summary, ph, lang);
        SENSEI_PAUSE();

    }



    //Guardamos el .csv con los resultados de la EE añadiendo el sufijo "_YYYYMMDD_hhmmss.csv"
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S.csv", t);


    //Guardamos en .csv la información de los VALORES ESTIMADOS g(x*) en cada periodo 
    total_len = base_len + strlen("_estimated_values_") + strlen(timestamp) + 1;

    char* estimated_values_File = (char*)malloc(total_len); // nombre del .csv 
    snprintf(estimated_values_File, total_len, "%.*s_estimated_values_%s", (int)base_len, info_File, timestamp);

    save_SE_results_matrix_to_csv(estimated_values_File, nJ, nR, nDM, nWLM, nPM, nFM, nFM_avg, m, nPeriods, 
        ind_J_Dem, wlm_positions, pm_positions, fm_positions, fm_avg_positions, estimated_values, ph, lang);

    printf(messages[MSG_FILE_SAVED_ESTIMATED_VALUES][lang], estimated_values_File);

    //Guardamos en .csv la información de los COEFICIENTES DE VARIACION de las estimaciones de cada periodo 
    total_len = base_len + strlen("_posterior_CV_") + strlen(timestamp) + 1;

    char* posterior_CV_File = (char*)malloc(total_len); // nombre del .csv 
    snprintf(posterior_CV_File, total_len, "%.*s_posterior_CV_%s", (int)base_len, info_File, timestamp);

    save_SE_results_matrix_to_csv(posterior_CV_File, nJ, nR, nDM, nWLM, nPM, nFM, nFM_avg, m, nPeriods, 
        ind_J_Dem, wlm_positions, pm_positions, fm_positions, fm_avg_positions, posterior_CV, ph, lang);

    printf(messages[MSG_FILE_SAVED_POSTERIOR_CV][lang], posterior_CV_File);    

    //Guardamos en .csv la información de los RESIDUOS NORMALIZADOS de las estimaciones de cada periodo
    total_len = base_len + strlen("_normalized_residuals_") + strlen(timestamp) + 1;

    char* normalized_residuals_File = (char*)malloc(total_len); // nombre del .csv 
    snprintf(normalized_residuals_File, total_len, "%.*s_normalized_residuals_%s", (int)base_len, info_File, timestamp);

    save_SE_results_matrix_to_csv(normalized_residuals_File, nJ, nR, nDM, nWLM, nPM, nFM, nFM_avg, m, nPeriods, 
        ind_J_Dem, wlm_positions, pm_positions, fm_positions, fm_avg_positions, normalized_residuals, ph, lang);

    printf(messages[MSG_FILE_SAVED_NORMALIZED_RESIDUALS][lang], normalized_residuals_File);
    

    SENSEI_PAUSE();

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
    if (remove(rpt_File) == 0) {
        printf(messages[MSG_INTERMEDIATE_DELETED][lang], rpt_File);
    } else {
        printf(messages[MSG_INTERMEDIATE_DELETE_FAILED][lang], rpt_File);
    }
    // if (remove(binary_File) == 0) {
    //     printf("%s eliminado\n\n", binary_File);
    // } else {
    //     printf("No se ha podido eliminar %s \n\n", binary_File);
    // }

    SENSEI_PAUSE();

    return 0;

}
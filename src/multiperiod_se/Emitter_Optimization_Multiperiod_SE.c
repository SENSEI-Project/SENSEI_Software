#include "functions_SENSEI.h"  

// ARCHIVO PARA BUSCAR EL COEF. EMISOR ÓPTIMO EN UN NODO CONCRETO

int main() {   

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // salida en UTF-8
        SetConsoleCP(CP_UTF8);       // entrada en UTF-8
        system("chcp 65001 > nul");  // asegura UTF-8 en consola
    #endif

    double E, E_aug;
    double delta = 0.1; 
    double alpha = 0.01;
    double augmented_emitter_coef;
    int emitter_index;
    int add_emitter = 1;

    /************************************************************
    ************************************************************
    * CONFIGURACIÓN de la EE según "config_multiperiod_SE.txt"
    ************************************************************
    ************************************************************/

    //Hiperparámetros del algoritmo
    double epsilon, lambda_init, tol;
    int max_it, max_attempts;

    char* base_Path = NULL; //Subcarpeta
    char* info_File = NULL; //Modelo hidráulico (.inp)
    //Archivos (.csv) de medidas / pseudomedidas
    char *dm_File = NULL, *wlm_File = NULL, *pm_File = NULL, *fm_File = NULL, *fm_avg_File = NULL;

    //Información necesaria para añadir un emisor en un nodo a partir de un cierto momento
    char* emitter_ID = NULL;
    double emitter_coef;
    int period_to_add_emitter;

    int lang;

    //Lectura de "config_multiperiod_SE.txt" e inicialización de variables
    read_config_file_multiperiod_SE("config_multiperiod_SE.txt", &base_Path, &info_File, &dm_File, 
        &wlm_File, &pm_File, &fm_File, &fm_avg_File, &epsilon, &max_it, &tol, &lambda_init, 
        &max_attempts, &emitter_ID, &emitter_coef, &period_to_add_emitter, &lang);

    // period_to_add_emitter = 7; //Lo podemos "forzar también desde aquí"
    lang = 0; //Como este archivo .c es de uso "casero" (no para el repositorio ni para la DEMO, usamos español)

    //RUTA Y ARCHIVOS DECLARADOS 
    printf("\033[4;37mRUTA Y ARCHIVOS DECLARADOS:\033[0m \n");
    printf("RUTA (subcarpeta): ");
    if (base_Path == NULL){
        printf("\033[0;31m NULL. Debes declarar una subcarpeta (PATH en config_multiperiod_SE.txt) "
        "y todos los archivos .csv e .inp deben estar dentro de esa carpeta \033[0m \n");
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }
    else {printf("\033[1;32m%s \033[0m \n", base_Path);}

    printf("Archivo inp de informacion: ");
    if (info_File == NULL){printf("\033[1;33m NULL \033[0m \n");}
    else {printf("\033[1;32m %s \033[0m \n", info_File);}

    printf("Archivo csv medidas de demanda en junctions: ");
    if (dm_File == NULL){printf("\033[1;33m NULL \033[0m \n");}
    else {printf("\033[1;32m %s \033[0m \n", dm_File);}

    printf("Archivo csv medidas de nivel de agua en tanques: ");
    if (wlm_File == NULL){printf("\033[1;33m NULL \033[0m \n");}
    else {printf("\033[1;32m %s \033[0m \n", wlm_File);}

    printf("Archivo csv medidas de presion en junctions: ");
    if (pm_File == NULL){printf("\033[1;33m NULL \033[0m \n");}
    else {printf("\033[1;32m %s \033[0m \n", pm_File);}

    printf("Archivo csv medidas de flujo instantaneo en links: ");
    if (fm_File == NULL){printf("\033[1;33m NULL \033[0m \n");}
    else {printf("\033[1;32m %s \033[0m \n", fm_File);}

    printf("Archivo csv medidas de flujo promediado en links: ");
    if (fm_avg_File == NULL){printf("\033[1;33m NULL \033[0m \n\n");}
    else {printf("\033[1;32m %s \033[0m \n\n", fm_avg_File);}
   
 
    //PARÁMETROS DEL ALGORITMO
    printf("\033[4;37mPARAMETROS DEL ALGORITMO:\033[0m \n");
    printf("Tasa de perturbacion para el jacobiano (epsilon): \033[1;32m%.4f\033[0m \n", epsilon);
    printf("Numero maximo de iteraciones por periodo (max_it): \033[1;32m%d\033[0m \n", max_it);
    printf("Tolerancia (tol): \033[1;32m%e\033[0m \n", tol);
    printf("Valor inicial del parametro de regularizacion (lambda) en cada periodo: \033[1;32m%.4f\033[0m \n", lambda_init);
    printf("Numero maximo de intentos para regularizar antes de pasar al siguiente periodo (max_attempts): \033[1;32m%d\033[0m \n\n", max_attempts);


    int scale = 1;  //1 para regularizar J^t*W*J con lambda*D^t*D; 0 para hacerlo con lambda*I (DEJAR 1)

    //Verificar que dm_File ha sido declarado
    if (dm_File == NULL){
        printf("\033[0;31mTIENES QUE INCLUIR UN ARCHIVO CON INFORMACION DE SENSORES SOBRE LAS DEMANDAS EN CADA JUNCTION DE DEMANDA "
            "(EN SU DEFECTO, USA PSEUDOMEDIDAS).\nCIERRA Y ARREGLALO\033[0m \n\n");
        SENSEI_PAUSE();
    } 

    //Verificar que wlm_File ha sido declarado
    if (wlm_File == NULL){
        printf("\033[0;31mTIENES QUE INCLUIR UN ARCHIVO CON INFORMACION DE SENSORES SOBRE LOS NIVELES EN CADA TANQUE "
            "(EN SU DEFECTO, USA PSEUDOMEDIDAS).\nCIERRA Y ARREGLALO\033[0m \n\n");
        SENSEI_PAUSE();
    } 

    SENSEI_PAUSE();


    //Control sobre dm_File
    if (dm_File == NULL){
        printf("\033[0;31mTIENES QUE INCLUIR UN ARCHIVO CON INFORMACION DE SENSORES SOBRE LAS DEMANDAS EN CADA JUNCTION DE DEMANDA\033[0m \n");
        printf("\033[0;31m(EN SU DEFECTO, PON PSEUDOMEDIDAS). CIERRA Y ARREGLALO\033[0m \n\n");
        SENSEI_PAUSE();
    } 

    //Control sobre wlm_File
    if (wlm_File == NULL){
        printf("\033[0;31mTIENES QUE INCLUIR UN ARCHIVO CON INFORMACION DE SENSORES SOBRE LOS NIVELES EN CADA TANQUE\033[0m \n");
        printf("\033[0;31m(EN SU DEFECTO, PON PSEUDOMEDIDAS). CIERRA Y ARREGLALO\033[0m \n\n");
        SENSEI_PAUSE();
    } 

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
        printf("\033[0;31mERROR al abrir info_File = %s (%s) \033[0m\n", info_File, msg);
        printf("\033[0;31mRevisa el nombre o el contenido del archivo \033[0m\n\n");
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

    //Leemos emitter_index de acuerdo al nodo que queramos
    // EN_getnodeindex(ph, "J62", &emitter_index);
    EN_getnodeindex(ph, emitter_ID, &emitter_index);

    //LECTURA DE PARÁMETROS TEMPORALES 
    long int duration, patternStep, hydStep;
    EN_gettimeparam(ph, EN_DURATION, &duration); //Duración de la serie temporal
    EN_gettimeparam(ph, EN_PATTERNSTEP, &patternStep); //Frecuencia de los datos 
    EN_gettimeparam(ph, EN_HYDSTEP, &hydStep); //Hydraulic Step (frecuencia de parada del motor de EPANET en simulación)

    int nPeriods = duration/patternStep;  //Número de periodos de la EE
    int nSteps = nPeriods+1;  //Número de instantes (se considera también el final del último periodo)

    printf("SE REALIZARA UNA ESTIMACION DE ESTADO DE \033[1;32m%d PERIODOS\033[0m \n", nPeriods);
    printf("Cada periodo dura \033[1;32m%.2f minutos\033[0m (patternStep) \n", patternStep/60.0);
    printf("Hydraulic step: \033[1;32m%.2f minutos\033[0m \n\n", hydStep/60.0);


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

    printf("NUMERO DE ELEMENTOS EN LA RED \n");
    printf("%d NODOS, que corresponden a %d JUNCTIONS, %d EMBALSES y %d TANQUES \n", nN, nJ, nR, nT);
    printf("%d LINKS, que corresponden a %d TUBERIAS, %d BOMBAS y %d VALVULAS \n", nL, nPI, nPU, nV);

    int nJ_Trans; //Número de junctions de tránsito (demanda base nula)
    int nJ_Dem; //Número de junctions de demanda (que no son de tránsito)
    int *ind_J_Trans = NULL, *ind_J_Dem = NULL;

    //Clasifica los junctions según tránsito o demanda y almacena sus índices
    classify_transit_demand_junctions(nJ, &nJ_Trans, &nJ_Dem, &ind_J_Trans, &ind_J_Dem, ph);
    printf("Los junctions se reparten en %d junctions de TRANSITO y %d de DEMANDA \n\n", nJ_Trans, nJ_Dem);

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

    printf("Hay %d controles de tipo EN_LOWLEVEL o EN_HILEVEL y %d de tipo EN_TIMER o EN_TIMEOFDAY \n\n", nCtrl_01, nCtrl_23);

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
        printf("Hay %d tuberias y bombas afectadas por controles: \n", Controls_Links_Summary.nPipePump);
        for (int i = 0; i < Controls_Links_Summary.nPipePump ; i++){
            EN_getlinkid(ph, Controls_Links_Summary.pipe_pump_indices[i], element_id);
            printf("%s) Estado inicial: ", element_id);
            if (Controls_Links_Summary.pipe_pump_status[i] == 0) printf("CLOSED \n");
            else printf("OPEN \n");
        }
        printf("\n");
    }

    if (Controls_Links_Summary.nValves > 0){
        printf("Hay %d valvulas afectadas por controles: \n", Controls_Links_Summary.nValves);
        for (int i = 0; i < Controls_Links_Summary.nValves; i++){
            EN_getlinkid(ph, Controls_Links_Summary.valve_indices[i], element_id);
            printf("%s) Estado inicial: ", element_id);
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
        printf("Hay %d embalses con patron asociado \n", nR_pattern);
        for (int i = 0; i < nR_pattern; i++){
            EN_getnodeid(ph, Reservoir_Patterns_Info[i].reservoir_index, element_id);
            printf("Embalse %s; heads: ", element_id);
            for (int t = 0; t < nPeriods; t++){
                printf("%.2f  ", Reservoir_Patterns_Info[i].head[t]);
            }
            printf("\n");
        }
        printf("\n\n");
    }


    printf("\033[0;32mCONTEO DE LOS ELEMENTOS DE LA RED Y LECTURA INICIAL FINALIZADOS \033[0m\n");
    printf("\033[0;32m--------------------------------------------------------------- \033[0m\n");

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
        printf("\033[0;31mERROR: Se esperaban %d series de medidas/pseudomedidas de nivel en tanques, pero se han leido %d lineas de datos\033[0m \n", nT, nWLM);
        printf("\033[0;31mRevisa el contenido de wlm_File (%s) \033[0m\n", wlm_File);
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
        printf("\033[0;31mHay %d medidas/pseudomedidas pero deberia haber (T+1)*nT = %d*%d = %d \033[0m\n", measurements_count, nSteps, nT, nSteps*nT);
        printf("\033[0;31mRevisa el contenido de %s \033[0m\n", wlm_File);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    } else {
        printf("Se han recogido %d medidas / pseudomedidas de nivel en tanques ", measurements_count);
        printf("(%d para cada uno de los %d tanques) \n\n", nSteps, nT);
    }

    measurements_count = count_measurements(nDM, nSteps, CV_dm);
    if (measurements_count != nPeriods*nDM){
        printf("\033[0;31mHay %d medidas/pseudomedidas pero deberia haber T*nDM = %d*%d = %d \033[0m\n", measurements_count, nPeriods, nDM, nPeriods*nDM);
        printf("\033[0;31mRevisa el contenido del %s \033[0m\n", dm_File);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    } else {
        printf("Se han recogido %d medidas / pseudomedidas de demanda ", measurements_count);
        printf("(%d para cada uno de los %d junctions de demanda) \n\n", nPeriods, nDM);
    }

    //Almacenamos posiciones útiles para elementos con medida
    int* wlm_positions = get_sensor_positions_vector(nWLM, nT, wlm_logic); //Vector con la posición de los elementos en los que hay sensor de nivel (nJ+nR+1-based)
    int* pm_positions = get_sensor_positions_vector(nPM, nJ, pm_logic); //Vector con la posición de los elementos en los que hay sensor de presión (1-based)
    int* fm_positions = get_sensor_positions_vector(nFM, nL, fm_logic); //Vector con la posición de los elementos en los que hay sensor de flujo instantáneo (1-based)
    int* fm_avg_positions = get_sensor_positions_vector(nFM_avg, nL, fm_avg_logic); //Vector con la posición de los elementos en los que hay sensor de flujo promediado (1-based)

    if (nPM > 0){
        printf("Hay datos de medidores de presion en %d junctions, que son: ", nPM);
        for (int i = 0; i < nPM; i++){
            EN_getnodeid(ph, pm_positions[i], element_id);
            printf("%s  ", element_id);
        }
        printf("\n");
    }
    
    if (nFM > 0){
        printf("Hay datos de medidores INSTANTENOS de flujo en %d enlaces, que son: ", nFM);
        for (int i = 0; i < nFM; i++){
            EN_getlinkid(ph, fm_positions[i], element_id);
            printf("%s  ", element_id);
        }
        printf("\n");
    }

    if (nFM_avg > 0){
        printf("Hay datos de medidores PROMEDIADOS de flujo en %d enlaces, que son: ", nFM_avg);
        for (int i = 0; i < nFM; i++){
            EN_getlinkid(ph, fm_avg_positions[i], element_id);
            printf("%s  ", element_id);
        }
        printf("\n");
    }
    printf("\n");
    

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


    printf("\033[0;32mLECTURA y ALMACENAMIENTO de MEDIDAS / PSEUDOMEDIDAS desde archivos .csv BIEN HECHO\033[0m\n");
    printf("\033[0;32m----------------------------------------------------------------------------------\033[0m\n\n");

    SENSEI_PAUSE();


    /*************************************************************************
    *************************************************************************
    * VECTOR de MEDIDAS / PSEUDOMEDIDAS y PESOS
    *************************************************************************
    *************************************************************************/

    int m = nDM + nWLM + nPM + nFM + nFM_avg;  //Número de medidas/pseudomedidas por periodo
    printf("En cada periodo tenemos m = %d medidas (incluye pseudomedidas) \n\n", m);
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


    printf("\033[0;32mAGRUPACION DE CVs, MEDIDAS y PESOS REALIZADA \033[0m\n");
    printf("\033[0;32m-------------------------------------------- \033[0m\n\n");

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

    printf("\033[0;32mCONFIGURACIONES PREVIAS PARA LA EE REALIZADAS \033[0m\n");
    printf("\033[0;32m--------------------------------------------- \033[0m\n\n");
    SENSEI_PAUSE();


    for (int k = 0; k < 5; k++){

        E = 0;
        E_aug = 0;

        augmented_emitter_coef = emitter_coef + delta;

        printf("\033[1;34mIniciamos BUCLE GRANDE con emitter_coef = %.8f \n\n\n \033[0m", emitter_coef);
        // printf("\033[1;34mDelta = %.5f | Alfa = %.5f \n\n \033[0m", delta, alpha);
        SENSEI_PAUSE();

        Controls_Links_Summary.valve_status[0] = 2;  //Forzamos que la válvula empiece abierta

        for (int i = 0; i < nT; i++){  
            init_tank_levels_new_period[i] = wlm_values[i][0];
        }

        for (int t = 0; t < nPeriods; t++){

            printf("\033[1;35m==================\033[0m \n");
            printf("\033[1;35m==================\033[0m \n");
            printf("\033[1;35mPERIODO t = %d \033[0m \n", t);
            printf("\033[1;35m==================\033[0m \n");
            printf("\033[1;35m==================\033[0m \n\n");


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


            prepare_ph_for_new_period(t, nJ, nR, nT, add_emitter, period_to_add_emitter, 
                emitter_index, emitter_coef, nCtrl_23, nR_pattern, patternStep, nJ_Dem, ind_J_Dem, 
                demands_fitted_last_period, init_tank_levels_new_period, min_tank_levels, 
                max_tank_levels, intermediate_File, rpt_File, binary_File, Reservoir_Patterns_Info, 
                &Controls_Links_Summary, Controls_23, ph, lang);


            // EN_saveinpfile(ph, "INICIO_EE.inp"); 

            //Cargamos como variables el nivel inicial de los tanques
            Current_Vars.init_tank_levels = get_initial_tank_levels(nJ, nR, nT, ph);
            //Cargamos como variables la demanda de arranque en los nodos de demanda
            Current_Vars.base_demands = get_base_demands_vector_from_inp(nJ_Dem, ind_J_Dem, ph);
            
            //Simulación hidráulica (prolongada durante un periodo) para la configuración de arranque
            gx = run_single_period_simulation(patternStep, hydStep, nJ, nR, nT, nJ_Dem, nDM, nWLM, 
                nPM, nFM, nFM_avg, m, m_aug, wlm_positions, pm_positions, fm_positions, 
                fm_avg_positions, ind_J_Dem, nCtrl_01, nCtrl_23, nPI, nPU, t, 1, Controls_01, 
                Controls_23, &Controls_Links_Summary, ph, lang);


            //Almacenamos estado simulado de los elementos de control al final del periodo
            get_status(Controls_01, Controls_23, &Controls_Links_Summary, ph);
        
            //Residuos ponderados por la raíz cuadrada de los pesos
            sqrt_weighted_residuals = compute_sqrt_weighted_residuals(1, m_aug, z_t, gx, weights_vector_t, &c);

            printf("Valor inicial de la funcion objetivo: \n");
            SWSE = compute_SWSE_SE(sqrt_weighted_residuals, nDM, nWLM, nPM, nFM, nFM_avg, nWLM, lang);    
            
            while (it < max_it){

                printf("\033[1;35m==============\033[0m\n");
                printf("\033[1;35mITERACION %d \033[0m\n", it+1);
                printf("\033[1;35m==============\033[0m\n\n\n");


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
                        
                int break_while = 0;

                for (int attempt = 0; attempt < max_attempts; attempt ++){

                    printf("\033[1;34mINTENTO DE MEJORA (REGULARIZANDO): %d/%d \033[0m\n", attempt+1, max_attempts);

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


                    if (SWSE_prov < SWSE){  //Si mejoramos...
                        printf("\033[4;32mDIRECCION DE DESCENSO\033[0m \n\n");
            

                        //Consolidamos los valores (hasta ahora provisionales) en Current_Vars
                        update_current_variables_with_provisional_variables_multiperiod_SE(nJ_Dem, nT, 
                            &Current_Vars, Prov_Vars, ph);

                        //Calculamos tasa de cambio entre g(x_prov) (nueva) y g(x) (anterior)
                        conv_rate = compute_convergence_rate(1, m_aug, gx_prov, gx);
                        printf("Tasa de cambio (o de convergencia): %.6f \n\n", conv_rate);
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
                            printf("\033[4;33mMAXIMO DE ITERACIONES ALCANZADO. Pasamos a la EE del siguiente periodo\033[0m \n\n");
                            SENSEI_PAUSE();
                        } else if (convergence == 1){  //Si ha convergido, finalizamos ajuste y pasamos al siguiente periodo
                            printf("\033[4;32mHEMOS CONVERGIDO. Pasamos a la EE del siguiente periodo\033[0m\n\n");
                            break_while = 1;
                            SENSEI_PAUSE();
                            break;
                        }

                        //Como hemos mejorado, reducimos la regularización de J^t*W*J
                        printf("Reducimos lambda a la decima parte de su valor \n");
                        lambda = lambda/10;

                        SENSEI_PAUSE();
                        break;

                    } else {
                        printf("\033[4;33mNO HEMOS MEJORADO\033[0m \n");
                        if (gx_prov != NULL){
                            free(gx_prov);
                        }

                        if (attempt < max_attempts-1){
                            printf("Aumentamos lambda (x10) y PASAMOS AL SIGUIENTE INTENTO: \n\n");
                            lambda = 10*lambda;
                        } else {
                            printf("\033[4;33mHemos agotado los intentos. PASAMOS A LA EE del siguiente periodo\033[0m\n\n");
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

            get_status(Controls_01, Controls_23, &Controls_Links_Summary, ph);

            end_iteration_and_print_results(t, nJ, nR, nT, nDM, nPM, nFM, nFM_avg, num_unknowns, 
                m, m_aug, ind_J_Dem, pm_positions, fm_positions, fm_avg_positions, z_t, SWSE, weights_vector_t, demands_fitted_last_period,
                init_tank_levels_new_period, gx, estimated_values, posterior_CV, normalized_residuals, 
                sd_final_tank_level, sqrt_W_matrix, R_matrix, J, &c, &Controls_Links_Summary, ph, lang);

            if (period_to_add_emitter <= t){
                E += SWSE;
            }
            SENSEI_PAUSE();

        }

        printf("\033[4;34mSuma de SWSE a lo largo de los periodos donde consideras que hay fuga: "
            "E = %.4f \n\033[0m", E);
        printf("\033[4;34m======================================================================== \n\n\033[0m", E);
        SENSEI_PAUSE();


        // EN_close(ph);
        // EN_open(ph, "DOBLE_INICIO.inp", rpt_File, binary_File);

        Controls_Links_Summary.valve_status[0] = 2;  //Forzamos que la valvula empiece abierta (chapuza)

        for (int i = 0; i < nT; i++){
            init_tank_levels_new_period[i] = wlm_values[i][0];
        }


        for (int t = 0; t < nPeriods; t++){

            printf("\033[1;35m==================\033[0m \n");
            printf("\033[1;35m==================\033[0m \n");
            printf("\033[1;35mPERIODO t = %d \033[0m \n", t);
            printf("\033[1;35m==================\033[0m \n");
            printf("\033[1;35m==================\033[0m \n\n");

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


            prepare_ph_for_new_period(t, nJ, nR, nT, add_emitter, period_to_add_emitter, emitter_index, 
                emitter_coef, nCtrl_23, nR_pattern, patternStep, nJ_Dem, ind_J_Dem, 
                demands_fitted_last_period, init_tank_levels_new_period, min_tank_levels, 
                max_tank_levels, intermediate_File, rpt_File, binary_File, Reservoir_Patterns_Info, 
                &Controls_Links_Summary, Controls_23, ph, lang);



            // EN_saveinpfile(ph, "INICIO_EE.inp"); 

            //Cargamos como variables el nivel inicial de los tanques
            Current_Vars.init_tank_levels = get_initial_tank_levels(nJ, nR, nT, ph);
        
            //Cargamos como variables la demanda de arranque en los nodos de demanda
            Current_Vars.base_demands = get_base_demands_vector_from_inp(nJ_Dem, ind_J_Dem, ph);
        


            //Simulación hidráulica (prolongada durante un periodo) para la configuración de arranque
            gx = run_single_period_simulation(patternStep, hydStep, nJ, nR, nT, nJ_Dem, nDM, nWLM, nPM, 
                nFM, nFM_avg, m, m_aug, wlm_positions, pm_positions, fm_positions, fm_avg_positions, 
                ind_J_Dem, nCtrl_01, nCtrl_23, nPI, nPU, t, 1, Controls_01, Controls_23, 
                &Controls_Links_Summary, ph, lang);


            //Almacenamos estado simulado de los elementos de control al final del periodo
            get_status(Controls_01, Controls_23, &Controls_Links_Summary, ph);
        
            //Residuos ponderados por la raíz cuadrada de los pesos
            sqrt_weighted_residuals = compute_sqrt_weighted_residuals(1, m_aug, z_t, gx, weights_vector_t, &c);

            printf("Valor inicial de la funcion objetivo: \n");
            SWSE = compute_SWSE_SE(sqrt_weighted_residuals, nDM, nWLM, nPM, nFM, nFM_avg, nWLM, lang);
            
            
            while (it < max_it){

                printf("\033[1;35m==============\033[0m\n");
                printf("\033[1;35mITERACION %d \033[0m\n", it+1);
                printf("\033[1;35m==============\033[0m\n\n\n");


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


                int break_while = 0;

                for (int attempt = 0; attempt < max_attempts; attempt ++){

                    printf("\033[1;34mINTENTO DE MEJORA (REGULARIZANDO): %d/%d \033[0m\n", attempt+1, max_attempts);

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


                    if (SWSE_prov < SWSE){  //Si mejoramos...
                        printf("\033[4;32mDIRECCION DE DESCENSO\033[0m \n\n");
        
                        //Consolidamos los valores (hasta ahora provisionales) en Current_Vars
                        update_current_variables_with_provisional_variables_multiperiod_SE(nJ_Dem, nT, 
                            &Current_Vars, Prov_Vars, ph);

                        //Calculamos tasa de cambio entre g(x_prov) (nueva) y g(x) (anterior)
                        conv_rate = compute_convergence_rate(1, m_aug, gx_prov, gx);
                        printf("Tasa de cambio (o de convergencia): %.6f \n\n", conv_rate);
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
                            printf("\033[4;33mMAXIMO DE ITERACIONES ALCANZADO. Pasamos a la EE del siguiente periodo\033[0m \n\n");
                            SENSEI_PAUSE();
                        } else if (convergence == 1){  //Si ha convergido, finalizamos ajuste y pasamos al siguiente periodo
                            printf("\033[4;32mHEMOS CONVERGIDO. Pasamos a la EE del siguiente periodo\033[0m\n\n");
                            break_while = 1;
                            SENSEI_PAUSE();
                            break;
                        }

                        //Como hemos mejorado, reducimos la regularización de J^t*W*J
                        printf("Reducimos lambda a la decima parte de su valor \n");
                        lambda = lambda/10;

                        SENSEI_PAUSE();
                        break;

                    } else {  //Si no mejoramos...
                        printf("\033[4;33mNO HEMOS MEJORADO\033[0m \n");
                        if (gx_prov != NULL){
                            free(gx_prov);
                        }

                        if (attempt < max_attempts-1){
                            printf("Aumentamos lambda (x10) y PASAMOS AL SIGUIENTE INTENTO: \n\n");
                            lambda = 10*lambda;
                        } else {
                            printf("\033[4;33mHemos agotado los intentos. PASAMOS A LA EE del siguiente periodo\033[0m\n\n");
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

            get_status(Controls_01, Controls_23, &Controls_Links_Summary, ph);

            end_iteration_and_print_results(t, nJ, nR, nT, nDM, nPM, nFM, nFM_avg, num_unknowns, 
                m, m_aug, ind_J_Dem, pm_positions, fm_positions, fm_avg_positions, z_t, SWSE, weights_vector_t, demands_fitted_last_period,
                init_tank_levels_new_period, gx, estimated_values, posterior_CV, normalized_residuals, 
                sd_final_tank_level, sqrt_W_matrix, R_matrix, J, &c, &Controls_Links_Summary, ph, lang);
            
            SENSEI_PAUSE();

            if (period_to_add_emitter <= t){
                E_aug += SWSE;
            }

        }

        printf("\033[4;34mSuma de SWSE durante los periodos donde consideramos fuga: "
            "E_aug = %.4f \n\033[0m", E_aug);
        printf("\033[4;34m========================================================== \n\n\033[0m", E_aug);

        SENSEI_PAUSE();

        double derivada = (E_aug - E)/delta;

        printf("\033[1;34mDERIVADA: %.4f \033[0m\n", derivada);
        printf("\033[1;34m==================\033[0m\n\n");


        emitter_coef = emitter_coef - alpha * derivada;

        printf("\033[4;34mNUEVO COEFICIENTE EMISOR: %.8f \033[0m\n", emitter_coef);
        printf("\033[0;34m===============================\033[0m\n\n\n");

        SENSEI_PAUSE();

    }

    SENSEI_PAUSE();
    SENSEI_PAUSE();
    SENSEI_PAUSE();
    SENSEI_PAUSE();

    EN_close(ph);

    printf("Eliminando archivos intermedios... \n\n");
    if (remove(intermediate_File) == 0) {
        printf("%s eliminado\n\n", intermediate_File);
    } else {
        printf("Error al eliminar %s \n", intermediate_File);
    }
    if (remove(rpt_File) == 0) {
        printf("%s eliminado\n", rpt_File);
    } else {
        printf("Error al eliminar %s \n", rpt_File);
    }
    // if (remove(binary_File) == 0) {
    //     printf("%s eliminado\n", binary_File);
    // } else {
    //     printf("Error al eliminar %s \n", binary_File);
    // }

    printf("Eliminando proyecto...\n");
    EN_deleteproject(ph);  // Eliminar el proyecto
    printf("Proyecto eliminado correctamente.\n");

    SENSEI_PAUSE();

    return 0;

}
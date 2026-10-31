#include "functions_SENSEI.h"  
#include "messages_SENSEI.h"

// ESTIMACIÓN DE ESTADO INSTANTÁNEA

int main() {

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // salida en UTF-8
        SetConsoleCP(CP_UTF8);       // entrada en UTF-8
        system("chcp 65001 > nul");  // asegura UTF-8 en consola
    #endif

    /************************************************************
    ************************************************************
    * CONFIGURACIÓN de la EE según "config_snapshot_SE.txt"
    ************************************************************
    ************************************************************/

    const char* config_File = "config_snapshot_SE.txt";

    //Hiperparámetros del algoritmo
    double epsilon, tol;
    int max_it, lang;

     char* base_Path = NULL; //Subcarpeta
    char* info_File = NULL; //Modelo hidráulico (.inp)
    //Archivos (.csv) de medidas / pseudomedidas
    char *dm_File = NULL, *wlm_File = NULL, *pm_File = NULL, *fm_File = NULL, *fm_avg_File = NULL;

    char* emitter_ID = NULL; //ID de emisor (por si queremos optimizar ese parámetro en un nodo dado)

    //Lectura de "config_snapshot_SE.txt" e inicialización de variables
    read_config_file_snapshot_SE(config_File, &base_Path, &info_File, &dm_File, 
        &wlm_File, &pm_File, &fm_File, &epsilon, &max_it, &tol, &emitter_ID, &lang);


    printf("%s", messages[MSG_WELCOME_SNAPSHOT_SE][lang]);  // Mensaje de bienvenida a la terminal

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


    //PARÁMETROS DEL ALGORITMO
    printf("\n%s", messages[MSG_HEADER_ALGO_PARAMS][lang]);
    printf(messages[MSG_LABEL_EPSILON][lang], epsilon);
    printf(messages[MSG_LABEL_MAX_IT_SIMPLE][lang], max_it);
    printf(messages[MSG_LABEL_TOL][lang], tol);

    
    //INFORMACIÓN EXTRA RESPECTO A FUGAS
    int add_emitter = 0, emitter_index;
    printf("%s", messages[MSG_LEAK_EXTRA_INFO_HEADER][lang]);
    if (emitter_ID == NULL){
        printf("%s", messages[MSG_SE_SNAP_NO_NODE_SPECIFIED][lang]);
    } else {    
        printf(messages[MSG_SE_SNAP_SEARCH_EMITTER][lang], emitter_ID);

        add_emitter = 1; //Activamos variable (útil para alguna función)        
    }


    //Verificar que dm_File ha sido declarado
    if (dm_File == NULL){
        printf("%s", messages[MSG_ERR_MISSING_DM_SENSORS][lang]);
        SENSEI_PAUSE();
    } 

    //Control sobre wlm_File
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

    Variables_Snapshot_SE Current_Vars; //Estructura para almacenar el valor de las variables de estado en cada iteración
    Variables_Snapshot_SE Prov_Vars; //Estructura para almacenar el valor provisional de las variables de estado tras resolver ec. Gauss-Newton

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

    int T = 1;  //Establecemos un único instante (estimación de estado INSTANTÁNEA)
    EN_settimeparam(ph, EN_DURATION, 0);  //Establecemos duración en 0 (para no prolongar la simulación en el tiempo)


    const char* suffix = "_instant.inp";
    size_t base_len = strlen(info_File) - 4; 
    size_t total_len = base_len + strlen(suffix) + 1; 
    char* intermediate_File = (char*)malloc(total_len); //nombre del .inp para hacer simulaciones y actualizar valores
    snprintf(intermediate_File, total_len, "%.*s%s", (int)base_len, info_File, suffix);

    EN_saveinpfile(ph, intermediate_File); //Copia de la topología de la red, sin patrones, para 1 periodo

    if (add_emitter){ //Índice del nodo en el que buscamos el emisor (corresponde a emitter_ID)
        error = EN_getnodeindex(ph, emitter_ID, &emitter_index);
        if (error){
            char msg[256];
            EN_geterror(error, msg, 255);
            printf(messages[MSG_ERR_EMITTER_ID][lang], emitter_ID, msg, config_File);
            SENSEI_PAUSE();
            exit(EXIT_FAILURE);
        }
    }

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

    //Lectura de niveles mínimos y máximos de los tanques 
    double* min_tank_levels = (double*)malloc(nJ*sizeof(double));
    double* max_tank_levels = (double*)malloc(nJ*sizeof(double));

    get_min_max_tank_levels(nJ, nR, nT, &min_tank_levels, &max_tank_levels, ph); 
    
    // for (int i = 0; i < nT; i++){
    //     EN_getnodeid(ph, nJ+nR+i+1, element_id);
    //     printf("%s) Nivel min: %.2f | Nivel max: %.2f \n", element_id, min_tank_levels[i], max_tank_levels[i]);
    // }
    // printf("\n\n");

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
    define_initialize_vectors_matrices_for_sensors_info(nT, T, &wlm_logic, &wlm_values, &CV_wlm);

    //Para demanda
    int nDM = nJ_Dem; //El número de medidores de demanda debe coincidir con el número de nodos de demanda
    int* dm_logic = NULL;
    double** dm_values = NULL, **CV_dm = NULL;
    define_initialize_vectors_matrices_for_sensors_info(nDM, T, &dm_logic, &dm_values, &CV_dm);

    //Para medidores de presión    
    int* pm_logic = NULL; //Vector lógico para saber si hay medida o no en cada elemento
    double** pm_values = NULL, **CV_pm = NULL; 
    define_initialize_vectors_matrices_for_sensors_info(nJ, T, &pm_logic, &pm_values, &CV_pm);

    //Para medidores de flujo instantáneos 
    int* fm_logic = NULL; 
    double** fm_values = NULL, **CV_fm = NULL; 
    define_initialize_vectors_matrices_for_sensors_info(nL, T, &fm_logic, &fm_values, &CV_fm);

    //Para medidores de flujo promediados
    int* fm_avg_logic = NULL; 
    double** fm_avg_values = NULL, **CV_fm_avg = NULL; 
    define_initialize_vectors_matrices_for_sensors_info(nL, T, &fm_avg_logic, &fm_avg_values, &CV_fm_avg);


    int nWLM = store_wlm_info(T, nR, nT, nJ, wlm_File, wlm_logic, wlm_values, CV_wlm, ph, lang);
    if (nWLM != nT) { //Controlamos que haya tantas medidas/pseudomedidas de nivel como tanques
        printf(messages[MSG_ERR_WLM_COUNT_MISMATCH][lang], nT, nWLM, wlm_File);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }
    store_dm_info(T, nDM, nJ_Dem, ind_J_Dem, dm_File, dm_values, CV_dm, ph, lang);
    //La condición de nDM = nJ_Dem se comprueba implicitamente en store_dm_info 
    int nPM = store_pm_info(T, nJ, pm_File, pm_logic, pm_values, CV_pm, ph, lang);
    int nFM = store_fm_info(T, nL, fm_File, fm_logic, fm_values, CV_fm, 0, ph, lang);


    //Almacenamos posiciones útiles para elementos con medida
    int* wlm_positions = get_sensor_positions_vector(nWLM, nT, wlm_logic); //Vector con la posición de los elementos en los que hay sensor de nivel (nJ+nR+1-based)
    int* pm_positions = get_sensor_positions_vector(nPM, nJ, pm_logic); //Vector con la posición de los elementos en los que hay sensor de presión (1-based)
    int* fm_positions = get_sensor_positions_vector(nFM, nL, fm_logic); //Vector con la posición de los elementos en los que hay sensor de flujo instantáneo (1-based)


    if (nPM > 0){
        printf(messages[MSG_PM_ROWS_ID][lang], nPM);
        for (int i = 0; i < nPM; i++){
            EN_getnodeid(ph, pm_positions[i], element_id);
            printf("%s ", element_id);
        }
        printf("\n\n");
    }
    
    if (nFM > 0){
        printf(messages[MSG_FM_ROWS_ID][lang], nFM);
        for (int i = 0; i < nFM; i++){
            EN_getlinkid(ph, fm_positions[i], element_id);
            printf("%s ", element_id);
        }
        printf("\n\n");
    }
    printf("\n");


    printf("%s", messages[MSG_MEASUREMENTS_READ_OK][lang]);

    SENSEI_PAUSE();


    /*************************************************************************
    *************************************************************************
    * VECTOR de MEDIDAS / PSEUDOMEDIDAS y PESOS
    *************************************************************************
    *************************************************************************/

    int m = nDM + nWLM + nPM + nFM;  //Número de medidas/pseudomedidas recodigas
    printf(messages[MSG_MEASURES][lang], m);
    int num_unknowns = nJ_Dem + nT;  //Número de variables de estado 

    //Definimos vectores de la dimensión adecuada para la propagación de
    //incertidumbre y para guardar los resultados de las estimaciones
    double* estimated_values = (double*) malloc(m * sizeof(double)); //Vector para guardar los valores ajustados
    double* posterior_CV = (double*) malloc(m * sizeof(double));  //Vector de CVs de las estimaciones (var. de estado y resto de variables)
    double* normalized_residuals = (double*) malloc(m * sizeof(double)); //Vector de CVs de residuos normalizados
    
    //Matriz de tamaño (m,1) con el CV de cada medida/pseudomedida (fila) 
    double** CV_matrix = get_CV_matrix(T, m, nDM, nWLM, nPM, nFM, 0, wlm_positions,
        pm_positions, fm_positions, NULL, CV_dm, CV_wlm, CV_pm, CV_fm, CV_fm_avg);

    // printf("Matriz de CV de medidas/pseudomedidas: \n\n");
    // for (int i = 0; i < m; i++){
    //     for (int t = 0; t < T; t++){
    //         printf("%.3f ", CV_matrix[i][t]);
    //     }
    //     printf("\n");
    // }
    // printf("\n\n");

    // system("pause");

    //Vector de tamaño m con los valores medidos/pseudomedidos ordenados
    double* z = get_measurements_vector(T, m, nDM, nWLM, nPM, nFM, 0, wlm_positions,
        pm_positions, fm_positions, NULL, dm_values, wlm_values, pm_values, fm_values, fm_avg_values);


    // printf("Medidas / pseudomedidas en todos los periodos: \n\n");
    // for (int i = 0; i < m; i++){
    //     for (int t = 0; t < T; t++){
    //         printf("%.4f ", z[i+m*t]);
    //     }
    //     printf("\n");
    // }
    // printf("\n\n");

    // system("pause");

    //Vector de tamaño m con los pesos de las medidas / pseudomedidas
    double* weights_vector = compute_weights_vector(T, m, z, CV_matrix);

    // printf("Pesos en todos los periodos: \n\n");
    // for (int i = 0; i < m; i++){
    //     for (int t = 0; t < T; t++){
    //         printf("%.4f ", weights_vector[t*m+i]);
    //     }
    //     printf("\n");
    // }
    // printf("\n\n");

    // system("pause");

    //Inicialización de cholmod
    cholmod_common c;
    cholmod_start(&c); 

    //Matrices diagonales cholmod sparse con distintas opciones:
    //0) Diagonal con pesos 1) Diagonal con sqrt(pesos) 2) Diagonal con inversa de pesos
    cholmod_sparse* W_matrix = diagonal_matrix_from_vector(T, m, weights_vector, 0, &c);
    cholmod_sparse* sqrt_W_matrix = diagonal_matrix_from_vector(T, m, weights_vector, 1, &c);
    cholmod_sparse* R_matrix = diagonal_matrix_from_vector(T, m, weights_vector, 2, &c);

    printf("%s", messages[MSG_GROUPING_CVS_DONE][lang]);

    SENSEI_PAUSE();

    
    /*************************************************************************
    *************************************************************************
    * PREPARACIÓN PARA EL ALGORITMO DE ESTIMACIÓN DE ESTADO
    *************************************************************************
    *************************************************************************/

    // Establecemos todos los patrones de longitud 1 y con el (único) factor
    // multiplicador como 1 (para evitar problemas) 
    int patternCount; 

    double* new_pattern = (double*)malloc(1*sizeof(double));
    new_pattern[0] = 1.0;
    
    EN_getcount(ph, EN_PATCOUNT, &patternCount);
    if (patternCount > 0){
        for (int i = 1; i <= patternCount; i++) {
            EN_setpattern(ph, i, new_pattern, 1);
        }
    }


    EN_saveinpfile(ph, intermediate_File); //Copia de la topología de la red, sin patrones, para 1 instante
    EN_closeH(ph);  //Cerramos motor hidráulico
    EN_close(ph);  //Cerramos el ph asociado a info_File
    EN_open(ph, intermediate_File, rpt_File, binary_File); //Cargamos intermediate_File al ph


    //Definimos las variables provisionales (con vectores de la dimensión requerida)
    Prov_Vars.base_demands = (double*) malloc(nJ_Dem*sizeof(double));
    Prov_Vars.init_tank_levels = (double*)malloc(nT*sizeof(double));

    cholmod_sparse *J = NULL;  //Matriz Jacobiana

    double *gx, *gx_prov = NULL; //Valores a comparar con las medidas (tras simulación)

    cholmod_dense *sqrt_weighted_residuals = NULL; //Residuos ponderados por la raíz cuadrada de los pesos
    cholmod_dense *delta_x = NULL; //Incremento calculado para las variables de estado

    int nan_count; //Contador de elementos NaN (en matrices que calculemos) 
    int inf_count; //Contador de elementos infinitos (en matrices que calculemos)

    double conv_rate;  //Tasa de cambio entre aproximaciones (valor que compararemos con la tolerancia)

    double emitter_coef;  //Coeficiente emisor (que ajustaremos en caso de haberlo configurado para el nodo con ID emitter_ID)

    printf("%s", messages[MSG_SE_PRECONFIG_DONE][lang]);

    printf("%s", messages[MSG_SE_ALGO_START][lang]);
    SENSEI_PAUSE();

    //print_memory_usage();
    
    int convergence = 0; //Variable para indicar que hay que hemos convergido (y forzar un break DESPUÉS de actualizar valores)

    int it = 0; //Contador de iteraciones
    
    double SWSE, SWSE_prov;   //Función objetivo (Sum of Weighted Squared Errors)

    //Preparamos el ph con valores de arranque (semilla)
    prepare_ph_snapshot_SE(nJ, nR, nT, nJ_Dem, ind_J_Dem, 
        dm_values, wlm_values, min_tank_levels, max_tank_levels, ph);

    //Guardamos el inp de arranque como "INICIO_EE.inp"
    // EN_saveinpfile(ph, "INICIO_EE.inp"); 

    //Cargamos como variables el nivel de arranque de los tanques
    Current_Vars.init_tank_levels = get_initial_tank_levels(nJ, nR, nT, ph);
    // printf("\033[1;36mNiveles iniciales de los tanques: \033[0m");
    // for (int i = 0; i < nT; i++){
    //     printf("\033[1;36m%.4f  \033[0m", Current_Vars.init_tank_levels[i]);
    // }
    // printf("\n\n");
    
    //Cargamos como variables la demanda de arranque en los nodos de demanda
    Current_Vars.base_demands = get_base_demands_vector_from_inp(nJ_Dem, ind_J_Dem, ph);
    // printf("Demandas (medidas / pseudomedidas) de los nodos de demanda: ");
    // for (int i = 0; i < nJ_Dem; i++){
    //     printf("%.4f  ", Current_Vars.base_demands[i]);
    // }
    // printf("\n\n");


    if (add_emitter){
        //Cargamos como variable el coef. emisor de arranque del nodo seleccionado
        EN_getnodevalue(ph, emitter_index, EN_EMITTER, &emitter_coef);
        Current_Vars.emitter_coef = emitter_coef;
    }

    
    //Simulación hidráulica (instantánea) para la configuración de arranque
    gx = run_snapshot_simulation(nJ, nR, nT, nJ_Dem, nDM, nWLM, nPM, nFM, m, 
        wlm_positions, pm_positions, fm_positions, ind_J_Dem, ph);

    //Residuos ponderados por la raiz cuadrada de los pesos
    sqrt_weighted_residuals = compute_sqrt_weighted_residuals(T, m, z, gx, weights_vector, &c);

    printf("%s", messages[MSG_OBJFUNC_INITIAL_HEADER][lang]);
    SWSE = compute_SWSE_SE(sqrt_weighted_residuals, nDM, nWLM, nPM, nFM, 0, 0, lang);
    
    SENSEI_PAUSE();

    while (it < max_it || convergence != 0){

        printf(messages[MSG_ITERATION_HEADER][lang], it + 1);

        //Calculamos matriz jacobiana
        if (J != NULL) {
            cholmod_free_sparse(&J, &c);
            J = NULL;  // buena práctica para evitar punteros colgantes
        }
        J = compute_Jacobian_snapshot_SE(num_unknowns, add_emitter, emitter_index, epsilon, nDM, 
            nWLM, nPM, nFM, wlm_positions, pm_positions, fm_positions, nJ, nR, nT, m, nJ_Dem, 
            ind_J_Dem, min_tank_levels, max_tank_levels, Current_Vars, gx, &c, ph);
                
        // cholmod_print_sparse(J, "MATRIZ JACOBIANA", &c);  
        // check_nan_inf_matrix(&nan_count, &inf_count, J); //Comprobación de NaN y +-inf en J
        // printf("En la matriz JACOBIANA hay %d NaN y %d +-inf \n\n", nan_count, inf_count);

        // save_dense_matrix_to_txt(J, "J.txt", &c);

        // system("pause");

        if (delta_x != NULL){
            cholmod_free_dense(&delta_x, &c);
            delta_x = NULL;
        }
        //Resolvemos (J^t*W*J)*Delta_x = (J^t*W)*(z-g(x))
        delta_x = solve_Gauss_Newton_equation(num_unknowns+add_emitter, J, sqrt_W_matrix, 
            sqrt_weighted_residuals, &c, lang);

        //Trasladamos el incremento Delta_x a la estructura de variables provisionales
        compute_provisional_variables_snapshot_SE(delta_x, nJ_Dem, nT, add_emitter, ind_J_Dem, 
            min_tank_levels, max_tank_levels, Current_Vars, &Prov_Vars, ph, lang);

        //Imponemos los valores provisionales (los modificamos en el ph)
        edit_ph_provisional_variables_snapshot_SE(nJ_Dem, nT, nJ, nR, add_emitter, emitter_index, 
            ind_J_Dem, Prov_Vars, ph);

        //Guardar .inp provisional 
        // EN_saveinpfile(ph, "PROVISIONAL.inp");

        //Simulamos para las variables provisionales
        gx_prov = run_snapshot_simulation(nJ, nR, nT, nJ_Dem, nDM, nWLM, nPM, nFM, 
            m, wlm_positions, pm_positions, fm_positions, ind_J_Dem, ph);

        //Residuos ponderados por la raíz cuadrada de los pesos
        sqrt_weighted_residuals = compute_sqrt_weighted_residuals(T, m, z, gx_prov, weights_vector, &c);

        //Valor PROVISIONAL de la función objetivo
        SWSE_prov = compute_SWSE_SE(sqrt_weighted_residuals, nDM, nWLM, nPM, nFM, 0, 0, lang);


        if (SWSE_prov < SWSE){ //Si mejoramos...
            printf("%s", messages[MSG_DESCENT_DIRECTION_UPDATE][lang]);
            
            //Consolidamos los valores (hasta ahora provisionales) en Current_Vars
            update_current_variables_with_provisional_variables_snapshot_SE(nJ_Dem, nT, 
                add_emitter, &Current_Vars, Prov_Vars, ph);

            //Calculamos tasa de cambio entre g(x_prov) (nueva) y g(x) (anterior)
            conv_rate = compute_convergence_rate(T, m, gx_prov, gx);
            printf(messages[MSG_CONV_RATE_SIMPLE][lang], conv_rate);
            if (conv_rate < tol){ //Comparación con la tolerancia
                convergence = 1;
            }
            
            //Actualizamos g(x) como g(x_prov)
            for (int i = 0; i < m; i++){
                gx[i] = gx_prov[i];
            }

            //Actualizamos el valor de la función objetivo (SWSE)
            SWSE = SWSE_prov;
            
            //Avanzamos 1 iteración
            it += 1;

            if (gx_prov != NULL){
                free(gx_prov);
            }

            if (it == max_it){ //Si agotamos las iteraciones, finalizamos ajuste
                printf("%s", messages[MSG_MAX_IT_REACHED][lang]);
                SENSEI_PAUSE();
                break;
            } else if (convergence == 1){ //Si ha convergido, finalizamos ajuste
                printf("%s", messages[MSG_MAX_IT_REACHED][lang]);
                SENSEI_PAUSE();
                break;
            }
            SENSEI_PAUSE();
        } else {
            printf("%s", messages[MSG_NO_IMPROVEMENT][lang]);
            SENSEI_PAUSE();
            break;
        }


    }


    if (add_emitter){
        printf(messages[MSG_EMITTER_COEF_ADJUSTED][lang], emitter_ID, Current_Vars.emitter_coef);
        double pressure, leaked_flow;
        EN_getnodevalue(ph, emitter_index, EN_PRESSURE, &pressure);
        leaked_flow = Current_Vars.emitter_coef*pow(pressure,0.5);
        printf(messages[MSG_EMITTER_LEAK_FLOW][lang], emitter_ID, leaked_flow);
        SENSEI_PAUSE();
    }


    //Almacenar los valores ajustados
    for (int i = 0; i < m; i++){
        estimated_values[i] = gx[i];
    }
    //Propagación incertidumbre y cálculo de residuos estudentizados
    propagate_uncertainty_compute_normalized_residuals_snapshot_SE(num_unknowns+add_emitter, nFM, m, 
        gx, z, posterior_CV, normalized_residuals, weights_vector, sqrt_W_matrix, R_matrix, J, &c, lang);

    
    //Guardamos el .csv con los resultados de la EE con el sufijo "_results_YYYYMMDD_hhmmss.csv"
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S.csv", t);

    total_len = base_len + strlen("_results_") + strlen(timestamp) + 1;

    char* results_File = (char*)malloc(total_len); // nombre del .csv 
    snprintf(results_File, total_len, "%.*s_results_%s", (int)base_len, info_File, timestamp);

    save_snapshot_SE_results_to_csv(results_File, add_emitter, emitter_index, nJ, nR, nDM, nWLM, 
        nPM, nFM, m, ind_J_Dem, wlm_positions, pm_positions, fm_positions, z, estimated_values, 
        posterior_CV, normalized_residuals, CV_matrix, Current_Vars, ph, lang);

    printf(messages[MSG_FILE_SAVED_SE_RESULTS][lang], results_File);    

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
    printf("\n\n");

    SENSEI_PAUSE();

    return 0;

}
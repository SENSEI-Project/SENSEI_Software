#include "functions_SENSEI.h"  

// GENERACIÓN DE MEDIDAS SINTÉTICAS PARA UN INSTANTE DE TIEMPO

int main() {

    //Ahora mismo, está construído para generar medidas en un único instante de
    //tiempo sobre Net1.inp tras haber añadido a la red un coef. emisor de 5.0
    //en el nodo "J21". Se "miden" las demandas, el nivel del tanque, las
    //presiones en todos los nodos y el flujo en P110 y P112. Se añade ruido de
    //acuerdo al CV especificado 

    char* path = "Examples_Net1";  //Ruta (subcarpeta)
    char* base_info_File = "Net1.inp"; //Archivo .inp sobre el que generar las medidas

    char info_File[256];  
    snprintf(info_File, sizeof(info_File), "%s/%s", path, base_info_File);


    //Lista de nodos en los que simulamos tener PM (piezómetro)
    const char* pm_ID_list[] = {"J11", "J12", "J13", "J21", "J22", "J23", "J31", "J32"};
    //Lista de enlaces en los que simulamos tener FM (medidor instantáneo de flujo)
    const char* fm_ID_list[] = {"P110", "P112"};


    //COEIFICENTES DE VARIACIÓN DE LAS MEDIDAS (el mismo para cada tipo de medida)
    double cv_DM = 0.05;  //CV medidas demanda
    double cv_WLM = 0.15; //CV medidas nivel
    double cv_PM = 0.01; //CV medidas presión
    double cv_FM = 0.01; //CV medidas flujo instantáneo


    srand(100);   //SEMILLA ALEATORIA

    char* rpt_File = "report.rpt";  //nombre del report (necesario para EN_open(ph,...))
    char* binary_File = "salida.bin"; //nombre de la salida binaria (necesario para EN_open(ph,...))
    
    char element_id[60];  //Variable para acceder a los ID de los elementos de la red
    int element_index;  //Variable para acceder al índice de los elementos de la red 

    EN_Project ph;  // Project handler
    EN_createproject(&ph);  // Crear nuevo proyecto


    int error = EN_open(ph, info_File, rpt_File, binary_File);
    if (error){
        char msg[256];
        EN_geterror(error, msg, 255);
        printf("\033[0;31mERROR al abrir %s: (%s) \033[0m\n\n", info_File, msg);
        SENSEI_PAUSE();
        exit(EXIT_FAILURE);
    }

   
    //Conteo de los elementos de la red 
    int nN, nL, nJ, nR, nT, nPI, nPU, nV;
    count_elements_in_network(&nN, &nL, &nJ, &nR, &nT, &nPI, &nPU, &nV, ph);
    printf("Hay %d NODOS y %d LINKS. \n", nN, nL);
    printf("Hay %d JUNCTIONS, %d EMBALSES y %d TANQUES \n", nJ, nR, nT);
    printf("Hay %d TUBERIAS, %d BOMBAS y %d VALVULAS \n\n", nPI, nPU, nV);


    int nJ_Trans; //Número de jucnctions de tránsito (demanda base nula)
    int nJ_Dem; //Número de junctions de demanda (que no son de tránsito)
    int *ind_J_Trans = NULL, *ind_J_Dem = NULL;

    //Clasifica los junctions según tránsito o demanda y almacena sus índices
    classify_transit_demand_junctions(nJ, &nJ_Trans, &nJ_Dem, &ind_J_Trans, &ind_J_Dem, ph);
    printf("En total, hay %d junctions de TRANSITO y %d de DEMANDA \n\n", nJ_Trans, nJ_Dem);


    //Lectura de niveles mínimos y máximos de los tanques 
    double* min_tank_levels = (double*)malloc(nJ*sizeof(double));
    double* max_tank_levels = (double*)malloc(nJ*sizeof(double));

    get_min_max_tank_levels(nJ, nR, nT, &min_tank_levels, &max_tank_levels, ph); 


    printf("\033[0;32mCONTEO DE LOS ELEMENTOS DE LA RED Y PARAMETROS DE TIEMPO REALIZADO \033[0m\n");
    printf("\033[0;32m------------------------------------------------------------------ \033[0m\n");

    SENSEI_PAUSE();


    int nDM = nJ_Dem;  //Tantos datos para demanda como junctions de demanda haya
    int nWLM = nT;  //Tanto datos para nivel de agua como tanques haya
    int nPM = sizeof(pm_ID_list) / sizeof(pm_ID_list[0]); //Los PM que hayamos definido al comienzo del archivo 
    int nFM = sizeof(fm_ID_list) / sizeof(fm_ID_list[0]); //Los FM que hayamos definido al comienzo del archivo 
    
    int m = nDM+nWLM+nPM+nFM;

    printf("Vamos a almacenar %d medidas (incluye tambien pseudomedidas) \n\n");


    int* wlm_positions = (int*)malloc(nWLM*sizeof(int));
    for (int i = 0; i < nWLM; i++){
        wlm_positions[i] = i+1; //Ponemos el índice de los tanques por orden (1-based): 1, 2, ..., nWLM
    }

    int* pm_positions = (int*)malloc(nPM*sizeof(int));
    for (int i = 0; i < nPM; i++){
        EN_getnodeindex(ph, pm_ID_list[i], &element_index);
        pm_positions[i] = element_index;
    }

    int* fm_positions = (int*)malloc(nFM*sizeof(int));
    for (int i = 0; i < nFM; i++){
        EN_getlinkindex(ph, fm_ID_list[i], &element_index);
        fm_positions[i] = element_index;
    }


    // Ordenamos los indices de los nodos con PM y de los nodos con FM
    qsort(pm_positions, nPM, sizeof(int), compare_ints);
    qsort(fm_positions, nFM, sizeof(int), compare_ints);

    // print para comprobar que se han ordenado bien
    for (int i = 0; i < nPM; i++){
        printf("%d ", pm_positions[i]);
    }
    printf("\n\n");
    for (int i = 0; i < nFM; i++){
        printf("%d ", fm_positions[i]);
    }
    printf("\n\n");

    SENSEI_PAUSE();


    /***********************************************************
    ************************************************************
    ****** SIMULACIÓN
    ************************************************************
    ************************************************************/

    
    //Añadimos COEFICIENTE EMISOR de 5.0 en "J21" 
    EN_getnodeindex(ph, "J21", &element_index);
    EN_setnodevalue(ph, element_index, EN_EMITTER, 5.0);
    //También podemos "generar" otro problema (rugosidad diferente; lo que sea)
    //o no generar ninguno y dejarlo todo "normal"

    //Simulamos con las condiciones dadas en info_File (+ posible "problema" añadido)
    double* gx = run_snapshot_simulation(nJ, nR, nT, nJ_Dem, nDM, nWLM, nPM, nFM, m, 
        wlm_positions, pm_positions, fm_positions, ind_J_Dem, ph);


    //Cambiar a 0 los flujos "muy pequeños" de simulación (son 0 numéricos)
    for (int i = m-nFM; i < m; i++){
        if (fabs(gx[i]) < 0.05){ 
            gx[i] = 0.0;
        }
    }


    /*****************************************************************
    ** PRINT DE MEDIDAS (TAL CUAL, SIMULADAS Y SIN AÑÁDIR RUIDO)
    ******************************************************************/

    printf("\033[4;32mMEDIDAS BIEN GENERADAS\033[0m\n\n");

    SENSEI_PAUSE();

    for (int i = 0; i < m; i++){
        printf("%.4f \n", gx[i]);
    }
    printf("\n\n\n");

    SENSEI_PAUSE();


    /*****************************************************************
    ******************************************************************
    ** AÑADIMOS RUIDO generado mediante N(0,sd^2), donde sd = CV*|z|
    ******************************************************************
    ******************************************************************/

    double* gx_noise = (double*)malloc(m*sizeof(double));

    //RUIDO PARA LAS DEMANDAS
    for (int i = 0; i < nDM; i++){  
        double medida = gx[i];
        double ruido = 0;
        if (medida != 0){  //Añadimos ruido solamente si la medida no es nula
            ruido = normal_sample(medida*cv_DM);
        }
        gx_noise[i] = medida + ruido;
    }

    //RUIDO PARA LOS NIVELES
    for (int i = nDM; i < nDM+nWLM; i++){  
        double medida = gx[i];
        double ruido = 0;
        if (medida != 0){  //Añadimos ruido solamente si la medida no es nula
            ruido = normal_sample(medida*cv_WLM);
        }
        gx_noise[i] = medida + ruido;
        if (gx_noise[i] < min_tank_levels[i-nDM]){
            gx_noise[i] = min_tank_levels[i-nDM]; //Acotar al nivel mínimo si al añadir ruido queda por debajo de cota
        } else if (gx_noise[i] > max_tank_levels[i-nDM]){
            gx_noise[i] = max_tank_levels[i-nDM]; //Acotar al nivel máximo si al añadir ruido queda por encima de cota
        }
    }

    //RUIDO PARA LAS PRESIONES
    for (int i = nDM+nWLM; i < nDM+nWLM+nPM; i++){  
        double medida = gx[i];
        double ruido = 0;
        if (medida != 0){  //Añadimos ruido solamente si la medida no es nula
            ruido = normal_sample(medida*cv_PM);
        }
        gx_noise[i] = medida + ruido;
    }

    //RUIDO PARA LOS FLUJOS INSTANTÁNEOS
    for (int i = nDM+nWLM+nPM; i < nDM+nWLM+nPM+nFM; i++){ 
        double medida = gx[i];
        double ruido = 0;
        if (medida != 0){  //Añadimos ruido solamente si la medida no es nula
            ruido = normal_sample(medida*cv_FM);
        }
        gx_noise[i] = medida + ruido;
    }


    /*****************************************************************
    ** PRINT DE MEDIDAS SINTÉTICAS (ya con el RUIDO añadido)
    ******************************************************************/

    printf("\033[4;32mRUIDO SUMADO A LAS MEDIDAS\033[0m\n\n");    

    SENSEI_PAUSE();

    for (int i = 0; i < m; i++){
        printf("%.4f \n", gx_noise[i]);
    }
    printf("\n\n\n");

    SENSEI_PAUSE();


    /*****************************************************************
    ** GUARDAMOS MEDIDAS SINTÉTICAS EN ARCHIVOS 
    ******************************************************************/

    //Guardamos los .csv con los resultados de la EE con la info de la hora
    //(por ejemplo, "PATH/dm_synthetic_YYYYMMDD_hhmmss_nombreinfoFile.csv")
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "synthetic_%Y%m%d_%H%M%S_", t);
    
    //DEMANDA
    const char *prefix = "dm_";
    const char *suffix = ".csv";
    size_t base_len = strlen(base_info_File) - 4;  // quitar ".inp"
    size_t total_len = strlen(path) + 1 + strlen(prefix) + strlen(timestamp) + base_len + strlen(suffix) + 1;
    char *dm_File = malloc(total_len);
    snprintf(dm_File, total_len, "%s/%s%s%.*s%s", path, prefix, timestamp, (int)base_len, base_info_File, suffix);


    //NIVEL DE AGUA
    prefix = "wlm_";
    total_len = strlen(path) + 1 + strlen(prefix) + strlen(timestamp) + base_len + strlen(suffix) + 1;
    char *wlm_File = malloc(total_len);
    snprintf(wlm_File, total_len, "%s/%s%s%.*s%s", path, prefix, timestamp, (int)base_len, base_info_File, suffix);


    //PRESIÓN
    prefix = "pm_";
    total_len = strlen(path) + 1 + strlen(prefix) + strlen(timestamp) + base_len + strlen(suffix) + 1;
    char *pm_File = malloc(total_len);
    snprintf(pm_File, total_len, "%s/%s%s%.*s%s", path, prefix, timestamp, (int)base_len, base_info_File, suffix);


    //FLUJO INSTANTÁNEO
    prefix = "fm_";
    total_len = strlen(path) + 1 + strlen(prefix) + strlen(timestamp) + base_len + strlen(suffix) + 1;
    char *fm_File = malloc(total_len);
    snprintf(fm_File, total_len, "%s/%s%s%.*s%s", path, prefix, timestamp, (int)base_len, base_info_File, suffix);



    //Ahora sí, guardamos las medidas sintéticas
    save_synthetic_measurements_to_csv(dm_File, wlm_File, pm_File, fm_File, NULL, cv_DM, cv_WLM, 
        cv_PM, cv_FM, 0, nJ, nR, nDM, nWLM, nPM, nFM, 0, m, 1, ind_J_Dem, wlm_positions, 
        pm_positions, fm_positions, NULL, gx_noise, ph);


    SENSEI_PAUSE();

    EN_close(ph);

    printf("Eliminando proyecto...\n");
    EN_deleteproject(ph);  // Eliminar el proyecto
    printf("Proyecto eliminado correctamente.\n\n");


    if (remove(rpt_File) == 0) {
        printf("%s eliminado\n", rpt_File);
    } else {
        printf("No se ha podido eliminar %s \n", rpt_File);
    }
    // if (remove(binary_File) == 0) {
    //     printf("%s eliminado\n", binary_File);
    // } else {
    //     printf("Error al eliminar %s \n", binary_File);
    // }
    printf("\n\n");

    SENSEI_PAUSE();

    return 0;

}
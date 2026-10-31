#ifndef FUNCTIONS_SENSEI_H   
#define FUNCTIONS_SENSEI_H

#include "epanet2_2.h"

#include <math.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <psapi.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <suitesparse/cholmod.h>



typedef struct {
    int index; //Índice del control (1-based)
    int control_type; //EN_LOWLEVEL o EN_HILEVEL
    int linkIndex;
    int is_valve;
    double setting;
    int nodeIndex;
    double level;
    char sentence[200];
} Ctrl_01;  //Estructura para almacenar información de reglas de control de tipo EN_LOWLEVEL o EN_HILEVEL



typedef struct {
    int index; //Índice del control (1-based)
    int control_type; //Va a ser EN_TIMER o EN_TIMEOFDAY
    int linkIndex; 
    int is_valve; 
    double setting; 
    int period;
    double sec_in_period;
    char sentence[200];
} Ctrl_23;  //Estructura para almacenar información de reglas de control de tipo EN_TIMER o EN_TIMEOFDAY



typedef struct {
    int *pipe_pump_indices;
    int *pipe_pump_status;
    int nPipePump;
    int *valve_indices;
    int *valve_status;
    double *last_valve_setting;
    int nValves;
} Ctrl_Links_Summary;  //Estructura para almacenar el estado de los elementos de control en cada momento (EE multiperiodo)
//Sirve para hacer el seguimiento de la evolución y así forzar bien el estado inicial de cara al siguiente periodo



typedef struct {
    int reservoir_index; //Índice del embalse (1-based, como EPANET hace)
    double head[500]; //Reservamos memoria para 500 valores (si T > 500, aumentar tamaño)
} Reservoir_Pattern_Info; //Estructura para guardar el nivel piezométrico de embalses con patrón asociado



typedef struct {
    double** demand_patterns; 
    double* base_demands;
    double ** xPumpCurves;
    double ** yPumpCurves;
    double* valves_settings;
    double* roughness_coefficients;
    double* common_DW_coefs;
    double* common_HW_coefs;
    double* init_tank_levels;
    double* tank_diameters;
} Calib_Variables;  //Variables que se pueden optimizar en el proceso de calibración



typedef struct {
    double* base_demands;
    double* init_tank_levels;
} Variables_Multiperiod_SE;  //Variables de estado de la EE multiperiodo



typedef struct {
    double* base_demands;
    double* init_tank_levels;
    double emitter_coef;
} Variables_Snapshot_SE;  //Variables de estado de la EE instantánea (incluye la posibilidad de considerar coef. emisor como variable en un nodo)




typedef struct {
    int mod_patterns; //Para modificar los patrones
    int mod_base_demands; //Para modificar demandas base
    int mod_curves_X; //Para modificar las coordenadas X (Flujo) de las curvas de las bombas
    int mod_curves_Y; //Para modificar las coordenadas Y (Head) de las curvas de las bombas
    int mod_valves_settings; //Para modificar los settings las válvulas
    int mod_individual_roughness; //Para modificar el coef. de rugosidad inidividual de cada tubería.
    int mod_common_roughness_DW; //Para modificar coef. de rugosidad común (DW) para grupos de tuberías (luego se convierte a HW)
    int mod_common_roughness_HW; //Para modificar coef. de rugosidad común (HW) para grupos de tuberías
    int mod_init_tank_levels; //Para modificar los niveles iniciales de tanques  
    int mod_tank_diameters; //Para modificar los diámetros de los tanques  
} Options; //Opciones de calibración (sirve para activar las variables asociadas)



typedef struct {
    char cluster_id[50]; //ID del grupo de tuberías (hasta 50 caracteres)
    int pipe_index[500]; //Lista de hasta 500 enteros para almacenar el índice de las tuberías
    int pipe_count; //Número de tuberías en el cluster
    double diameter[500];  //Lista de hasta 500 doubles para almacenar el diámetro de las tuberías
    char material[100]; //Material de las tuberías de un mismo grupo
    double avg_age;  //Edad media de las tuberías de ese cluster
    double common_DW_coef;  //Chi de DW inicial 
    double common_HW_coef;  //Coef de rugosidad comun de HW
} Cluster_Pipes_Info;  //Sirve para guardar ordenadamente clusters de tuberías, (opcionalmente) material 
// y edad introducidas por el usuario, y demás información (se usa para opciones extra de calibración)



typedef struct {
    int index; //Índice del control (1-based)
    int control_type; 
    int linkIndex;
    double setting;
    int nodeIndex; 
    double level;
    int valve_index; //Indice de la válvula (restando nPI + nPU + 1)
} Setting_Control_Info; //Información de reglas de control que activan setting en válvulas (CALIBRACIÓN)



//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


// FUNCIONES DE LECTURA DE ARCHIVOS .txt DE CONFIGURACIÓN
// ------------------------------------------------------

void read_config_file_calibration(const char* config_File, char **base_Path, char **info_File, 
    char **dm_File, char **patterns_File, char **wlm_File, char **pm_File, char **fm_File, 
    char **fm_avg_File, char **clusters_pipes_File, char** common_HW_File, 
    char** common_DW_File, char** main_pipes_File, char** options_File, double* epsilon, 
    double* lambda_init, int* max_loops, int* max_it, int* max_attempts, double* tol, int *lang);

void read_config_file_multiperiod_SE(const char* config_File, char **base_Path, char **info_File, 
    char **dm_File, char **wlm_File, char **pm_File, char **fm_File, char **fm_avg_File, 
    double* epsilon, int* max_it, double* tol, double* lambda_init, int* max_attempts, 
    char **emitter_ID, double* emitter_coef, int* period_to_add_emitter, int *lang);

void read_config_file_snapshot_SE(const char* config_File, char **base_Path, char **info_File, 
    char **dm_File, char **wlm_File, char **pm_File, char **fm_File, double* epsilon, 
    int* max_it, double* tol, char **emitter_ID, int *lang);


// FUNCIONES DE PRECONFIGURACIÓN DE CALIBRACIÓN
// --------------------------------------------

int set_all_demands_nodes_rows(const char* dm_File, const char* patterns_File, int lang);

int read_options_variables(Options *Opt, const char* options_File, int*** CM, int lang);

int check_roughness_options_set_mod_MP(Options Opt, const char* clusters_pipes_File, 
    const char* common_DW_File, const char* common_HW_File, const char* main_pipes_File, int lang);


// FUNCIONES DE CONTEO Y CLASIFICACIÓN DE ELEMENTOS
// ------------------------------------------------

void count_elements_in_network(int* nN, int* nL, int* nJ, int* nR, int* nT, int* nPI, int* nPU, 
    int* nV, EN_Project ph);

void classify_transit_demand_junctions(int nJ, int *nJ_Trans, int *nJ_Dem, int **ind_J_Trans, int **ind_J_Dem, EN_Project ph);

int count_demand_patterns_from_inp(int nJ, int** unique_patterns, char*** demand_patterns_id, EN_Project ph);

void get_min_max_tank_levels(int nJ, int nR, int nT, double** min_tank_levels, 
    double** max_tank_levels, EN_Project ph);

double** get_demand_patterns_from_inp(int nDemPat, int T, char** demand_patterns_id,
    int* unique_patterns, EN_Project ph, int lang);

int store_main_pipes(int nL, int** indMP, const char* main_pipes_File, EN_Project ph, int lang);

double* get_roughness_coefficients_from_inp(int nPI, int nMP, int mod_MP, int* indMP, EN_Project ph);

int classify_pipes_by_cluster(const char *clusters_pipes_File, EN_Project ph, 
    Cluster_Pipes_Info *Clusters_Pipes_Info, int lang);

void classify_common_group_coef(int nCP, int HW, const char *common_coef_File, 
    Cluster_Pipes_Info *Clusters_Pipes_Info, EN_Project ph, int lang);

double* convert_roughness_coefs_from_DW_to_HW(int nPI, int nCP, double* common_DW_coefs, 
    Cluster_Pipes_Info* Clusters_Pipes_Info, EN_Project ph);

double* extend_common_roughness_coefs_to_all_pipes(int nPI, int nCP, double* common_HW_coefs, 
    Cluster_Pipes_Info* Clusters_Pipes_Info, EN_Project ph);

double* get_base_demands_vector_from_inp(int nJ_Dem, int* ind_J_Dem, EN_Project ph);

int store_pump_curves_info(double*** xCurves, double*** yCurves, 
    int** pumpCurveIndices_out, EN_Project ph, int lang);

double* get_tank_diameters(int nJ, int nR, int nT, EN_Project ph);

void get_valves_settings(int nPI, int nPU, int nV, double** valves_settings_out, EN_Project ph);

int classify_control_settings_to_edit(int nControls, int nPI, int nPU, 
    Setting_Control_Info* Setting_Controls_Info, EN_Project ph);

void store_controls(int nPI, int nPU, int* nCtrl_01, int* nCtrl_23, long patternStep,
    Ctrl_01 *Controls_01, Ctrl_23 *Controls_23, EN_Project ph);

void summarize_links_affected_by_controls(int nPI, int nPU, int nCtrl_01, int nCtrl_23, 
    Ctrl_01 *Controls_01, Ctrl_23 *Controls_23, Ctrl_Links_Summary* Controls_Links_Summary, EN_Project ph);

void get_initial_status_clean_section_intermediate_file(Ctrl_Links_Summary* Controls_Links_Summary, 
    const char* intermediate_File, EN_Project ph, int lang);

void get_initial_valve_status_clean_section_intermediate_file(int** initial_valve_status, 
    int nPI, int nPU, int nV, const char* info_File, const char* intermediate_File, EN_Project ph);

int store_reservoir_patterns_info(int T, int nJ, int nR, 
    Reservoir_Pattern_Info* Reservoir_Patterns_Info, EN_Project ph);


// FUNCIONES PARA LECTURA y ALMACENAMIENTO de MEDIDAS / PSEUDOMEDIDAS
// ------------------------------------------------------------------

void define_initialize_vectors_matrices_for_sensors_info(int num_elements, int T, int** logic_vector,
    double*** info_matrix, double*** uncert_matrix);

int store_wlm_info(int T, int nR, int nT, int nJ, const char* wlm_File, int* wlm_logic, 
    double** wlm_values, double** CV_wlm, EN_Project ph, int lang);

void store_dm_info(int T, int nDM, int nJ_Dem, int* ind_J_Dem, const char* dm_File, 
    double** dm_values, double** CV_dm, EN_Project ph, int lang);

int store_pm_info(int T, int nJ, const char* pm_File, int* pm_logic, double** pm_values, 
    double** CV_pm, EN_Project ph, int lang);

int store_fm_info(int T, int nL, const char* fm_File, int* fm_logic, double** fm_values, 
    double** CV_fm, int avg, EN_Project ph, int lang);

void store_patterns_info(int T, int nDemPat, const char* patterns_File, double** dm_values, 
    double** CV_dm, EN_Project ph);

int count_measurements(int num_elements, int T, double** uncert_matrix);

int* get_sensor_positions_vector(int num_sensors, int num_elements, int* logic_vector);


// FUNCIONES PARA DEFINIR ESTRUCTURAS DE CV, MEDIDAS / PSEUDOMEDIDAS y PESOS
// -------------------------------------------------------------------------

double** get_CV_matrix(int T, int m, int nDM, int nWLM, int nPM, int nFM, int nFM_avg,
    int* wlm_positions, int* pm_positions, int* fm_positions, int* fm_avg_positions, 
    double** CV_dm, double** CV_wlm, double** CV_pm, 
    double** CV_fm, double** CV_fm_avg);

double* get_measurements_vector(int T, int m, int nDM, int nWLM, int nPM, int nFM, int nFM_avg,
    int* wlm_positions, int* pm_positions, int* fm_positions, int* fm_avg_positions,
    double** dm_values, double** wlm_values, double** pm_values, double** fm_values, double** fm_avg_values);

double* compute_weights_vector(int T, int m, double* z, double** CV_matrix);

cholmod_sparse* diagonal_matrix_from_vector(int T, int m, double* weights_vector, int diag_option, cholmod_common *c);


// FUNCIONES QUE SE USAN EN LOS ALGORITMOS DE CALIBRACIÓN / EE
// -----------------------------------------------------------

void define_provisional_variables_calibration(Calib_Variables *Prov_Vars, Options Opt, int nDemPat,
    int T, int nJ_Dem, int nPumpCurves, int nV, int nCP, int mod_MP, int nMP, int nPI, int nT);

void prepare_ph_snapshot_SE(int nJ, int nR, int nT, int nJ_Dem, int* ind_J_Dem, double** dm_values, 
    double** wlm_values, double* min_tank_levels, double* max_tank_levels, EN_Project ph);

void prepare_ph_for_new_period(int t, int nJ, int nR, int nT, int add_emitter, 
    int period_to_add_emitter, int emitter_index, double emitter_coef, int nCtrl_23, 
    int nR_pattern, long patternStep, int nJ_Dem, int* ind_J_Dem, double* last_demands,
    double* initial_tank_levels, double* min_tank_levels, double* max_tank_levels, 
    char* intermediate_File, char* rpt_File, char* binary_File,
    Reservoir_Pattern_Info* Reservoir_Patterns_Info, Ctrl_Links_Summary* Controls_Links_Summary,
    Ctrl_23* Controls_23, EN_Project ph, int lang);

void get_status(Ctrl_01* Controls_01, Ctrl_23* Controls_23, 
    Ctrl_Links_Summary* Controls_Links_Summary, EN_Project ph);

void update_time_controls_inp(int nCtrl_23, int t, long patternStep, const char *intermediate_File, 
    Ctrl_23 *Controls_23, EN_Project ph, int lang);

double* get_initial_tank_levels(int nJ, int nR, int nT, EN_Project ph);

    
// FUNCIONES DE SIMULACIÓN (y relacionadas)
// ----------------------------------------

double* run_single_period_simulation(long patternStep, long hydStep, int nJ, int nR, int nT, 
    int nJ_Dem, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int m, int m_aug, 
    int* wlm_positions, int* pm_positions, int* fm_positions, int* fm_avg_positions, 
    int* ind_J_Dem, int nCtrl_01, int nCtrl_23, int nPI, int nPU, int period, int check_ctrl,
    Ctrl_01* Controls_01, Ctrl_23* Controls_23, Ctrl_Links_Summary* Controls_Links_Summary, 
    EN_Project ph, int lang);

double* run_snapshot_simulation(int nJ, int nR, int nT, int nJ_Dem, int nDM, int nWLM, 
    int nPM, int nFM, int m, int* wlm_positions, int* pm_positions, int* fm_positions, 
    int* ind_J_Dem, EN_Project ph);

double* run_whole_synthetic_simulation(long patternStep, long hydStep, int T, int nJ, int nR, 
    int nT, int nJ_Dem, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int nDemPat, int m, 
    int all_demands_nodes_rows, int* wlm_positions, int* pm_positions, int* fm_positions, 
    int* fm_avg_positions, int* ind_J_Dem, EN_Project ph);

double* run_whole_simulation_calibration(long patternStep, long hydStep, int T, int nJ, int nR, 
    int nT, int nJ_Dem, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int nDemPat, int m, 
    int all_demands_nodes_rows, int* wlm_positions, int* pm_positions, int* fm_positions, 
    int* fm_avg_positions, int* ind_J_Dem, EN_Project ph, int lang);

void check_active_controls(int nPI, int nPU, int nCtrl_01, int nCtrl_23, int period, long time,
    Ctrl_01* Controls_01, Ctrl_23* Controls_23, Ctrl_Links_Summary* Controls_Links_Summary, EN_Project ph);


// FUNCIONES PARA EVALUAR LA FUNCIÓN OBJETIVO (y relacionadas)
// -----------------------------------------------------------

cholmod_dense* compute_sqrt_weighted_residuals(int T, int m, double* z, double* gx,
    double* weights_vector, cholmod_common* c);

double compute_SWSE_SE(cholmod_dense* sqrt_weighted_residuals, int nDM, int nWLM, 
    int nPM, int nFM, int nFM_avg, int final_nWLM, int lang);

double compute_SWSE_calibration(cholmod_dense* sqrt_weighted_residuals, int print_by_period, 
    int T, int m, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int lang);

double compute_convergence_rate(int T, int m, double* gx_new, double* gx_old);


// FUNCIONES PARA EL CÁLCULO DEL JACOBIANO
// ---------------------------------------

cholmod_sparse* compute_Jacobian_multiperiod_SE(int num_unknowns, long patternStep, long hydStep, 
    double epsilon, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int* wlm_positions, 
    int* pm_positions, int* fm_positions, int* fm_avg_positions, int nJ, int nR, int nT, int m, 
    int m_aug, int nJ_Dem, int* ind_J_Dem, double* min_tank_levels, double* max_tank_levels, 
    double* gx, cholmod_common* c, int nCtrl_01, int nCtrl_23, int nPI, int nPU, int t, 
    Ctrl_01* Controls_01, Ctrl_23* Controls_23, Ctrl_Links_Summary* Controls_Links_Summary, 
    EN_Project ph, int lang);

cholmod_sparse* compute_Jacobian_snapshot_SE(int num_unknowns, int add_emitter, int emitter_index,
    double epsilon, int nDM, int nWLM, int nPM, int nFM, int* wlm_positions, int* pm_positions, 
    int* fm_positions, int nJ, int nR, int nT, int m, int nJ_Dem, int* ind_J_Dem, 
    double* min_tank_levels, double* max_tank_levels, Variables_Snapshot_SE Current_Vars, 
    double* gx, cholmod_common* c, EN_Project ph);

cholmod_sparse* compute_Jacobian_calibration(int* columns, long patternStep, long hydStep, int T, 
    double epsilon, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int* wlm_positions, 
    int* pm_positions, int* fm_positions, int* fm_avg_positions, int nJ, int nR, int nT, int nPI, 
    int nPU, int nV, int m, int nDemPat, int nCP, int nMP, int stage, int** CM, int mod_MP, 
    int all_demands_nodes_rows, int nPumpCurves, int* pumpCurveIndices, int nJ_Dem, int* ind_J_Dem, 
    int nSetCtrls, Setting_Control_Info* Setting_Controls_Info, 
    Cluster_Pipes_Info* Clusters_Pipes_Info, int* indMP, double* min_tank_levels, 
    double* max_tank_levels, Calib_Variables Current_Vars, Options Opt, double* gx, 
    char* inp_file_name, char* rpt_File, char* binary_File, cholmod_common* c, EN_Project ph, int lang);


// FUNCIONES PARA RESOLVER ECUACIONES / SISTEMAS formato cholmod
// -------------------------------------------------------------

cholmod_dense* solve_SDP_linear_system(cholmod_sparse* U, cholmod_dense* b, cholmod_common* c, int lang);

cholmod_sparse* solve_SDP_matricial_system(cholmod_sparse *A, cholmod_sparse *B, cholmod_common *c, int lang);

cholmod_dense* solve_Gauss_Newton_equation(int num_unknowns, cholmod_sparse* J, 
    cholmod_sparse* sqrt_W_matrix, cholmod_dense* sqrt_weighted_residuals, cholmod_common *c, int lang);

cholmod_dense* solve_Levenberg_Marquardt_equation(int num_unknowns, cholmod_sparse* J, cholmod_sparse* sqrt_W_matrix,
    cholmod_dense* sqrt_weighted_residuals, double lambda, int scale, cholmod_common *c, int lang);

cholmod_sparse* add_regularization_term(double lambda, int num_unknowns, double* DtD_diag,
    cholmod_sparse* U, cholmod_common* c, int lang);


// FUNCIONES PARA CALCULAR Y ACTUALIZAR VARIABLES 
// ----------------------------------------------

void compute_provisional_variables_multiperiod_SE(cholmod_dense* delta_x, int nJ_Dem, int nT, 
    int* ind_J_Dem, double* min_tank_levels, double* max_tank_levels, 
    Variables_Multiperiod_SE Current_Vars, Variables_Multiperiod_SE *Prov_Vars, EN_Project ph);

void compute_provisional_variables_snapshot_SE(cholmod_dense* delta_x, int nJ_Dem, int nT, 
    int add_emitter, int* ind_J_Dem, double* min_tank_levels, double* max_tank_levels, 
    Variables_Snapshot_SE Current_Vars, Variables_Snapshot_SE *Prov_Vars, EN_Project ph, int lang);

void compute_provisional_variables_calibration(double omega, int num_unknowns, 
    cholmod_dense* delta_x, int stage, int** CM, int T, int nDemPat, int nJ_Dem, int nPumpCurves, 
    int* pumpCurveIndices, int nSetCtrls, int nPI, int nV, int nCP, int nMP, int mod_MP, int nT, 
    int nJ, int nR, int* ind_J_Dem, double* min_tank_levels, double* max_tank_levels, 
    Setting_Control_Info* Setting_Controls_Info, Calib_Variables Current_Vars, 
    Calib_Variables *Prov_Vars, Cluster_Pipes_Info* Clusters_Pipes_Info, EN_Project ph, int lang);

void update_current_variables_with_provisional_variables_multiperiod_SE(int nJ_Dem, int nT, 
    Variables_Multiperiod_SE* Current_Vars, Variables_Multiperiod_SE Prov_Vars, EN_Project ph);

void update_current_variables_with_provisional_variables_snapshot_SE(int nJ_Dem, int nT, 
    int add_emitter, Variables_Snapshot_SE* Current_Vars, 
    Variables_Snapshot_SE Prov_Vars, EN_Project ph);

void update_current_variables_with_provisional_variables_calibration(int stage, int** CM, int T, 
    int nDemPat, int nJ_Dem, int nPumpCurves, int nSetCtrls, int nPI, int nV, int nCP, int nMP, 
    int mod_MP, int nT, Cluster_Pipes_Info* Clusters_Pipes_Info, Calib_Variables* Current_Vars, 
    Calib_Variables Prov_Vars, EN_Project ph);


// FUNCIONES PARA EDITAR EN EL PH EL VALOR DE VARIABLES
// ----------------------------------------------------

void edit_ph_all_current_variables(char* inp_file_name, char* rpt_File, char* binary_File, 
    int nPumpCurves, double** xPumpCurves, double** yPumpCurves, int* pumpCurveIndices, int nPI, 
    int nPU, int nV, int nMP, int mod_MP, int* indMP, int nJ, int nR, int nT, int nJ_Dem, 
    int* ind_J_Dem, int nDemPat, int T, int nSetCtrls, Calib_Variables Current_Vars, Options Opt, 
    Setting_Control_Info* Setting_Controls_Info, EN_Project ph, int lang);

void edit_ph_initial_status(Ctrl_Links_Summary* Controls_Links_Summary, EN_Project ph);

void edit_ph_init_valve_setting(Ctrl_Links_Summary* Controls_Links_Summary, EN_Project ph);

void edit_ph_provisional_variables_multiperiod_SE(int nJ_Dem, int nT, int nJ, int nR, 
    int* ind_J_Dem, Variables_Multiperiod_SE Prov_Vars, EN_Project ph);

void edit_ph_provisional_variables_snapshot_SE(int nJ_Dem, int nT, int nJ, int nR, int add_emitter, 
    int emitter_index, int* ind_J_Dem, Variables_Snapshot_SE Prov_Vars, EN_Project ph);

void edit_ph_provisional_variables_calibration(int stage, int** CM, int T, int nDemPat, int nJ_Dem, 
    int nPumpCurves, int* pumpCurveIndices, int nSetCtrls, int nPI, int nPU, int nV, int nCP, 
    int nMP, int mod_MP, int* indMP, int nT, int nJ, int nR, char* inp_file_name, char* rpt_File, 
    char* binary_File, int* ind_J_Dem, Setting_Control_Info* Setting_Controls_Info, 
    Cluster_Pipes_Info* Clusters_Pipes_Info, Calib_Variables Current_Vars, 
    Calib_Variables Prov_Vars, Options Opt, EN_Project ph, int lang);

void edit_ph_base_demands(int nJ_Dem, int* ind_J_Dem, double* base_demands_vector, EN_Project ph);

void edit_ph_initial_tank_levels(int nJ, int nR, int nT, EN_Project ph, double* init_tank_levels);

void edit_ph_pump_curves(char* inp_file_name, char* rpt_File, char* binary_File, int nPumpCurves, 
    double** xPumpCurves, double** yPumpCurves, int* pumpCurveIndices, EN_Project ph, int lang);

void edit_ph_roughness_coefficients(int nPI, int nMP, int mod_MP, int* indMP,
    double* roughness_coefficients, EN_Project ph);

void edit_ph_demand_patterns(int nDemPat, int T, double** demand_patterns, EN_Project ph, int lang);

void edit_ph_setting_controls(int nSetCtrls, double* valves_settings, 
    Setting_Control_Info* Setting_Controls_Info, EN_Project ph);

void edit_ph_valves_settings(int nPI, int nPU, int nV, double* valves_settings, EN_Project ph);

void edit_ph_tank_diameters(int nJ, int nR, int nT, EN_Project ph, double* tank_diameters);


// FUNCIONES PARA LA PROPAGACIÓN DE INCERTIDUMBRE (y relacionado)
// --------------------------------------------------------------

void propagate_uncertainty_compute_normalized_residuals_multiperiod_SE(int t, int num_unknowns, 
    int nFM, int nFM_avg, int nT, int m, int m_aug, double* gx, double* z_t, double** posterior_CV, 
    double** normalized_residuals, double* sd_final_tank_level, double* weights_vector_t, 
    cholmod_sparse* sqrt_W_matrix, cholmod_sparse* R_matrix, cholmod_sparse* J, cholmod_common *c, int lang);

void propagate_uncertainty_compute_normalized_residuals_snapshot_SE(int num_unknowns, int nFM, 
    int m, double* gx, double* z, double* posterior_CV, double* normalized_residuals, 
    double* weights_vector, cholmod_sparse* sqrt_W_matrix, cholmod_sparse* R_matrix, 
    cholmod_sparse* J, cholmod_common *c, int lang);

void uncertainty_propagation_calibration(int num_unknowns, int T, int nFM, int nFM_avg, 
    int m, double* gx, double* z, double* posterior_sd, double* weights_vector, 
    cholmod_sparse* sqrt_W_matrix, cholmod_sparse* R_matrix, cholmod_sparse* J, 
    cholmod_common *c, int lang);

void end_iteration_and_print_results(int t, int nJ, int nR, int nT, int nDM, int nPM, int nFM, 
    int nFM_avg, int num_unknowns, int m, int m_aug, int* ind_J_Dem, int* pm_positions, 
    int* fm_positions, int* fm_avg_positions, double* z_t, double SWSE, double* weights_vector_t, 
    double* last_demands, double* init_tank_levels_next_period, double* gx, double** estimated_values, 
    double** posterior_CV, double** normalized_residuals, double* sd_final_tank_level, 
    cholmod_sparse* sqrt_W_matrix, cholmod_sparse* R_matrix, cholmod_sparse* J, cholmod_common *c, 
    Ctrl_Links_Summary* Controls_Links_Summary, EN_Project ph, int lang);


// FUNCIONES PARA GUARDAR MATRICES / RESULTADOS EN ARCHIVOS .csv 
// -------------------------------------------------------------

void save_SE_results_matrix_to_csv(char* file_name, int nJ, int nR, int nDM, int nWLM, int nPM, 
    int nFM, int nFM_avg, int m, int T, int* ind_J_Dem, int* wlm_positions, int* pm_positions, 
    int* fm_positions, int* fm_avg_positions, double** results_matrix, EN_Project ph, int lang);

void save_posterior_sd_CV_calib_parameters(char* file_name, int T, int nJ_Dem, int nJ, int nR, 
    int nT, int nPI, int nPU, int nV, int mod_MP, int nMP, int nCP, int nPumpCurves, int nDemPat, 
    int* indMP, int* ind_J_Dem, int* pumpCurveIndices, double* posterior_sd, int** CM,
    char **demand_patterns_id, Calib_Variables Current_Vars, 
    Cluster_Pipes_Info *Clusters_Pipes_Info, EN_Project ph, int lang);

void save_snapshot_SE_results_to_csv(char* file_name, int add_emitter, int emitter_index, int nJ, 
    int nR, int nDM, int nWLM, int nPM, int nFM, int m, int* ind_J_Dem, int* wlm_positions, 
    int* pm_positions, int* fm_positions, double* z, double* estimated_values, 
    double* posterior_CV, double* normalized_residuals, double** CV_matrix, 
    Variables_Snapshot_SE Current_Vars, EN_Project ph, int lang);

void save_synthetic_measurements_to_csv(char* dm_File, char* wlm_File, char* pm_File, char* fm_File, 
    char* fm_avg_File, double cv_DM, double cv_WLM, double cv_PM, double cv_FM, double cv_FM_avg, 
    int nJ, int nR, int nDM, int nWLM, int nPM, int nFM, int nFM_avg, int m, int T, int* ind_J_Dem, 
    int* wlm_positions, int* pm_positions, int* fm_positions, int* fm_avg_positions, 
    double* measurements_vector, EN_Project ph);


// DEMÁS FUNCIONES VARIADAS 
// ------------------------

void sort_first_n(int *arr, int n);

int cmpfunc(const void *a, const void *b);

void save_dense_matrix_to_txt(cholmod_sparse *A, const char *file_name, cholmod_common *c);

void check_nan_inf_matrix(int* nan_count, int* inf_count, cholmod_sparse* matrix);

void print_memory_usage(int lang);

char* strsep(char **stringp, const char *delim);

char* trim(char *str);

double normal_sample(double sd);

int compare_ints(const void *a, const void *b);


// ------------------------------------------------------------------
//  Interactive stepping
//
//  The programs pause at each stage so a user can follow the run. The
//  pauses are opt-in: set SENSEI_INTERACTIVE=1 in the environment to
//  enable them. Left unset, the programs run unattended, which is what
//  batch reproduction of the published results requires.
// ------------------------------------------------------------------

extern int sensei_interactive;   // -1 not yet resolved, 0 off, 1 on
void sensei_pause(void);
#define SENSEI_PAUSE() sensei_pause()

#endif

typedef struct{
    double lat;
    double lon;
    double elv;
	double est;
    double ist;
    double imf_by;
    double f107;
    double em;
    double date;
}pomme_inputs;

typedef struct{
	double x;
	double y;
	double z;
	double h;
	double f;
	double dec;
	double inc;
}pomme_results;

void init_field_control(int* control);
void exclude_field_control(int* control);
void compute_mag_elements(pomme_results* res, double x, double y, double z, double decl_t, double decl_m, double inc_t, double inc_m, double h_t, double h_m, double f_t, double f_m);
void print_json_results(pomme_results* res);
void pommecalc(double date,double lat,double lon, double elv, double est,double ist, double imf_by,double f107,double em);	
double tcalc_julian_day_from_fyear(double fyear);


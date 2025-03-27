#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "POMME10_core.h"
#include "POMME10_ext.h"
#include "POMME10_crust.h"
#include "POMME10_gsm2geo.h"
#include "POMME10_sm2geo.h"
#include "POMME10_qfac.h"


#include "pom.h"
#include "model.h"

void exclude_field_control(int* control){

   control[POS_SM] = 0;
   control[POS_SM_IND] = 0;
   control[POS_F107] =      0;     /* degree of SM field, default = (*model).deg_sm, 0=off */
   control[POS_F107_IND] =  0;     /* degree of field induced by SM field, default = (*model).deg_sm, 0=off */
   control[POS_EST] =       0;                  /* Est dependent field: 1=on, 0=off */
   control[POS_IST] =       0;                  /* Ist dependent field: 1=on, 0=off */
   control[POS_EST_FAC] =   0;                  /* 1 = use model Est factor, 0 = use factor 1.0 */

   control[POS_GSM] =       0;     /* degree of GSM field, default = (*model).deg_gsm, 0=off */
   control[POS_GSM_IND] =   0;     /* degree of field induced by GSM field, default = (*model).deg_gsm, 0=off */
   control[POS_IMFBY] =     0;                 /* IMF-By dependent field: 1=on, 0=off */
   control[POS_IMFBY_IND] = 0;                 /* Induction by IMF-By dependent field: 1=on, 0=off */
   control[POS_EM] =        0;      /* degree of GSM field, default = (*model).deg_gsm, 0=off */
   control[POS_EM_IND] =    0; 
 	
}

void init_field_control(int* control){

	control[POS_0] = 0; /*Degree of main field?*/
    control[POS_1] = 0;


    control[POS_SM] = DEG_SM;
    control[POS_SM_IND] = DEG_SM;
    control[POS_F107] =      DEG_F107;     /* degree of SM field, default = (*model).deg_sm, 0=off */
    control[POS_F107_IND] =  DEG_F107;     /* degree of field induced by SM field, default = (*model).deg_sm, 0=off */
    control[POS_EST] =       1;                  /* Est dependent field: 1=on, 0=off */
    control[POS_IST] =       1;                  /* Ist dependent field: 1=on, 0=off */
    control[POS_EST_FAC] =   1;                  /* 1 = use model Est factor, 0 = use factor 1.0 */
  
    control[POS_GSM] =       DEG_GSM;     /* degree of GSM field, default = (*model).deg_gsm, 0=off */
    control[POS_GSM_IND] =   DEG_GSM;     /* degree of field induced by GSM field, default = (*model).deg_gsm, 0=off */
    control[POS_IMFBY] =     1;                 /* IMF-By dependent field: 1=on, 0=off */
    control[POS_IMFBY_IND] = 1;                 /* Induction by IMF-By dependent field: 1=on, 0=off */
    control[POS_EM] =        DEG_EM;      /* degree of GSM field, default = (*model).deg_gsm, 0=off */
    control[POS_EM_IND] =    DEG_EM;      /* degree of field induced by GSM field, default = (*model).deg_gsm, 0=off */
    
}

void compute_mag_elements(pomme_results* res, double x, double y, double z, double decl_t, double decl_m, double inc_t, double inc_m, double h_t, double h_m, double f_t, double f_m){

	res->x = x;
	res->y = y;
	res->z = z;
	res->dec = decl_t - decl_m;
	res->inc = inc_t - inc_m;
	res->h = h_t - h_m;
	res->f = f_t - f_m;

}

double tcalc_julian_day_from_fyear(double fyear) {    
    int years, ExtraDay = 0, leapdays;
    double jd;
    years = floor(fyear);
    if((years % 4 == 0 && years % 100 != 0) || years % 400 == 0)
        ExtraDay = 1;
    leapdays = years / 4 - years / 100 + years / 400;
    jd = years * 365 + leapdays + (fyear - years) * (365 + ExtraDay);
    return jd;
}

void print_json_results(pomme_results* res){
	
	printf("{\"x\":%.6f, \"y\":%.6f, \"z\":%.6f, \"h\":%.6f, \"f\":%.6f, \"decline\": %.6f, \"incline\":%.6f}",
		res->x, res->y, res->z, res->h, res->f, res->dec, res->inc);	
}


pomme_results pommecalc(double date, double lat, double lon, double elv, double est,double ist,double imf_by,double f107,double em){
	
	int* control = calloc(MAXCONTROL, sizeof(int));
	pomme_results results;
	
	
	double x, y, z, h, f, incl, decl, min_date;
    double x_m, y_m, z_m, h_m, f_m, incl_m, decl_m;
    double x_t, y_t, z_t, h_t, f_t, incl_t, decl_t;
	double fday;

	fday = tcalc_julian_day_from_fyear(date-2000);
	// compute  external field
	init_field_control(control);

	pomme_geodetic(control, fday, elv, lat, lon, est, ist, imf_by, f107, em, &x, &y, &z, &h, &f, &incl, &decl);

	// compute internal field
	control[POS_0] = 15; /*Degree of main field?*/
	control[POS_1] = 15;
 
	pomme_geodetic(control, fday, elv, lat, lon, est, ist, imf_by, f107, em, &x_t, &y_t, &z_t, &h_t, &f_t, &incl_t, &decl_t);

	// exclude all the field from internal field

	exclude_field_control(control);
	
	pomme_geodetic(control, fday, elv, lat, lon, est, ist, imf_by, f107, em, &x_m, &y_m, &z_m, &h_m, &f_m, &incl_m, &decl_m);

	compute_mag_elements(&results, x, y, z, decl_t, decl_m, incl_t, incl_m, h_t, h_m, f_t, f_m);
	
	//print_json_results(&results);
	free(control);
	//printf("x: %f, y: %f, z: %f, h: %f, f: %f, decl: %f, incl: %f \n", *x, *y, *z, *h, *f, *decl, *incl);

	return results;
}




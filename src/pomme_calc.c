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
#include "pomme_calc.h"

#ifndef _WINDOWS_
/* accepted long command line options */
static int option_index = 0;
static struct option long_opts[] = {
    {"min_lat",     1, 0, 'a'},

    {"lam",         1, 0, 'a'},

    {"min_lon",     1, 0, 'o'},

    {"lom",         1, 0, 'o'},

    {"min_date",    1, 0, 'd'},

    {"dm",          1, 0, 'd'},

    {"elevation",   1, 0, 'e'},

    {0, 0, 0, 0}
};
#endif
#ifdef _WINDOWS_
    char *optarg = NULL;
#endif

int main(int argc, char **argv)
   {
  
     double outData[7];
     double lon, geod_lat, geod_alt, fday;                      /* Position and time */
     //double E_st=0, I_st=0, IMF_By=0, f107=120.0, Em=0.5;       /* Magnetic indices for time varying external field */
     double E_st, I_st, IMF_By, f107, Em;
     double x, y, z, h, f, incl, decl, min_date;
     double x_m, y_m, z_m, h_m, f_m, incl_m, decl_m;
     double x_t, y_t, z_t, h_t, f_t, incl_t, decl_t;
     int i, model_nbytes, c;
     int control[MAXCONTROL];
     struct geomag_input in_vals;
     struct geomag_result *results;
   /*control[POS_0] = 0;
   if(control[POS_0]>15) control[POS_1] = 15;
   else control[POS_1] = 0;//15;*/
   
//   control[POS_0] = 15; /*Degree of main field?*/
//   control[POS_1] = 15;

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
 
//   control[POS_SM] = 0;
//   control[POS_SM_IND] = 0;
//   control[POS_F107] =      0;     /* degree of SM field, default = (*model).deg_sm, 0=off */
//   control[POS_F107_IND] =  0;     /* degree of field induced by SM field, default = (*model).deg_sm, 0=off */
//   control[POS_EST] =       0;                  /* Est dependent field: 1=on, 0=off */
//   control[POS_IST] =       0;                  /* Ist dependent field: 1=on, 0=off */
//   control[POS_EST_FAC] =   0;                  /* 1 = use model Est factor, 0 = use factor 1.0 */
 
//   control[POS_GSM] =       0;     /* degree of GSM field, default = (*model).deg_gsm, 0=off */
//   control[POS_GSM_IND] =   0;     /* degree of field induced by GSM field, default = (*model).deg_gsm, 0=off */
//   control[POS_IMFBY] =     0;                 /* IMF-By dependent field: 1=on, 0=off */
//   control[POS_IMFBY_IND] = 0;                 /* Induction by IMF-By dependent field: 1=on, 0=off */
//   control[POS_EM] =        0;      /* degree of GSM field, default = (*model).deg_gsm, 0=off */
//   control[POS_EM_IND] =    0; 
 
   /*fday = (int) jfday;
   geod_alt = (double) jgeod_alt;
   geod_lat = (double) jgeod_lat;
   lon = (double) jlon;
   E_st = (double) jest;
   I_st = (double) jist;
   IMF_By = (double) jimf_by;
   f107 = (double) jf107;
   Em = (double) jEm;*/
  
    in_vals.display_type = XML;
       /* parse command line input using getopt() */
#ifdef _WINDOWS_    
    while ((c = GetOption(argc, argv, "inf:a:o:d:e:E:I:m:B:R:T:", &optarg)) != -1) {
#else
    while ((c = getopt_long(argc, argv, "inf:a:o:d:e:E:I:m:B:R:T:", long_opts, &option_index)) != -1) {
#endif
        switch (c) {
            case 'a': /* min latitude */
                in_vals.min_lat = atof(optarg);
                break;

            case 'o': /* min longitude */
                in_vals.min_lon = atof(optarg);
                break;

            case 'd': /* min date */
                in_vals.min_date = atof(optarg);
                break;

            /*case 'c': /* coordinate system 
                in_vals.coord_sys = (optarg[0] == 'C') ? GEOCENTRIC : GEODETIC;
                break;*/

            case 'e': /* elevation */
                in_vals.elev = atof(optarg);
                break;
            
            case 'E': /*E_st*/
                E_st = atof(optarg);
                break;
                
            case 'I': /*I_st*/
                I_st = atof(optarg);
                break;
                
            case 'B': /*IMF_By*/
                IMF_By = atof(optarg);
                break;
                
            case 'R': /*F 10.7*/
                f107 = atof(optarg);
                break;
                
            case 'm': /*Em*/
                Em = atof(optarg);
                break;
                
            case 'T':
                switch (optarg[0]) {
                    case 'C': in_vals.display_type = CSV; break;
                    case 'X':
                    default:
                        in_vals.display_type = XML; break;
                }
           
        }
    }

   fday = tcalc_julian_day_from_fyear(in_vals.min_date-2000);
   pomme_geodetic(control, fday, in_vals.elev, in_vals.min_lat, in_vals.min_lon, E_st, I_st, IMF_By, f107, Em, &x, &y, &z, &h, &f, &incl, &decl);



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
   
   pomme_geodetic(control, fday, in_vals.elev, in_vals.min_lat, in_vals.min_lon, E_st, I_st, IMF_By, f107, Em, &x_m, &y_m, &z_m, &h_m, &f_m, &incl_m, &decl_m);


   results = alloc_result(1, 1, 1);
   results->lons[0] = in_vals.min_lon;
   results->lats[0] = in_vals.min_lat;
   results->magc[0][0][0].x_component = x;
   results->magc[0][0][0].y_component = y;
   results->magc[0][0][0].z_component = z;
   results->magc[0][0][0].declination = decl - decl_m;
   results->magc[0][0][0].inclination = incl - incl_m;
   results->magc[0][0][0].horiz_intensity = h - h_m;
   results->magc[0][0][0].total_intensity = f - f_m;
   results->dates[0] = in_vals.min_date;

//   printf("x = %lf, x_del = %lf\n", x, x_t - x_m);
switch(in_vals.display_type)
{
    case CSV:
        //in_vals.component = TOTAL_INTENSITY;
        in_vals.component = ALL;
        print_result_csv(results, stdout, &in_vals);
        break;
    case XML:
    default:
        in_vals.component = ALL;
        print_result_xml(results, stdout, &in_vals);
        break;
}
   
   /*
  outData[0] = 5.1;//decl;
  outData[1] = 5.2;//incl;
  outData[2] = 5.3;//f;
  outData[3] = 5.4;//h;
  outData[4] = 5.5;//x;
  outData[5] = 5.6;//y;
  outData[6] = 5.7;//z;
  */
  
  //(*env)->SetDoubleArrayRegion(env, result, 0, 7, (const jdouble*) outData);
  
  return 0;// result;
  
}

void print_result_xml(
        struct geomag_result* results,
        FILE* ostream,
        const struct geomag_input* in)
{
    int i, j, k;
    char unit_string[32];
    print_xml_header(ostream);
    for (k = 0; k < results->ndates; k++) {
        for (i = results->nlats - 1; i >= 0; i--) {
            for (j = 0; j < results->nlons; j++) {
                fprintf(ostream,"<RESULT>\n");
                print_xml_date(ostream,results->dates[k] );
                fprintf(ostream, "<LATITUDE>\n%.5f\n", results->lats[i]);
                fprintf(ostream,"</LATITUDE>\n");
                // geomag_input struct always stores elevation in km
/*                switch(in->dist_units)
                {
                    case KM:
                        strcpy(unit_string,"km");
                        break;
                    case METERS:
                        strcpy(unit_string,"m");
                        break;
                    case FEET:
                        strcpy(unit_string,"feet");
                        break;
                    default:
                        strcpy(unit_string,"km");
                        break;
                }*/
                strcpy(unit_string,"km");
                fprintf(ostream, "<LONGITUDE>\n%.5f\n", results->lons[j]);
                fprintf(ostream,"</LONGITUDE>\n");
                fprintf(ostream, "<ELEVATION units=\"%s\">\n%.5f\n", unit_string,in->elev);
                fprintf(ostream,"</ELEVATION>\n");
                switch (in->component) {
                    case INCLINATION:
                        fprintf(ostream, "<INCLINATION>\n%.5f\n</INCLINATION>\n", results->magc[i][j][k].inclination);
                        break;
                    case HOR_INTENSITY:
                        fprintf(ostream, "<HORINTENSITY>\n%.5f\n</HORINTENSITY>\n", results->magc[i][j][k].horiz_intensity);
                        break;
                    case TOTAL_INTENSITY:
                        fprintf(ostream, "<TOTALINTENSITY>\n%.5f\n</TOTALINTENSITY>\n", results->magc[i][j][k].total_intensity);
                        break;
                    case Y_COMPONENT:
                        fprintf(ostream, "<YCOMPONENT>\n%.5f\n</YCOMPONENT>\n", results->magc[i][j][k].y_component);
                        break;
                    case X_COMPONENT:
                        fprintf(ostream, "<XCOMPONENT>\n%.5f\n</XCOMPONENT>\n", results->magc[i][j][k].x_component);
                        break;
                    case Z_COMPONENT:
                        fprintf(ostream, "<ZCOMPONENT>\n%.5f\n</ZCOMPONENT>\n", results->magc[i][j][k].z_component);
                        break;
                    case DECLINATION:
                        fprintf(ostream, "<DECLINATION>\n%.5f\n</DECLINATION>\n", results->magc[i][j][k].declination);
                        break;
                    case ALL:
                    default:
                        fprintf(ostream, "<DECLINATION>\n%.5f\n</DECLINATION>\n", results->magc[i][j][k].declination);
                        fprintf(ostream, "<INCLINATION>\n%.5f\n</INCLINATION>\n", results->magc[i][j][k].inclination);
                        fprintf(ostream, "<HORINTENSITY>\n%.5f\n</HORINTENSITY>\n", results->magc[i][j][k].horiz_intensity);
                        fprintf(ostream, "<TOTALINTENSITY>\n%.5f\n</TOTALINTENSITY>\n", results->magc[i][j][k].total_intensity);
                        fprintf(ostream, "<YCOMPONENT>\n%.5f\n</YCOMPONENT>\n", results->magc[i][j][k].y_component);
                        fprintf(ostream, "<XCOMPONENT>\n%.5f\n</XCOMPONENT>\n", results->magc[i][j][k].x_component);
                        fprintf(ostream, "<ZCOMPONENT>\n%.5f\n</ZCOMPONENT>\n", results->magc[i][j][k].z_component);
                        break;
                }
                fprintf(ostream,"</RESULT>\n");
            }
        }
    }
    print_xml_footer(ostream);
} // print result xml


void print_xml_header(FILE* ostream)
{
    fprintf(ostream, "<MAGGRIDRESULT>\n");
}

void print_xml_footer(FILE* ostream)
{
    fprintf(ostream, "</MAGGRIDRESULT>\n");
}

void print_xml_date(FILE* ostream, double date)
{
    fprintf(ostream, "<DATE>\n%.5f\n",date);
    fprintf(ostream, "</DATE>\n");
}

struct geomag_result* alloc_result(int lats, int lons, int dates) {
    struct geomag_result* result = NULL;  
    int i, j;
    
    result = malloc(sizeof(struct geomag_result));
    if (result == NULL) exit(_OUT_OF_MEMORY);

    result->nlats = lats;
    result->nlons = lons;
    result->ndates = dates;

    /* allocate memory for the results of the computation */
    result->magc = calloc(lats, sizeof(struct geomag_comps**));
    if (result->magc == NULL) exit(_OUT_OF_MEMORY);
    for (i = 0; i < lats; i++) {
        result->magc[i] = calloc(lons, sizeof(struct geomag_comps*));
        if (result->magc[i] == NULL) exit(_OUT_OF_MEMORY);

        for (j = 0; j < lons; j++) {
            result->magc[i][j] = calloc(dates, sizeof(struct geomag_comps));
            if (result->magc[i][j] == NULL) exit(_OUT_OF_MEMORY);
        }
    }

    /* allocate memory for dot values */
    result->dot_magc = calloc(lats, sizeof(struct geomag_comps*));
    if (result->dot_magc == NULL) exit(_OUT_OF_MEMORY);
    for (i = 0; i < lats; i++) {
        result->dot_magc[i] = calloc(lons, sizeof(struct geomag_comps));
        if (result->dot_magc[i] == NULL) exit(_OUT_OF_MEMORY);
    }

    /* allocate memory for labels */
    result->models = calloc(dates, sizeof(char*));
    if (result->models == NULL) exit(_OUT_OF_MEMORY);
    for (i = 0; i < dates; i++) {
        result->models[i] = calloc(MODEL_NAME_SIZE + 2, sizeof(char));
    }

    result->epochs = calloc(dates, sizeof(double));
    if (result->epochs == NULL) exit(_OUT_OF_MEMORY);

    result->dates = calloc(dates, sizeof(double));
    if (result->dates == NULL) exit(_OUT_OF_MEMORY);

    result->lats = calloc(lats, sizeof(double));
    if (result->lats == NULL) exit(_OUT_OF_MEMORY);

    result->lons = calloc(lons, sizeof(double));
    if (result->lons == NULL) exit(_OUT_OF_MEMORY);

    return result;
}

static double
tcalc_julian_day(int year, int month, double day)

{
  double a, b;
  double jd;

  if (month <= 2)
    {
      year -= 1;
      month += 12;
    }

  a = floor(year / 100.0);
  b = 2.0 - a + floor(a / 4.0);

  jd = floor(365.25*(year + 4716)) + floor(30.6001*(month+1)) + day + b - 1524.5 - 2451544.500000;

  return (jd);
}

static double tcalc_julian_day_from_fyear(double fyear) {    
    int years, ExtraDay = 0, leapdays;
    double jd;
    years = floor(fyear);
    if((years % 4 == 0 && years % 100 != 0) || years % 400 == 0)
        ExtraDay = 1;
    leapdays = years / 4 - years / 100 + years / 400;
    jd = years * 365 + leapdays + (fyear - years) * (365 + ExtraDay);
    return jd;
}

void print_result_csv(
        struct geomag_result* results,
        FILE* ostream,
        const struct geomag_input* in)
{
    int i, j, k;
    fprintf(ostream,"Date,Alt(km),Lat(Deg),Lon(Deg),D(deg),I(deg),H(nT),X(nT),Y(nT),Z(nT),F(nT)\n");
    for (k = 0; k < results->ndates; k++) {
        for (i = results->nlats - 1; i >= 0; i--) {
            for (j = 0; j < results->nlons; j++) {
                fprintf(ostream, "%.6f,",results->dates[k]);
                fprintf(ostream, "%.1f,",in->elev);
                fprintf(ostream, "%.1f,", results->lats[i]);

                fprintf(ostream, "%.1f,", results->lons[j]);

                switch (in->component) {
                    case INCLINATION:
                        fprintf(ostream, "%.5f\n", results->magc[i][j][k].inclination);
                        //fprintf(ostream, "%.2f\n", results->dot_magc[i][j].inclination*60.0);
                        break;
                    case HOR_INTENSITY:
                        fprintf(ostream, "%.5f\n", results->magc[i][j][k].horiz_intensity);
                        //fprintf(ostream, "%.1f\n", results->dot_magc[i][j].horiz_intensity);
                        break;
                    case TOTAL_INTENSITY:
                        fprintf(ostream, "%.5f\n", results->magc[i][j][k].total_intensity);
                        //fprintf(ostream, "%.1f\n", results->dot_magc[i][j].total_intensity);
                        break;
                    case Y_COMPONENT:
                        fprintf(ostream, "%.5f\n", results->magc[i][j][k].y_component);
                        //fprintf(ostream, "%.1f\n", results->dot_magc[i][j].y_component);
                        break;
                    case X_COMPONENT:
                        fprintf(ostream, "%.5f\n", results->magc[i][j][k].x_component);
                        //fprintf(ostream, "%.1f\n", results->dot_magc[i][j].x_component);
                        break;
                    case Z_COMPONENT:
                        fprintf(ostream, "%.5f\n", results->magc[i][j][k].z_component);
                        //fprintf(ostream, "%.1f\n", results->dot_magc[i][j].z_component);
                        break;
                    case DECLINATION:
                        fprintf(ostream, "%.5f\n", results->magc[i][j][k].declination);
                        //fprintf(ostream, "%.2f\n", results->dot_magc[i][j].declination*60.0);
                        break;
                    case ALL:
                    default:
                        fprintf(ostream, "%.5f,", results->magc[i][j][k].declination);
                        fprintf(ostream, "%.5f,", results->magc[i][j][k].inclination);
                        fprintf(ostream, "%.5f,", results->magc[i][j][k].horiz_intensity);
                        fprintf(ostream, "%.5f,", results->magc[i][j][k].x_component);
                        fprintf(ostream, "%.5f,", results->magc[i][j][k].y_component);
                        fprintf(ostream, "%.5f,", results->magc[i][j][k].z_component);
                        fprintf(ostream, "%.5f\n", results->magc[i][j][k].total_intensity);

                        /*fprintf(ostream, "%.2f,", results->dot_magc[i][j].declination*60.0);
                        fprintf(ostream, "%.2f,", results->dot_magc[i][j].inclination*60.0);
                        fprintf(ostream, "%.1f,", results->dot_magc[i][j].horiz_intensity);
                        fprintf(ostream, "%.1f,", results->dot_magc[i][j].x_component);
                        fprintf(ostream, "%.1f,", results->dot_magc[i][j].y_component);
                        fprintf(ostream, "%.1f,", results->dot_magc[i][j].z_component);
                        fprintf(ostream, "%.1f\n", results->dot_magc[i][j].total_intensity);*/

                        break;
                }
            }
        }
    }

} // print result csv

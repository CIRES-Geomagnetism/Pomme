#include "gsm2geo.h"

#define R_E               6371.2  /* Magnetic reference radius  */
#define MAX_0_DEG         720     /* Max degree of static field */
#define MAX_0_NCOEFF      (MAX_0_DEG * (MAX_0_DEG + 2))
#define MAX_V_DEG         20      /* Max degree of time derivatives (all same) */
#define MAX_V_NCOEFF      (MAX_V_DEG * (MAX_V_DEG + 2))

#define MAX_EXT_DEG       3       /* Max degree of external field, limited to 3 */
#define MAX_EXT_NCOEFF    (MAX_EXT_DEG * (MAX_EXT_DEG + 2))  


#define POS_0             0       /* static internal */
#define POS_1             1       /* SV */
#define POS_SM            4       /* static ring current */
#define POS_SM_IND        5       /* if asymmetric then could induce field, not implemented */
#define POS_EST           6       /* external RC field */
#define POS_IST           7       /* induced RC field */
#define POS_GSM           8       /* external GSM static field */
#define POS_GSM_IND       9       /* induced internal*/
#define POS_IMFBY         10      /* external IMFBY correlated field */
#define POS_IMFBY_IND     11      /* induced internal*/
#define POS_EST_FAC       12      /* factor for Est/Ist field */
#define POS_F107          13      /* external F107 correlated field in SM */
#define POS_F107_IND      14      /* induced internal*/
#define POS_EM            15      /* external Em correlated field in GSM */
#define POS_EM_IND        16      /* induced internal*/
#define MAXCONTROL        17      /* number of control parameters */

#define MAX_S_COEFF       15       /* spatial coefficients, must change also in gsm2geo and sm2geo  */
#define MAX_T_COEFF       13       /* temporal coefficients */
#define MAXPERIOD         7        /* const, 24h, 12h, 8h ..., must change also in gsm2geo and sm2geo*/

typedef int tcontrol[MAXCONTROL];

#define TEST 0
#define MIN_POLE_DIST   (0.001 * M_PI/180.0) /* Sperical coordinates not defined closer to poles */

extern int imin(int i1, int i2);

extern void get_ext(double g1, double g2, double g3, 
                    double phi, double theta, double rrel, 
                    double *x, double  *y, double  *z); 
extern void get_int(double g1, double g2, double g3,
                    double phi, double theta, double rrel, 
                    double *x, double *y, double *z); 
int plmbar_(double *p,double *dp,double *costh,int *degree,int *inorm);
int magfdz_(double *p,double *dp,double *theta,double *phi,double *rad,
                    int *degree,double *g,double *dx,double *dy,double *dz,
                    double *dpot,double *x,double *y,double *z,double *h,
                    double *f,double *ainc,double *d,double *pot);
int magfdz_ext_(double *p,double *dp,double *theta,double *phi,double *rad,
                    int *degree,double *g,double *dx,double *dy,double *dz,
                    double *dpot,double *x,double *y,double *z,double *h,
                    double *f,double *ainc,double *d,double *pot);

void sm2geo(double fday,int deg_wanted,                                          /* chosen */
                   int deg_ext,int deg_day,double coeff[SM2GEO_MAXNCOEFF],              /* given */
                   double smcoeff[MAX_EXT_NCOEFF],double geocoeff[MAX_EXT_NCOEFF]);

void sm2geo_green(double fday,int deg_want,                               /* chosen */
                         int deg_ext,int deg_day,double coeff[SM2GEO_MAXNCOEFF], /* given */
                         tgsm2geo_green green_day_r,tgsm2geo_green green_day_i);

void gsm2geo(double fday,int deg_wanted,                                               /* chosen */
                    int deg_ext,int deg_day,int deg_ann,double coeff[GSM2GEO_MAXNCOEFF],      /* given */
                    double gsmcoeff[MAX_EXT_NCOEFF],double geocoeff[MAX_EXT_NCOEFF]);

void gsm2geo_green(double fday,int deg_want,                                           /* chosen */
                          int deg_ext,int deg_day,int deg_ann,double coeff[SM2GEO_MAXNCOEFF], /* given */
                          tgsm2geo_green green_day_r,tgsm2geo_green green_day_i,
                          tgsm2geo_green green_ann_r,tgsm2geo_green green_ann_i);

void geodetic2geocentric(double phi, double h,double *latrad, double *r); /* phi is geodetic latitude in radian*/

void geocentric2geodetic_vec(double theta, double delta, double bx, double by, double bz,
                                    double *x, double *y, double *z);

void pomme_geodetic (int control[MAXCONTROL],
                     double fday, double geod_alt, double geod_lat, double lon,
                     double est, double ist, double imf_by, double f107, double Em,  /* time lagged but not peaked or subtracted mean */
                     double *geod_x, double *geod_y, double *geod_z, double *h, double *f, double *incl, double *decl);
void pomme_geocentric (int control[MAXCONTROL],
                     double fday, double r, double lat, double lon,
                     double est, double ist, double imf_by, double f107, double Em,  /* time lagged but not peaked or subtracted mean */
                     double *x, double *y, double *z);

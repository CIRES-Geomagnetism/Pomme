#define MAX_S_COEFF 15
#define MAXPERIOD 7             /* const, 24h, 12h, 8h */

typedef double tgsm2geo_green[MAX_S_COEFF][MAX_S_COEFF][MAXPERIOD];


int gsm2geo_init(char fname[],int *deg_ext,int *deg_day,int *deg_ann,int maxcoeff,double coeff[]);
void gsm2geo(double fday,int deg_want,                             
             int deg_ext,int deg_day,int deg_ann,double coeff[],   
             double gsmcoeff[],double geocoeff[]);

void gsm2geo_green(double fday,int deg_want,                           /* chosen */
                   int deg_ext,int deg_day,int deg_ann,double coeff[], /* given */
                   tgsm2geo_green green_day_r,tgsm2geo_green green_day_i,
                   tgsm2geo_green green_ann_r,tgsm2geo_green green_ann_i);

#define MAX_S_COEFF 15
#define MAXPERIOD 7             /* const, 24h, 12h, 8h */
typedef double tsm2geo_green[MAX_S_COEFF][MAX_S_COEFF][MAXPERIOD];

int sm2geo_init(char fname[],int *deg_ext,int *deg_day,int maxcoeff,double coeff[]);
void sm2geo(double fday,int deg_want,                        
             int deg_ext,int deg_day,double coeff[], 
             double smcoeff[],double geocoeff[]);

void sm2geo_green(double fday,int deg_want,                     /* chosen */
                  int deg_ext,int deg_day,double coeff[], /* given */
                  tsm2geo_green green_day_r,tsm2geo_green green_day_i);






#define _OUT_OF_MEMORY -103
#define MODEL_NAME_SIZE 8
/*** enumeration ***/

typedef enum { 
    DECLINATION, 
    INCLINATION, 
    HOR_INTENSITY,
    TOTAL_INTENSITY,
    X_COMPONENT,
    Y_COMPONENT,
    Z_COMPONENT,
    ALL
} decl_comp_t;

typedef enum {
    LIST,
    GRID,
    XML,
    CSV
} display_t;

/*** data structures ***/

struct geomag_input {
    decl_comp_t component;
    display_t display_type;
    
    double min_date;
    double max_date;             /* decimal years */
    double year_delta;

    double min_lat;
    double max_lat;              /* decimal degrees */
    double lat_delta;

    double min_lon;
    double max_lon;              /* decimal degrees */
    double lon_delta;

    double elev;                 /* in dist_units */
};

struct geomag_comps {
    double x_component; 
    double y_component;
    double z_component;
    double declination; /* decimal degrees */
    double inclination; /* decimal degrees */
    double horiz_intensity;
    double total_intensity;
};
    
struct geomag_result {
    struct geomag_comps*** magc; /* 3-dimensional array of geomagnetic components (date, lon, lat) */
    struct geomag_comps** dot_magc; /* 2-dimensional array of geomagnetic components (dot values for calculating component values into the future) */
    char** models;
    double* epochs;
    double* dates;
    double* lats;
    double* lons;
    int nlons; //ncols
    int nlats; //nrows
    int ndates; //nlevels
};

/*** function prototypes ***/


        
void print_xml_header();
        
void print_xml_footer(FILE* );

void print_xml_date(FILE* , double );

struct geomag_result* alloc_result(int lats, int lons, int dates);

void print_result_xml(struct geomag_result* results, FILE* ostream, const struct geomag_input* in);

static double tcalc_julian_day(int year, int month, double day);

static double tcalc_julian_day_from_fyear(double fyear);

void print_result_csv(struct geomag_result* results, FILE* ostream, const struct geomag_input* in);
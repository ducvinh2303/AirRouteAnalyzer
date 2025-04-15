// KIT107 Assignment 2
/*
 * Specification for the Period ADT
 * Author <<Duc Vinh Vu - 641029>>
 * Version <<20/9/23>>
 */

 #ifndef PERIOD_H
 #define PERIOD_H
 
 #include "node.h"
 #include "route.h"
 
 typedef node period_format; // Using linked list implementation
 
 struct period_int;
 typedef struct period_int *period;
 
 void init_period(period *pp);
 period_format get_routes(period p);
 void set_routes(period p, period_format f);
 void add_route_to_period(period d, route b);
 
 #endif
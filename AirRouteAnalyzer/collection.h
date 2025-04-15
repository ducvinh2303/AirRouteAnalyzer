// KIT107 Assignment 2
/*
 * Specification for the Collection ADT
 * Author <<Duc Vinh Vu - 641029>>
 * Version <<20/9/23>>
 */

 #ifndef COLLECTION_H
 #define COLLECTION_H
 
 #include "node.h"
 #include "route.h"
 #include "period.h"
 
 typedef node collection_format; // Using linked list implementation
 
 struct collection_int;
 typedef struct collection_int *collection;
 
 void init_collection(collection *lp);
 collection_format get_periods(collection l);
 void set_periods(collection l, collection_format f);
 void add_route_to_collection(collection l, route r);
 void route_changes(collection l); 
 void most_connected(collection l); 
 void costs_for_route(collection l); 
 
 #endif
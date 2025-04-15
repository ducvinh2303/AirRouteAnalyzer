// KIT107 Assignment 2
/*
 * Implementation for the Collection ADT
 * Author <<Duc Vinh Vu - 641029>>
 * Version <<20/9/23>>
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 #include <stdbool.h>
 #include "collection.h"
 
 
 /* Types */
 
 struct collection_int
 {
     collection_format first_period; // Pointer to the first period in the collection
 };
 
 
 /* Functions */
 
 /*
  * 'Constructor' for collection
  * Param cp pointer to collection being constructed
  */
 void init_collection(collection *lp)
 {
     // Allocate memory for the collection structure
     *lp = (collection)malloc(sizeof(struct collection_int));
     // Initialize the first_period field to NULL (empty list)
     (*lp)->first_period = NULL;
 }
 
 /*
  * Getter for periods
  * Param c collection variable to examine
  * Return first_period field
  */
 collection_format get_periods(collection l)
 {
     // Return the pointer to the first period in the collection
     return l->first_period;
 }
 
 /*
  * Setter for periods
  * Param c collection variable to update
  * Param f value to be placed into the collection's first_period field
  */
 void set_periods(collection l, collection_format f)
 {
     // Set the first_period field to the provided value
     l->first_period = f;
 }
 
 /*
  * Doer function to add a route to the collection
  * Param l collection variable to update
  * Param r value to be added to the collection in ascending order of year and month
  */
 void add_route_to_collection(collection l, route r)
 {
     // Extract the year and month from the route
     int year = get_year(r);
     int month = get_month(r);
     
     // Check if the collection is empty
     if (l->first_period == NULL) {
         // Create a new period for this route
         period new_period;
         init_period(&new_period);
         
         // Add the route to the new period
         add_route_to_period(new_period, r);
         
         // Create a new node to contain the period
         collection_format new_node = (collection_format)malloc(sizeof(struct node_int));
         if (new_node == NULL) {
             fprintf(stderr, "Error allocating memory for new node.\n");
             exit(1);
         }
         
         // Set the period as the data in the new node
         set_data(new_node, new_period);
         set_next(new_node, NULL);
         
         // Add the new node to the collection
         l->first_period = new_node;
         return;
     }
     
     // The collection is not empty, so find the proper position for the new route
     collection_format current = l->first_period;
     collection_format previous = NULL;
     
     // Find the period for this route's year and month
     while (current != NULL) {
         period current_period = (period)get_data(current);
         
         // We need to get the year and month from the first route in the period
         if (get_routes(current_period) != NULL) {
             route first_route = (route)get_data(get_routes(current_period));
             int current_year = get_year(first_route);
             int current_month = get_month(first_route);
             
             // Check if we found the period for this route
             if (current_year == year && current_month == month) {
                 // Add the route to this period
                 add_route_to_period(current_period, r);
                 return;
             }
             
             // If we've gone past where this period should be, insert it here
             if (current_year > year || (current_year == year && current_month > month)) {
                 break;
             }
         }
         
         previous = current;
         current = get_next(current);
     }
     
     // We didn't find the period, so create a new one
     period new_period;
     init_period(&new_period);
     
     // Add the route to the new period
     add_route_to_period(new_period, r);
     
     // Create a new node to contain the period
     collection_format new_node = (collection_format)malloc(sizeof(struct node_int));
     if (new_node == NULL) {
         fprintf(stderr, "Error allocating memory for new node.\n");
         exit(1);
     }
     
     // Set the period as the data in the new node
     set_data(new_node, new_period);
     
     // Insert the new node at the appropriate position
     if (previous == NULL) {
         // Insert at the beginning
         set_next(new_node, l->first_period);
         l->first_period = new_node;
     } else {
         // Insert in the middle or at the end
         set_next(new_node, current);
         set_next(previous, new_node);
     }
 }
 
 /*
  * Utility doer function to constructively convert a string to lower case returning the result
  * Param s1 the string to convert
  * Return s2 the converted string (without altering the parameter)
  */
 char *to_lower(char *s1)
 {
     // Variable for the length of the string
     int l;
     // Variable for the converted string
     char *s2;
 
     // Create a new string as a copy of the given string with lower-case letters
     l = (int)strlen(s1);
     s2 = (char *)malloc((l+1) * sizeof(char));
     for (int i = 0; i < l; i++)
     {
         s2[i] = tolower(s1[i]);
     }
     s2[l] = '\0';    // Terminate the new string
 
     // Return the lower-case copy of the string
     return s2;
 }
 
 /*
  * Doer function to display how the number of routes has changed in a range of periods
  * Param l collection variable to process
  */
 void route_changes(collection l)
 {
     // Variables for user input
     int start_year;
     int start_month;
     int end_year;
     int end_month;
     // Variable for current node in collection
     collection_format current;
     // Array of month names for output
     const char* MONTHS_TEXT[12] = { 
         "January", "February", "March", "April", "May", "June",
         "July", "August", "September", "October", "November", "December" 
     };
     // Variables for data storage
     int max_entries = 10;
     int** data_array = NULL;
     int count = 0;
     int* count_sub_array = NULL;
     int* marked = NULL;
     
     printf("Route Changes by Period\n");
     printf("--------------------------\n");
     
     // Enter the input value
     printf("Enter start year: ");
     scanf("%d", &start_year);
     printf("Enter start month: ");
     scanf("%d", &start_month);
     printf("Enter end year: ");
     scanf("%d", &end_year);
     printf("Enter end month: ");
     scanf("%d", &end_month);
 
     // Swap times when the end date is before the start date
     if ((start_year > end_year) || (start_year == end_year && start_month > end_month)) {
         int temp_month = start_month;
         start_month = end_month;
         end_month = temp_month;
 
         int temp_year = start_year;
         start_year = end_year;
         end_year = temp_year;
     }
 
     // Dynamically allocate memory for pointer arrays containing data
     data_array = (int**)malloc(max_entries * sizeof(int*));
 
     // Check and notify when memory allocation fails
     if (data_array == NULL) {
         printf("Memory allocation failed.\n");
         return;
     }
 
     current = l->first_period; // Initialize pointer variable
 
     // The loop retrieves each data in the current pointer variable
     while (current != NULL) {
         period current_period = (period)get_data(current);
         if (get_routes(current_period) != NULL) {
             route first_route = (route)get_data(get_routes(current_period));
             
             // Two variables get time in data
             int year = get_year(first_route); 
             int month = get_month(first_route);
             
             // Flight retrieval condition is within the time period entered
             if ((year > start_year || (year == start_year && month >= start_month)) && 
                 (year < end_year || (year == end_year && month <= end_month))) {
                 if (count >= max_entries) {
                     max_entries *= 2;
                     data_array = (int**)realloc(data_array, max_entries * sizeof(int*));
                     if (data_array == NULL) {
                         printf("Memory allocation failed.\n");
                         return;
                     }
                 }
                 data_array[count] = (int*)malloc(2 * sizeof(int)); // The subarray contains the month and year
                 data_array[count][0] = month;
                 data_array[count][1] = year;
                 count++;
             }
         }
         current = get_next(current); // Move to the next data
     }
     
     // Allocate memory for counting arrays
     count_sub_array = (int*)malloc(count * sizeof(int)); // Array to count routes per period
     marked = (int*)malloc(count * sizeof(int)); // Array to mark counted periods
 
     // Initialize the count_sub_array and marked array
     for (int i = 0; i < count; i++) {
         count_sub_array[i] = 0;
         marked[i] = 0;
     }
 
     // Count routes for each unique period
     current = l->first_period;
     while (current != NULL) {
         period current_period = (period)get_data(current);
         if (get_routes(current_period) != NULL) {
             route first_route = (route)get_data(get_routes(current_period));
             int year = get_year(first_route);
             int month = get_month(first_route);
             
             // Check if this period is in our range
             if ((year > start_year || (year == start_year && month >= start_month)) && 
                 (year < end_year || (year == end_year && month <= end_month))) {
                 
                 // Find this period in our data_array
                 for (int i = 0; i < count; i++) {
                     if (!marked[i] && data_array[i][0] == month && data_array[i][1] == year) {
                         // Count routes in this period
                         period_format route_node = get_routes(current_period);
                         int route_count = 0;
                         while (route_node != NULL) {
                             route_count++;
                             route_node = get_next(route_node);
                         }
                         count_sub_array[i] = route_count;
                         marked[i] = 1;
                         break;
                     }
                 }
             }
         }
         current = get_next(current);
     }
 
     // Check if the results are returned
     if (count == 0) {
         // Print a message when no results are found
         printf("\nNo route data from %s %d to %s %d!\n", 
                MONTHS_TEXT[start_month - 1], start_year, 
                MONTHS_TEXT[end_month - 1], end_year);
     } else {
         // Print out the results found
         for (int i = 0; i < count; i++) {
             if (!marked[i]) { // Skip duplicates
                 printf("In %s %d, there are %d routes\n", 
                        MONTHS_TEXT[data_array[i][0] - 1], 
                        data_array[i][1], 
                        count_sub_array[i]);
             }
         }
     }
 
     // Free up memory when no longer needed
     for (int i = 0; i < count; i++) {
         free(data_array[i]);
     }
     free(data_array);
     free(count_sub_array);
     free(marked);
 }
     
 /*
  * Doer function to display histogram of port connectivity in the specified period
  * Param l collection variable to process
  */
 void most_connected(collection l)
 {
     // Variables for user input
     int input_year;
     int input_month;
     // Variables for processing
     collection_format current;
     char** ports_arr = NULL;
     int count = 0;
     char** unique_ports = NULL;
     int* port_count = NULL;
     int unique_count = 0;
     // Array of month names for output
     const char* MONTHS_TEXT[12] = { 
         "January", "February", "March", "April", "May", "June",
         "July", "August", "September", "October", "November", "December" 
     };
 
     printf("\nMost Connected Port for a Period\n");
     printf("-----------------------------------\n");
 
     // Enter input data
     printf("Enter year: ");
     scanf("%d", &input_year);
     printf("Enter month: ");
     scanf("%d", &input_month);
 
     // Validate month input
     if (input_month < 1) input_month = 1;
     if (input_month > 12) input_month = 12;
 
     current = l->first_period; // Initialize pointer variable
 
     // The loop retrieves each data in the current pointer variable
     while (current != NULL) {
         period current_period = (period)get_data(current);
         if (get_routes(current_period) != NULL) {
             route first_route = (route)get_data(get_routes(current_period));
             int month = get_month(first_route);
             int year = get_year(first_route);
 
             // Check conditions the time entered and the data time
             if (input_year == year && input_month == month) {
                 // Get all routes for this period
                 period_format route_node = get_routes(current_period);
                 while (route_node != NULL) {
                     route r = (route)get_data(route_node);
                     char* port1 = get_port1(r);
                     char* port2 = get_port2(r);
 
                     ports_arr = (char**)realloc(ports_arr, (count + 2) * sizeof(char*));
                     if (ports_arr == NULL) {
                         fprintf(stderr, "Memory allocation failed.\n");
                         return;
                     }
 
                     // Allocate memory and copy port1 value to ports_arr array
                     ports_arr[count] = (char*)malloc((strlen(port1) + 1) * sizeof(char));
                     strcpy(ports_arr[count], port1);
                     count++;
 
                     // Allocate memory and copy port2 value to ports_arr array
                     ports_arr[count] = (char*)malloc((strlen(port2) + 1) * sizeof(char));
                     strcpy(ports_arr[count], port2);
                     count++;
 
                     route_node = get_next(route_node);
                 }
                 break; // Found the period, no need to continue
             }
         }
         current = get_next(current);
     }
 
     // Check if the results are returned
     if (count == 0) {
         printf("\nNo route data for %s %d!\n", MONTHS_TEXT[input_month - 1], input_year);
     } else {
         unique_ports = (char**)malloc(count * sizeof(char*)); // Array for unique ports
         port_count = (int*)malloc(count * sizeof(int)); // Array for port counts
         
         // Find unique ports and count occurrences
         for (int i = 0; i < count; i++) {
             int found = 0;
 
             // Check if this port is already in unique_ports
             for (int j = 0; j < unique_count; j++) {
                 if (strcmp(unique_ports[j], ports_arr[i]) == 0) {
                     port_count[j]++;
                     found = 1;
                     break;
                 }
             }
 
             if (!found) {
                 unique_ports[unique_count] = (char*)malloc((strlen(ports_arr[i]) + 1) * sizeof(char));
                 strcpy(unique_ports[unique_count], ports_arr[i]);
                 port_count[unique_count] = 1;
                 unique_count++;
             }
         }
 
         // Sort ports alphabetically
         for (int i = 0; i < unique_count; i++) {
             for (int j = i + 1; j < unique_count; j++) {
                 if (strcmp(unique_ports[i], unique_ports[j]) > 0) {
                     // Swap port names
                     char* temp = unique_ports[i];
                     unique_ports[i] = unique_ports[j];
                     unique_ports[j] = temp;
                     
                     // Swap counts
                     int temp_count = port_count[i];
                     port_count[i] = port_count[j];
                     port_count[j] = temp_count;
                 }
             }
         }
 
         // Print histogram
         for (int i = 0; i < unique_count; i++) {
             printf("%-16s %-3d | ", unique_ports[i], port_count[i]);
             for (int j = 0; j < port_count[i]; j++) {
                 printf("*");
             }
             printf("\n");
         }
 
         // Free memory
         for (int i = 0; i < unique_count; i++) {
             free(unique_ports[i]);
         }
         free(unique_ports);
         free(port_count);
     }
 
     // Free port_arr memory
     for (int i = 0; i < count; i++) {
         free(ports_arr[i]);
     }
     free(ports_arr);
 }
 
 /*
  * Function to convert string to Lowercase type
  * Param str the string to convert
  */
 void convert_to_lowercase(char* str) {
     for (int i = 0; str[i] != '\0'; i++) {
         str[i] = tolower(str[i]);
     }
 }
 
 /*
  * Function to convert string to Capitalize type
  * Param str the string to convert
  */
 void convert_to_capitalize(char* str) {
     int capitalize_next = 1; // Start with capitalization
     for (int i = 0; str[i] != '\0'; i++) {
         if (isalpha(str[i])) {
             if (capitalize_next) {
                 str[i] = toupper(str[i]); // Convert to uppercase
                 capitalize_next = 0; // Do not capitalize next
             }
             else {
                 str[i] = tolower(str[i]); // Keep lowercase
             }
         }
         else {
             // Encountering a space or other character, begin the next capitalization
             capitalize_next = 1;
         }
     }
 }
 
 /*
  * Doer function to display the maximum and minimum cost of a route in real (current) dollars 
  * Param l collection variable to process
  */
 void costs_for_route(collection l)
 {
     // Variables for user input
     char first_port[100];
     char second_port[100];
     // Variables for processing
     collection_format current;
     double* arr_price_now = NULL;
     int count = 0;
     
     printf("\nRoute Costs\n");
     printf("--------------\n");
 
     // Enter input data
     printf("Enter first port in the route: ");
     scanf("%s", first_port);
     printf("Enter second port in the route: ");
     scanf("%s", second_port);
 
     // Convert input data to lowercase format
     convert_to_lowercase(first_port);
     convert_to_lowercase(second_port);
 
     current = l->first_period; // Initialize pointer variable
 
     // Search through all periods and routes
     while (current != NULL) {
         period current_period = (period)get_data(current);
         period_format route_node = get_routes(current_period);
         
         // Check all routes in this period
         while (route_node != NULL) {
             route r = (route)get_data(route_node);
             char* port1 = get_port1(r);
             char* port2 = get_port2(r);
             
             // Make copies for case-insensitive comparison
             char* port1_lower = strdup(port1);
             char* port2_lower = strdup(port2);
             convert_to_lowercase(port1_lower);
             convert_to_lowercase(port2_lower);
             
             // Check if this route matches our search
             if ((strcmp(port1_lower, first_port) == 0 && strcmp(port2_lower, second_port) == 0) || 
                 (strcmp(port2_lower, first_port) == 0 && strcmp(port1_lower, second_port) == 0)) {
                 
                 double current_price = get_price_now(r);
                 
                 // Add this price to our array
                 arr_price_now = (double*)realloc(arr_price_now, (count + 1) * sizeof(double));
                 if (arr_price_now == NULL) {
                     fprintf(stderr, "Memory allocation failed.\n");
                     free(port1_lower);
                     free(port2_lower);
                     return;
                 }
                 
                 arr_price_now[count] = current_price;
                 count++;
             }
             
             free(port1_lower);
             free(port2_lower);
             route_node = get_next(route_node);
         }
         
         current = get_next(current);
     }
 
     // Convert imported data to capitalize format for display
     convert_to_capitalize(first_port);
     convert_to_capitalize(second_port);
 
     // Check if the results are returned
     if (count == 0) {
         printf("\nNo route data between %s and %s!\n", first_port, second_port);
     } else {
         // Find min and max prices
         double min_value = arr_price_now[0]; 
         double max_value = arr_price_now[0];
 
         for (int i = 1; i < count; i++) {
             // Get the smallest value in the array
             if (arr_price_now[i] < min_value) {
                 min_value = arr_price_now[i];
             }
             // Get the largest value in the array
             if (arr_price_now[i] > max_value) {
                 max_value = arr_price_now[i];
             }
         }
         
         // Print out the results found
         printf("\nRoute between %s and %s has\n- minimum cost of $%.2lf\n- maximum cost of $%.2lf\n", 
                first_port, second_port, min_value, max_value);
     }
     
     // Free memory
     free(arr_price_now);
 }
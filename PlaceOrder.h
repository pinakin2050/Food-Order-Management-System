#ifndef ADDDISH_H_INCLUDED
#define ADDDISH_H_INCLUDED
    #include "AddDish.h"
#endif // ADDDISH_H_INCLUDED

#ifndef PLACEORDER_H_INCLUDED
#define PLACEORDER_H_INCLUDED

#define PATH_ORDER_DETAILS_FILE "E:\\Project Work\\Food Order Management System\\Code\\Food-Order-Management-System\\Data Files\\Order Details.txt"
#define SIZE_ORDER_ID_STRING 100
#define SIZE_ORDER_ID_ARRAY 50

///Gets the Dish ids from user to place order
void GetOrderDetails(void);

///Display the ordered dishes
TreeMatchedDish_t* PrepareDishListByDishType(TreeMatchedDish_t *root_node, FILE *fptr, const char* dish_type_to_display);

///Display the ordered dishes
void DisplayOrder(void);

///Prepares the ordered dish tree from the order details file
TreeMatchedDish_t* PrepareOrderList(TreeMatchedDish_t *root_node, const unsigned int *ptr_arr_order_details, const int size_arr_order_details);

///Generates the Bill
void GenerateBill(TreeMatchedDish_t *root_node);

static unsigned int bill_amount = 0;

#endif // PLACEORDER_H_INCLUDED

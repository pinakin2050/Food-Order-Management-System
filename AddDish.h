#ifndef STDIO_H_INCLUDED
    #define STDIO_H_INCLUDED
    #include <stdio.h>
#endif // STDIO_H_INCLUDED

#ifndef ADDDISH_H_INCLUDED
#define ADDDISH_H_INCLUDED

#define DISH_DETAILS_FILE_PATH "E:\\Project Work\\Food Order Management System\\Code\\Food-Order-Management-System\\Data Files\\Dish Details.txt"
#define TEMP_DISH_DETAILS_FILE_PATH "E:\\Project Work\\Food Order Management System\\Code\\Food-Order-Management-System\\Data Files\\Temp Dish Details File.txt"
#define SIZE_DISH_NAME 50
#define SIZE_DISH_TYPE 20
#define SIZE_DISH_TASTE_TYPE 20
#define SIZE_DISH_INGREDIENTS 1000

///Structure to store the dish records
typedef struct Dish
{
    unsigned short dish_id;
    unsigned short dish_price;
    unsigned short dish_suitable_person_count;
    float dish_rating;
    char dish_name[SIZE_DISH_NAME];
    char dish_type[SIZE_DISH_TYPE]; ///Starter, Main Course, Dessert, Beverage
    char dish_taste_type[SIZE_DISH_TASTE_TYPE];
    char dish_ingredients[SIZE_DISH_INGREDIENTS];
    char dish_availability_status;

} Dish_t;

///Structure to store the records of matched dish fetched through searching
typedef struct TreeMatchedDish
{
    struct TreeMatchedDish *left_link;
    Dish_t dish_node;
    struct TreeMatchedDish *right_link;
    long int record_position;
}TreeMatchedDish_t;

///Adds the New Dish in the Dish data File
void AddDishDetails(void);

///Verifies that duplicate dish is not getting added in the Dish data file
int VerifyDishDetails(Dish_t *ptr_new_dish, FILE *fptr);

///Updates the details of desired dish
void UpdateDishDetails(void);

///Searches for the dish name and prepares the tree of matched dishes
TreeMatchedDish_t* SearchDish(TreeMatchedDish_t *root_node, FILE *fptr);

/// Displays the list of matched dish records
void DisplayMatchedDishRecords(const TreeMatchedDish_t *root_node);

///Deletes the dish record
void DeleteDish(void);

///Deletes the tree of matched dish records
void DeleteMatchedDishTree(TreeMatchedDish_t *root);

#endif // ADDDISH_H_INCLUDED

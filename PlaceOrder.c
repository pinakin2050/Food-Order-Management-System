#include "AddDish.h"
#include "PlaceOrder.h"
#include "CommonFunctions.h"

///Gets the Dish ids from user to place order
void GetOrderDetails()
{
    TreeMatchedDish_t *root_node_dish_list = NULL;

    FILE *fptr_dish_details = fopen(DISH_DETAILS_FILE_PATH, "rb");
    AssertFileState(fptr_dish_details);

    FILE *fptr_order_details = fopen(PATH_ORDER_DETAILS_FILE, "a+");
    AssertFileState(fptr_order_details);

    char *str_order_id = (char *)malloc(SIZE_ORDER_ID_STRING);
    char choice;

    label_select_more_dish:
    printf("Press 1 to view List of Starters.\n");
    printf("Press 2 to view List of Main Course.\n");
    printf("Press 3 to view List of Desserts.\n");
    printf("Press 4 to view List of Beverages.\n");
    printf("Enter your choice: ");
    scanf(" %c", &choice);

    ClearScreen();

    ///Prepare the dish list based on the dish type.
    switch(choice)
    {
        case '1': root_node_dish_list = NULL;
                  root_node_dish_list = PrepareDishListByDishType(root_node_dish_list, fptr_dish_details, "Starter");
                  break;

        case '2': root_node_dish_list = NULL;
                  root_node_dish_list = PrepareDishListByDishType(root_node_dish_list, fptr_dish_details, "Main Course");
                  break;

        case '3': root_node_dish_list = NULL;
                  root_node_dish_list = PrepareDishListByDishType(root_node_dish_list, fptr_dish_details, "Dessert");
                  break;

        case '4': root_node_dish_list = NULL;
                  root_node_dish_list = PrepareDishListByDishType(root_node_dish_list, fptr_dish_details, "Beverage");
                  break;

        default: printf("Invalid Choice! Do you want to try again? [Y/N]: ");
                 scanf(" %c", &choice);

                 if(choice == 'Y' || choice == 'y')
                 {
                     ClearScreen();
                     goto label_select_more_dish;
                 }
                 else
                 {
                     ClearScreen();
                     goto label_display_dish_list_function_end;
                 }
    }

    if(root_node_dish_list == NULL)
    {
        printf("No Dish Found! Do you want to try again for another type of dish? [Y/N]: ");
        scanf(" %c", &choice);

        if(choice == 'Y' || 'y')
        {
            ClearScreen();
            goto label_select_more_dish;
        }
        else
        {
            ClearScreen();
           goto label_display_dish_list_function_end;
        }
    }

    ///Display dish list
    DisplayMatchedDishRecords(root_node_dish_list);

    printf("Enter Ids of dishes you want to order in comma separated manner: ");
    scanf(" %[^\n]s", str_order_id);

    ///Get the dish id(s) and save to Order Details file.
    if(fprintf(fptr_order_details,"%s\n",str_order_id) <= 0)
    {
        ClearScreen();
        printf("\nError occurred while writing order details in file!\n");
        fclose(fptr_dish_details);
        fclose(fptr_order_details);
        exit(EXIT_FAILURE);
    }

    ClearScreen();

    printf("\nDo you want to select more dishes? [Y/N]: ");
    scanf(" %c", &choice);

    if(choice == 'Y' || choice == 'y')
    {
        memset(str_order_id, '\0', SIZE_ORDER_ID_STRING);
        DeleteMatchedDishTree(root_node_dish_list);
        ClearScreen();
        goto label_select_more_dish;
    }

    label_display_dish_list_function_end:
    DeleteMatchedDishTree(root_node_dish_list);
    fclose(fptr_dish_details);
    fclose(fptr_order_details);
}


///Prepares the tree of list based on the type of dish passed
TreeMatchedDish_t* PrepareDishListByDishType(TreeMatchedDish_t *root_node, FILE *fptr, const char* dish_type_to_display)
{
    unsigned int size_matched_dish_tree = 0;

    Dish_t temp_dish_object;

    rewind(fptr);

    while(fread(&temp_dish_object, sizeof(Dish_t), 1, fptr) != 0)
    {
        if((strcasecmp(dish_type_to_display, temp_dish_object.dish_type) == 0) && (temp_dish_object.dish_availability_status == 'Y' || temp_dish_object.dish_availability_status == 'y'))
        {
            ++size_matched_dish_tree;

            TreeMatchedDish_t *new_matched_dish = (TreeMatchedDish_t *)calloc(1, sizeof(TreeMatchedDish_t));

            ///Store matched dish name in new node of the search list tree
            new_matched_dish -> dish_node = temp_dish_object;

            ///Store the position of the matched record
            new_matched_dish -> record_position = ftell(fptr) - sizeof(Dish_t);

            if(root_node == NULL)
            {
                root_node = new_matched_dish;
            }
            else
            {
                TreeMatchedDish_t *temp_root_node = root_node;

                while(1)
                {
                    if(new_matched_dish->dish_node.dish_id < temp_root_node->dish_node.dish_id)
                    {
                       if(temp_root_node -> left_link == NULL)
                       {
                            temp_root_node -> left_link = new_matched_dish;
                            break;
                       }

                       temp_root_node = temp_root_node -> left_link;
                    }
                    else
                    {
                       if(temp_root_node -> right_link == NULL)
                       {
                            temp_root_node -> right_link = new_matched_dish;
                            break;
                       }

                       temp_root_node = temp_root_node -> right_link;
                    }
                }
            }
        }
    }

    if(size_matched_dish_tree == 0)
    {
        root_node = NULL;
    }

    return root_node;
}


///Display the ordered dishes
void DisplayOrder()
{
    FILE *fptr_order_details = fopen(PATH_ORDER_DETAILS_FILE, "r");
    AssertFileState(fptr_order_details);

    char *str_order_id = (char *)malloc(SIZE_ORDER_ID_STRING);
    char *token;
    unsigned int arr_order_id[SIZE_ORDER_ID_ARRAY], index_arr_order_id = 0;

    while(fscanf(fptr_order_details, "%s", str_order_id) > 0)
    {
        token = strtok(str_order_id, ",");

        while(token != NULL)
        {
            arr_order_id[index_arr_order_id++] = atoi(token);
            token = strtok(NULL, ",");
        }

        memset(str_order_id, '\0', SIZE_ORDER_ID_STRING);
    }

    TreeMatchedDish_t *root_node_order_list = NULL;
    root_node_order_list = PrepareOrderList(root_node_order_list, arr_order_id, index_arr_order_id);

    ClearScreen();

    DisplayMatchedDishRecords(root_node_order_list);

    if(root_node_order_list != NULL)
    {
        printf("\nConfirm and place the order? [Y/N]: \n");
        char choice;
        scanf(" %c", &choice);

        if(choice == 'Y' || choice == 'y')
        {
            ClearScreen();
            printf("\nThe order has been placed successfully.\n");
        }

        printf("\nDo you want to generate bill? [Y/N]: ");
        scanf(" %c", &choice);

        if(choice == 'Y' || choice == 'y')
        {
            ClearScreen();
            printf("\n********* Your Bill *********\n");
            GenerateBill(root_node_order_list);
            printf("\nTotal Bill Amount = %u INR", bill_amount);

            fflush(stdin);
            printf("\n\nPress Any Key to Exit.\n");
            getchar();
        }
    }

    fclose(fptr_order_details);

    remove(PATH_ORDER_DETAILS_FILE);

    DeleteMatchedDishTree(root_node_order_list);
}


///Prepares the ordered dish tree from the order details file
TreeMatchedDish_t* PrepareOrderList(TreeMatchedDish_t *root_node, const unsigned int *ptr_arr_order_details, const int size_arr_order_details)
{
    unsigned int size_matched_dish_tree = 0;

    FILE *fptr = fopen(DISH_DETAILS_FILE_PATH, "rb");
    AssertFileState(fptr);

    Dish_t temp_dish_object;

    for(unsigned int i = 0; i < size_arr_order_details; ++i)
    {
        rewind(fptr);

       while(fread(&temp_dish_object, sizeof(Dish_t), 1, fptr) != 0)
        {
            if(temp_dish_object.dish_id == ptr_arr_order_details[i])
            {
                ++size_matched_dish_tree;

                TreeMatchedDish_t *new_matched_dish = (TreeMatchedDish_t *)calloc(1, sizeof(TreeMatchedDish_t));

                ///Store matched dish name in new node of the search list tree
                new_matched_dish -> dish_node = temp_dish_object;

                ///Store the position of the matched record
                new_matched_dish -> record_position = ftell(fptr) - sizeof(Dish_t);

                if(root_node == NULL)
                {
                    root_node = new_matched_dish;
                    break;
                }
                else
                {
                    TreeMatchedDish_t *temp_root_node = root_node;

                    while(1)
                    {
                        if(new_matched_dish->dish_node.dish_id <= temp_root_node->dish_node.dish_id)
                        {
                            if(temp_root_node -> left_link == NULL)
                            {
                                    temp_root_node -> left_link = new_matched_dish;
                                    break;
                            }

                            temp_root_node = temp_root_node -> left_link;
                        }
                        else
                        {
                            if(temp_root_node -> right_link == NULL)
                            {
                                    temp_root_node -> right_link = new_matched_dish;
                                    break;
                            }

                            temp_root_node = temp_root_node -> right_link;
                        }
                    }

                    break;
                }
            }
        }
    }

    if(size_matched_dish_tree == 0)
    {
        root_node = NULL;
    }

    fclose(fptr);

    return root_node;
}


///Generates the Bill
void GenerateBill(TreeMatchedDish_t *root_node)
{
    if(root_node != NULL)
    {
        GenerateBill(root_node->left_link);

        printf("1 X %s = %u INR\n", root_node->dish_node.dish_name, root_node->dish_node.dish_price);
        bill_amount += root_node->dish_node.dish_price;

        GenerateBill(root_node->right_link);
    }
}

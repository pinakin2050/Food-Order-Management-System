#include "AddDish.h"
#include "CommonFunctions.h"

///Adds the New Dish in the Dish data File
void AddDishDetails()
{
    FILE *fptr = fopen(DISH_DETAILS_FILE_PATH, "a+b");

    ///If can't open file then print error message and exit.
    AssertFileState(fptr);

    char ch;

    Dish_t new_dish;

    do
    {
        ///Go to the end of file for adding the new dish record at the end
        fseek(fptr, 0, SEEK_END);

        ///if no record is present then make dish_id as 1
        if(ftell(fptr) == 0)
        {
            new_dish.dish_id = 1;
        }
        else ///Else get the id from last record & increment it by one
        {
            ///Go to the start of the last record
            fseek(fptr, -sizeof(Dish_t), SEEK_CUR);

            Dish_t previous_dish;

            ///Read last record and exit if can't read
            if(fread(&previous_dish, sizeof(Dish_t), 1, fptr) != 1)
            {
                printf("\nCan't read the last record of the Add Dish Details file.\n");
                fclose(fptr);
                exit(EXIT_FAILURE);
            }

            ///Store latest id in the new record
            new_dish.dish_id = previous_dish.dish_id + 1;

            memset(&previous_dish, '\0', sizeof(Dish_t));
        }

        ClearScreen();

        printf("Enter Dish Name[%u]: ", (unsigned int)sizeof(new_dish.dish_name));
        scanf(" %[^\n]s", &new_dish.dish_name);


        ///Check if added dish already exists
        if(VerifyDishDetails(&new_dish, fptr))
        {
            printf("\nThe dish already exist! Kindly add different dish.\n");
            goto label_add_more_dish;
        }

        printf("Enter Dish type[Starter, Main Course, Dessert, Beverage]: ");
        scanf(" %[^\n]s", &new_dish.dish_type);

        printf("Enter Dish Taste [%u]: ", (unsigned int)sizeof(new_dish.dish_taste_type));
        scanf(" %[^\n]s", &new_dish.dish_taste_type);

        printf("Enter the main ingredients of the dish [%u]: ", (unsigned int)sizeof(new_dish.dish_ingredients));
        scanf(" %[^\n]s", &new_dish.dish_ingredients);

        printf("Enter the price of the dish: ");
        scanf("%hd", &new_dish.dish_price);

        printf("Enter the total number of person for whom this dish is suitable: ");
        scanf("%hd", &new_dish.dish_suitable_person_count);

        new_dish.dish_availability_status = 'Y';

        new_dish.dish_rating = 0.0f;

        ///If write operation is successful then go ahead else exit with failure status
        if(fwrite(&new_dish, sizeof(Dish_t), 1, fptr) != 0)
        {
            printf("\n\nThe dish is added successfully.\n\n");
        }
        else
        {
            printf("\n\nError occurred while adding the dish.\n\n");
            fclose(fptr);
            exit(EXIT_FAILURE);
        }

        label_add_more_dish:
        printf("Do you want to add more dish? [Y/N]: ");
        scanf(" %c", &ch);

        ///Clear the object for next data.
        memset(&new_dish, '\0', sizeof(Dish_t));

        ClearScreen();

    }while(ch == 'Y' || ch == 'y');

    fclose(fptr);
}


///Verifies that duplicate dish is not getting added in the Dish data file
int VerifyDishDetails(Dish_t *ptr_new_dish, FILE *fptr)
{
    ///Go to the end of the file
    fseek(fptr, 0, SEEK_END);

    char duplicate_dish_flag = 0;

    ///If file is empty then return false
    if(ftell(fptr) == 0)
    {
        return duplicate_dish_flag;
    }

    Dish_t temp_dish;

    ///Go to the beginning of the file
    //fseek(fptr, 0, SEEK_SET);
    rewind(fptr);

    while(fread(&temp_dish, sizeof(Dish_t), 1, fptr) != 0)
    {
        ///If added dish name is already present then update duplicate dish flag to true
        if(strcasecmp(temp_dish.dish_name, ptr_new_dish->dish_name) == 0)
        {
            duplicate_dish_flag = 1;
            break;
        }
    }

    ///Go to file end before returning
    fseek(fptr, 0, SEEK_END);

    return duplicate_dish_flag;
}


///Updates the details of desired dish
void UpdateDishDetails()
{
    TreeMatchedDish_t *root_node_matched_dish_tree = NULL;

    FILE *fptr = fopen(DISH_DETAILS_FILE_PATH, "r+b");

    AssertFileState(fptr);

    label_search_dish:
    root_node_matched_dish_tree = NULL;
    root_node_matched_dish_tree = SearchDish(root_node_matched_dish_tree, fptr);

    ///Go to end of function if search list is empty
    if(root_node_matched_dish_tree == NULL)
    {
        goto label_update_details_function_end;
    }

    label_display_matched_list:
    DisplayMatchedDishRecords(root_node_matched_dish_tree);

    ///Get the record id to edit the record
    unsigned int id_record_to_update;

    printf("\nEnter Id of the record to update: ");
    scanf("%u", &id_record_to_update);

    TreeMatchedDish_t *temp_root_node = root_node_matched_dish_tree;
    while(temp_root_node != NULL)
    {
        ///Go to left sub-tree if ID is lesser than root node id
        if(id_record_to_update < temp_root_node->dish_node.dish_id)
        {
            if(temp_root_node->left_link == NULL)
            {
                ClearScreen();
                printf("\nId is not found! Please enter valid id.\n");

                printf("\nDo you want to try again?[Y/N]: ");
                char temp_ch;
                scanf(" %c", &temp_ch);

                if(temp_ch == 'Y' || temp_ch == 'y')
                {
                    ClearScreen();
                    goto label_display_matched_list;
                }
                else
                {
                    DeleteMatchedDishTree(root_node_matched_dish_tree);
                    goto label_search_again;
                }
            }

            temp_root_node = temp_root_node->left_link;
        }
        else if(id_record_to_update > temp_root_node->dish_node.dish_id)
        {
            if(temp_root_node->right_link == NULL)
            {
                ClearScreen();
                printf("\nId is not found! Please enter valid Id.\n");

                printf("\nDo you want to try again?[Y/N]: ");
                char temp_ch;
                scanf(" %c", &temp_ch);

                if(temp_ch == 'Y' || temp_ch == 'y')
                {
                    ClearScreen();
                    goto label_display_matched_list;
                }
                else
                {
                    DeleteMatchedDishTree(root_node_matched_dish_tree);
                    goto label_search_again;
                }
            }

            temp_root_node = temp_root_node->right_link;
        }
        else
        {
            label_update_dish_details:
            printf("Press 1 to edit dish name.\n");
            printf("Press 2 to edit dish type.\n");
            printf("Press 3 to edit dish taste type.\n");
            printf("Press 4 to edit dish ingredients.\n");
            printf("Press 5 to edit dish price.\n");
            printf("Press 6 to edit suitable person count.\n");
            printf("Press 7 to edit dish availability status.\n");
            printf("Enter your choice: ");
            int choice;
            scanf("%d", &choice);

            switch(choice)
            {
                case 1: printf("\nEnter new name for the dish[%d]: ", SIZE_DISH_NAME);
                        char dish_new_name[SIZE_DISH_NAME];
                        scanf(" %[^\n]s", &dish_new_name);
                        strcpy(temp_root_node->dish_node.dish_name, dish_new_name);
                        printf("\nThe dish name is updated successfully.\n");
                        break;

                case 2: printf("\nEnter new type for the dish[%d]: ", SIZE_DISH_TYPE);
                        char dish_new_type[SIZE_DISH_TYPE];
                        scanf(" %[^\n]s", &dish_new_type);
                        strcpy(temp_root_node->dish_node.dish_type, dish_new_type);
                        printf("\nThe dish type is updated successfully.\n");
                        break;

                case 3: printf("\nEnter new taste type for the dish[%d]: ", SIZE_DISH_TASTE_TYPE);
                        char dish_new_taste_type[SIZE_DISH_TASTE_TYPE];
                        scanf(" %[^\n]s", &dish_new_taste_type);
                        strcpy(temp_root_node->dish_node.dish_taste_type, dish_new_taste_type);
                        printf("\nThe dish taste type is updated successfully.\n");
                        break;

                case 4: printf("\nEnter new ingredients for the dish[%d]: ", SIZE_DISH_INGREDIENTS);
                        char dish_new_ingredients[SIZE_DISH_INGREDIENTS];
                        scanf(" %[^\n]s", &dish_new_ingredients);
                        strcpy(temp_root_node->dish_node.dish_ingredients, dish_new_ingredients);
                        printf("\nThe dish ingredients is updated successfully.\n");
                        break;

                case 5: printf("\nEnter new price for the dish: ");
                        unsigned short dish_new_price;
                        scanf("%hd", &dish_new_price);
                        temp_root_node->dish_node.dish_price = dish_new_price;
                        printf("\nThe dish price is updated successfully.\n");
                        break;

                case 6: printf("\nEnter new suitable person count: ");
                        unsigned short dish_new_suitable_person_count;
                        scanf("%hd", &dish_new_suitable_person_count);
                        temp_root_node->dish_node.dish_suitable_person_count = dish_new_suitable_person_count;
                        printf("\nThe dish suitable person count is updated successfully.\n");
                        break;

                case 7: printf("\nEnter new availability status for the dish[Y/N]: ");
                        //char dish_new_availabilty_status;
                        //scanf(" %c", &dish_new_availabilty_status);
                        scanf(" %c", &(temp_root_node->dish_node.dish_availability_status));
                        //temp_root_node->dish_node.dish_availability_status = toupper(dish_new_availabilty_status);
                        printf("\nThe dish availability status is updated successfully.\n");
                        break;

                default: printf("\nInvalid choice! Please enter correct choice.\n");
                         printf("Do you want to try again?[Y/N]: ");
                         char temp_ch;
                         scanf(" %c", &temp_ch);

                         if(temp_ch == 'Y' || temp_ch == 'y')
                         {
                             goto label_update_dish_details;
                         }
                         else
                         {
                             goto label_search_again;
                         }
            }

            ClearScreen();

            printf("\nDo you want to update other details for current dish?[Y/N]: ");
            char temp_ch;
            scanf(" %c", &temp_ch);

            if(temp_ch == 'Y' || temp_ch == 'y')
            {
                ClearScreen();
                goto label_update_dish_details;
            }

            ///Set the pointer at the beginning of the record to update data.
            fseek(fptr, (temp_root_node->record_position), SEEK_SET);

            ///Update the existing record with new details.
            if(fwrite(&(temp_root_node->dish_node), sizeof(Dish_t), 1, fptr))
            {
                printf("\nThe data is saved successfully.\n");
                break;
            }
            else
            {
                printf("\nFailure occurred while saving data!\n");
                fclose(fptr);
                exit(EXIT_FAILURE);
            }
        }
    }

    printf("\nDo you want to edit another record from the matched records?[Y/N]: ");
    char temp_ch;
    scanf(" %c", &temp_ch);

    if(temp_ch == 'Y' || temp_ch == 'y')
    {
        ClearScreen();
        goto label_display_matched_list;
    }

    label_search_again:
    printf("\nDo you want to search again for new dish?[Y/N]: ");
    scanf(" %c", &temp_ch);

    if(temp_ch == 'Y' || temp_ch == 'y')
    {
        ClearScreen();
        DeleteMatchedDishTree(root_node_matched_dish_tree);
        goto label_search_dish;
    }

    label_update_details_function_end:
    ClearScreen();
    fclose(fptr);
}


///Searches for the dish name and prepares the Tree of matched dishes
TreeMatchedDish_t* SearchDish(TreeMatchedDish_t *root_node, FILE *fptr)
{
    unsigned int size_matched_dish_tree = 0;
    char dish_name_to_search[SIZE_DISH_NAME], temp_matched_dish_name[SIZE_DISH_NAME];

    ///Using the label to search again in case if no dish is found
    label_search_dish:
    rewind(fptr);

    ClearScreen();

    printf("Enter Dish Name to search: ");
    scanf(" %[^\n]s", &dish_name_to_search);

    Dish_t temp_dish_object;

    while(fread(&temp_dish_object, sizeof(Dish_t), 1, fptr) != 0)
    {
        ///Storing Dish name in temp string to avoid lower case conversion of actual data
        strcpy(temp_matched_dish_name, temp_dish_object.dish_name);

        if(strstr(strlwr(temp_matched_dish_name), strlwr(dish_name_to_search)) != NULL)
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
        printf("\nNo Records found!!\n\nDo you want to search for other dish?[Y/N]: ");

        char choice;
        scanf(" %c", &choice);

        if(choice == 'Y' || choice == 'y')
        {
            ClearScreen();
            goto label_search_dish;
        }
    }

    return root_node;
}


/// Displays the list of matched records
void DisplayMatchedDishRecords(const TreeMatchedDish_t *root_node)
{
    if(root_node != NULL)
    {
        DisplayMatchedDishRecords(root_node->left_link);

        printf("\nId: %u\n", root_node->dish_node.dish_id);
        printf("Name: %s\n", root_node->dish_node.dish_name);
        printf("Type: %s\n", root_node->dish_node.dish_type);
        printf("Taste Type: %s\n", root_node->dish_node.dish_taste_type);
        printf("Ingredients: %s\n", root_node->dish_node.dish_ingredients);
        printf("Price: %u\n", root_node->dish_node.dish_price);
        printf("Suitable Person Count: %u\n", root_node->dish_node.dish_suitable_person_count);
        printf("Availability Status: %s\n\n", ((root_node->dish_node.dish_availability_status == 'Y') || (root_node->dish_node.dish_availability_status == 'y')) ? "Available": "Not Available");

        DisplayMatchedDishRecords(root_node->right_link);
    }
}


///Deletes the dish record
void DeleteDish()
{
    FILE *fptr_existing_file = fopen(DISH_DETAILS_FILE_PATH, "r+b");

    AssertFileState(fptr_existing_file);

    TreeMatchedDish_t *root_node_matched_dish_tree = NULL;
    root_node_matched_dish_tree = SearchDish(root_node_matched_dish_tree, fptr_existing_file);

    ///Go to end of function if search list is empty
    if(root_node_matched_dish_tree == NULL)
    {
        goto label_delete_record_function_end;
    }

    label_display_matched_list:
    DisplayMatchedDishRecords(root_node_matched_dish_tree);

    ///Get the record id to delete the record
    unsigned int id_record_to_delete;

    printf("\nEnter Id of the record to delete: ");
    scanf("%u", &id_record_to_delete);

    TreeMatchedDish_t *temp_root_node = root_node_matched_dish_tree;

    while(temp_root_node != NULL)
    {
        if(id_record_to_delete < temp_root_node->dish_node.dish_id)
        {
            if(temp_root_node->left_link == NULL)
            {
                ClearScreen();
                printf("\nId is not found! Please enter valid Id.\n");

                printf("\nDo you want to try again?[Y/N]: ");
                char temp_ch;
                scanf(" %c", &temp_ch);

                if(temp_ch == 'Y' || temp_ch == 'y')
                {
                    ClearScreen();
                    goto label_display_matched_list;
                }
                else
                {
                    fclose(fptr_existing_file);
                    goto label_delete_record_function_end;
                }
            }

            temp_root_node = temp_root_node->left_link;
        }
        else if(id_record_to_delete > temp_root_node->dish_node.dish_id)
        {
            if(temp_root_node->right_link == NULL)
            {
                ClearScreen();
                printf("\nId is not found! Please enter valid Id.\n");

                printf("\nDo you want to try again?[Y/N]: ");
                char temp_ch;
                scanf(" %c", &temp_ch);

                if(temp_ch == 'Y' || temp_ch == 'y')
                {
                    ClearScreen();
                    goto label_display_matched_list;
                }
                else
                {
                    fclose(fptr_existing_file);
                    goto label_delete_record_function_end;
                }
            }

            temp_root_node = temp_root_node->right_link;
        }
        else
        {
            break;
        }
    }

    Dish_t temp_dish_object;

    unsigned char count_deleted_record = 0;

    rewind(fptr_existing_file);

    FILE *fptr_new_file = fopen(TEMP_DISH_DETAILS_FILE_PATH,"a+b");

    AssertFileState(fptr_new_file);

    while(fread(&temp_dish_object, sizeof(Dish_t), 1, fptr_existing_file) != 0)
    {
        if(temp_dish_object.dish_id != id_record_to_delete)
        {
            if(fwrite(&temp_dish_object, sizeof(Dish_t), 1, fptr_new_file) != 1)
            {
                printf("\n Error Occurred while writing to new file.\n");
                fclose(fptr_existing_file);
                fclose(fptr_new_file);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            ++count_deleted_record;
        }
    }

    if(count_deleted_record == 0)
    {
        printf("\nCouldn't delete the record! Please try again.\n");

        fclose(fptr_existing_file);
        fclose(fptr_new_file);

        if(remove(TEMP_DISH_DETAILS_FILE_PATH) != 0)
        {
            printf("\nCouldn't delete the temporary file!\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("\nThe record is deleted successfully.\n");

        fflush(stdin);
        printf("\n\nPress Any Key to Exit.\n");
        getchar();

        fclose(fptr_existing_file);
        fclose(fptr_new_file);

        if(remove(DISH_DETAILS_FILE_PATH) != 0)
        {
            printf("\nCouldn't delete the old file!\n");
            exit(EXIT_FAILURE);
        }

        if(rename(TEMP_DISH_DETAILS_FILE_PATH, DISH_DETAILS_FILE_PATH) != 0)
        {
            printf("\nCouldn't rename the temporary file!\n");
            exit(EXIT_FAILURE);
        }
    }

    label_delete_record_function_end:
    ///Delete the tree of matched dish records
    DeleteMatchedDishTree(root_node_matched_dish_tree);
}


///Deletes the tree of matched dish records
void DeleteMatchedDishTree(TreeMatchedDish_t *root)
{
    if(root != NULL)
    {
        DeleteMatchedDishTree(root->left_link);
        DeleteMatchedDishTree(root->right_link);
        free(root);
    }
}


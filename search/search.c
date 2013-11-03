#include "search.h"

int main(int argc, char **argv)
{
    FILE *input;
    char operator[50];
    char term[1000];
    char **terms = calloc(100, sizeof(char *));
    int i = 0;
    int result;
    char discard;
    Trie *file_contents;
    SortedListPtr result_list;
    SortedListIteratorPtr iter;
    void *list_item;

    //Check number of arguments
    if(argc != 2)
    {
        fprintf(stderr, "ERROR: Wrong number of arguments.\n");
        return 1;
    }

    //Open file
    input = fopen(argv[1], "r");

    //Check that it worked
    if(input == NULL)
    {
        fprintf(stderr, "Unable to open file %s.\n", argv[1]);
        return 2;
    }

    file_contents = preprocess_file(input);

    //Get queries
    printf("Enter a query. Enter q to quit\n");
    while((scanf("%s", operator)) != EOF)
    {
        if(strcmp(operator, "q") == 0)
        {
            break;
        }

        //Get terms
        while((result = scanf("%s %[^\n]", term, &discard)) > 0)
        {
            terms[i] = malloc(sizeof(term));
            strcpy(terms[i], term);
            i++;
            if(result == 2) break;
        }

        //Realloc terms
        terms = realloc(terms, i*sizeof(char *));

        //Check if or query
        if(strcmp(operator, "so") == 0)
        {
            result_list = or_query(terms, file_contents);
        }
        //Check if and query
        else if(strcmp(operator, "sa") == 0)
        {
            result_list = and_query(terms, file_contents);
        }
        //Check for invalid query
        else
        {
            fprintf(stderr, "Invalid query. Use sa or so.\n");
            continue;
        }

        //Print results
        iter = SLCreateIterator(result_list);
        while((list_item = SLNextItem(iter)) != NULL)
        {
            printf("%s ", ((FileNode *) list_item)->file_name);
        }

        //Prompt for new query
        printf("Enter a query. Enter q to quit\n");
    }

    //Free terms
    while(i >= 0)
    {
        free(terms[i]);
    }
    free(terms);

    //Free trie
    destroy_trie(file_contents);


    return 0;

}

/*
 * Preprocess file to make queries fast
 */
Trie *preprocess_file(FILE *input)
{
    int fscanf_result = 1;
    char term[1000];
    char file[1000];
    void *dummy[1000];
    Trie *trie = create_trie(destroy_data_in_sorted_list, insert_data_into_sorted_list);
    TrieNode *curr_node;
    FileNode *fnode;

    while(fscanf_result != EOF)
    {
        //First, get term
        fscanf_result = fscanf(input, "%s %s", (char *)dummy, term);
        if(fscanf_result < 2)
        {
            continue;
        }

        //Next, add the term to the trie
        insert_word(term, NULL, trie);
        curr_node = find_word(term, trie);

        //Term is in trie, now add files
        do {
            fscanf_result = fscanf(input, "%s %d", file, (int *)dummy);
            fnode = create_filenode(file);
            trie->insert_data(curr_node, (void *) fnode);
        } while(fscanf_result > 1);
    }
    return trie;
}


/*
 * Handles OR queries
 */
SortedListPtr or_query(char **terms, Trie *trie)
{
    SortedListPtr list = SLCreate(compare_file_nodes);
    SortedListIteratorPtr iter;
    TrieNode *found;
    void *list_item;
    int i = 0;

    while(terms[i] != NULL)
    {
        found = find_word(terms[i], trie);
        if(found != NULL && found->data != NULL)
        {
            //Create an iterator and add everything to our output list
            iter = SLCreateIterator((SortedListPtr) found->data);
            while((list_item = SLNextItem(iter)) != NULL)
            {
                SLInsert(list, list_item);
            }
            SLDestroyIterator(iter);
        }
    }

    return list;
}


/*
 * Destroys a sorted list
 */
void destroy_data_in_sorted_list(void *data)
{
    void *list_item;
    SortedListPtr list = (SortedListPtr) data;
    SortedListIteratorPtr iter = SLCreateIterator(list);

    //Free all the file nodes
    while((list_item = SLNextItem(iter)) != NULL)
    {
        destroy_filenode((FileNode *) list_item);
    }
    SLDestroyIterator(iter);

    //Free list
    SLDestroy(list);
}

/*
 * Inserts our data into a sorted list
 */
void insert_data_into_sorted_list(TrieNode *node, void *data)
{
    if(node->data == NULL)
    {
        SLCreate(compare_file_nodes);
    }

    SLInsert((SortedListPtr) node->data, data);
}

/*
 * Compares file nodes alphabetically by file name
 */
int compare_file_nodes(void *p1, void *p2)
{
    FileNode *f1 = (FileNode *) p1;
    FileNode *f2 = (FileNode *) p2;

    return strcmp(f1->file_name, f2->file_name);
}

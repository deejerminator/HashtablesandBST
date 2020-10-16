#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
#include <iomanip>
using namespace std;

#define CAT_NAME_LEN    25
#define APP_NAME_LEN    50
#define VERSION_LEN     20
#define UNIT_SIZE       3
// =================================================================
// STRUCTS INFO
struct app_info{
    char category[CAT_NAME_LEN];   // name of category
    char app_name[APP_NAME_LEN];   // name of application
    char version[VERSION_LEN];     // version number
    float size;                    // size of application
    char units[UNIT_SIZE];         // GB or MB
    float price;                   // price in $ of the application
};

struct tree{
    struct app_info record;
    struct tree *left;
    struct tree *right;
};

struct categories{
    char category[CAT_NAME_LEN];    // name of category
    struct tree *root;              // pointer to root of search tree for this category
};

struct hash_table_entry{
    char app_name[APP_NAME_LEN]; // name of the application
    struct tree *app_node;       // pointer to node in the tree containing the application
    struct hash_table_entry *next;
};
// =================================================================
// FUNCTIONS RELATED TO HASH TABLES
bool isPrime(int item){         // test to see if the current number is prime, defined from project description
    int limit, factor = 2;
    limit = (long)( sqrtf( (float) item ) + 0.5f );
    while( (factor <= limit) && (item % factor) )
        factor++;
    return( factor > limit );
}

int prime(int item){            // function to find the nearest prime number to the given amount of apps.
    item = item * 2;            // item * 2 because of project definition
    bool primetest = false;

    while(primetest != true){   // if the current number isn't a prime, we  + 1, to try and find the nearest prime.
        primetest = isPrime(item);
        if (primetest == false)
            item++;
    }
    return item;                // return item after finding the nearest prime number.
}

int modulo(string appName, int key){ // simple function to add all ascii values of the string, and modulo the prime number
                                    // this helps find the index the app belongs to.
    int temp = 0;

    for(int i = 0; i < appName.length(); i++){
        temp = int(appName[i]) + temp;
    }
    return  temp % key;
}

void hash_insert(struct tree* node, struct hash_table_entry *temp[], int tableSize){

    int index = modulo(node->record.app_name, tableSize);       // calls modulo function to find index
    struct hash_table_entry* newEntry = new hash_table_entry(); // allocate memory for new hashtable entry

    newEntry->app_node = node;                                  // update hash entry with appdata
    strcpy(newEntry->app_name, node->record.app_name);
    newEntry->next = temp[index];                               // push currentApp to the head of list, if list is null, newEntry->next
    temp[index] = newEntry;                                    // allocates more memory and makes it NULL in case of new data
}

void search(string item, hash_table_entry *hashTable[], int tableSize){
    const char *newstring = item.c_str();               // created newString for strcmp.
    int index = modulo(item, tableSize);                // finds the index where item should be at.
    hash_table_entry *temp = hashTable[index];          // created a temp to make sure not to overwrite actual data.

    while(temp != NULL){
        if(strcmp(temp->app_name, newstring) == 0){
            cout << "Found Application: " << temp->app_name << endl;
            cout << "\tCategory: " << temp->app_node->record.category << endl;
            cout << "\tApplication Name: " << temp->app_node->record.app_name << endl;
            cout << "\tVersion: " << temp->app_node->record.version << endl;
            cout << "\tSize: " << fixed << setprecision(1) << temp->app_node->record.size << endl;
            cout << "\tUnits: " << temp->app_node->record.units << endl;
            cout << "\tPrice: $" << fixed << setprecision(2) << temp->app_node->record.price << endl;
            break;
        }
        temp = temp->next;
    }
    if(temp == NULL){cout <<"Application " << item << " not found.\n"; }
}
// =================================================================
// FUNCTIONS RELATED TO BST
struct tree* newNode(struct app_info currentApp) { // function to create a new node and store the currentApp

    struct tree *temp = new tree();

    temp->record = currentApp;
    temp->left = temp->right = NULL;
    return temp;
}

struct tree* insert(struct tree* treenode,  struct app_info currentApp, struct hash_table_entry *hashelement[], int tableSize){

    if (treenode == NULL){                              // if a tree is empty, return a new node, or find the emptry node it belongs to
        struct tree *nodetemp = newNode(currentApp);
        hash_insert(nodetemp, hashelement, tableSize);  // this will insert the given app into the hash table
        return nodetemp;                                // after finding the location the currentApp belongs to.
    }
    if (strcmp(currentApp.app_name, treenode->record.app_name) <= 0){
        treenode->left = insert(treenode->left, currentApp, hashelement, tableSize);
    }
    else{
        treenode->right = insert(treenode->right, currentApp, hashelement, tableSize);
    }
    return treenode;
}

int main() {

    struct app_info appData;
    struct tree* tree = NULL;
    struct hash_table_entry **hash_table;

    int tableSize;
    int catAmount;
    int appAmount;
    int queryAmount;
    string currentLine = "";
    string searchItem = "";

    getline(cin, currentLine);
    catAmount = stoi(currentLine);          // first number of input file is the number of categories

    struct categories cat[catAmount];       // initalize cat[] array with the category amount taken from the first line

    for (int i = 0; i < catAmount; i++){    // store the each of the categories as strings into cat.category.
        getline(cin, currentLine);
        strcpy(cat[i].category, currentLine.c_str());
        cat[i].root = tree;                 // allocate memory for the roots of each tree, the initial node.
    }

    getline(cin, currentLine);
    appAmount = stoi(currentLine);          // get app amount from input
    tableSize = prime(appAmount);           // get table f

    hash_table = new hash_table_entry*[tableSize];
    for(int i = 0; i < tableSize; i++){     // allocate memory and set it to NULL
        hash_table[i] = NULL;
    }

    for (int i = 0; i < appAmount; i++){

        getline(cin, currentLine);
        strcpy(appData.category, currentLine.c_str());      // save category as string
        getline(cin, currentLine);
        strcpy(appData.app_name, currentLine.c_str());      // save app name as string
        getline(cin, currentLine);
        strcpy(appData.version, currentLine.c_str());       // save version # as string
        getline(cin, currentLine);
        appData.size = stof(currentLine);                   // save app size in float
        getline(cin, currentLine);
        strcpy(appData.units, currentLine.c_str());         // save units as a string MB or GB
        getline(cin, currentLine);
        appData.price = stof(currentLine);                  // save price as float

        for (int i = 0; i < catAmount; i++){                        // find which category the app belongs to.
            if(strcmp(cat[i].category, appData.category) == 0){     // and save it in the category's tree
                cat[i].root = insert(cat[i].root, appData, hash_table, tableSize);
                break;
            }
        }
}

    getline(cin, currentLine);
    queryAmount = stoi(currentLine);        // save number as query amount and run as many times as queries are needed.
    if (queryAmount == 0){ return 0; }
    for(int i = 0; i < queryAmount; i++){

       getline(cin, currentLine);
       size_t  findapp = currentLine.find("find app"); // this helps me determine which query is being called,
        if (findapp != string::npos){                     // will update with future query calls later
            searchItem = currentLine.substr(9 );
        }
        search(searchItem, hash_table, tableSize);
        if(i < queryAmount-1){cout << endl;}
    }
return 0;
}

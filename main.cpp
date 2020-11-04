#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
#include <iomanip>
using namespace std;

#define CAT_NAME_LEN    25
#define APP_NAME_LEN    60
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

// =====================================================================================
// functions related to queries
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
    if(temp == NULL){cout <<"Application " << item << " not found." << endl; }
}
void CATinorder(struct tree* root){
    if(root != NULL){
        CATinorder(root->left);
        cout << "\t" << root->record.app_name << endl;
        CATinorder(root->right);
    }
}

void searchCategory(string item, struct categories *cat, int catAmount){
    const char *newString = item.c_str();
    bool found = false;
    for (int i = 0; i < catAmount; i++){
        tree* temp = cat[i].root;
        if(strcmp(newString, cat[i].category) == 0){
            if(cat[i].root != NULL){ cout << "Category: " << item << endl; CATinorder(temp);}
            else{cout << "Category " << item << " no apps found." << endl;}
            found = true;
        }
    }
    if(found == false){cout << "Category " << item << " not found." << endl;}
}

void FREEinorder(struct tree* root, int &count){

    if(root != NULL){
        FREEinorder(root->left, count);
        if(root->record.price == 0){
            cout << "\t" << root->record.app_name << endl;
            count++;
        }
        FREEinorder(root->right, count);
    }
}
void searchFree(struct categories *cat, int catAmount){


    for (int i = 0; i < catAmount; i++){
        int count = 0;
        tree* temp = cat[i].root;
        cout << "Free Applications in Category: " << cat[i].category << endl;

        if(cat[i].root == NULL){cout << "\tNo free applications found." << endl;}
        else{
            FREEinorder(temp, count);
            if(count == 0) {cout << "\tNo free applications found." << endl;}
        }
        cout << endl;
    }
}

void RangeA(string firstl, string lastl, struct tree* root, int &count){

    if(root != NULL){
        RangeA(firstl, lastl, root->left, count);

        const char* first = firstl.c_str();
        const char* last = lastl.c_str();
        string app = root->record.app_name;
        string firstletters = app.substr(0,firstl.length());
        string lastletters = app.substr(0, lastl.length());
        const char* comp = firstletters.c_str();
        const char* comp2 = lastletters.c_str();

        if(strcmp(comp, first) >= 0 && strcmp(comp2, last) <= 0){
            cout << "\t" << root->record.app_name << endl;
            count++;
        }

        RangeA(firstl, lastl, root->right, count);
    }
}

void RangeP(float firstprice, float lastprice, struct tree* root, int &count){

    if(root != NULL){
        RangeP(firstprice, lastprice, root->left, count);

        if(root->record.price >= firstprice && root->record.price <= lastprice){
            cout << "\t" << root->record.app_name << endl;
            count++;
        }

        RangeP(firstprice, lastprice, root->right, count);
    }

}

void Range(string item, string newCat, struct categories *cat, int catAmount){
    int count = 0;
    size_t suba = item.find("app");
    size_t subp = item.find("price");
    const char* category = newCat.c_str();
    struct categories* temp = cat;

    if(suba != string::npos){
        size_t aq1 = item.find('"');
        size_t aq2 = item.find('"', aq1 + 1 );
        size_t aq3 = item.find('"', aq2 + 1);
        size_t aq4 = item.find('"', aq3 + 1 );
        string firstl = item.substr(aq1+1, aq2-aq1-1);
        string lastl = item.substr(aq3+1, aq4-aq3-1);

        for (int i = 0; i < catAmount; i++){
            if(strcmp(category,cat[i].category) == 0){
                cout  << "Applications in Range (" << firstl << "," << lastl << ") in Category: " << newCat << endl;
                RangeA(firstl, lastl, temp[i].root, count);
            }
        }

        if (count == 0){cout << "\tNo applications found in " << newCat << " for the given range (" << firstl << "," << lastl << ")" << endl;}
    }

    if(subp != string::npos){
        size_t whitespace = item.find(' ', 6);
        string price1 = item.substr(6, whitespace-6);
        float firstprice = stof(price1);
        string price2 = item.substr(whitespace+1);
        float lastprice = stof(price2);

        for (int i = 0; i < catAmount; i++){
            if(strcmp(category,cat[i].category) == 0){
                cout  << "Applications in Range ($" << firstprice << ",$" << lastprice << ") in Category: " << newCat << endl;
                RangeP(firstprice, lastprice, temp[i].root, count);
            }
        }
        if (count == 0){cout << "\tNo applications found in " << newCat << " for the given range ($" << firstprice << ",$" << lastprice << ")" << endl;}
    }

}
struct tree* minValueNode(struct tree* node){

    struct tree* current = node;

    while(current->left != NULL){
        current = current->left;
    }
    return current;
}

struct tree* DeleteTreeNode(struct tree* node, struct tree* root){
// When node to be deleted is head node
    // base case
    if (root == NULL) return root;

    // If the key to be deleted is smaller than the root's key,
    // then it lies in left subtree
    if(strcmp(node->record.app_name, root->record.app_name) < 0)
        root->left = DeleteTreeNode(node, root->left);

        // If the key to be deleted is greater than the root's key,
        // then it lies in right subtree
    else if (strcmp(node->record.app_name, root->record.app_name) > 0)
        root->right = DeleteTreeNode(node, root->right);

        // if key is same as root's key, then This is the node
        // to be deleted
    else
    {
        // node with only one child or no child
        if (root->left == NULL)
        {
            struct tree *temp = root->right;
            delete(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct tree *temp = root->left;
            delete(root);
            return temp;
        }

        // node with two children: Get the inorder successor (smallest
        // in the right subtree)
        struct tree* temp = minValueNode(root->right);

        // Copy the inorder successor's content to this node
        root->record = temp->record;

        // Delete the inorder successor
        root->right = DeleteTreeNode(temp, root->right);
    }
    return root;
}

void DeleteSearch(string sApp, string sCategory, hash_table_entry *hashTable[], struct categories* cat, int catAmount, int tableSize, bool &isfound){
    cout << "---- MADE IT TO DELETESEARCH FUNCTION ----" << endl;

    const char *app = sApp.c_str();               // created newString for strcmp.
    const char *category = sCategory.c_str();
    int index = modulo(app, tableSize);                // finds the index where item should be at.
    hash_table_entry *head = hashTable[index];          // created a temp to make sure not to overwrite actual data.
    hash_table_entry *temp = head;

    cout << "app = " << app << endl;
    cout << "category = " << category << endl;

    if(strcmp(head->app_name, app) == 0){
        cout << "- made it to if(strcmp(head->app_name, app) == 0)" << endl;
        for (int i = 0; i < catAmount; i++){
            if(strcmp(cat[i].category, category) == 0) {

                cout << "-- made it to if(strcmp(cat[i].category, category) == 0)" << endl;
                cat[i].root = DeleteTreeNode(head->app_node, cat[i].root);
                cout << "---- CATEGORY AFTER DELETETREENODE" << endl;
                searchCategory(category, cat, catAmount);

                cout << "-- made it out of DeleteTreeNode(head->app_node, cat[i].root);" << endl;
                cout << "-- head = " << head->app_name << endl;
                cout << "-- temp = " << temp->app_name << endl;

                if(head->next != NULL) {
                    cout << "--- made it to second if if(head->next != NULL)" << endl;
                    strcpy(head->app_name, head->next->app_name);
                    head = head->next;
                    head->next = head->next->next;
                }

                delete(temp);
                isfound = true;
                return;
            }
        }
    }

    else{
        hash_table_entry * prev = temp;
        temp = temp->next;
        cout << "- made it to else statement" << endl;
        while (prev != NULL) {

            if (strcmp(temp->app_name, app) == 0) {
                for (int i = 0; i < catAmount; i++) {
                    if (strcmp(cat[i].category, category) == 0) {
                        cat[i].root = DeleteTreeNode(head->app_node, cat[i].root);
                        prev->next = prev->next->next;
                        delete (temp);
                        isfound = true;
                        return;
                    }
                }
            }

            prev = prev->next;
            temp = temp->next;
        }
    }

}

void DeleteApp(string searchItem, struct categories* cat, hash_table_entry *hashTable[], int catAmount, int tableSize){
    cout << "---- MADE IT TO DELETEAPP FUNCTION ---- " << endl;

    size_t cq1 = searchItem.find('"');
    size_t cq2 = searchItem.find('"', cq1 + 1 );
    size_t aq1 = searchItem.find('"', cq2 + 1);
    size_t aq2 = searchItem.find('"', aq1 + 1 );
    string category = searchItem.substr(cq1+1, cq2-cq1-1);
    string app = searchItem.substr(aq1+1, aq2-aq1-1);
    bool isfound = false;
    cout << "---- CATEGORY BEFORE DELETE APP" << endl;
    searchCategory(category, cat, catAmount);

    cout << "APP = " << app << endl;
    cout << "CATEGORY = " << category << endl;

    DeleteSearch(app, category, hashTable, cat, catAmount, tableSize, isfound);
    cout << "NOW SEARCHING FOR " << app << " IN HASH TABLE" << endl;
    search( app, hashTable, tableSize);
    cout << "IS FOUND = " << isfound << endl;
    cout << "NOW SEARCHING FOR "<< app <<" IN BST" << endl;
    searchCategory(category, cat, catAmount);



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
    // queries
    string findprice = "find price free";
    string range = "range";
    string delapp = "delete";
    string report = "report";

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

        getline(cin, currentLine, '\n');
        strcpy(appData.category, currentLine.c_str());      // save category as string
        getline(cin, currentLine, '\n');
        strcpy(appData.app_name, currentLine.c_str());      // save app name as string
        getline(cin, currentLine, '\n');
        strcpy(appData.version, currentLine.c_str());       // save version # as string
        getline(cin, currentLine, '\n');
        appData.size = stof(currentLine);                   // save app size in float
        getline(cin, currentLine, '\n');
        strcpy(appData.units, currentLine.c_str());         // save units as a string MB or GB
        getline(cin, currentLine, '\n');
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

    while(getline(cin, currentLine)){
        size_t  findapp = currentLine.find("find app");
        size_t  findcat = currentLine.find("find category");
        size_t findprice = currentLine.find("find price free");
        size_t range = currentLine.find("range");
        size_t delapp = currentLine.find("delete");

        if (findapp != string::npos){
            size_t q1 = currentLine.find('"');
            size_t q2 = currentLine.find('"', q1+1);
            searchItem = currentLine.substr(q1+1, q2-q1-1);
            search(searchItem, hash_table, tableSize);
            cout << endl;
        }

        if (findcat != string::npos){
            size_t q1 = currentLine.find('"');
            size_t q2 = currentLine.find('"', q1+1);
            searchItem = currentLine.substr(q1+1, q2-q1-1);
            searchCategory(searchItem, cat, catAmount);
            cout << endl;
        }

        if (findprice != string::npos){
            searchFree(cat, catAmount);
        }

        if (range != string::npos){
            searchItem = currentLine;
            size_t cq1 = searchItem.find('"');
            size_t cq2 = searchItem.find('"', cq1 + 1 );
            string category = searchItem.substr(cq1 + 1, cq2-cq1-1);
            string subItem = searchItem.substr(cq2 + 2);

            Range(subItem, category, cat, catAmount);
            cout << endl;
        }

        if (delapp != string::npos){
            searchItem = currentLine;
            DeleteApp(searchItem, cat, hash_table, catAmount, tableSize);
        }

    }

return 0;
}
#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
using namespace std;

#define CAT_NAME_LEN    25
#define APP_NAME_LEN    50
#define VERSION_LEN     10
#define UNIT_SIZE       3
// =================================================================
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
bool isPrime(int item){
    int limit, factor = 2;
    limit = (long)( sqrtf( (float) item ) + 0.5f );
    while( (factor <= limit) && (item % factor) )
        factor++;
    return( factor > limit );
}

int prime(int item){
    item = item * 2;
    bool primetest = false;

    while(primetest != true){
        primetest = isPrime(item);
        if (primetest == false)
            item++;
    }
    return item;
}

int modulo(string appName, int key){

    int temp = 0;

    for(int i = 0; i < appName.length(); i++){
        temp = int(appName[i]) + temp;
    }
    return  temp % key;
}

void hash_insert(struct tree* node, struct hash_table_entry *temp[], int tableSize){
    string appName = node->record.app_name;
    int index = modulo(appName, tableSize);
    struct hash_table_entry* newEntry = new hash_table_entry();

    newEntry->app_node = node;
    strcpy(newEntry->app_name, node->record.app_name);
    newEntry->next = temp[index];
    temp[index] = newEntry;

}
// =================================================================

struct tree* newNode(struct app_info currentApp) {

    struct tree *temp = new tree();

    temp->record = currentApp;
    temp->left = temp->right = NULL;
    return temp;
}

struct tree* insert(struct tree* treenode,  struct app_info currentApp, struct hash_table_entry *hashelement[], int tableSize){

    if (treenode == NULL){
        struct tree *nodetemp = newNode(currentApp);
        hash_insert(nodetemp, hashelement, tableSize);
        return nodetemp;
    }// if a tree is empty, return a new node

    if (strcmp(currentApp.app_name, treenode->record.app_name) <= 0){
        treenode->left = insert(treenode->left, currentApp, hashelement, tableSize);
    }
    else{
        treenode->right = insert(treenode->right, currentApp, hashelement, tableSize);
    }
    return treenode;
}
void inorder(struct tree* root){
    if(root != NULL){
        inorder(root->left);
        cout << "category: " << root->record.category << endl;
        cout << "name: " << root->record.app_name << endl;
        cout << "version: " << root->record.version << endl;
        cout << "size: " << root->record.size << endl;
        cout << "units: " << root->record.units << endl;
        cout << "price: " << root->record.price << endl;
        inorder(root->right);
    }
}

void bstreport(struct tree* root){

}

void printlist(struct hash_table_entry *hash_table[], int tableSize){
struct hash_table_entry *temp;
    for(int i = 0; i < tableSize; i++){
        cout << "hashtable[" << i << "]: ";
        temp = hash_table[i];
        if(temp == NULL){cout << "NULL";}
        while(temp != NULL){
            cout << "\"" << temp->app_name << "\" ";
            temp = temp->next;
        }
        cout << endl;
    }
}

void search(string item, hash_table_entry *hashTable[], int tableSize){
    const char *newstring = item.c_str();
    int index = modulo(item, tableSize);
    //cout << "modulo = " << index << endl;
    hash_table_entry *temp = hashTable[index];
    int count = 1;
    while(temp != NULL){
        if(strcmp(temp->app_name, newstring) == 0){
            cout << "Found Application: " << temp->app_name;
            cout << "\tApplication Name: " << temp->app_node->record.app_name;
            cout << "\t"
            break;
        }
        count++;
        temp = temp->next;
    }
    if(temp == NULL){cout <<" NOT FOUND " << endl; }
}

int main() {

    struct app_info appData;
    struct tree* tree = NULL;
    struct hash_table_entry **hash_table;

    int tableSize;
    int catAmount;
    int appAmount;
    const char *temp;
    string currentLine = "";
    string searchItem = "";

    getline(cin, currentLine);
    catAmount = stoi(currentLine); // first number of input file is the number of categories

    string catNames[catAmount];
    struct categories cat[catAmount];

    for (int i = 0; i < catAmount; i++){ // this is going to read each line after, and place it in an string array
        getline(cin, currentLine);
        catNames[i] = currentLine;
    }

    for (int i = 0; i < catAmount; i++){ // store strings into cat.category.
        temp = catNames[i].c_str();
        strcpy(cat[i].category, temp);
        cat[i].root = tree;
    }

    getline(cin, currentLine);
    appAmount = stoi(currentLine); // get app amount from input
    tableSize = prime(appAmount);

    hash_table = new hash_table_entry*[tableSize];
    for(int i = 0; i < tableSize; i++){
        hash_table[i] = NULL;
    }

    for (int i = 0; i < appAmount; i++){
        // save category as string
        getline(cin, currentLine);
        strcpy(appData.category, currentLine.c_str());
        // save app name as string
        getline(cin, currentLine);
        strcpy(appData.app_name, currentLine.c_str());
        // save version # as string
        getline(cin, currentLine);
        strcpy(appData.version, currentLine.c_str());
        // save app size in float
        getline(cin, currentLine);
        appData.size = stof(currentLine);
        // save units as a string MB or GB
        getline(cin, currentLine);
        strcpy(appData.units, currentLine.c_str());
        // save price as float
        getline(cin, currentLine);
        appData.price = stof(currentLine);

        for (int i = 0; i < catAmount; i++){
            if(strcmp(cat[i].category, appData.category) == 0){
                cat[i].root = insert(cat[i].root, appData, hash_table, tableSize);
                break;
            }
        }
}
cout << "TEST: " << endl;
/*
    for (int i = 0; i < catAmount; i++){
        cout << "================= FOR CATEGORY: " << cat[i].category << " =================" << endl;
        inorder(cat[i].root);
    }
*/
    //printlist(hash_table, tableSize);
    cout << "testing search: " << endl;
    while(getline(cin, currentLine)){
       // cout << "current line: " << currentLine << endl;
        size_t  findapp = currentLine.find("find app");
        if (findapp != string::npos){
            searchItem = currentLine.substr(9 );
          //  cout << searchItem << endl;
        }
        search(searchItem, hash_table, tableSize);
    }


return 0;
}

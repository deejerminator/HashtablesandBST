#include <iostream>
#include <string>
#include <string.h>
#include <math.h>
using namespace std;

#define CAT_NAME_LEN    25
#define APP_NAME_LEN    50
#define VERSION_LEN     20
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

int BST_height(struct tree* root){
    if(root == NULL){
        return 0;
    }
    else{
        int lheight = BST_height(root->left);
        int rheight = BST_height(root->right);

        if (lheight < rheight){return (rheight+1);}
        else{return (lheight+1);}
    }
}

int BST_count(struct tree* root){
    int count = 1;
    if(root == NULL){return 0;}
    else{
        count = count + BST_count(root->left);
        count = count + BST_count(root->right);
        return count;
    }
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
    hash_table_entry *temp = hashTable[index];
    int count = 1;
    while(temp != NULL){
        if(strcmp(temp->app_name, newstring) == 0){
            cout << "Found Application: " << temp->app_name << endl;
            cout << "\tCategory: " << temp->app_node->record.category << endl;
            cout << "\tApplication Name: " << temp->app_node->record.app_name << endl;
            cout << "\tVersion: " << temp->app_node->record.version << endl;
            cout << "\tSize: " << temp->app_node->record.size << endl;
            cout << "\tUnits: " << temp->app_node->record.units << endl;
            cout << "\tPrice: $" << temp->app_node->record.price << endl << endl;
            break;
        }
        count++;
        temp = temp->next;
    }
    if(temp == NULL){cout <<"Application: " << item << " not found\n\n"; }
}

int main() {

    struct app_info appData;
    struct tree* tree = NULL;
    struct hash_table_entry **hash_table;

    int tableSize;
    int catAmount;
    int appAmount;
    const char *temp;
    const char* report;
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

/*
    for (int i = 0; i < catAmount; i++){
        cout << "================= FOR CATEGORY: " << cat[i].category << " =================" << endl;
        inorder(cat[i].root);
    }
*/
    //printlist(hash_table, tableSize);
    int queryAmount;
    getline(cin, currentLine);
    queryAmount = stoi(currentLine);

    for(int i = 0; i < queryAmount-1; i++){

       getline(cin, currentLine);
       size_t  findapp = currentLine.find("find app");
        if (findapp != string::npos){
            searchItem = currentLine.substr(9 );
        }
        search(searchItem, hash_table, tableSize);
    }

    // get report
    /*
    getline(cin, currentLine);
    report = currentLine.c_str();
    if (strcmp(report, "report") == 0){
        cout << "\t**** REPORT ****\n\n";
      */
        /*
         * FOR BST
         * print the category name, a count of the total number of nodes in the tree, the height of the tree,
         * the height of the root node's left subtree, and the height of the root node's right subtree.
         *
         * FOR HASH
         * print a table that lists for each chain length L, 0 <= L <= LMAX, the number of chains of length L,
         * up the the maximum chain length LMAX that your hash table contains. In addition, compute the load factor
         * a for the hash table, giving n and m.
         *
         * Implement the find app <app_name> command by directly searching the BST instead of the hash table. The
         * easiest way to do this may be to use the hash table to extract the <category_name> and then search the
         * appropriate BST. Compare the time to find an app <app_name> using the hash table, and by searching the BST
         * for its category.
         * */

        // for bst
/*
        for(int i = 0; i < catAmount; i++){
            cout << "BST CATEGORY: " << cat[i].category << endl;
            if(cat[i].root == NULL){
                cout << "TOTAL NODES: 0" << endl;
                cout << "MAX HEIGHT: 0" << endl;
                cout << "HEIGHT LEFT: 0" << endl;
                cout << "HEIGHT RIGHT: 0\n\n";
            }
            else {
                int lheight = BST_height(cat[i].root->left);
                int rheight = BST_height(cat[i].root->right);
                cout << "TOTAL NODES: " << BST_count(cat[i].root) << endl;
                cout << "MAX HEIGHT: ";
                if (lheight < rheight){cout << rheight << endl;}
                else{cout << lheight << endl;}
                cout << "HEIGHT LEFT: " << lheight << endl;
                cout << "HEIGHT RIGHT: " << rheight << endl << endl;


            }
        }
    }
    */


return 0;
}

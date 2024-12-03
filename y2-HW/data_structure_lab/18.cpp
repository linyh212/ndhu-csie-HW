#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#define MAX_ENTRIES 16  // maxinum number of data item each node can hold.
#define MAX_NAME_LEN 50 // maxinum number of character for item name

typedef struct{
    double minX, minY;
    double maxX, maxY;
} BoundingBox;

typedef struct{
    double latitude, longitude;
    char name[MAX_NAME_LEN];
} DataItem;

typedef struct RTreeNode{
    int isLeaf;                     // 是否為葉節點is leaf or not
    int count;                      // number of node
    BoundingBox mbr;                // minimum bounding rectangle
    struct RTreeNode* children[MAX_ENTRIES+1];
    DataItem data[MAX_ENTRIES];
} RTreeNode;

typedef struct{
    DataItem item;
    double distance;
} SearchResult;

// insert data
void insert(RTreeNode** root, DataItem item){

}

// search data
SearchResult* search(RTreeNode* root, double latitude, double longitude, int n){

}

// print tree
void printTree(RTreeNode* root, int depth){
    for(int i=0; i<depth; i++) printf("  ");
    printf("Node (count=%d, isLeaf=%d):\n", root->count, root->isLeaf);
    for(int i=0; i<root->count; i++){
        if(root->isLeaf){
            for(int j=0; j<depth+1; j++) printf("  ");
            printf("Data: %s (%.2f, %.2f)\n", root->data[i].name, root->data[i].latitude, root->data[i].longitude);
        } else{
            printTree(root->children[i], depth+1);
        }
    }
}

int main(){
    RTreeNode* root=createNode(1);
    DataItem item;
    double latitude, longitude;
    SearchResult* results;    
    char command[50];    
    while(1){
        scanf("%s", command);
        if(strcmp("insert", command)==0){
            scanf("%lf%lf%s", &item.latitude, &item.longitude, item.name);
            insert(&root, item);
        }
        else if(strcmp("print", command)==0){
            printTree(root, 0);
        }
        else if(strcmp("exit", command)==0){
            break;
        }
        else if(strcmp("search", command)==0){
            scanf("%lf%lf", &latitude, &longitude;);
            result=search(root, latitude, longitude);
        }
        else{
            printf("Unknow command.\n");
        }
    }
    return 0;
}

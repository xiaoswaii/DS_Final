#include <iomanip>
#include <queue>
#include <fstream>
#include <string>
#include<iostream>

using namespace std;

string note;

struct BstNode {
	int data;
	BstNode* left;
	BstNode* right;
};

BstNode* GetNewNode(int data) {
	BstNode* newNode = new BstNode();
	newNode->data = data;
	newNode->left = newNode->right = NULL;
	return newNode;
}

BstNode* Insert(BstNode* root,int data) {
	if(root == NULL) {
		root = GetNewNode(data);
	}
	else if(data < root->data) {
		root->left = Insert(root->left,data);
	}
	else {
		root->right = Insert(root->right,data);
	}
	return root;
}

void deleteTree(BstNode*& root)  {  
    if (root == NULL) return;  
    deleteTree(root->left);  
    deleteTree(root->right);  
    delete(root);
    root = NULL;
}

void getInorder(BstNode* root, string &inorderStr) {
    if (root) {                     
        getInorder(root->left, inorderStr);          
        inorderStr.append(to_string(root->data)+" ");
        getInorder(root->right, inorderStr);
    }
}

void Parenthesis(BstNode* root, string &str) {
    string temp = to_string(root->data);
    str.append(temp);
    if(root->left == NULL && root->right == NULL) return;
    if(root->left != NULL) {
        str.append("(");
        Parenthesis(root->left, str);
    }
    else if(root->left == NULL) {
        str.append("(");
        str.append("-");
    }
    str.push_back(' ');
    if(root->right != NULL) {
        Parenthesis(root->right, str);
        str.append(")");
    }
    else if(root->right == NULL) {
        str.append("-");
        str.append(")");
    }

    return;
}

void writeFileBoundary (string ans) {
    fstream file;
    file.open("./BTree_boundary.txt",ios::app);
    if(!file){
        ofstream outfile ("./BTree_boundary.txt");
        outfile << ans << endl;
        outfile.close();
    }
    else {
        file << ans << endl;
        file.close();
    }
}

void LeftBound(BstNode* root) {
    if(root->left != NULL) {
        //cout << root->data << " ";
        string shit = to_string(root->data);
        note = note.append(shit);
        note = note.append(" ");
        LeftBound(root->left);
    }
    else {
        string a = to_string(root->data);
        note = note.append(a);
        //cout<<note;
        writeFileBoundary(note);
        note = "";
    }
}

void Levelorder(BstNode* root, int storeOrder[100]) {
    queue<BstNode*> q;
    q.push(root);                     
    fstream file;
    file.open("BTree.txt", ios::out | ios::app);                        
    while (!q.empty()){                   
        int nodeCount = q.size();
        int count = 0;
        while (nodeCount > 0) {
            BstNode *temp = q.front();
            for(int i = count; i < 1000; i++) {
                if(storeOrder[i] == temp->data) {
                    if(nodeCount == 1){
                        file << temp->data;
                    }
                    else{
                        file << setw(3)<<setiosflags(ios::left)<< temp->data;
                    }
                    count = count + 1;
                    break;
                }
                else {
                    file <<"   ";
                    count = count + 1;
                }
            }
            q.pop();
            if(temp->left != nullptr) q.push(temp->left);
            if(temp->right != nullptr) q.push(temp->right);
            nodeCount = nodeCount - 1 ;
        }
        file << endl;
    }
}

void writeFileParenthesis (string ans) {
    fstream file;
    file.open("./BTree_PRep.txt",ios::app);
    if(!file){
        ofstream outfile ("./BTree_PRep.txt.txt");
        outfile << ans << endl;
        outfile.close();
    }
    else {
        file << ans << endl;
        file.close();
    }
}

int main() {
    int myData[100][1000]={0};
    int storeLevel[1000]={0};
    int countColumn = 0;
    int countRow = 0;
    int countArr = 0;
    int countLevelOrder = 0;
    char input;
    string set;
    string str="";
    remove("./BTree.txt");
    remove("./BTree_boundary.txt");
    remove("./BTree_PRep.txt");
    string levelStr="";
    int arr[100];
    int sizearr=0;
    ifstream myfile;
    myfile.open("./input.txt");
    while(myfile.get(input)){
        if (int(input) == -62) {
            continue;
        }
        else if (int(input) == -96) {
            countColumn = countColumn + 1;
        }
        else if (int(input == 10)) {
            countColumn = 0;
            countRow = countRow + 1;
        }
        else if(int(input) == 13){
            continue;
        }
        else {
            myData[countRow][countColumn] = (myData[countRow][countColumn] *10 + (int(input)-48));
        }
    } 

    // Creating an empty tree
    BstNode* root = NULL;

	/*Code to do BTree Left Boundary*/
    for(int i = 0 ; i < 100 ; i++){
        deleteTree(root);
        for(int j = 0 ; j< 1000 ; j++) {
            if(myData[i][j] != 0){
                root= Insert(root,myData[i][j]);
            }
        }
        LeftBound(root);
    }

    deleteTree(root);

    /*Code to do BTree Parenthesis*/
    for(int i = 0 ; i < 100 ; i++){
        deleteTree(root);
        for(int j = 0 ; j< 1000 ; j++) {
            if(myData[i][j] != 0){
                root= Insert(root,myData[i][j]);
            }
        }
        Parenthesis(root,str);
        writeFileParenthesis(str);
        str="";
    }

    deleteTree(root);
    ofstream outfile ("./BTree.txt");
    /*Code to do BTree Textual*/
    for(int i = 0 ; i < 100 ; i++){
        string inorderStr="";
        int storeOrder[1000]={0};
        int countStoreOrder = 0;
        deleteTree(root);
        for(int j = 0 ; j< 1000 ; j++) {
            if(myData[i][j] != 0){
                root= Insert(root,myData[i][j]);
            }
        }
        getInorder(root,inorderStr);
        for(int i = 0 ; i<inorderStr.length() ; i++){
            if(inorderStr[i] !=' ' ) {
            storeOrder[countStoreOrder] = (storeOrder[countStoreOrder] *10 + (int(inorderStr[i])-48));
            }
            else{
            countStoreOrder=countStoreOrder + 1;
            } 
        }

        for(int i = 0 ; i<1000 ; i++){
            if(storeOrder[i]!=0){
            }
        }

        Levelorder(root,storeOrder);
    }
}
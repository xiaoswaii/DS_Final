#include <iostream>
#include <cstdio>
#include <queue>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <string>
using namespace std;
 
struct splay
{
    int data;
    splay* lchild;
    splay* rchild;
};
 
class SplayTree {
    public:
        string note;
        SplayTree()
        {
        }
 
        // RR(Y rotates to the right)
        splay* RR_Rotate(splay* k2) {
            splay* k1 = k2->lchild;
            k2->lchild = k1->rchild;
            k1->rchild = k2;
            return k1;
        }
 
        // LL(Y rotates to the left)
        splay* LL_Rotate(splay* k2) {
            splay* k1 = k2->rchild;
            k2->rchild = k1->lchild;
            k1->lchild = k2;
            return k1;
        }
 
        // An implementation of top-down splay tree
        splay* Splay(int data, splay* root) {
            if (!root)
                return NULL;
            splay header;
            /* header.rchild points to L tree;
            header.lchild points to R Tree */
            header.lchild = header.rchild = NULL;
            splay* LeftTreeMax = &header;
            splay* RightTreeMin = &header;
            while (1)
            {
                if (data < root->data)
                {
                    if (!root->lchild)
                        break;
                    if (data < root->lchild->data)
                    {
                        root = RR_Rotate(root);
                        // only zig-zig mode need to rotate once,
                        if (!root->lchild)
                            break;
                    }
                    /* Link to R Tree */
                    RightTreeMin->lchild = root;
                    RightTreeMin = RightTreeMin->lchild;
                    root = root->lchild;
                    RightTreeMin->lchild = NULL;
                }
                else if (data > root->data)
                {
                    if (!root->rchild)
                        break;
                    if (data > root->rchild->data)
                    {
                        root = LL_Rotate(root);
                        // only zag-zag mode need to rotate once,
                        if (!root->rchild)
                            break;
                    }
                    /* Link to L Tree */
                    LeftTreeMax->rchild = root;
                    LeftTreeMax = LeftTreeMax->rchild;
                    root = root->rchild;
                    LeftTreeMax->rchild = NULL;
                }
                else
                    break;
            }
            /* assemble L Tree, Middle Tree and R tree */
            LeftTreeMax->rchild = root->lchild;
            RightTreeMin->lchild = root->rchild;
            root->lchild = header.rchild;
            root->rchild = header.lchild;
            return root;
        }
 
        splay* New_Node(int data) {
            splay* p_node = new splay;
            if (!p_node)
            {
                fprintf(stderr, "Out of memory!\n");
                exit(1);
            }
            p_node->data = data;
            p_node->lchild = p_node->rchild = NULL;
            return p_node;
        }
 
        splay* Insert(int data, splay* root) {
            static splay* p_node = NULL;
            if (!p_node)
                p_node = New_Node(data);
            else
                p_node->data = data;
            if (!root)
            {
                root = p_node;
                p_node = NULL;
                return root;
            }
            root = Splay(data, root);
            /* This is BST that, all datas <= root->data is in root->lchild, all datas >
            root->data is in root->rchild. */
            if (data < root->data)
            {
                p_node->lchild = root->lchild;
                p_node->rchild = root;
                root->lchild = NULL;
                root = p_node;
            }
            else if (data > root->data)
            {
                p_node->rchild = root->rchild;
                p_node->lchild = root;
                root->rchild = NULL;
                root = p_node;
            }
            else
                return root;
            p_node = NULL;
            return root;
        }
 
        void getInorder(splay* root, string &inorderStr) {
            if (root) {                     
                getInorder(root->lchild, inorderStr);          
                inorderStr.append(to_string(root->data)+" ");
                getInorder(root->rchild, inorderStr);
            }
        }

        void deleteTree(splay*& root)  {  
            if (root == NULL) return;  
        
            /* first delete both subtrees */
            deleteTree(root->lchild);  
            deleteTree(root->rchild);  
        
            /* then delete the node */
            //cout << "Deleting node: " << root->data << endl;  
            delete(root);
            root = NULL; 
        }

        void Parenthesis(splay* root, string &str) {
            string temp = to_string(root->data);
            str.append(temp);
            if(root->lchild == NULL && root->rchild == NULL) return;
            if(root->lchild != NULL) {
                str.append("(");
                Parenthesis(root->lchild, str);
            }
            else if(root->lchild == NULL) {
                str.append("(");
                str.append("-");
            }
            str.push_back(' ');
            if(root->rchild != NULL) {
                Parenthesis(root->rchild, str);
                str.append(")");
            }
            else if(root->rchild == NULL) {
                str.append("-");
                str.append(")");
            }

            return;
        }       

        void LeftBound(splay* root) {
            if(root->lchild != NULL) {
                //cout << root->data << " ";
                string shit = to_string(root->data);
                note = note.append(shit);
                note = note.append(" ");
                LeftBound(root->lchild);
            }
            else {
                string a = to_string(root->data);
                note = note.append(a);
                //cout<<note;
                writeFileBoundary(note);
                note = "";
            }
        }

        void Levelorder(splay* root, int storeOrder[100]) {
            queue<splay*> q;
            q.push(root);                     
            fstream file;
            file.open("SplayTree.txt", ios::out | ios::app);                        
            while (!q.empty()){                   
                int nodeCount = q.size();
                int count = 0;
                while (nodeCount > 0) {
                    splay *temp = q.front();
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
                    if(temp->lchild != nullptr) q.push(temp->lchild);
                    if(temp->rchild != nullptr) q.push(temp->rchild);
                    nodeCount = nodeCount - 1 ;
                }
                file << endl;
            }
        }       

        void writeFileParenthesis (string ans) {
            fstream file;
            file.open("./STree_PRep.txt",ios::app);
            if(!file){
                ofstream outfile ("./STree_PRep.txt.txt");
                outfile << ans << endl;
                outfile.close();
            }
            else {
                file << ans << endl;
                file.close();
            }
        }

        void writeFileBoundary (string ans) {
            fstream file;
            file.open("./STree_boundary.txt",ios::app);
            if(!file){
            ofstream outfile ("./STree_boundary.txt.txt");
            outfile << ans << endl;
            outfile.close();
            }
            else {
                file << ans << endl;
                file.close();
            }
    }
};
 
int main()
{
    SplayTree st;
    int myData[100][1000]={0};
    int countColumn = 0;
    int countRow = 0;
    char input;
    string str="";
    ifstream myfile;
    splay *root;
    root = NULL;
    remove("./SplayTree.txt");
    remove("./STree_boundary.txt");
    remove("./STree_PRep.txt");
    myfile.open("./input.txt");
    while(myfile.get(input)){
        //cout<<int(input)<<" ";
        if (int(input) == -62) {
            continue;
        }
        else if (int(input) == -96) {
            countColumn = countColumn + 1;
            // cout<< countColumn << endl;
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
            // cout<< "now data: " << myData[countRow][countColumn] << endl;
        }
    }

    /*Code to do SplayTree Left Boundary*/
    for(int i = 0 ; i < 100 ; i++){
        st.deleteTree(root);
        for(int j = 0 ; j< 1000 ; j++) {
            if(myData[i][j] != 0){
                root= st.Insert(myData[i][j],root);
            }
        }
        st.LeftBound(root);
    }

    /*Code to do SplayTree Parenthesis*/
    for(int i = 0 ; i < 100 ; i++){
        st.deleteTree(root);
        for(int j = 0 ; j< 1000 ; j++) {
            if(myData[i][j] != 0){
                root= st.Insert(myData[i][j],root);
            }
        }
        st.Parenthesis(root,str);
        st.writeFileParenthesis(str);
        str="";
    }

    st.deleteTree(root);
    ofstream outfile ("./SplayTree.txt");
    /*Code to do SplayTree Textual*/
    for(int i = 0 ; i < 100 ; i++){
        string inorderStr="";
        int storeOrder[1000]={0};
        int countStoreOrder = 0;
        st.deleteTree(root);
        for(int j = 0 ; j< 1000 ; j++) {
            if(myData[i][j] != 0){
                root= st.Insert(myData[i][j],root);
            }
        }
        st.getInorder(root,inorderStr);
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

        st.Levelorder(root,storeOrder);
    }
    return 0;
}
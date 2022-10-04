// 資訊二甲 10827101林語潔  10827137李宜軒
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int count = 0;

struct DATA {
    int dataNum = 0;
    string schoolID;
    string schoolName;
    string majorID;
    string majorName;
    string type;
    string level;
    int studentNum;
    int teacherNum;
    int graduateNum;
    string city;
    string system;
    void print() {
        printf("%d", count++);
        printf(": [%d] %s, %s, %s, %s, %d\n", dataNum, schoolName.c_str(), majorName.c_str(), type.c_str(), level.c_str(), studentNum);
    }  // print
};

struct SchoolType {
    vector<DATA> schools;  
    void print() {
        for (int i = 0; i < schools.size(); i++) {
            schools[i].print();
        }  // for
    }      // print
};

class TwoThreeNode {
   public:
    vector<SchoolType> array;         // 存放多間學校 
    vector<TwoThreeNode*> children;   // 存放指標([0]left [1]middle [2]right) 
    TwoThreeNode* parent;             // 指向parent 
    TwoThreeNode() { 
        array.clear();
        parent = NULL;
        children.clear();
    }  // Contructor
    
    
    ~TwoThreeNode(){
      for (int i = 0; i < children.size(); i++) {
        delete children[i];
      } // for
    } // Destructor  
	  
};

class TwoThree {
   //private:
    //TwoThreeNode* root;

   public:
   	TwoThreeNode* root;
    int height(TwoThreeNode* root) {
        if (!root->children.empty()) {
            return height(root->children[0]) + 1;
        }  // if
        else {
            return 1;
        }  // else
    }      // height

    TwoThreeNode* getroot() const {
        return root;
    }  // getroot

    TwoThree();
    void Split(TwoThreeNode* ptr);
    TwoThreeNode* Insert(DATA& input, TwoThreeNode* ptr);
    vector<DATA> SearchSchool( string school ) ;

    void PrintRoot() {
        count = 1;

        if ( root->array.size() == 1 ) { // only one school
        	root->array[0].print();
		} // if
		else { // two school
		  if ( root->array[0].schools[0].dataNum < root->array[1].schools[0].dataNum ) {
        	root->array[0].print();
        	root->array[1].print();
		  } // if
		  else {
			root->array[1].print();
			root->array[0].print();
		  } // else
		} // else
    }      // PrintRoot
    
    
    ~TwoThree() {
        delete root;
        root = NULL;
    }  // Destructor
    

};         // class TwoThree

struct AVLNode {
    int dataNum;
    string majorName;
    int height;
    AVLNode* next;
    AVLNode* left;
    AVLNode* right;
};

class AVL {
   public:
    vector<DATA> list ;
    AVLNode * root ;
    AVLNode * GetNewNode(int dataNum, string majorName) ;
    AVLNode * InsertANode(AVLNode* nodePtr, int dataNum, string majorName) ;
    AVLNode * Insert(AVLNode* nodePtr, int dataNum, string majorName) ;
    AVLNode * InsertSameNode(AVLNode* nodePtr, int dataNum, string majorName) ;
    AVLNode * LLRotate(AVLNode* x) ;
    AVLNode * RRRotate(AVLNode* x) ;
    AVLNode * LRRotate(AVLNode* x) ;
    AVLNode * RLRotate(AVLNode* x) ;
    vector<DATA> Retrieval( string majorName, vector<DATA> list ) ;
    void  SetRoot( AVLNode* copyRoot ) ;
    int maxDepth(AVLNode* root) ;
    int BF(AVLNode* nodePtr) ;
    void PrintNode(AVLNode* node, vector<DATA> list);
};

// ****************************************   23Tree  ***************************************

TwoThree::TwoThree() {
    root = NULL;
}  // Constructor


void Sort(TwoThreeNode* ptr) { 
    int smallestIndex ;
    for ( int i = 0; i < ptr->array.size(); i++ ) {
        smallestIndex = i ;
        for ( int j = i; j < ptr->array.size(); j++) {
            if ( ptr->array[smallestIndex].schools[0].schoolName > ptr->array[j].schools[0].schoolName ) {
                smallestIndex = j ;
            } // if
        } // for

        swap(ptr->array[smallestIndex], ptr->array[i]) ;
    }  // for
}  // Sort


void Sort2(TwoThreeNode* ptr) {
    int smallestIndex ;
    for ( int i = 0; i < ptr->children.size(); i++ ) {
        smallestIndex = i ;
        for ( int j = i; j < ptr->children.size(); j++) {
            if (ptr->children[smallestIndex]->array[0].schools[0].schoolName > ptr->children[j]->array[0].schools[0].schoolName ) {
                smallestIndex = j ;
            }  // if
        } // for

        swap(ptr->children[smallestIndex], ptr->children[i]);
    } // for
} // Sort2


void TwoThree ::Split(TwoThreeNode* ptr) {
    Sort(ptr);
    if (ptr->array.size() > 2) { // 有三個數 要split 
        if (ptr->parent == NULL) {  // root grow up
            ptr->parent = new TwoThreeNode();
            root = ptr->parent;
            ptr->parent->children.push_back(ptr);                // 雙向節點 
        }  // if

        ptr->parent->array.push_back(ptr->array[1]);            // 中間值往上提 
        TwoThreeNode* rightChild = new TwoThreeNode();
        if (ptr->children.size() == 4) {
            ptr->children[2]->parent = rightChild;
            ptr->children[3]->parent = rightChild;
            rightChild->children.push_back(ptr->children[2]);   // 雙向節點  
            rightChild->children.push_back(ptr->children[3]);   // 雙向節點  
            ptr->children.erase(ptr->children.begin() + 3);     // erase the second element 
            ptr->children.erase(ptr->children.begin() + 2);     // erase the first element 
        }  // if

        rightChild->array.push_back(ptr->array[2]);             // 最大值給 rightChild
        ptr->array.erase(ptr->array.begin() + 2);
        ptr->array.erase(ptr->array.begin() + 1);
        rightChild->parent = ptr->parent;
        ptr->parent->children.push_back(rightChild);            // 雙向節點 
        Sort2(ptr->parent);                                     // 將下面的point 
        
        Split(ptr->parent);
    }  // if
}  // Split


TwoThreeNode* TwoThree ::Insert( DATA& input, TwoThreeNode* ptr ) {
    if (ptr == NULL) { //root only
        ptr = new TwoThreeNode();
        SchoolType school;
        school.schools.push_back(input);
        ptr->array.push_back(school); // 將資料存入樹狀結構中 
        root = ptr; // 更新root 
        return NULL;
    }  // if
    else {
        for (int i = 0; i < ptr->array.size(); i++) { // 檢查學校是否出現過 
            if (input.schoolName == ptr->array[i].schools[0].schoolName) {
                ptr->array[i].schools.push_back(input);
                return NULL;
            }  // if
        }      // for check same name

        if (ptr->array.size() == 1) { // 已存入一間學校 
            if (ptr->children.empty()) { // is leaf 
                SchoolType school;
                school.schools.push_back(input);
                ptr->array.push_back(school);
                return ptr;
            }  // if
            else { // not leaf recursion 
                if (input.schoolName > ptr->array[0].schools[0].schoolName) { // walk right
                    return Insert(input, ptr->children[1]); 
                }  // if
                else { // walk left
                    return Insert(input, ptr->children[0]); 
                } // else
            } // else
        } // else if
        else if (ptr->array.size() == 2) { // 已存入兩間學校 
            if (ptr->children.empty()) { // is leaf 
                SchoolType school;
                school.schools.push_back(input);
                ptr->array.push_back(school);
                return ptr;
            }  // if
            else {
                if (input.schoolName > ptr->array[1].schools[0].schoolName) { // walk right
                    return Insert(input, ptr->children[2]);
                }  // if
                else if (input.schoolName < ptr->array[1].schools[0].schoolName && input.schoolName > ptr->array[0].schools[0].schoolName) {
                    return Insert(input, ptr->children[1]); // walk middle
                }  // else if
                else {
                    return Insert(input, ptr->children[0]); // walk left
                } // else
            } // else
        } // else if
    } // else

    return NULL;
}  // Insert


vector<DATA> TwoThree :: SearchSchool( string school ) {
	TwoThreeNode* ptr = root ;
	vector<DATA> theSchools ;
	int height1 = height(ptr) ;
	int nowheight = 1 ;

	while( ptr != NULL && nowheight <= height1 ) {

	  for (int i = 0; i < ptr->array.size(); i++) { // 檢查學校
        if ( school == ptr->array[i].schools[0].schoolName ) {
        	for( int j = 0; j < ptr->array[i].schools.size(); j++ ) {
        		theSchools.push_back(ptr->array[i].schools[j]) ;
			} // for
				
            return theSchools; // jump out funtion
        } // if
      } // for
	  	
	  if ( ptr->children.size() == 0 )
	  	break ;
	  
	  if (ptr->array.size() == 1) { // 已存入一間學校 
        if ( school > ptr->array[0].schools[0].schoolName) { // walk right
            ptr = ptr->children[1]; 
        }  // if
        else { // walk left
            ptr = ptr->children[0]; 
        } // else
      } // if
      else if (ptr->array.size() == 2) { // 已存入兩間學校 
        if (school > ptr->array[1].schools[0].schoolName) { // walk right
            ptr = ptr->children[2] ;
        }  // if
        else if ( school < ptr->array[1].schools[0].schoolName && school > ptr->array[0].schools[0].schoolName) {
            ptr = ptr->children[1] ; // walk middle
        }  // else if
        else {
            ptr = ptr->children[0] ; // walk left
        } // else
	  } // else if	
	  nowheight ++ ;
	} // while
	
} // SearchPath()


// ****************************************  AVL Tree ***************************************

int AVL::BF(AVLNode* nodePtr) {
    int bf = 0;
    bf = maxDepth(nodePtr->left) - maxDepth(nodePtr->right);
    return bf;
}  // BF()

AVLNode* AVL::LLRotate(AVLNode* x) {
    AVLNode* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}  // LLRotate()

AVLNode* AVL::RRRotate(AVLNode* x) {
    AVLNode* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}  // RRRotate()

AVLNode* AVL::LRRotate(AVLNode* x) {
    AVLNode* y = x->left;
    AVLNode* z = y->right;
    y->right = z->left;
    x->left = z->right;
    z->right = x;
    z->left = y;
    return z;
}  // LRRotate()

AVLNode* AVL::RLRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* z = y->left;
    y->left = z->right;
    x->right = z->left;
    z->left = x;
    z->right = y;
    return z;
}  // RLRotate()

AVLNode* AVL::GetNewNode(int dataNum, string majorName) {
    AVLNode* newNode = new AVLNode();
    newNode->dataNum = dataNum;
    newNode->majorName = majorName;
    newNode->height = 1;
    newNode->next = NULL;
    newNode->left = newNode->right = NULL;
    return newNode;
}  // GetNewNode()

/* When the majorName are the same,
add into the same node */
AVLNode* AVL::InsertSameNode(AVLNode* nodePtr, int dataNum, string majorName) {
    AVLNode* walkPtr = nodePtr;
    while (walkPtr->next != NULL) {
        walkPtr = walkPtr->next;
    }  // end while
    walkPtr->next = new AVLNode();
    walkPtr->next->dataNum = dataNum;
    walkPtr->next->majorName = majorName;
    walkPtr->next->next = NULL;
    walkPtr->next->left = walkPtr->next->right = NULL;
    return nodePtr;  // return walkPtr OR nodePtr??
}  // InsertSameNode()

/* Just insert the new item as a new leaf */
AVLNode* AVL::InsertANode(AVLNode* nodePtr, int dataNum, string majorName) {
    if (nodePtr == NULL) {
        nodePtr = GetNewNode(dataNum, majorName);
    }  // end if
    else if (strcmp(majorName.c_str(), nodePtr->majorName.c_str()) == 0) {
        nodePtr = InsertSameNode(nodePtr, dataNum, majorName);
    }  // end else if
    else if (strcmp(majorName.c_str(), nodePtr->majorName.c_str()) < 0) {
        nodePtr->left = Insert(nodePtr->left, dataNum, majorName);
    }  // end else if
    else {
        nodePtr->right = Insert(nodePtr->right, dataNum, majorName);
    }  // end else

    return nodePtr;
}  // Insert()

/* After insert a new node,
check if the heights of left and right differ by at most 1 */
AVLNode* AVL::Insert(AVLNode* nodePtr, int dataNum, string majorName) {
    nodePtr = InsertANode(nodePtr, dataNum, majorName);

    if (BF(nodePtr) >= 2 && BF(nodePtr->left) >= 0) {
        nodePtr = LLRotate(nodePtr);
    }  // end if
    else if (BF(nodePtr) <= -2 && BF(nodePtr->right) <= 0) {
        nodePtr = RRRotate(nodePtr);
    }  // end else if
    else if (BF(nodePtr) >= 2 && BF(nodePtr->left) <= 0) {
        nodePtr = LRRotate(nodePtr);
    }  // end else if
    else if (BF(nodePtr) <= -2 && BF(nodePtr->right) >= 0) {
        nodePtr = RLRotate(nodePtr);
    }  // end else if
    return nodePtr;

}  // Insert()

vector<DATA> AVL::Retrieval( string majorName, vector<DATA>  list ) {
	vector<DATA> store ;
	AVLNode * nodePtr = root ;
	
	while ( nodePtr != NULL ) {
		if ( strcmp(majorName.c_str(), nodePtr->majorName.c_str()) == 0 ) {
			for( ; nodePtr != NULL; nodePtr = nodePtr->next ) {
              store.push_back( list[nodePtr->dataNum-1] ) ;
            } // for
            return store ;
		} // end if
		else if ( strcmp(majorName.c_str(), nodePtr->majorName.c_str()) > 0 ) {
			nodePtr = nodePtr->right ;
		} // end else if
		else if ( strcmp(majorName.c_str(), nodePtr->majorName.c_str()) < 0 ) {
			nodePtr = nodePtr->left ;
		} // end else if
	} // end while
	
	return store ;
} // Retrieval()

void AVL::SetRoot( AVLNode* copyRoot ) {
	root = copyRoot ;
} // SetRoot()

/* Calculate the height */
int AVL::maxDepth(AVLNode* nodePtr) {
    if (nodePtr == NULL) {
        return 0;
    }  // end if
    else {
        int leftDepth = maxDepth(nodePtr->left);
        int rightDepth = maxDepth(nodePtr->right);
        if (leftDepth > rightDepth)
            return (leftDepth + 1);
        else
            return (rightDepth + 1);
    }  // end else
}  // maxDepth()

void AVL::PrintNode(AVLNode* node, vector<DATA> list) {
    for (int i = 1; node != NULL; i++) {
        cout << i << ": ";
        int num = node->dataNum - 1;
        cout << "[ " << list[num].dataNum << " ]"
             << "\t";
        cout << list[num].schoolName << "\t";
        cout << list[num].majorName << "\t";
        cout << list[num].type << "\t";
        cout << list[num].level << "\t";
        cout << list[num].studentNum << "\n";
        node = node->next;
    }  // end for
}  // PrintNode()

vector<DATA> Intersect( vector<DATA> tttree, vector<DATA> avl ) {
	vector<DATA> intersect ;
	for ( int index1 = 0; index1 < tttree.size(); index1++ ) {
		for ( int index2 = 0; index2 < avl.size(); index2++ ) {;
			if ( tttree[index1].dataNum == avl[index2].dataNum ) {
				intersect.push_back( tttree[index1] ) ;
			} // end if
		} // end for
	} // end for

	return intersect ;
} // Intersection()

// **************************************** Read File ***************************************

/*read input data.
First read a char then turn it into string, then into integer */
int inputCharToInt(fstream& inputfile) {
    int i = 0;
    int inputNum = 0;
    char inputAchar;
    char temp[30] = {0};
    inputAchar = inputfile.get();
    while (inputAchar != '\t' && inputAchar != '\n' && inputAchar != EOF) {
        if (inputAchar >= '0' && inputAchar <= '9') {
            temp[i] = inputAchar;
            i++;
        }  // end if
        inputAchar = inputfile.get();
    }  // end while
    temp[i] = '\0';
    inputNum = atoi(temp);
    return inputNum;
}  // inputChatToInt()

string inputString(fstream& inputfile) {
    char inputAchar = inputfile.get();
    int i = 0;
    char String[50] = {0};
    string text = "\0";
    while (inputAchar != '\t' && inputAchar != '\n' && inputAchar != EOF) {
        String[i] = inputAchar;
        i++;
        inputAchar = inputfile.get();
    }  // end while

    String[i] = '\0';
    return String;
}  // inputString()

void intoVector(fstream& inputfile, DATA& temp) {
    temp.dataNum++;

    string text = "\0";
    text = inputString(inputfile);
    temp.schoolID = text;

    text = "\0";
    text = inputString(inputfile);
    temp.schoolName = text;

    text = "\0";
    text = inputString(inputfile);
    temp.majorID = text;

    text = "\0";
    text = inputString(inputfile);
    temp.majorName = text;

    text = "\0";
    text = inputString(inputfile);
    temp.type = text;

    text = "\0";
    text = inputString(inputfile);
    temp.level = text;

    int tempNum = 0;
    tempNum = inputCharToInt(inputfile);
    temp.studentNum = tempNum;

    tempNum = 0;
    tempNum = inputCharToInt(inputfile);
    temp.teacherNum = tempNum;

    tempNum = 0;
    tempNum = inputCharToInt(inputfile);
    temp.graduateNum = tempNum;

    text = "\0";
    text = inputString(inputfile);
    temp.city = text;

    text = "\0";
    text = inputString(inputfile);
    temp.system = text;

}  // intoVector()

void ReadFile(fstream& inputfile, vector<DATA>& listdata) {
    DATA temp;
    string garbage;
    for (int i = 0; i < 3; i++) {
        getline(inputfile, garbage);
    }  // end for

    while (inputfile) {
        intoVector(inputfile, temp);
        if (temp.schoolID != "")
            listdata.push_back(temp);
    }  // end while
}  // readfile()




int main() {
    string command = "\0";
    cout << endl
         << "*** Search Tree Utilities **" << endl;
    cout << "* 0. QUIT                  *" << endl;
    cout << "* 1. Build 2-3 tree        *" << endl;
    cout << "* 2. Build AVL tree        *" << endl;
    cout << "* 3. Intersection Query    *" << endl;
    cout << "*************************************" << endl;
    cout << "Input a choice(0, 1, 2, 3): ";
    cin >> command;
    while ( command != "0" && command != "1" && command != "2" && command != "3" ) {
        cout << "Command does not exist!" << endl;
        cout << "Input a choice(0, 1, 2, 3):  " << endl;
        cin >> command;
    }  // while
    vector<DATA> list ;
    bool run1 = false ;
    bool run2 = false ;
    TwoThree twoThreeTree ;
    
    while (command != "0") {
        
        string filename ;
        string fileNum ;
        fstream inputFile ;
        //TwoThree twoThreeTree ;
        AVL aAVLTree ;
        AVLNode * root = NULL;
        
        if ( command == "1" ) {
	      run1 = true ;
	      run2 = false ;
	      list.clear() ;
          cout << "Input a file number (e.g., 201, 202, 203, ...) : ";
          cin >> fileNum;
          filename = "input" + fileNum + ".txt";
          inputFile.open(filename.c_str(), ios::in);

          if (!inputFile) {
            cout << "File " << fileNum << " does not exist." << endl;
          }  // end if
          else {
            ReadFile(inputFile, list);  // Read data and save into vector
            inputFile.close();
		    
			TwoThreeNode* ptr = NULL ;
			twoThreeTree.root = NULL ;
            for (int i = 0; i < list.size(); i++) {
                ptr = twoThreeTree.Insert(list[i], twoThreeTree.getroot());
                if (ptr != NULL) { // 學校沒出現過 已存入樹中 
                    twoThreeTree.Split(ptr);
                }  // if
            } //  for

            printf("Tree height = %d\n", twoThreeTree.height(twoThreeTree.getroot()));
            twoThreeTree.PrintRoot();
        
          } // else
    	} // end if
	
	
        else if ( command == "2" ) {
          if ( run1 == false ) {
          	cout << "### Choose 1 first. ###" << endl ;
		  } // end if
		  else {
		  
		  run2 = true ;
          int height = 0;
          int i = 0;  
          while (i < list.size()) {
            root = aAVLTree.Insert(root, list[i].dataNum, list[i].majorName);
            i++ ;
          }  // end while
          cout << endl << "Tree height: ";
          height = aAVLTree.maxDepth(root);
          cout << height << endl;
          cout << "root: " << endl;
          aAVLTree.SetRoot( root ) ;
          aAVLTree.PrintNode(root, list);
        } // end else
        } // end else if

	    else if ( command == "3" ) {
	      if ( run1 == false || run2 ==false ) {
	      	cout << "### Choose 1 and 2 first. ###" << endl ;
		  } // end if
		  else {
		    vector<DATA> tttree ;
	        vector<DATA> avl ;
		    vector<DATA> intersect ;
		    string schoolName, majorName ;
		    cout << "Enter a college name to search [*]: " ;
		    cin >> schoolName ;
		    cout << "Enter a department name to search [*]: " ;
		    cin >> majorName ;

		    if ( schoolName == "*" && majorName == "*" ) {
		      intersect = Intersect( list, list ) ;
		    } // end if
		    else if ( schoolName == "*" && majorName != "*" ) {
		      avl = aAVLTree.Retrieval( majorName, list ) ;
		  	  intersect = Intersect( list, avl ) ;
		    } // end else if
		    else if ( schoolName != "*" && majorName == "*" ) {
		      tttree = twoThreeTree.SearchSchool( schoolName ) ;
		  	  intersect = Intersect( tttree, list ) ;
	        } // end else if
	        else {
	          avl = aAVLTree.Retrieval( majorName, list ) ;
	          tttree = twoThreeTree.SearchSchool( schoolName ) ;
	          if ( tttree.size() == 0 && avl.size() != 0 ) {
			  } // if
			  else
	            intersect = Intersect( tttree, avl ) ;
		    } // end else
				    
            int count = 1 ;
            for ( int i = 0; i< intersect.size(); i++ ) {
              printf( "%d", count ) ;
              printf(": [%d] %s, %s, %s, %s, %d\n", intersect[i].dataNum, intersect[i].schoolName.c_str(),
			        intersect[i].majorName.c_str(), intersect[i].type.c_str(), intersect[i].level.c_str(), intersect[i].studentNum ) ;
			  count++ ;
		    } // end for

          } // end else if
        } // end else
		
        cout << endl ;
        cout << "*** Search Tree Utilities **" << endl;
        cout << "* 0. QUIT                  *" << endl;
        cout << "* 1. Build 2-3 tree        *" << endl;
        cout << "* 2. Build AVL tree        *" << endl;
        cout << "* 3. Intersection Query    *" << endl;
        cout << "*************************************" << endl;
        cout << "Input a choice(0, 1, 2, 3): ";
        cin >> command;
        while (command != "0" && command != "1" && command != "2" && command != "3") {
            cout << "Command does not exist!" << endl;
            cout << "Input a choice(0, 1, 2, 3):  " << endl;
            cin >> command;
        }  // end while
    } // end while

}  // main()

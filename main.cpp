#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
using namespace std;
#define V 7

//Graph of paths to Continents
int cost [7] [7]= 
    { 
        { 999999, 1303, 3991, 4, 8734, 8418, 16396},                //North America
        { 1303, 999999, 14, 6706,  9748, 5423, 16396},              //Europe
        { 3991, 14, 999999 , 8418 , 10503 , 2839 , 9177},           //Africa
        { 4 ,  6706 , 8418 , 999999, 7463 , 7962 , 13121},          //Asia
        { 8734, 9748 , 10503 , 7463 , 999999, 8491 , 6400},         //Australia
        { 8418, 5423 , 2839 , 7962 ,  8491, 999999 , 9805 },        //South America
        { 16396, 16396, 9177, 13121 , 6400, 9805, 999999}           //Antartica 
    }; 

//AVL struct
struct Tree {
    string name;
    string artist;
    string year;
    string path;
    int height;
    Tree *left;
    Tree *right;
};
Tree *root = NULL;
Tree *parent = root;

int height(Tree *temp){
    if(temp==NULL)
        return 0;

    return temp->height;
}

int Max(int left, int right){
    if(left>right)
    return left;

    return right;
}

int Balance(Tree *temp){
    if(temp==NULL)
        return 0;

    return height(temp->left)-height(temp->right);    
}

Tree* newNode(string name )  
{  
    string artist,year,path;
    Tree* node = new Tree;
    cout<<"Enter Artist Name : "<<endl;
    cin>>artist;
    cout<<"Enter Year : "<<endl;
    cin>>year;
    cout<<"Enter Path of the song : "<<endl;
    cin>>path;
    node->name = name;
    node->artist=artist;
    node->year=year;
    node->path=path;
    node->left = NULL;  
    node->right = NULL;  
    node->height = 1; 
    return node;  
}


Tree *rightRotate(Tree *temp)  
{  
    Tree *X = temp->left;  
    Tree *Y = X->right;  
  
    // Perform rotation  
    X->right = temp;  
    temp->left = Y; 
  
    // Update heights  
    temp->height = max(height(temp->left),height(temp->right)) + 1;  

    X->height = max(height(X->left),height(X->right)) + 1;  
  
    // Return new root  
    return X;  
}  
  
 
Tree *leftRotate(Tree *temp)  
{  
    Tree *X = temp->right;  
    Tree *Y = X->left;  
  
    // Perform rotation  
    X->left = temp;  
    temp->right = Y; 
  
    // Update heights  
    temp->height = max(height(temp->left),height(temp->right)) + 1;  

    X->height = max(height(X->left),height(X->right)) + 1;  
  
    // Return new root  
    return X;  
}

Tree* minValueNode(Tree* node)  
{  
    Tree* current = node;  
  
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)  
        current = current->left;  
  
    return current;  
}


Tree* insert(Tree * node, Tree *inNode, string name){
    //Base case
    if(node==NULL){
        Tree *temp = inNode;
        return temp;
    }
       
    if(name.compare(node->name)<0)
        node->left = insert(node->left,inNode,name);

    else if(name.compare(node->name)>0)
        node->right =insert( node->right,inNode,name);
    
    //Equal Case
    else
        return node;
    
    node->height = 1 + max(height(node->left),height(node->right));

    int balance = Balance(node);

    // Left Left Case  
    if (balance > 1 && name.compare(node->left->name)<0)  
        return rightRotate(node);
  
    // Right Right Case  
    if (balance < -1 && name.compare(node->right->name)>0)  
        return leftRotate(node);  
  
    // Left Right Case  
    if (balance > 1 && name.compare(node->left->name)>0)  
    {  
        node->left = leftRotate(node->left);  
        return rightRotate(node);  
    }  
  
    // Right Left Case  
    if (balance < -1 && name.compare(node->right->name)<0)  
    {  
        node->right = rightRotate(node->right);  
        return leftRotate(node);  
    } 

    return node;
}


Tree* Delete(Tree * node ,string name){
    
    //Base Condition
    if(node == NULL)
    return node;

    if(name.compare(node->name)<0){
        parent=node;
        node->left = Delete(node->left,name);
    }
        
    else if(name.compare(node->name)>0){
        parent=node;
        node->right = Delete(node->right,name);
    }

    else
    {
        // node with only one child or no child  
        if( (node->left == NULL) || (node->right == NULL) )  
        { 
            Tree *temp = NULL;
            if(node->left!=NULL)
            temp=node->left;
            else if(node->right!=NULL)
            temp = node->right;
  
            // No child case  
            if (temp == NULL)  
            {               
                temp = node;  
                node = NULL;  
            }  
            else // One child case  
            *node = *temp;
            free(temp);  
        }

        else
        {  
            Tree* temp = minValueNode(node->right);
            node->name = temp->name; 
            node->artist=temp->artist;
            node->year = temp->artist;
            node->path = temp->path;
            node->right = Delete(node->right,temp->name);  
        }
    }

    if (node == NULL)  
    return node;  

    node->height = 1 + max(height(node->left),height(node->right));

    int balance = Balance(node);

// Left Left Case  
    if (balance > 1 && name.compare(node->left->name)<0)  
        return rightRotate(node);
  
    // Right Right Case  
    if (balance < -1 && name.compare(node->right->name)>0)  
        return leftRotate(node);  
  
    // Left Right Case  
    if (balance > 1 && name.compare(node->left->name)>0)  
    {  
        node->left = leftRotate(node->left);  
        return rightRotate(node);  
    }  
  
    // Right Left Case  
    if (balance < -1 && name.compare(node->right->name)<0)  
    {  
        node->right = rightRotate(node->right);  
        return leftRotate(node);  
    }

    return node;
}

void writeToFile(Tree* node){

    string s=node->name+","+node->artist+","+node->year+","+node->path+",\n";

    // Create and open a text file
    ofstream MyFile("filename.txt",ios_base::app);

    // Write to the file

    MyFile << s;

    // Close the file
    MyFile.close();
}

void readFromFile(){
    
    string myText;

    ifstream MyReadFile("filename.txt");

    while (getline (MyReadFile, myText)) {

        char delim[]=",";
        char *token;

        //cout << "Text before being tokenized: " << myText << endl;

        token=strtok((char *)myText.c_str(), delim);
        string name(token);

        token=strtok(nullptr, delim);
        string artist(token);

        token=strtok(nullptr, delim);
        string year(token);
        
        token=strtok(nullptr, delim);
        string path(token);

        Tree* node= new Tree;
        node->name = name;
        node->artist=artist;
        node->year=year;
        node->path=path;
        node->left = NULL;  
        node->right = NULL;  
        node->height = 1; 
        root = insert(root, node,node->name);

    }

    // Close the file
    MyReadFile.close();

}

// Updating contents of file Before Exiting Program so that it retain the lastly made changes
void update(Tree *temp){

    ofstream MyFile("filename.txt",ios_base::app);
    // Write to the file
    if(temp!=NULL)
    {
        string s=temp->name+","+temp->artist+","+temp->year+","+temp->path+",\n";
        // Create and open a text file
        
        MyFile << s;
        update(temp->left);
        update(temp->right); 
    }
    else
    MyFile.close();
       
}
    
void updateFile(){

    int status = remove("filename.txt");
    if(status==0){
        update(root);
    }   
    else{
        cout<<"\nErorr Occurred! Couldn't find any file to update";
    }
    
}


//Struct for manipulation of current operations
struct Song {
    string name;
};


struct Stack
{
    int size;
    int top;
    Song *S;
};

void create(struct Stack *st)
{
    int size;
    st->size=20;
    st->top=-1;
    st->S=new Song [size];
}

void displayStack(struct Stack st)
{
    int i; 
    for(i=st.top ; i>=0;i--)
    {
        cout<<i+1<<" . "<<st.S[i].name<<"\n";
    }
}

bool searchStack(struct Stack *st, Song *x)
{
    bool isFound=false;
    int i; 
    for(i=st->top ; i>=0;i--)
    {
        if(st->S[i].name.compare(x->name)==0){
            isFound=true;
        }
    }
    return isFound;
}


void push(struct Stack *st,Song *x)
{
    if(st->top==st->size-1)
    cout<<"Stack Overflow  \n";
    else
    {  
            st->top++;
            st->S[st->top].name=x->name;
    }
}


Song* pop(struct Stack *st)
{
    Song *x=new Song;
    if(st->top==-1)
        return NULL;
    else{
        x->name=st->S[st->top].name;
        st->top--;
    }
    
    return x;
}

int isEmpty(struct Stack st)
{
    if (st.top == -1)
        return 1;
    return 0;
}

Tree* searchSong(string name){
    Tree *temp=root;
    bool found=false;
    while(temp!=NULL){ 
        if(name.compare(temp->name)<0){
            temp=temp->left;
        }
        else if(name.compare(temp->name)==0){
            found=true;
            break;
        }   
        else{
            temp=temp->right;
        }        
    }
    if(found==true)
        return temp;   
    else
        return NULL;      

}


void playSong(Tree* node){
    cout<<"-------Currently Playing "<<node->name<<"---------"<<endl;
}

struct Queue
{
    string name;
    string artist;
    string year;
    string path;
    Queue *next;
};
Queue *front= NULL;
Queue *rear=NULL;
int size=10;
int pos=0;

void enQueue(Tree* node){
    Queue *temp= new Queue;
    temp->name=node->name;
    temp->artist=node->artist;
    temp->year=node->year;
    temp->path=node->path;
    temp->next=NULL;

    if(pos==size){
        cout<<"Queue is full"<<endl;
    }
    else
    {
        if(rear==NULL){
            front=rear=temp;
        }
        else
        {
            rear->next=temp;
            rear=temp;
        }
        pos++; 
        
    }

}

// Struct for Queue to make a Playlist
Queue* deQueue(){

    Queue *temp=front;
    if(front==NULL){
        cout<<"Queue is empty"<<endl;
        return NULL;
    }
    else
    {
       front=front->next;
       //delete(temp);
        pos--;
       return temp;
     
    }

}

void displayQueue(){
Queue *temp=front;
while(temp!=rear->next){
     cout<<temp->name<<" | "<<temp->artist<<" | "<<temp->year<<endl;
    temp=temp->next;
}
cout<<"\n";
}


void playNow(Queue* node){
    cout<<"-------Currently Playing "<<node->name<<"---------"<<endl;
}

void addToPlaylist(Tree* song){
        enQueue(song);
}

void streamPlaylist(int songscount){
    int i=0;
    if(front==NULL){
        cout<<"No song to play"<<endl;
    }
    else
    {
        while(i<songscount){
            Queue* getSong=deQueue();
            playNow(getSong);
            i++;
        }

    }
    
}


//Struct to make priority Queue of most played songs
struct pQueue
    {
        string name;
        int count;
        struct pQueue *next;
    };
    pQueue *pFront = NULL;
    pQueue *pRear = NULL;
    int pSize = 100;
    int pPos = 0;
    int c = 1;

void displayTopHits(int num){
    int i=0;
    pQueue *temp = pFront;
    while(temp!=pRear->next && i<=num){
        cout<<temp->name<<" was played "<<temp->count<<" times"<<endl;
        temp=temp->next;
        i++;
    }
    if(i!=num){
        cout<<"you only played "<<i<<" songs till date"<<endl;

    }
    cout<<endl;
}    

pQueue*SearchDelPQueue(string name){
    if(pFront==NULL)
    return NULL;

    pQueue *temp = pFront;
    pQueue *prev = NULL;
    while(temp!=pRear->next){
        if(temp->name==name)
        {
            if(temp==pFront){
                pFront=pFront->next;
                pPos--;
                return temp;
            }
            
            else if(temp==pRear)
            {
                prev->next=NULL;
                pRear=prev;
                pPos--;
                return temp;
            }
                
            else
            {
                prev->next=temp->next;
                temp->next=NULL;
                pPos--;
            }
            return temp;
        }
        prev=temp;
        temp=temp->next;
    }
    return NULL;
}
 

void enqueuePriority(string name){
    pQueue *temp = new pQueue;
    pQueue *found ;
    found = SearchDelPQueue(name);
    if(found == NULL)
    {   
        temp->count=c;
        temp->name=name;
        temp->next=NULL;
    }
    else 
    {
        c = found->count+c;
        temp->count= c;
        temp->name=name;
        temp->next=NULL;
    }
    
    if (pPos==pSize)
    {
        cout<<"Queue is Full\n";
        return;
    }

    if(pPos==0){
        pFront=pRear=temp;
        pPos++;
        return;
    }

    else if(c<=pRear->count)
    {
        pRear->next=temp;
        pRear=temp;
        pPos++;
        return;
    }

    else if(c>=pFront->count)
    {
        temp->next=pFront;
        pFront=temp;
        pPos++;
        return;

    }
    else
    {
        pQueue *index = pFront;
        pQueue *prev = NULL;
        int in = 0;
        while(in < pSize){
            if(c>=index->count)
            break;
            prev=index;
            index = index->next;
            in++;
        }
        prev->next=temp;
        temp->next=index;
        pPos++;
        return;
    }
}

void Inorder(Tree *temp){
    if(temp!=NULL)
    {
        Inorder(temp->left);
        cout<<temp->name<<" | "<<temp->artist<<" | "<<temp->year<<endl;
        Inorder(temp->right); 
    }
}


void displayCost(int minDistanceArray [], int vertices, string cont, string continents[7],int source,int dest){
    for(int i=0; i<vertices; i++){
        if(i==dest)
        cout<<"The minimum cost from "<<continents[source]<<" to "<<continents[i]<<" is "<<minDistanceArray[i]<<endl;
    }
}

void dijkstra(int cost[7][7], int vertices, int start,string cont, string continents[7],int dest){
 
    int minDistanceArray[7]={0};
    bool AlreadyVisited[7]={false};
    int count,currentMinIndex, nextvertex;

//Populating MinDistance Array
    for(int i=0; i<vertices; i++)
        minDistanceArray[i]=cost[start][i];
    
    minDistanceArray[start]=0;
    AlreadyVisited[start]=true;
    count=1;

    while(count<vertices-1){
        
        currentMinIndex=999999;

        //Traversing the MinDistanceArray to find the next Min index to move to
        for(int i=0; i<vertices; i++){
            if(minDistanceArray[i]<currentMinIndex && !AlreadyVisited[i]){
                currentMinIndex=minDistanceArray[i];
                nextvertex=i;
            }
        }
        AlreadyVisited[nextvertex]=1;

        //Changing Output Array if Desired
        for(int i=0; i<vertices; i++){
            if(AlreadyVisited[i]==false){
                if(currentMinIndex+cost[nextvertex][i]<minDistanceArray[i]){
                    minDistanceArray[i]=currentMinIndex+cost[nextvertex][i];
                }
            }
        }
        count++;
    }
    displayCost(minDistanceArray,vertices,cont,continents,start,dest);
}

void displayContinents()
{
    string continents [7] = {"North America","Europe","Africa","Asia","Australia","South America","Antartica "};
    for(int i = 0 ; i<=6 ; i++){
        cout<<i+1<<" "<<continents[i]<<endl;
    }
}


void graphInitialize(){
    int vertices=7;
    int start,numm;
    int dest;
    string continents[7] = { "North America", "Europe","Africa", "Asia","Australia","South America","Antarctica" };
    displayContinents();
    cout<<"Enter Your Location"<<endl;
    cin>>numm;
    start=numm-1;
    cout<<"Enter Artist Location"<<endl;
    cin>>dest;
    string cont=continents[start];
    dijkstra(cost,vertices,start,cont,continents,dest-1);
}


// Helper Queue Struck for Prims Algorithm
struct PQ
{
    int source;
    int dest;
    int weight;
    struct PQ *next;
};

PQ *pqFront = NULL;
PQ *pqRear = NULL;
int pqSize = 100;
int pqPos = 0;

PQ *OutpqFront = NULL;
PQ *Outlast = NULL;

void enQueuePrims(int s, int d, int w){
    PQ *temp= new PQ;
    temp->source=s;
    temp->dest=d;
    temp->weight=w;
    temp->next=NULL;

    if(pos==pqSize){
        cout<<"Queue is full"<<endl;
    }
    else
    {
        if(pqRear==NULL){
            pqFront=pqRear=temp;
        }
        else
        {
            if(temp->weight<=pqFront->weight){
                temp->next=pqFront;
                pqFront=temp;
            }
            else if(temp->weight>=pqRear->weight){
                pqRear->next=temp;
                pqRear=temp;
            }
            else
            {
                PQ *ptr=pqFront;
                PQ *prev=pqFront;
                while(ptr!=pqRear->next){
                    if(temp->weight<ptr->weight){
                        prev->next=temp;
                        temp->next=ptr;
                        break;
                        
                    }
                    prev=ptr;
                    ptr=ptr->next;
                }
            }   
        }
        pqPos++; 
        
    }

}

PQ* deQueuePrims(){
    PQ *ptr= pqFront;
    if(pqFront==NULL){
        cout<<"Queue is empty"<<endl;
        return NULL;
    }
    else
    {
       pqFront=pqFront->next;
       pqPos--;
       return ptr;
    }
}

void InsertMST(int s, int d, int w)
{
    PQ *temp= new PQ;
    temp->source=s;
    temp->dest=d;
    temp->weight=w;
    temp->next=NULL;

    if(OutpqFront==NULL)
    {
        OutpqFront=Outlast=temp;
    }
    else
    {
        Outlast->next=temp;
        Outlast=temp;
    }
}

void displayMST(){
    int cost=0;
    PQ *temp=OutpqFront;
    while(temp!=NULL){
        cost+=temp->weight;
        temp=temp->next;
    } 
    cout<<"Kilometers Travelled in Shortest way to Tour the World "<<cost<<endl;
}

void prims( int s , int graph [V][V])
{
    int start = s;
    int count = 0;
    bool visited [V] = {false};
    InsertMST(start,0,0);
    visited[start]=true;
    count++;

    while (count<=V-1){

        for (int i = 0 ; i<V;i++)
        {
            if(graph[start][i]!=0 && visited[i] == false)
            {
                enQueuePrims(start,i,graph[start][i]);     
            }    
        }

        PQ *temp = deQueuePrims();
        //displayPQ();
        count++;
        start = temp->dest;
        visited[temp->dest]=true;
        InsertMST(temp->source,temp->dest,temp->weight);
    }
    displayMST();   
}

void printMenu()
{
    string print [10] = {"Press 0 to Exit the Program","Press 1 for Inserting a New Song","Press 2 for Deleting a Song","Press 3 for Playing a Song","Press 4 to create a playlist","Press 5 for recently Played Song","Press 6 to View Most Played Songs","Press 7 for Finding Shortest Path to the Artist","Enter 8 to Display Song List","Enter 9 to Calculate World Tour Distance From your location "};
    for(int i = 0 ; i <160 ; i++)
    {
        if (i<20 || i>140)
        cout<<" ";
        else
        cout<<"=";
    }
    cout<<endl<<endl;

    for(int x = 1 ; x<=10 ; x++){
        for(int y = 0 ; y <=70 ; y++)
        {
            if (y==70){
                cout<<print[x-1]<<endl<<endl;
            }
            else
            cout<<" ";
        }
    }
    for(int i = 0 ; i <160 ; i++)
    {
        if (i<20 || i>140)
        cout<<" ";
        else
        cout<<"=";
    }
    cout<<endl<<endl;
}


int main()
{
    int opt;
    string n;
    int num;
    Tree *temp1;
    Tree *node;
    Song *song= new Song;
    struct Stack st;
    create(&st);
    readFromFile();
  
        do{
            printMenu();
            cout<<"\n Enter your choice: ";
            cin>>opt;
            if(cin.fail()){
                cout<<"Invalid Input";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else{
                if (opt == 0){
                updateFile();
                break;
                }
                else
                {
                    switch(opt)
                    {
                        case 1:
                        //INSERTION
                            node=new Tree;
                            cout<<"Enter Song Name : "<<endl;
                            cin>>node->name;
                            cout<<"Enter Artist Name : "<<endl;
                            cin>>node->artist;
                            cout<<"Enter Year : "<<endl;
                            cin>>node->year;
                            cout<<"Enter Path of the song : "<<endl;
                            cin>>node->path;
                            node->left = NULL;  
                            node->right = NULL;  
                            node->height = 1;
                            if(searchSong(node->name)==NULL) 
                            root = insert(root, node,node->name);
                            else
                            cout<<"Song Already Exists";
                            break;

                        case 2:
                        //DELETION
                        Inorder(root);
                            cout<<"Enter Name of the Song : "<<endl;
                            cin>>n;
                            if(searchSong(n)==NULL)
                            cout<<"Song Not Found";
                            else
                            root = Delete(root,n);
                            break;

                        case 3: 
                        //PLAY SONG
                        Inorder(root);
                            cout<<"Enter Name of the Song which you would like to play: "<<endl;
                            cin>>n;
                            temp1 = searchSong(n);
                            if(temp1!=NULL){
                                cout<<"Song name: "<<temp1->name<<"| Artist: "<<temp1->artist<<"| Year: "<<temp1->year<<endl;
                                playSong(temp1);
                                song->name=temp1->name;
                                push(&st,song);
                                enqueuePriority(song->name);
                            }
                            else{
                            cout<<"Sorry we could not find the desired song"<<endl;
                            }
                            break;   

                        case 4: 
                        //CREATE PLAYLIST
                            cout<<"Enter the number of songs you would like to add to your playlist "<<endl;
                            cin>>num;

                            if(cin.fail() || num<-1){
                                cout<<"Invalid input "<<endl;
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            }
                            else
                            {
                                if(num>10)
                                    cout<<"You can add maximum 10 songs"<<endl;
                                else
                                {
                                    for(int i=0; i<num; i++){
                                        cout<<"Enter Name of the Song : "<<endl;
                                        cin>>n;
                                        Tree* playSong=searchSong(n);
                                        if(playSong==NULL){
                                            cout<<"The song does not exist"<<endl;
                                        }
                                        else
                                        {
                                            addToPlaylist(playSong);
                                            song->name=playSong->name;
                                            push(&st,song);
                                            enqueuePriority(song->name);
                                        } 
                                    }
                                    streamPlaylist(num);  
                                    }
                                }
                            break;

                        case 5:
                        //RECENTLY PLAYED
                            if(st.top==-1){
                                cout<<"No song was played"<<endl;
                            }
                            else
                            {
                            displayStack(st);
                            }

                            break;

                        case 6: 
                        //MOST PLAYED
                            cout<<"THE TOP 3 SONGS ARE "<<endl;
                            displayTopHits(3);
                            break;

                        case 7: 
                        //FINDING SHORTEST PATH TO THE ARTIST
                            graphInitialize();
                            break;

                        case 8:
                        //DISPLAY LIST
                            Inorder(root);
                            break;

                        case 9:
                            int s;
                            displayContinents();
                            cout<<"Enter Your Location"<<endl;
                            cin>>s;
                            prims(s,cost);
                            break;

                        default:
                            cout<<"\n Plz enter correct choice "<<"\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            break;
                    }
                }
            }
        }while(true);
}
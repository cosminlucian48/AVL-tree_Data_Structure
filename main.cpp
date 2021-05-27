#include <iostream>
#include <fstream>
#include <queue>
#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;

string file_out1("test1.out");
ofstream output_file(file_out1);


// arborele
class AVL
{
private:
    static int cardinal;
public:
    int key;
    AVL *left;
    AVL *right;
    int height;
    static void incr(){cardinal++;}
    static void dcr(){cardinal--;}
    static int card(){return cardinal;}
    static void resetCard(){cardinal=0;}
};

int AVL::cardinal=0;

// maximul dintre doua numere folosit la rotatii
int maximul(int a, int b)
{
    return (a > b)? a : b;
}
// inalitmea aroberlui
int height(AVL *N)
{
    if (N == nullptr)
        return 0;
    return N->height;
}


/* functie pentru crearea nodului,
 cu frunzele stanga si dreapta nule*/
AVL* newAVL(int key)
{

    AVL* nod = new AVL();
    AVL::incr();
    nod->key = key;
    nod->left = nullptr;
    nod->right = nullptr;
    nod->height = 1; // nodul, adaugat initial ca si o frunza
    return(nod);
}

// rotare la dreapta pentru radacina sub-arbore cu radacina y
AVL *rotireDreapta(AVL *y)
{
    AVL *x = y->left;
    AVL *T2 = x->right;

    // rotatia
    x->right = y;
    y->left = T2;

    // actualizare inaltime
    y->height = maximul(height(y->left),
                        height(y->right)) + 1;
    x->height = maximul(height(x->left),
                        height(x->right)) + 1;

    // noua radacina
    return x;
}

// rotire la stanga pentru sub-arbore cu radacina x
AVL *rotireStanga(AVL *x)
{
    AVL *y = x->right;
    AVL *T2 = y->left;

    // rotatia
    y->left = x;
    x->right = T2;

    // actualizare inaltime
    x->height = maximul(height(x->left),
                        height(x->right)) + 1;
    y->height = maximul(height(y->left),
                        height(y->right)) + 1;

    // noua radacina
    return y;
}

// indicele de balanta (diferente dintre noduri)
// daca indicele este mai mare decat 1 nu este balansat (cazul stanga stanga sau stanga dreapta)
// daca indicele este mai mic decat -1 nu este balansat (cazul dreapta dreapta sau dreapta stanga)

int getBalance(AVL *N)
{
    if (N == nullptr)
        return 0;
    return height(N->left) - height(N->right);
}
/* daca avem un arbore cu elemente returnam cel mai din stanga element (cel mai mic) */
AVL * min(AVL* nod)
{
    if(nod== nullptr)
        return nullptr;
    AVL* current = nod;

    /* loop pentru cel mai din stanga element */
    while (current->left != nullptr)
        current = current->left;
    return current;
}
AVL* max(AVL* nod)
{
    if(nod== nullptr)
        return nullptr;
    AVL* current = nod;

    /* loop pentru cel mai din dreapta element */
    while (current->right != nullptr)
        current = current->right;
    return current;
}
// functie recursiva pentru inserare care returneaza apoi arborele
AVL* insereaza(AVL* nod, int key)
{

    /* 1. inserarea normala ca intr-un arbore de cautare binara */
    if (nod == nullptr)
        return(newAVL(key));

    if (key < nod->key)
        nod->left = insereaza(nod->left, key);

    else if (key > nod->key)
        nod->right = insereaza(nod->right, key);
    else // pentru elementele care sunt egale nu facem nimic
    {
        cout<<"Elementul "<<key<<" exista deja.\n";
        output_file<<"Elementul "<<key<<" exista deja.\n";
        return nod;
    }
    /* 2. actualizam inaltimea nodului parinte */
    nod->height = 1 + maximul(height(nod->left),
                              height(nod->right));

    /* 3. luam indicele de balansare al parintelui */
    int balance = getBalance(nod);

    // avem 4 cazuri de nebalansare

    // stanga-stanga
    if (balance > 1 && key < nod->left->key)
        return rotireDreapta(nod);

    // dreapta-dreapta
    if (balance < -1 && key > nod->right->key)
        return rotireStanga(nod);

    // stanga-dreapta
    if (balance > 1 && key > nod->left->key)
    {
        nod->left = rotireStanga(nod->left);
        return rotireDreapta(nod);
    }

    // dreapta-stanga
    if (balance < -1 && key < nod->right->key)
    {
        nod->right = rotireDreapta(nod->right);
        return rotireStanga(nod);
    }

    return nod;
}
// functie recursiva pentru stergere care returneaza apoi arborele
AVL* sterge(AVL* root, int key)
{

    // 1.stergem
    if (root == nullptr) {
        return root;
    }
    // daca elementul cautat este mai mic decat radacina, cautam in stanga
    if (key < root->key)
        root->left = sterge(root->left, key);

        // daca elementul cautat este mai mare decat radacina, cautam in dreapta
    else if (key > root->key)
        root->right = sterge(root->right, key);

        // daca elementul este radacina
    else {


        // nod cu un singur copil sau fara
        if ((root->left == nullptr) || (root->right == nullptr)) {
            AVL *temp = root->left ?   //daca left exista se atribuie left, daca nu, se va atribui right
                        root->left :
                        root->right;


            // fara copil
            if (temp == nullptr) {

                root->dcr();
                temp = root;
                root = nullptr;
            }
            else // un singur copil
            {*root = *temp;
                root->dcr();} // copiem continutul copilului (care nu e null)
            free(temp);
        } else {
            // nod cu doi copii: succesorul cel mai mic din sub-arborele din dreapta
            AVL *temp = min(root->right);

            // il copiem pe acesta in nodul nostru
            root->key = temp->key;

            // stergem succesorul apoi
            root->right = sterge(root->right,
                                    temp->key);
        }
    }

    // daca arborele avea un singur nod
    if (root == nullptr) {
        return root;
    }

    // 2. Actualizam inaltimea nodului curent
    root->height = 1 + maximul(height(root->left),
                               height(root->right));

    // 3. verificam daca a devenit nebalansat
    int balance = getBalance(root);

    // avem 4 cazuri

    // stanga-stanga
    if (balance > 1 &&
        getBalance(root->left) >= 0)
        return rotireDreapta(root);

    // stanga-dreapta
    if (balance > 1 &&
        getBalance(root->left) < 0) {
        root->left = rotireStanga(root->left);
        return rotireDreapta(root);
    }

    // dreapta-dreapta
    if (balance < -1 &&
        getBalance(root->right) <= 0)
        return rotireStanga(root);

    // dreapta-stanga
    if (balance < -1 &&
        getBalance(root->right) > 0) {
        root->right = rotireDreapta(root->right);
        return rotireStanga(root);
    }

    return root;
}


AVL* succesor(AVL* root,int key)
{

    AVL* succ = nullptr;

    while (1)
    {
        // daca elementul este mai mic decat radacina il cautam in stanga
        if (key < root->key)
        {
            // setam succesor ca si radacina inainte de a intra in sub-arborele stang
            succ = root;
            root = root->left;
        }

            // daca elementul este mai mare decat radacina il cautam in dreapta
        else if (key > root->key) {
            root = root->right;
        }

            // daca gasim valoarea pentru care cautam succesorul, succesorul este minimul din
            // sub-arborele drept al acestui nod(daca exista)

        else {
            if (root->right) {
                succ = min(root->right);
            }
            break;
        }

        // daca nu exista elementul in arbore
        if (!root) {
            return nullptr;
        }
    }
    //returnam succesorul, daca exista
    return succ;

}

AVL* predecesor(AVL* root, int key)
{
    AVL* pred = nullptr;
    while (1)
    {
        // daca cheia data este mai mic decat cheia root-ului, vizitam subarborele stang
        if (key < root->key) {
            root = root->left;
        }
        // daca cheia data este mai mare decat cheia root-ului, vizitam subarborele drept
        else if (key > root->key)
        {
            // updatam pred la valoarea nodului actual inainte de a intra in subarbore
            pred = root;
            root = root->right;
        }

        //daca gasim valoarea pentru care cautam predecesorul, predecesorul este maximul din
        // sub-arborele stang al acestui nod(daca exista)

        else {
            if (root->left) {
                pred = max(root->left);
            }
            break;
        }
        // daca nu exista elementul in arbore
        if (!root) {
            return nullptr;
        }
    }
    //returnam predecesorul, daca exista
    return pred;
}

//functie ajutatoare pt al klea element folosita in afisat elementul k
AVL* kth(AVL* root,int& k)
{
    // fara
    if (root == nullptr)
        return nullptr;

    // cautam in sub-arborele stang
    AVL* left = kth(root->left, k);

    // daca este gasit in sub-arborele drept
    if (left != nullptr)
        return left;

    // daca elementul curent este al k-lea, returneaza
    k--; //4 -> 3 -> 2 -> 1
    if (k == 0)
        return root;

    // altfel cautam in subarborele drept
    return kth(root->right, k);
}

// afisare al k-lea numar
AVL*  k_element(AVL* root,int k)
{
    long long k_aux=k;
    AVL* res = kth(root, k);
    return res;
}

//cardinal
int cardinal(AVL* root){
    if (root==nullptr)
        return 0;
    else return root->card();
}

//cautare element
AVL* este_in(AVL* root, int key)
{

    if (root == nullptr || root->key == key)
        return root;

    // elementul este mai mare decat radacina
    if (root->key < key)
        return este_in(root->right, key);

    // elementul este mai mic decat radacina
    return este_in(root->left, key);
}

//printuri
void preOrder(AVL *root)
{
    if(root != nullptr)
    {
        output_file << root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}
void Inorder(AVL* root)
{
    if (!root) {
        return;
    }
    Inorder(root->left);

    cout<<root->key<<" ";
    Inorder(root->right);
}
void printLevelOrder(AVL *root)
{
    // Base Case
    if (root == nullptr) return;

    // Create an empty queue for level order tarversal
    queue<AVL *> q;

    // Enqueue Root and initialize height
    q.push(root);

    while (!q.empty())
    {
        // nodCount (queue size) indicates number
        // of nods at current lelvel.
        long long nodCount = q.size();

        // Dequeue all nods of current level and
        // Enqueue all nods of next level
        while (nodCount > 0)
        {
            AVL *nod = q.front();
            output_file << nod->key << " ";
            q.pop();
            if (nod->left != nullptr)
                q.push(nod->left);
            if (nod->right != nullptr)
                q.push(nod->right);
            nodCount--;
        }
        cout << endl;
    }
}
void deleteTree(AVL* node)
{
    if (node == NULL) return;

    //se sterge pe subtree uri
    deleteTree(node->left);
    deleteTree(node->right);
    //fiecare nod
    delete node;
}

int main()
{



    string file_in1="test1.in",file_in2="test2.in",file_in3="test3.in",file_in4="test4.in",file_in5="test5.in",file_in6="test6.in", fisier;

    int i;
    bool bul1=true;
    int ok=0;
    while(bul1)
    {
        ok=0;
        cout<<"\nAlegeti testul:"<<"\n";
        cout<<"0. Inchide\n";
        cout<<"1. numere de la -1.5mil la 1mil creascator \n";
        cout<<"2. 100 de mii de numere ordonate de la 0-100k\n";
        cout<<"3. numere de la 1 la 5mil crescator.\n";
        cout<<"4. 9 numere random\n";
        cout<<"5. 100 de mii de numere random intre -100k - +100k\n";
        cout<<">>";
        cin>>i;
        switch(i)
        {
            case 0:{bul1=false;ok=1;break;}
            case 1: {fisier = file_in1;break;}
            case 2: {fisier = file_in2;break;}
            case 3: {fisier = file_in3;break;}
            case 4: {fisier = file_in4;break;}
            case 5: {fisier = file_in5;break;}
            default: {cout<<"\nAi gresit optiunea.\n";ok=1;break;}

        }
        if(ok==0){
            AVL *root = nullptr;
            ifstream input_file(fisier);
            int number;


            if (!input_file.is_open()) {
                cerr << "Could not open the file - '"
                     << fisier << "'" << endl;
                return EXIT_FAILURE;
            }

            //inserare elemente din fisier in arbore
            while (input_file >> number) {


                root = insereaza(root, number);

            }

            int j;
            bool bul2=true;//0.077000 sec
            while(bul2)
            {
                cout<<endl;
                cout<<"0.Iesire"<<endl;
                cout<<"1.Introduceti un numar in multime"<<endl;
                cout<<"2.Stergeti un numar din multime"<<endl;
                cout<<"3.Afisati multimele"<<endl;
                cout<<"4.Afisati cel mai mare element din multime"<<endl;
                cout<<"5.Afisati cel mai mic element din multime"<<endl;
                cout<<"6.Verificati daca elementul k este in multime"<<endl;
                cout<<"7.Afisati cardinalul multimii."<<endl;
                cout<<"8.Afisati al k-lea element."<<endl;
                cout<<"9.Afisati succesor elementului k:"<<endl;
                cout<<"10.Afisati predecesor elementului k:"<<endl;
                cout<<">>";cin>>j;

                switch(j)
                {
                    case 0: {
                        deleteTree(root);
                        root->resetCard();
                        bul2 =false;
                        break;
                    }

                    case 1: {

                        int nr;
                        cout<<"Scrie numarul-> ";
                        cin>>nr;

                        root=insereaza(root,nr);



                        break;
                    }
                    case 2:{
                        int nr;
                        cout<<"Scrie numarul-> ";
                        cin>>nr;
                        root=sterge(root,nr);
                        if(root!= nullptr)
                        {
                            cout<<"S-a sters "<<nr<<".\n";
                            output_file<<"S-a sters "<<nr<<".\n";
                        }
                        break;
                    }
                    case 3:{
                        Inorder(root);
                        break;
                    }
                    case 4:{
                        cout<<"Elementul cu valoarea maxima din arbore -> ";
                        cout<<max(root)->key;
                        output_file<<"Elementul cu valoarea maxima din arbore -> ";
                        output_file<<max(root)->key;
                        break;
                    }
                    case 5:{

                        cout<<min(root)->key;
                        cout<<"Elementul cu valoarea minima din arbore -> ";


                        output_file<<"Elementul cu valoarea minima din arbore -> ";
                        output_file<<min(root)->key;


                        break;
                    }
                    case 6:{

                        cout<<"Alegeti elementul pe care vreti sa-l cautati: ";
                        int a;
                        cin>>a;

                        AVL* nod_temp = este_in(root, a);
                        if(nod_temp== nullptr) {
                            cout << "\n" << a << " nu exista in multime.\n";
                            output_file << "\n" << a << " nu exista in multime.\n";
                        }
                        else{
                            cout<<"\nElementul "<<nod_temp->key<<" exista in multime.\n";
                            output_file<<"\nElementul "<<nod_temp->key<<" exista in multime.\n";
                        }

                        break;
                    }
                    case 7:{
                        cout<<"\n Cardinalul multimii este: ";
                        cout<<cardinal(root)<<"\n";
                        output_file<<"\n Cardinalul multimii este: ";
                        output_file<<cardinal(root)<<"\n";
                        break;
                    }
                    case 8:{
                        cout<<"Specificati indexul elementului cautat: ";
                        int a;
                        cin>>a;


                        AVL* nod_temp = k_element(root,a);
                        if(nod_temp== nullptr)
                        {
                            output_file<<"\n Nu exista element pe pozitia  "<<a<<".\n";
                            cout<<"\n Nu exista element pe pozitia  "<<a<<".\n";;
                        }
                        else {
                            cout << "\nElementul de pe pozitia  " << a << " are valoarea " << nod_temp->key << ".\n";
                            output_file << "\nElementul de pe pozitia  " << a << " are valoarea " << nod_temp->key << ".\n";
                        }

                        break;
                    }
                    case 9:{
                        cout<<"Mentionati succesorul carui element il cautati: ";
                        int a;
                        cin>>a;

                        AVL* nod_temp =succesor(root,a);
                        if(nod_temp== nullptr)
                        {
                            output_file<<"\n Succesorul lui "<<a<<" nu exista.\n";
                            cout<<"\n Succesorul lui "<<a<<" nu exista.\n";
                        }
                        else
                        {
                            output_file<<"\nSuccesorul lui  "<<a<<" este "<<nod_temp->key<<".\n";
                            cout<<"\nSuccesorul lui  "<<a<<" este "<<nod_temp->key<<".\n";
                        }

                        break;
                    }
                    case 10:{
                        cout<<"Mentionati predecesorul carui element il cautati: ";
                        int a;
                        cin>>a;

                        AVL* nod_temp =predecesor(root,a);
                        if(nod_temp== nullptr)
                            {
                            output_file<<"\n Predecesorul lui "<<a<<" nu exista.\n";
                            cout<<"\n Predecesorul lui "<<a<<" nu exista.\n";
                            }
                        else
                        {
                            output_file<<"\nPredecesorul lui  "<<a<<" este "<<nod_temp->key<<".\n";
                            cout<<"\nPredecesorul lui  "<<a<<" este "<<nod_temp->key<<".\n";
                        }
                        break;
                    }
                    default: {cout<<"\nAi gresit optiunea.\n";break;}
                }

            }

        }
    }
    return 0;
}
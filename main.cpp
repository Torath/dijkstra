#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <vector>
#include <chrono>


using namespace std::chrono;
using namespace std;

int** tworzGrafA(int w, int krawedzie);
int** tworzGrafB(int w, int p);
void trybA();
void trybB();
void wypiszNajkrotsza(int** graf, int w);
void wypiszDijkstra(int** wynikGrafu,int length);
void wypiszTab(int** tab,int h,int w);
int** dijkstra(int** graf,int w);


int main()
{

  cout << "Wybierz tryb wykonania programu:"<<endl;
  cout << "1. Tryb A:"<<endl;
  cout << "2. Tryb B:"<<endl;
  int tryb;
  cin >> tryb;
  if(tryb==1)trybA();
  if(tryb==2)trybB();

}

void trybA(){
  //wczytaj liczbe wierzcholkow grafu (od 1 do N)
  cout <<endl<< "Podaj liczbe wierzcholkow grafu:"<<endl;
  int wierzcholki;
  cin >> wierzcholki;

  //wczytaj liczbe krawedzi (minimum połowa maksymalnej ilości krawędzi w grafie)
  int min=(wierzcholki*(wierzcholki-1) )/4;
  cout <<endl<< "Podaj liczbe krawedzi (minimum "<< min <<"):"<<endl;
  int krawedzie;
  do{
    cin >> krawedzie;
  }while(krawedzie < min || krawedzie > min*2 );

  cout<<endl<<"Losowanie grafu spojnego. Liczba wierzchokow: "<<wierzcholki<<", krawedzi "<<krawedzie<<endl;
  int** graf;

    graf=tworzGrafA(wierzcholki,krawedzie);

    int** wynik;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    wynik=dijkstra(graf,wierzcholki);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

    std::cout <<endl<<endl<< "Czas wykonania algorytmu " << time_span.count() << " sekund.";
    wypiszDijkstra(wynik,wierzcholki);

    cout<<endl<<endl<<"Wyswietlic najkrotsza sciezke ze zrodla do danego wierzcholka? (podaj numer wierzcholka lub 0 aby wyjsc)"<<endl;
    int wybor=0;
    cin>>wybor;
    if(wybor==0)system(EXIT_SUCCESS);
    wypiszNajkrotsza(wynik,wybor);
}

int** tworzGrafA(int w, int krawedzie){
    //inicjalizacja zmiennych
    int** macierzSasiedztwa=0;
    macierzSasiedztwa=new int*[w];
    bool* odwiedzoneWierzcholki= new bool[w];

    //wypelnienie tablic zerami/false
    for(int i=0;i<w;i++){
        odwiedzoneWierzcholki[i]=false;
        macierzSasiedztwa[i]=new int[w];

        for(int j=0;j<w;j++){
            macierzSasiedztwa[i][j]=0;
        }
    }

    //losowanie par wierzcholkow dla krawedzi
    srand ( time(NULL) );
    for(int i=0;i<krawedzie;i++){

        //jeśli istnieją wierzcholki bez krawedzi - wybierz ten wierzcholek
        int randIndex=-1;
        for(int i=0;i<w;i++){
            if(odwiedzoneWierzcholki[i]==false){
                randIndex=i;
            }
        }

        //jeśli nie ma nieodwiedzonego wierzchołka - losuj dowolny
        if(randIndex == -1){
           randIndex = rand() % w;
        }
        odwiedzoneWierzcholki[randIndex]=true;

        //losuj drugą parę różną od pierwszego wybranego wierzchołka
        int randIndey;
        do {
        randIndey = rand() % w;
        }while(randIndey==randIndex);
        odwiedzoneWierzcholki[randIndey]=true;

        //losuj wagi dla wybranej krawedzi
        if(macierzSasiedztwa[randIndex][randIndey]==0){
            int weight= rand()% (( 100 + 1 ) - 1);
            macierzSasiedztwa[randIndex][randIndey]=weight;
            macierzSasiedztwa[randIndey][randIndex]=weight;
        }else{
            i--;
        }

    }

    wypiszTab(macierzSasiedztwa,w,w);
    return macierzSasiedztwa;
}

void trybB(){

    cout <<endl<< "Podaj zakres wierzcholkow od:"<<endl;
    int zakresOd,zakresDo,procentWypelnienia,liczbaWykonan;
    cin >> zakresOd;
    cout << "Podaj zakres wierzcholkow do:"<<endl;
    cin >> zakresDo;
    cout << "Podaj procent wypelnienia grafow(minimum 50):"<<endl;
    do{
    cin >> procentWypelnienia;
    }while(procentWypelnienia < 50 || procentWypelnienia > 100 );

    cout << "Podaj liczbe wykonan algorytmu dla kazdej liczby wierzcholkow:"<<endl;
    cin >> liczbaWykonan;
    for(int i=zakresOd;i<=zakresDo;i++){
        cout<<"Sredni czas dla "<<i<<" wierzcholkow: ";
        double* czasyWykonania=new double[liczbaWykonan];
        double srednia=0;
        for(int j=0;j<liczbaWykonan;j++){
            int** graf=tworzGrafB(i,procentWypelnienia);

            auto start = std::chrono::system_clock::now();
            graf=dijkstra(graf,i);
            auto stop = std::chrono::system_clock::now();

            srednia+=std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        }
        cout<<srednia/liczbaWykonan<<" mikrosekund"<<endl;

    }
}

int** tworzGrafB(int w, int p){
    int krawedzie=((w*(w-1))/2)*p/100;
    //inicjalizacja zmiennych
    int** macierzSasiedztwa=0;
    macierzSasiedztwa=new int*[w];
    bool* odwiedzoneWierzcholki= new bool[w];

    //wypelnienie tablic zerami/false
    for(int i=0;i<w;i++){
        odwiedzoneWierzcholki[i]=false;
        macierzSasiedztwa[i]=new int[w];

        for(int j=0;j<w;j++){
            macierzSasiedztwa[i][j]=0;
        }
    }

    //losowanie par wierzcholkow dla krawedzi
    srand ( time(NULL) );
    for(int i=0;i<krawedzie;i++){

        //jeśli istnieją wierzcholki bez krawedzi - wybierz ten wierzcholek
        int randIndex=-1;
        for(int i=0;i<w;i++){
            if(odwiedzoneWierzcholki[i]==false){
                randIndex=i;
            }
        }

        //jeśli nie ma nieodwiedzonego wierzchołka - losuj dowolny
        if(randIndex == -1){
           randIndex = rand() % w;
        }
        odwiedzoneWierzcholki[randIndex]=true;

        //losuj drugą parę różną od pierwszego wybranego wierzchołka
        int randIndey;
        do {
        randIndey = rand() % w;
        }while(randIndey==randIndex);
        odwiedzoneWierzcholki[randIndey]=true;

        //losuj wagi dla wybranej krawedzi
        if(macierzSasiedztwa[randIndex][randIndey]==0){
            int weight= rand()% (( 100 + 1 ) - 1);
            macierzSasiedztwa[randIndex][randIndey]=weight;
            macierzSasiedztwa[randIndey][randIndex]=weight;
        }else{
            i--;
        }

    }
    return macierzSasiedztwa;
}

void wypiszTab(int** tab,int height,int width){

    std::cout << "\n\nMacierz sasiedztwa grafu:"<<endl;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            std::cout <<std::setw(2)<< tab[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

int** dijkstra(int** graf,int w){
    int** result=new int*[2];
    result[0]=new int[w];
    result[1]=new int[w];
    //result[0]=d[u] result[1]=p[u]

    //inicjalizacja zmiennych
    vector<int> Q,S;
    //inicjalizacja zmiennych

    for(int i=0;i<w;i++){
        result[0][i]=INT_MAX;
        result[1][i]=-1;
        Q.push_back(i);
    }
    //ustaw źródło na 0
    result[0][0]=0;
    //zacznij od źródła
    while(!Q.empty()){

        //w Q znajdź w z najmniejsza wartoscia w d[u]
        int min_index=0;
        for(int i=0;i<Q.size();i++){
            if(result[0][Q[i]]<result[0][Q[min_index]]){
                min_index=i;
            }
        }

        //zdejm ten wierzcholek i wstaw do S
        S.push_back(Q[min_index]);
        Q.erase(Q.begin() + min_index);

        //przejrzyj nieodwiedzonych sasiadow wierzcholka
        for(int i=0;i<w;i++){

            //czy indeks jest nieodwiedzonym sasiadem
            bool czyOdwiedzony=true;
            for(int j=0;j<Q.size();j++){
                if(Q[j]==i){
                    czyOdwiedzony=false;
                }
            }
            if(graf[S.back()][i]!=0 && !czyOdwiedzony){
                //sprawdz wzor d(sasiad) > d(aktualny wierzcholek) + waga krawędzi a–s
                //jesli tak to ustawiamy wage i poprzednika
                if(result[0][i]>(result[0][S.back()]+graf[S.back()][i] )){
                    result[0][i]=result[0][S.back()]+graf[S.back()][i];
                    result[1][i]=S.back();
                }
                }
        }
    }
    return result;
}

void wypiszDijkstra(int** wynikGrafu, int length){

    cout<<endl<<endl;

    std::cout <<std::setw(4)<<" ";
    for(int i=0;i<length;i++){
        std::cout <<std::setw(2)<< "  w"<<(i+1);
    }
    cout<<endl;

    std::cout <<std::setw(4)<<"k";
    for(int i=0;i<length;i++){
        std::cout <<std::setw(4)<< wynikGrafu[0][i];
    }
    cout<<endl;
    std::cout <<std::setw(4)<<"p";

    for(int i=0;i<length;i++){
        std::cout <<std::setw(4)<< wynikGrafu[1][i]+1;
    }
    cout<<endl<<"k-koszt sciezki od zrodla"<<endl<<"p-poprzedni wierzcholek na sciezce"<<endl;

}

void wypiszNajkrotsza(int** graf, int w){
    w=w-1;
    cout<<endl<<"Sciezka z w1 do w"<<w+1<<" koszt: "<<graf[0][w]<<endl;


    while(graf[1][w]!=(-1)){
        cout<<"w"<<w+1<<"<-";
        w=graf[1][w];
    }
    cout<<"w1"<<endl;
}

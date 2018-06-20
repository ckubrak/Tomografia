#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;


template<class In>
class Kfold {
public:
    Kfold(int k, In _vecto);
    template<class Out>
    void getFold(int foldNo, Out training, Out testing);

private:
    In vecto;
    int folds;
    vector<int> whichFoldToGo;

};

template<class In>
Kfold<In>::Kfold(int _folds, In _vecto) :
        vecto(_vecto), folds(_folds) {
    if (folds <= 0)
    {
        cout<<"No se pueden crear "<< folds<<" folds";
        throw;
    }


    int foldNo = 0;
    int cont = 0;
    //random_shuffle ( vecto.begin(), vecto.end() );
    for (auto i = vecto.begin(); i != vecto.end(); i++) {
        //if (end-i>=0)cout << "CICLO i: " << end-i << endl;

        whichFoldToGo.push_back(++foldNo);

        if (foldNo == folds)
            foldNo = 0;

        cont++;
    }


    if (foldNo)
    {
         cout<<"Con este numero de folds (folds="<< folds <<") no se puede dividir en parter iguales"<<endl;
         throw;
    }


    random_shuffle(whichFoldToGo.begin(), whichFoldToGo.end());
}

template<class In>
template<class Out>
void Kfold<In>::getFold(int foldNo, Out training, Out testing) {

    int k = 0;
    auto i = vecto.begin();
    while (i != vecto.end()) {
        if (whichFoldToGo[k++] == foldNo) {
            *testing++ = *i++;
        } else
            *training++ = *i++;
    }
}


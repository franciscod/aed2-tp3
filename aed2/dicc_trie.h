#ifndef dicc_trie_algo2
#define dicc_trie_algo2
#include<assert.h>
#include<vector>
#include<map>
#include<algorithm>

using namespace std;

template<class T>
class DiccString{
private:
    map<char,DiccString<T>*> siguiente;
    DiccString* padre;
    string palabra;
    char letra;
    bool es_final;
    T* significado;
    int cant;

    void _definir(const string &clave, T &_significado, int index)
    {
        cant++;
        if(index == clave.size())
        {
            es_final = true;
            significado = new T(_significado);
        }
        else
        {
            if(siguiente.find(clave[index])==siguiente.end())
            {
                siguiente[clave[index]] = new DiccString<T>;
                siguiente[clave[index]]->padre = this;
                siguiente[clave[index]]->letra = clave[index];
                siguiente[clave[index]]->palabra = palabra+clave[index];
            }
            siguiente[clave[index]]->_definir(clave,_significado,index+1);
        }
    }

    bool _definido(const string &clave, int index)
    {
        if(index == clave.size())
            return es_final;
        if(siguiente.find(clave[index])==siguiente.end())
            return false;
        return siguiente[clave[index]]->_definido(clave,index+1);
    }

    T* _obtener(const string &clave, int index)
    {
        if(index == clave.size())
            return significado;
        return siguiente[clave[index]]->_obtener(clave,index+1);
    }

    void _borrar(const string &clave, int index)
    {
        cant--;
        if(index == clave.size())
        {
            es_final = false;
            significado = NULL;
        }
        else
        {
            if(siguiente[clave[index]]->cant == 1)
            {
                delete siguiente[clave[index]];
                siguiente.erase(clave[index]);
            }
        }
    }

public:
    DiccString()
    {
        siguiente.clear();
        padre = NULL;
        es_final = false;
        significado = NULL;
        letra = '\0'; ///El equivalente a NULL porque la raiz no representa ninguna letra
        palabra = "";
    }

    ~DiccString()
    {
        for(typename map<char,DiccString<T>*>::iterator it = siguiente.begin(); it != siguiente.end(); it++)
            delete (*it).second;
        delete significado;
    }

    void definir(const string &clave, T &_significado)
    {
        _definir(clave,_significado,0);
    }

    bool definido(const string &clave)
    {
        return _definido(clave,0);
    }

    T* obtener(const string &clave)
    {
        assert(definido(clave));
        return _obtener(clave,0);
    }

    void borrar(const string &clave)
    {
        assert(definido(clave));
        _borrar(clave,0);
    }

    vector<T*> claves()
    {
        vector<T*> resultado;
        if(es_final)
            resultado.push_back(significado);
        for(typename map<int,DiccString<T>*>::iterator it = siguiente.begin(); it != siguiente.end(); it++)
        {
            vector<T*> aux = *it.claves();
            for(int i=0;i<aux.size();i++)
                resultado.push_back(aux[i]);
        }
        return resultado;
    }

    class Iterador{
    private:
        DiccString<T>* dicc;

        void irAlFondo()
        {
            while(!dicc->siguiente.empty())
                dicc = (*(dicc->siguiente.begin())).second;
        }

        void irAlFondoUltimo()
        {
            while(!dicc->siguiente.empty())
                dicc = (*(dicc->siguiente.rbegin())).second;
        }

        int irAPapa(typename map<char,DiccString<T>*>::iterator &it)
        {
            char c = dicc->letra;
            if(dicc->padre == NULL)
                return 0;
            dicc = dicc->padre;
            typename map<char,DiccString<T>*>::iterator it2 = dicc->siguiente.find(c);
            it2++;
            if(it2 == dicc->siguiente.end())
                return 1;
            it = it2;
            return 2;
        }

        int irAPapaParaAtras(typename map<char,DiccString<T>*>::iterator &it)
        {
            char c = dicc->letra;
            if(dicc->padre == NULL)
                return 0;
            dicc = dicc->padre;
            if(dicc->es_final)
                return 2;
            typename map<char,DiccString<T>*>::iterator it2 = dicc->siguiente.find(c);
            if(it2 == dicc->siguiente.begin())
                return 1;
            it = it2;
            it--;
            return 3;
        }

    public:
        Iterador(DiccString<T>* _dicc)
        {
            dicc = _dicc;
            while(!dicc->siguiente.empty())
                dicc = (*(dicc->siguiente.begin())).second;
        }

        Iterador(DiccString<T>* _dicc, const string &clave)
        {
            assert(_dicc->definido(clave));
            dicc = _dicc;
            int index = 0;
            while(index != clave.size())
            {
                dicc = dicc->siguiente[clave[index]];
                index++;
            }
        }

        ~Iterador()
        {
            return;
        }

        string claveActual()
        {
            return dicc->palabra;
        }

        T* valorActual()
        {
            return dicc->significado;
        }

        bool avanzar()
        {
            if(!(dicc->siguiente).empty())
                irAlFondo();
            else
            {
                typename map<char,DiccString<T>*>::iterator it;
                int t = 1;
                while(t == 1)
                    t = irAPapa(it);
                if(t == 0)
                {
                    irAlFondo();
                    return false;
                }
                dicc = (*it).second;
                irAlFondo();
            }
            return true;
        }

        bool retroceder()
        {
            typename map<char,DiccString<T>*>::iterator it;
            int t = 1;
            while(t == 1)
                  t = irAPapaParaAtras(it);
            if(t == 0)
            {
                irAlFondoUltimo();
                return false;
            }
            else if(t == 3)
            {
                dicc = (*it).second;
                irAlFondoUltimo();
            }
            return true;
        }
    };
};


#endif

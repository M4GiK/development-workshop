#ifndef INC_no_leda_hh
#define INC_no_leda_hh

#include <sys/types.h>
#include <sys/time.h>
using namespace std;
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>

#include "utils.hh"

/*************************************************************************/
/*   Las siguientes funciones existen para no tener que usar LEDA.       */
/*   Cuando se trabaje bajo el cluster, este fichero no deber� utilizarse*/
/*   y se tendr� que modificar el c�digo fuente para cambiar:            */
/*   binary_locate, rand>>, rand(,) , sort, used_time                    */
/*************************************************************************/

    float used_time ();
    float used_time (float& time);
    double rand_();
    int rand_ (int a, int b);
    bool rand_ (double p);
    double abs (const double d);
    string _repeat_ (string str, int times);
    
    /* Al ser m�todos plantillas, y esto no es una clase (C++ permite    */
    /* mezclar :S), deben ir en el fichero de cabezeras (*.hh :S)        */
    template <typename T>
    int binary_locate_ (vector<T>& v, T& el, int i, int j) {    
        int m = (i + j) / 2;
        if (j < i)          return j;
        if (v[m] == el)     return m;
        else if (v[m] < el) return binary_locate_ (v, el, m+1, j);
        else                return binary_locate_ (v, el, i, m-1);
    }
    
    template <typename T>
    int binary_locate (vector<T>& v, T& el) {
        return binary_locate_ (v, el, 0, v.size() - 1);
    }
  
    template <typename T>
    class matrix {
        public:
            matrix ():r(0), c(0) {  }
            matrix ( int rows, int cols,T init=T() ) :r(rows),c(cols),
                           t(vector<vector<T> >(r, vector<T>(c,init))) {  }
            int rows () const                         { return r;    }
            int cols () const                         { return c;    }
            vector<T>& operator[] (int i)             { return t[i]; }
            const vector<T>& operator[] (int i) const { return t[i]; }
            int size() const                          { return t.size(); }
            string to_string( string delimiter = " ") const{
            	ostringstream out;
            	out << "[ ";
				for ( unsigned int i = 0; i < t.size(); i++ ){
					out << mb_print_vec( t[i] ) << delimiter;
				}
				out << "]";
				return out.str();
            }
            void erase( int index ){ t.erase( t.begin() + index ); r--; }
        protected:
            int r,c;
            vector<vector<T> > t;
    };
    
    template <typename elem>
    int partition (const vector<elem>& T, vector<int>& I, int e, int d) {
        elem x = T[I[e]]; 
        int i = e-1;
        int j = d+1;
        for (;;) {
            do {--j;} while (x < T[I[j]]);
            do {++i;} while (T[I[i]] < x);
            if (i>=j) {
            	return j;
           	}
            swap(I[i],I[j]);
        }
    }
    
    template <typename elem>
    void quick_sort (const vector<elem>& T, vector<int>& I,int e, int d) {
        if (e<d) {
            int q = partition(T,I,e,d);
            quick_sort (T,I,e,q);
            quick_sort (T,I,q+1,d);
        }
    }

    /*
     * Removing these guys, gcc get confused when we override pre-defined
     * macros, bad things will happen.
     */
    // #define max(a,b) ((a)>(b))?(a):(b)
    // #define min(a,b) ((a)<(b))?(a):(b)
    
#endif

#ifndef INC_iopacket2_nl
#define INC_iopacket2_nl
#include <openmpi/ompi/mpi/cxx/mpicxx.h>
#include <string>
#include <vector>
//using namespace std;

/*******************************************************/
/* Implementaci� de les classes iopacket sense usar la */ 
/* llibreria de LEDA per no entrar en conflicte amb la */  
/* llibreria standar. La interficie es exactament la   */
/* mateixa sense les estructures de LEDA, per tal que  */
/* el codi font sigui facilment transformable en cas   */
/* que es vulgui usar LEDA							   */
/*******************************************************/

class ipacket2 {
	public:

	ipacket2 (int src);
	~ipacket2 ();

	ipacket2& operator>> (bool& x);
	ipacket2& operator>> (char& x);
	ipacket2& operator>> (short& x);
	ipacket2& operator>> (int& x);
	ipacket2& operator>> (long& x);
	ipacket2& operator>> (float& x);
	ipacket2& operator>> (double& x);
	ipacket2& operator>> (char* x);
	ipacket2& operator>> (std::string& x);
	template <class T> ipacket2& operator>> (std::vector<T>& x);
/*	template <class T> ipacket2& operator>> (slist<T>& x);
	template <class T> ipacket2& operator>> (list<T>& x);
	template <class T> ipacket2& operator>> (set<T>& x);*/

	private:

    	int sze;
		int pos;
    	char* buf;
		void unpack (void* x, const MPI_Datatype datatype, const int outcount=1);
};

class opacket2 {
	public:

	opacket2 (int initsze=1024*1024);
	~opacket2 ();

	opacket2& operator<< (bool x);
	opacket2& operator<< (char x);
	opacket2& operator<< (short x);
	opacket2& operator<< (int x);
	opacket2& operator<< (long x);
	opacket2& operator<< (float x);
	opacket2& operator<< (double x);
	opacket2& operator<< (const char* const x);
	opacket2& operator<< (const std::string& x);
	template <class T> opacket2& operator<< (const std::vector<T>& x);
	/*template <class T> opacket2& operator<< (const slist<T>& x);
	template <class T> opacket2& operator<< (const list<T>& x);
	template <class T> opacket2& operator<< (const set<T>& x);	*/

	void send (int dst);

	private:		
  		int pos;
    	int sze;	
    	char* buf;
		void pack (const void* x, const MPI_Datatype datatype, const int incount=1);

};

template <class T> 
inline ipacket2& ipacket2::operator>> (std::vector<T>& x) {
 
  ipacket2& ip = *this;
  int size;
  ip >> size;
  x = std::vector<T>(size);
  for (int i=0; i<size; i++) {
    ip >> x[i];
  }	
  return *this;
}

template <class T> 
inline opacket2& opacket2::operator<< (const std::vector<T>& x) {

  opacket2& op=*this;
  op << (int) x.size();
  for (unsigned int i=0; i<x.size(); i++) {
    op << x[i];
  }	
  return *this;
}

#endif

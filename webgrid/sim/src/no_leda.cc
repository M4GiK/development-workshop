#include "no_leda.hh"
/*************************************************************************/
/*   Las siguientes funciones existen para no tener que usar LEDA.       */
/*   Cuando este fichero se compile bajo el cluster ba,deberá cambiarse  */
/*************************************************************************/
 
	clock_t _time_;
	int _first_ = true;

float used_time () {
    if (_first_) {
        _time_ = clock();
        _first_ = false;
        return 0;
    }    
    clock_t t;
    t = clock();
    return ((float)(t - _time_))/CLOCKS_PER_SEC;
}

float used_time (float& t) {
    float backup = t;
    t = used_time();
    return used_time() - backup;   
}

double rand_() {
    return double(rand())/RAND_MAX;
}

int rand_ (int a, int b) {
    return rand()%(b-a+1)+a;
}

bool rand_ (double p) {
	return (rand_() <= p);	
}

double abs (const double d) {
	if (d < 0) return -d;
	else       return  d;
}

string _repeat_ (string str, int times) {
    string res = "";
    for (int i = 0; i < times; ++i) {
        res += str;
    }
    return res;
}

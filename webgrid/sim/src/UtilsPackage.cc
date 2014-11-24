#include "UtilsPackage.hh"

vector<string> splitString(string s,string d) {
    vector<string> vs;
    int lastIdx,idx;

    vs.clear();
    if (s.find(d) < 0) {
        vs.push_back(s);
        return vs;
    }
    lastIdx = 0;
    do {
        idx = s.find(d, lastIdx);
        if (idx < lastIdx) {
            vs.push_back(s.substr(lastIdx));
            break;
        }
        vs.push_back(s.substr(lastIdx, idx-lastIdx));
        lastIdx = idx+1;
    } while (lastIdx <= int(s.length()));

    return vs;
}

string repeat(string str, int times) {
    string res = "";
    for (int i = 0; i < times; ++i) {
        res += str;
    }
    return res;
}


double parseDouble (char const *s) {
    char *err;
    double retval;

    retval = strtod(s, &err);
    if (*err != '\0') {
        error("UtilsPackage: string is not a double");
    }
    return retval;
}

long int parseInteger(char const *s) {
    char *err;
    long int retval;

    retval = strtol(s,&err,10);
    if (*err != '\0') {
        error("UtilsPackage: string is not an integer");
    }
    return retval;
}

string doubleToString(double d) {
    /* No existe una función específica del ANSI-C */
    char s[20]; string ret;
    sprintf(s, "%lf", d);
    ret = s;
    return ret;
}

string integerToString(long int i) {
    /* No existe una función específica del ANSI-C */
    char s[20]; string ret;

    sprintf(s, "%ld", i);
    ret = s;
    return ret;
}


void error (string msg) {
    cout << msg << endl;
    exit (-1);
}

vector<double> wrap (int number, ...) {
    vector<double> ret (number, 0);

    va_list l;
    va_start(l, number);

    for (int i = 0; i < number; i++) {
        ret[i] = va_arg(l, double);
    }
    va_end(l);
    return ret;
}

double _one_ () {
    return 1.0;
}

double _zero_ () {
    return 0.0;
}
 
double infinity() {
	return _one_() / _zero_();
}

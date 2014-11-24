#include "RandomPackage.hh"

#define abs_(x) ((x) < 0)?(-x):(x)

static long int seed = 1;

void sranf(long int const s) {
    seed = s;
}

long int genSeed() {
    return seed++;
}

double ranf(long int *seed) {
    /* Generates numbers in the rank (0,1] */
    long int a=16807,p=2147483647,b15=32768,b16=65536,
    ix,xhi,xalo,leftlo,fhi,k;
    double x;

    ix = *seed;
    xhi = ix / b16;
    xalo = (ix - xhi * b16) * a;
    leftlo = xalo / b16;
    fhi = xhi * a + leftlo;
    k = fhi / b15;
    ix = (((xalo - leftlo * b16) - p) + (fhi - k * b15) * b16) + k;
    if (ix <= 0)
        ix += p;
    *seed = ix;
    x = ((double) ix) * 4.656612875e-10;
    return (x);
}


double ranf() {
    return ranf(&seed);
}

int randint (int a, int b) {
    return ((int)(ranf()* RAND_MAX)) % (b-a+1)+a;
}

/* Exponential variate generation function with mean */
double expon(double const mean) {
    return -mean * log(ranf(&seed));
}

/* Uniform variate generation function with mean.'a' is lower bound, 'b' is
   upper bound  */
double uniform(double const a,double const b) {
    return a+(b-a)*ranf(&seed);
}

/* Gamma variate generation  */
double gam(double const a) {
    double u1,u2;
    double b,p,x;

reject:
    u1 = ranf(&seed);
    b = (2.718281828+a)/2.718281828;
    p = b*u1;
    u2 = ranf(&seed);
    if (p > 1.0) {
        x = -log((b-p)/a);
        if (log(u2) > (a-1.0)*log(x)) {
            goto reject;
        } else {
            return x;
        }
    } else {
        x = exp(log(p)/a);
        if (u2 > exp(-x)) {
            goto reject;
        } else {
            return x;
        }
    }
}

double erlang(int const a) {
    double x;
    int ia = a;

    x = ranf(&seed);
    for (;--ia;) {
        x = x*ranf(&seed);
    }
    x = -log(x);
    return x;
}

/* Normal variate generation. 'm' is mean, 's' is standard deviation */
double normal(double const m,double const s) {
    double u1,u2;
    double x;

    u1 = ranf(&seed);
    u2 = ranf(&seed);
    x = sqrt(-2.0*log(u1))*cos(2.0*3.14159265*u2);
    return m+x*s;
}

/* Triangle variate generation 0-1 */
double triangle(double const a)
{
    double x;

    if (a<0 || a>1) {
        return 0;
    }
    x = ranf(&seed);
    if (x<=a) {
        x = sqrt(a*x);
    } else {
        x = 1.0-sqrt((1.0-a)*(1.0-x));
    }
    return x;
}

/* Triangle variate generation a,b,c */
double triangle (double a, double b, double c) {
	double cdf_mid, d, x;
	double cdf = ranf(&seed);

	d = 2.0 / ( c - a );
	cdf_mid = 0.5 * d * ( b - a );
	if (cdf <= cdf_mid) {
    	x = a + sqrt ( cdf * ( b - a ) * ( c - a ) );
  	} else {
    	x = c - sqrt (( c - b) * (( c - b) - (cdf - cdf_mid) * (c - a)));
  	}
  	return x;
}

/* Beta variate generation */
double beta(double const a,double const b) {
    double u1,u2,v,w,con[4];

    con[1] = (a<b)?a:b;
    if (con[1]<=1) {
        con[1] = 1.0/con[1];
    } else {
        con[1]=sqrt((a+b-2.)/(2.*a*b-a-b));
    }
    con[2]=a+b;
    con[3]=a+1./con[1];
again:
    u1=ranf(&seed);
    u2=ranf(&seed);
    v=con[1]*log(u1/(1.-u1));
    w=a*exp(v);
    if(((con[2])*log((con[2])/(b+w))+ (con[3])*v-1.3862944) <
            (log(u1*u1*u2))) {
        goto again;
    } else {
        return(w/(b+w));
    }
}

double qnorm(double p,bool upper) {
    // Reference:
    // J. D. Beasley and S. G. Springer
    // Algorithm AS 111: "The Percentage Points of the Normal Distribution"
    //   Applied Statistics

    if( p < 0 || p > 1) {
        error("RandomPackage.cc: Stuendent's T wrong parameters");
    }
    double split =  0.42,
           a0    =  2.50662823884,
           a1    = -18.61500062529,
           a2    =  41.39119773534,
           a3    = -25.44106049637,
           b1    =  -8.47351093090,
           b2    =  23.08336743743,
           b3    = -21.06224101826,
           b4    =   3.13082909833,
           c0    = - 2.78718931138,
           c1    = - 2.29796479134,
           c2    =   4.85014127135,
           c3    =   2.32121276858,
           d1    =   3.54388924762,
           d2    =  1.63706781897,
           q     =  p - 0.5;
    double r, ppnd;

    if (abs_(q) <= split) {
        r = q*q;
        ppnd = q*(((a3*r+a2)*r+a1)*r+a0)/((((b4*r+b3)*r+b2)*r+b1)*r+1);
    } else {
    r = p;
    if ( q > 0 ) r = 1-p;
    if ( r > 0 ) {
        r = sqrt(-log(r));
        ppnd = ((( c3 * r + c2) * r + c1) * r + c0)/((d2 * r+ d1)* r + 1);
        if ( q < 0 ) ppnd=-ppnd;
        } else {
            ppnd = 0;
        }
    }
    if ( upper ) ppnd = 1 - ppnd;
    return ( ppnd );
}


double student (double p, double ndf, bool lower_tail) {
    // Algorithm 396: Approximated Student's t-quantiles by
    // G.W. Hill CACM 13(10), 619-620, October 1970

    if (p <= 0 || p >= 1 || ndf < 1) {
        error ("RandomPackage.cc: Student's T wrong parameters");
    }
    double eps = 1e-12;
    double HALF_PI = 1.570796326794896619231321691640;
    bool neg;
    double P, q, prob, a, b, c, d, y, x;
    if ((lower_tail && p > 0.5) || (!lower_tail && p < 0.5)) {
        neg = false;
        P = 2 * (lower_tail ? (1 - p) : p);
    }
    else {
        neg = true;
        P = 2 * (lower_tail ? p : (1 - p));
    }

    if (abs_(ndf - 2) < eps) {   /* df ~= 2 */
        q = sqrt(2 / (P * (2 - P)) - 2);
    }
    else if (ndf < 1 + eps) {  /* df ~= 1 */
        prob = P * HALF_PI;
        q = cos(prob)/sin(prob);
    } else {                  /* usual case;  including, e.g.,  df = 1.1 */
        a = 1 / (ndf - 0.5);
        b = 48 / (a * a);
        c = ((20700 * a / b - 98) * a - 16) * a + 96.36;
        d = ((94.5 / (b + c) - 3) / b + 1) * sqrt(a * HALF_PI) * ndf;
        y = pow(d * P, 2 / ndf);
        if (y > 0.05 + a) {
            x = qnorm(0.5 * P,false);
            y = x * x;
            if (ndf < 5)
                c += 0.3 * (ndf - 4.5) * (x + 0.6);
            c = (((0.05 * d * x - 5) * x - 7) * x - 2) * x + b + c;
            y = (((((0.4 * y + 6.3) * y + 36) * y + 94.5) /c-y-3)/b+1)* x;
            y = a * y * y;
            if (y > 0.002) /* FIXME: This is machine-precision dependent*/
                y = exp(y) - 1;
            else { /* Taylor of    e^y -1 : */
                y = (0.5 * y + 1) * y;
            }
        } else {
            y = ((1 / (((ndf + 6) / (ndf * y) - 0.089 * d - 0.822)
                * (ndf + 2) * 3) + 0.5 / (ndf + 4))
                * y - 1) * (ndf + 1) / (ndf + 2) + 1 / y;
        }
        q = sqrt(ndf * y);
    }
    if(neg) q = -q;
    return q;
}

/*
 * Energy.cc
 *
 *  Created on: 2011-09-09
 *      Author: Grzesiek and Joanna
 */

#include "Energy.hh"
#include <math.h>
#include <cmath>

Energy::Energy(int _nbr_hosts, int _nbr_tasks):
	nbr_hosts(_nbr_hosts),
	nbr_tasks(_nbr_tasks),
	hostClass(_nbr_hosts),
	MaxEnergy(0),
	EnergyI(0),
	EnergyII(0),
	Eparam(0)
{}

void Energy::assignClasses(vector<double> mips) {
	// get max mips
	double max_mips = 0.0;
	for (int m = 0; m < nbr_hosts; ++m){
			if( mips[m] > max_mips )
				max_mips = mips[m];
	}
	// compute class borders
	double classI = ((2.0*max_mips)/3.0) + 1.0;
	double classII = (max_mips/3.0) + 1.0;
	// assign classes
	for (int m = 0; m < nbr_hosts; ++m){
			if( mips[m] >= classI )
				hostClass[m] = 1;
			else if( mips[m] >= classII )
				hostClass[m] = 2;
			else
				hostClass[m] = 3;
	}
}

double Energy::getMaxEnergy(double makespan, vector<double> completion) {
	double sum = 0.0;
	for (int m = 0; m < nbr_hosts; ++m){
		int hc = hostClass[m];
		double v = vf[hc][0][0];
		sum += makespan * 1 * pow(v,2);
	}
	return sum/nbr_hosts;
}

double Energy::getEnergyCaseI(double makespan, vector<double> completion) {
	// idle
	vector<double> idle(nbr_hosts);
	for (int m = 0; m < nbr_hosts; ++m){
			idle[m] = makespan - completion[m];
	}

	// energy case I
	double sumMax = 0.0;
	double sumMin = 0.0;
	for (int m = 0; m < nbr_hosts; ++m){
		int hc = hostClass[m];
		double vmax = vf[hc][0][0];
		double fmax = vf[hc][1][0];
		double vmin = vf[hc][0][vfsize[hc]-1];
		double fmin = vf[hc][1][vfsize[hc]-1];
		sumMax += completion[m] * 1 * fmax * pow(vmax,2);
		sumMin += idle[m] * 1 * fmin * pow(vmin,2);
	}
	return (sumMax+sumMin)/nbr_hosts;
}

double Energy::getEnergyCaseII(double makespan, vector<double> completion, vector<double> ready) {

	double sumOpt = 0.0;
	double sumMin = 0.0;
	vector<double> idle(nbr_hosts);
	for (int m = 0; m < nbr_hosts; ++m){
		int hc = hostClass[m];
		double vopt = vf[hc][0][0];
		double fopt = vf[hc][1][0];
		for(int i=1; i<vfsize[hc]; ++i){
			double newCompletion = completion[m]/vf[hc][1][i];
			if ( newCompletion > makespan-ready[m] ) {
				vopt = vf[hc][0][i-1];
				fopt = vf[hc][1][i-1];
				break;
			}
		}
		double vmin = vf[hc][0][vfsize[hc]-1];
		double fmin = vf[hc][1][vfsize[hc]-1];
		completion[m] /= fopt;
		idle[m] = makespan - completion[m];
		sumOpt += completion[m] * 1 * fopt * pow(vopt,2);
		sumMin += idle[m] * 1* fmin * pow(vmin,2);
	}
	return (sumOpt+sumMin)/nbr_hosts;
}

void Energy::assignEnergy(vector<double> mips, double makespan, vector<double> completion, vector<double> ready) {
	assignClasses(mips);
	MaxEnergy = getMaxEnergy(makespan, completion);
	EnergyI = getEnergyCaseI(makespan, completion);
	EnergyII = getEnergyCaseII(makespan, completion, ready);
	Eparam = abs((EnergyI-EnergyII)/EnergyI)*100;
}


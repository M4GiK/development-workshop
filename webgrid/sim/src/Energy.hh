/*
 * Energy.hh
 *
 *  Created on: 2011-09-09
 *      Author: Grzesiek and Joanna
 */

#ifndef ENERGY_HH_
#define ENERGY_HH_

#include <vector>
#include "Scheduler.hh"

const int sizeI = 7;
const int sizeII = 5;
const int sizeIII = 4;
const int sizemax = sizeI;

const int vfsize[] = {sizeI,sizeII,sizeIII};

// volatge and frequency table
const double vf[3][2][sizemax] =
{
	{
		{ 1.5, 1.4, 1.3, 1.2, 1.1, 1.0, 0.9},
		{ 1.0, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4}
	},{
		{2.2, 1.9,  1.6,  1.3, 1.0, 0,0},
		{1.0, 0.85, 0.65, 0.5, 0.35, 0,0}
	},{
		{1.75, 1.4, 1.2, 0.9, 0,0,0},
		{ 1.0,  0.8, 0.6, 0.4, 0,0,0}
	}
};

class Energy {
	private:
		int nbr_hosts;
		int nbr_tasks;
		vector<int> hostClass;

	public:
		double MaxEnergy;
		double EnergyI;
		double EnergyII;
		double Eparam;

		Energy(int _nbr_hosts, int _nbr_tasks);
		void assignClasses(vector<double> mips);
		double getMaxEnergy(double makespan, vector<double> completion);
		double getEnergyCaseI(double makespan, vector<double> completion);
		double getEnergyCaseII(double makespan, vector<double> completion, vector<double> ready);
		void assignEnergy(vector<double> mips, double makespan, vector<double> completion, vector<double> ready);

};


#endif /* ENERGY_HH_ */

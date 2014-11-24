/*
 * Security.hh
 *
 *  Created on: 2011-09-08
 *      Author: Grzesiek and Joanna
 */

#ifndef SECURITY_HH_
#define SECURITY_HH_

#include <vector>
#include "Scheduler.hh"
#include "no_leda.hh"

class Security {
	private:
		int nbr_hosts;
		int nbr_tasks;
		double* tl_arr;
		double* sd_arr;
	public:
		Security(int _nbr_hosts, int _nbr_tasks);
		void getTl(hostsSet hosts);
		void getSd(tasksSet tasks);
		void ApplySecurity(matrix<double>& etc, double alpha=3);
};

#endif /* SECURITY_HH_ */

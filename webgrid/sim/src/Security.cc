/*
 * Security.cc
 *
 *  Created on: 2011-09-08
 *      Author: Grzesiek and Joanna
 */

#include "Security.hh"

Security::Security(int _nbr_hosts, int _nbr_tasks):
	nbr_hosts(_nbr_hosts),
	nbr_tasks(_nbr_tasks)
{}

void Security::getTl(hostsSet hosts)
{
	tl_arr = new double[nbr_hosts];
	for ( int i = 0; i < nbr_hosts; i++){
				tl_arr[i] = hosts[i]->getTl();
	}
}

void Security::getSd(tasksSet tasks)
{
	sd_arr = new double[nbr_tasks];
	for ( int i = 0; i < nbr_tasks; i++){
			sd_arr[i] = tasks[i]->getSd();
	}
}

void Security::ApplySecurity(matrix<double>& etc, double alpha)
{
	matrix<double> tfp(nbr_tasks, nbr_hosts);
	for ( int m = 0; m < nbr_hosts; m++ ){
			for ( int t = 0; t < nbr_tasks; t++ ){
				double sd = sd_arr[t];
				double tl = tl_arr[m];
				double p;
				if (sd <= tl){
					p=0;
				}
				else{
					p = 1-exp(-alpha*(sd - tl));
				}

				tfp[t][m] = p;

			}
		}

		for (int t = 0; t < nbr_tasks; ++t) {
			for (int m = 0; m < nbr_hosts; ++m) {
				etc[t][m] *= (1+tfp[t][m]);
		}   }
}


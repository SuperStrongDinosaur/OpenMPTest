//
//  wrong.cpp
//  OpenMPtest
//
//  Created by Alex Shirvinsky on 23/09/2019.
//  Copyright © 2019 Alex Shirvinsky. All rights reserved.
//

#include <omp.h>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<long> v(10000);
    for (size_t i = 0; i < v.size(); i++)
            v[i] = rand();

    double wall_timer = omp_get_wtime();
    
    #pragma omp parallel for
    {
	    for (size_t i = 0; i < v.size(); i++)
	    {   
	    	for (size_t j = 0; j < v.size() - i - 1; j++)  
	    		{
	       			if (v[j] > v[j + 1])  
	       			{
	            		std::swap(v[j], v[j + 1]);
	       			}
	        	}
	    }
    }

    wall_timer = omp_get_wtime() - wall_timer;
	std::cout << "time: " <<  wall_timer << std::endl;

    for (size_t i = 0; i < v.size() - 1; i++)
    	if (v[i] > v[i + 1]) {
    		std::cout << "Wrong!" << std::endl;
    		break;
    	}
    return 0;
}

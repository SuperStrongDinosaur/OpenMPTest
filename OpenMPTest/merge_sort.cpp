//
//  merge_sort.cpp
//  OpenMPtest
//
//  Created by Alex Shirvinsky on 18/09/2019.
//  Copyright © 2019 Alex Shirvinsky. All rights reserved.
//
#include <omp.h>
#include <iostream>
#include <vector>
#include <algorithm>

std::vector<long> merge(const std::vector<long>& l, const std::vector<long>& r) {
    std::vector<long> res;
    size_t left_it = 0, right_it = 0;
    
    while (left_it < l.size() && right_it < r.size()) {
        if (l[left_it] < r[right_it]) {
            res.emplace_back(l[left_it]);
            left_it++;
        } else {
            res.emplace_back(r[right_it]);
            right_it++;
        }
    }
    
    while (left_it < l.size()) {
        res.emplace_back(l[left_it]);
        left_it++;
    }
    
    while (right_it < r.size()) {
        res.emplace_back(r[right_it]);
        right_it++;
    }
    
    return res;
}

std::vector<long> mergesort_serial(std::vector<long>& v) {
    if (v.size() == 1)
        return v;

    std::vector<long>::iterator m = v.begin() + (v.size() / 2);
        
    std::vector<long> left(v.begin(), m);
    std::vector<long> right(m, v.end());

    left = mergesort_serial(left);
    right = mergesort_serial(right);

    return merge(left, right);
}

std::vector<long> mergesort(std::vector<long>& v, int threads) {
    if (threads > 1) {
        std::vector<long>::iterator m = v.begin() + (v.size() / 2);
        
        std::vector<long> left(v.begin(), m);
        std::vector<long> right(m, v.end());

        #pragma omp parallel sections
        {
            #pragma omp section
            left = mergesort(left, std::max(1, threads / 2));

            #pragma omp section
            right = mergesort(right, std::max(1, threads - threads / 2));
        }

        return merge(left, right);
    } else {
        return mergesort_serial(v);
    }
}

int main() {
    std::vector<long> v(1000000);
    
    for (size_t num_threads = 1; num_threads <= 16; num_threads++) {
        for (size_t i = 0; i < v.size(); i++)
            v[i] = rand();
        
        double wall_timer = omp_get_wtime();
        v = mergesort(v, num_threads);
        wall_timer = omp_get_wtime() - wall_timer;
        
        std::cout << "threads: " << num_threads << "  time: " <<  wall_timer << std::endl;
    }
}

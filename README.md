# Exam-Grades-Bayes-Nets-
Bayesian network representation &amp; inference ( for determining the exam grades)

# Synopsis
First the data is given the Bayes Representation.
Markov blanket are made aware through this representaion.
Thus it reduced to a simple count to learn and normalise to predict the inferences.
Laplacian smoothing is applied to avoid the unreliability in data in extremely biased cases.

To know more read the A4.pdf in "/Docs" or [Mausam's Assingnment 4](http://www.cse.iitd.ac.in/~mausam/courses/csl333/spring2015/A4/A4.pdf).

## How to Run

To compile :
g++ -std=c++11 startup_code.cpp network.cpp Graph_Node.cpp

To run :
$ executable node_file observations_file
Here,example:
$ ./a.out alarm.bif records.dat

## Contributors

[V.Bhargav Reddy](https://githib.com/code-hippo)

## License

Copyright (C) 2014  V.Bhargav

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

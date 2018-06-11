#ifndef _MISC_H_
#define _MISC_H_

#include "imagen.h"
#include "pca.h"
#include "knn.h"
#include "kfold.h"


double precision(int truePos, int falsePos);
double recall(int truePos, int falseNeg);
double accuracy(int truePos, int trueNeg, int falsePos, int falseNeg);
double mediaArmonica(double presicion, double recall);
int main_kfold(int pca, int k, int alfa, int kknn);
#endif

/****************************************************************
 ****
 **** This code is part of the book
 **** Introduction to High Performance Scientific Programming
 **** by Victor Eijkhout eijkhout@tacc.utexas.edu
 **** copyright 2010-2019
 ****
 ****************************************************************/

Êdouble sum1, sum2, sum3, sum4;
Êdouble temp1, temp2, temp3, temp4;
Ê
Êsum1 = 0.; sum2 = 0.; sum3 = 0.; sum4 = 0.;
Ê
for (i = 0; I < n; I ++) {
temp1 = *(a + 0) * *(b + 0);
temp2 = *(a + 1) * *(b + 1);
temp3 = *(a + 2) * *(b + 2);
temp4 = *(a + 3) * *(b + 3);
Ê
sum1 += temp1;
sum2 += temp2;
sum3 += temp3;
sum4 += temp4;
Ê
a += 4;
b += 4;
}
Ê
Êreturn sum1 + sum2 + sum3 + sum4;
Ê
Above function is still not optimized in terms of ÒtempÓ registers.
Ê
sum1 = 0.; sum2 = 0.; sum3 = 0.; sum4 = 0.;
temp1 = 0.; temp2 = 0.; temp3 = 0.; temp4 = 0.;
Ê
for (i = 0; I < n; I ++) {
Ê
sum1 += temp1;
temp1 = *(a + 0) * *(b + 0);
Ê
sum2 += temp2;
temp2 = *(a + 1) * *(b + 1);
Ê
sum3 += temp3;
temp3 = *(a + 2) * *(b + 2);
Ê
sum4 += temp4;
Ê temp4 = *(a + 3) * *(b + 3);
Ê
a += 4;
b += 4;
}
Ê
sum1 += temp1;
sum2 += temp2;
sum3 += temp3;
sum4 += temp4;
Ê
Êreturn sum1 + sum2 + sum3 + sum4;

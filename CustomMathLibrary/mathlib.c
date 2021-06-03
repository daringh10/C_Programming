#include <stdio.h>

//For our term limit, we define the constant epsilon
#define EPSILON 1e-14

//Code provided from asignment document
//If x is less than 0, return -x, otherwise return positive x
static inline double Abs(double x) {
    return x < 0 ? -x : x;
}

//Most code from lecture slides
// Sin function that takes a double as an input
//** We are using double instead of float because of the manitssa round
// off error, and doubles are better for exact approximations and
// high precision **
double Sin(double x) {

    //Numerator is the starting term in the P(x) function, but we skip the "official"
    //first term which is 0
    double numerator = x;

    //Since the first value in the series is just x we set
    //The denominator to 1 for term 1, !1, which makes everything much easier
    double denominator = 1;

    //The current sum is the first term, x
    double sum = x;

    //Redundant but helps to understand that temp starts out as the starting sum
    double temp = sum;

    //This loop finds the next term in the series and adds its value to the current sum
    for (double i = 3; Abs(temp) > EPSILON; i += 2) {

        //Takes into acount the sign changes
        //Then multiplies the current numerator by our input squared
        //This is an efficient way to continue to square our numerator
        numerator = (numerator * -x * x);

        //A way to apply facotorial to every other number
        //Multiples the current i rank by the previous rank that was skipped
        //and then multiples that to the mulitplant of all the previous i's
        denominator = denominator * i * (i - 1);

        //Divide the numerator and denominator to get the next value in the series
        temp = numerator / denominator;

        //The new total of the sum is all the previous terms
        //Added to the current term
        sum = sum + temp;
    }

    return sum;
}
//Most code from lecture slides
// Cos function that takes a double as an input
double Cos(double x) {
    //This time we set the numerator to 1 becaue the first term in the series,
    // is 1
    double numerator = 1.0;

    //Initializing denominator
    double denominator = 1.0;

    //The total sum of all terms as of the current term in series
    double sum = 1.0;

    //The current term in series being calculated
    double temp = sum;

    //This loop finds the next term in the series and adds its value to the current total value
    //Unlike sine, we start when k, or i, is 2, starting the loop after the first calculated term
    for (double i = 2.0; Abs(temp) > EPSILON; i += 2.0) {

        //Same for sine, just alternating signs and calculating numerator
        //and incrimenting x
        numerator = numerator * x * -x;

        //Same log for sine
        denominator = denominator * i * (i - 1.0);
        temp = numerator / denominator;
        sum = sum + temp;
    }

    return sum;
}
//Tan function that simply dividies Sin by Cos
double Tan(double x) {

    double sum = (Sin(x) / Cos(x));
    return sum;
}

//Most code from lecture slides
//e^x funtion, much easier because e^(x) is much better to derive
double Exp(double x) {

    //Establishing the first term which is 1 or 0!
    double temp = 1.0;
    double sum = temp;

    //A very simple way to multiply the previous term and x/i and add that value
    //To the sum
    for (double i = 1.0; Abs(temp) > EPSILON; ++i) {
        temp = x / i * temp;
        sum += temp;
    }

    return sum;
}

//Most code from lecture slides
//Function using Newtons method to calculate log
//The next value of the series is the current value - (y/y')
//We can express finding ln(x) by writing it as the function
//e^y - x = 0 or e^x - y = 0
//We will take the second approach
double Log(double x) {
    //Sets the first term to 1 becasue e^0 is 1
    double y_n = 1.0;

    //Simply sets e^(previous term) to y_k so we dont
    //Have to continuosuly write Exp(y_n)
    double y_k = Exp(y_n);

    for (double i = 0; Abs(y_k - x) > EPSILON; ++i) {
        //This is the simplified version of new guess = (old guess) + (x - e^yn)/e^yn
        y_n = y_n + (x - y_k) / y_k;
        y_k = Exp(y_n);
    }

    return y_n;
}

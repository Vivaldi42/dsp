/*

To run in the terminal:
    g++ -std=c++17 assignment_1_sin_cos.cpp -o ass1_sin_cos
    ./ass1_sin_cos
    python visual.py

*/

/*
Explanation:
The program generates discrete-time sine and cosine waves using a
complex exponential recursion. 

Define a complex state g[n] by:
    g[0] = 1 + j0
    g[n+1] = g[n] · z

where:
    z = cos(ω) + j sin(ω) = e^{jω} (Euler’s formula)

By induction, this recursion yields:
    g[n] = e^{jnω}

Therefore, the real and imaginary parts of g[n] are:
    Re{g[n]} = cos(nω)
    Im{g[n]} = sin(nω)

Thus, each multiplication by z rotates the complex state by ω radians on the unit
circle, producing one sample of cosine and sine. The frequency is controlled by ω,
and the amplitude remains ideally constant because |z| = 1.
*/

#include <iostream>
#include <fstream>
#include <cmath>

class Complex {
public:
    double re;
    double im;

    Complex(double real, double imag) : re(real), im(imag) {}

    Complex operator*(const Complex& other) const {
        // (a + jb)(c + jd) = (ac - bd) + j(ad + bc)
        return Complex(
            re * other.re - im * other.im,
            re * other.im + im * other.re
        );
    }
};

int main() {
    const double omega = 0.1;   // rotation in radians per sample
    const int N = 300;
    const char* filename = "sine_cosine.csv";

    // Constant rotator z = e^{j*ω} = cos(ω) + j.sin(ω)
    Complex z(std::cos(omega), std::sin(omega));

    // State g[n] = e^{jnω}
    Complex g(1.0, 0.0);

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    file << "n,cos,sin\n";
    for (int n = 0; n < N; n++) {
        file << n << "," << g.re << "," << g.im << "\n";
        g = g * z;   // recursive multiplication
    }

    file.close();
    std::cout << "Wrote " << N << " samples to " << filename << "\n";
    return 0;
}

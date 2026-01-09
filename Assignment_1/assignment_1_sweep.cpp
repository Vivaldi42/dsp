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
    // Sweep parameters (radians/sample)
    const double omega0 = 0.02;  // start angular frequency
    const double omega1 = 0.30;  // end angular frequency
    const int N = 800;          // number of samples
    const char* filename = "sweep.csv";

    const double alpha = (omega1 - omega0) / (N - 1);

    Complex g(1.0, 0.0);

    // Rotator z[n] = e^{j * omega[n]} (time-varying)
    Complex z(std::cos(omega0), std::sin(omega0));

    // Constant increment w = e^{j * alpha} so that z[n+1] = z[n] * w
    Complex w(std::cos(alpha), std::sin(alpha));

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: could not open " << filename << " for writing.\n";
        return 1;
    }

    // CSV header
    file << "n,cos,sin\n";

    for (int n = 0; n < N; ++n) {
        file << n << "," << g.re << "," << g.im << "\n";

        // Advance oscillator by current step
        g = g * z;

        // Increase frequency step for next sample (linear sweep)
        z = z * w;
    }

    file.close();
    std::cout << "Wrote " << N << " samples to " << filename << "\n";
    return 0;
}

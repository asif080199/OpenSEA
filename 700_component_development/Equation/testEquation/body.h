#ifndef BODY_H
#define BODY_H
#include <complex>
#include <vector>

class Body
{
public:
    Body();
    std::vector< std::complex<double> > listForceActive_user;
    std::vector< std::complex<double> > listForceActive_hydro;
    std::vector< std::complex<double> > listForceReact_user;
    std::vector< std::complex<double> > listForceReact_hydro;
    std::vector< std::complex<double> > listForceCross_user;
    std::vector< std::complex<double> > listForceCross_hydro;
    std::vector< std::complex<double> > ForceMass;
};

#endif // BODY_H

// -*- C++ -*-
// LHAPDFv5/v6 compatibility example
#include "LHAPDF/LHAPDF.h"
#include <iostream>
int main() {

    using namespace std;

    cout<<endl;

    const double x_1 = 1e-3, Q_1 = 200;
    const double x_2 = 3e-3, Q_2 = 100;

    // const double x = 1e-3, Q = 200;

    LHAPDF::PDFSet set = LHAPDF::getPDFSet("PDF4LHC15_nlo_mc");
    const vector<LHAPDF::PDF*> pdfs = set.mkPDFs();


    const size_t nmem = set.size()-1;
    vector<double> xAll_1, xAll_2;

    for (size_t imem = 0; imem <= nmem; imem++){
        xAll_1.push_back(pdfs[imem]->xfxQ(21,x_1,Q_1));
        xAll_2.push_back(pdfs[imem]->xfxQ(21,x_2,Q_2));
    }

    string labformat = "%2s%10s%12s%12s%12s%12s\n";
    string numformat = "%12.4e%12.4e%12.4e%12.4e%12.4e\n";

    // Calculate PDF uncertainty on gluon distribution.
    cout << "Gluon distribution at Q = " << Q_1 << " GeV (normal uncertainties)" << endl;
    printf(labformat.c_str()," #","x","xg","error+","error-","error");
    const LHAPDF::PDFUncertainty xErr_1 = set.uncertainty(xAll_1, -1); // -1 => same C.L. as set
    printf(numformat.c_str(), x_1, xErr_1.central, xErr_1.errplus, xErr_1.errminus, xErr_1.errsymm);

    cout<<endl;

    cout << "Gluon distribution at Q = " << Q_2 << " GeV (normal uncertainties)" << endl;
    printf(labformat.c_str()," #","x","xg","error+","error-","error");
    const LHAPDF::PDFUncertainty xErr_2 = set.uncertainty(xAll_2, -1); // -1 => same C.L. as set
    printf(numformat.c_str(), x_2, xErr_2.central, xErr_2.errplus, xErr_2.errminus, xErr_2.errsymm);

    cout<<endl;
    cout<<"w = "<<(xErr_1.errplus+xErr_1.central)*(xErr_2.errplus+xErr_2.central)/(xErr_1.central*xErr_2.central)<<endl;
    cout<<endl;


    return 0;
}

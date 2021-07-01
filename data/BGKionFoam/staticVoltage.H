{

        fvScalarMatrix VstaticEqn
        (
            fvm::laplacian(epsilon0, Vstatic)
        );

        VstaticEqn.solve();

};



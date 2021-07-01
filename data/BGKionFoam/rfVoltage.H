{

        fvScalarMatrix VrfEqn
        (
            fvm::laplacian(epsilon0, Vrf)
        );

        VrfEqn.solve();

};

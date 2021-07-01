{

	fluxIon = fvc::interpolate(UIon) & mesh.Sf();
	
        fvScalarMatrix rhoIonEqn
        (
            fvm::ddt(rhoIon) + fvm::div(fluxIon,rhoIon) - fvm::laplacian((K0reduced*p0/pGas)*kBoltz*TempGas/(zstateIon*elemQ),rhoIon)
        );

        rhoIonEqn.solve();


	
	nIon = rhoIon/(DaltonIon/1000/avagadroN);




	colFreq =  zstateIon*elemQ *  (pGas/K0reduced/p0) *  (1000 * avagadroN / DaltonIon);








 //   if(CCSvsK0.value() == 1)
//    {
//    	colFreq = 0.0001 * IonCCS * (pGas/kBoltz/TempGas) * sqrt( 1+ (DaltonGas/DaltonIon) ) * sqrt( 8* kBoltz * TempGas / (DaltonGas/avagadroN/1000/(22/7)) );





//    }

//    if(CCSvsK0.value() == 2)
//    {
 //   	colFreq = 0.0001 * ((3/16) * sqrt(2*22/7/kBoltz/TempGas)  * sqrt((DaltonGas+DaltonIon)/DaltonGas/DaltonIon*1000*avagadroN)  * ( zstateIon*elemQ/nGas/( K0reduced*(p0/pGas) ) )) * (pGas/kBoltz/TempGas) * sqrt( 1+ (DaltonGas/DaltonIon) ) * sqrt( 8* kBoltz * TempGas / (DaltonGas/avagadroN/1000/(22/7)) );

//    }


	//const Random randObj(time(NULL)) ;
	// scalar rndX = randObj.scalar01() ;


// Explicit Discretization for Electric Field acceleration source
// Implicit Discretization for the collisional damping source term

    fvVectorMatrix UIonEqn
        (
            fvm::ddt(UIon) + fvm::div(fluxIon,UIon) - (zstateIon/DaltonIon)*(elemQ*avagadroN*1000)*EnetField + fvm::Sp(colFreq,UIon)
        );

    UIonEqn.solve();
	CurrentDensity = nIon*zstateIon*elemQ*UIon;
    Current = fvc::interpolate(CurrentDensity) & mesh.Sf();






	scalar IonCoNum = 0.0;
	scalar IonCoNumMean = 0.0;



	if (mesh.nInternalFaces())
	    {
   	 	scalarField sumIonFlux
   	 	(
      	 		fvc::surfaceSum(mag(fluxIon))().primitiveField()
   	 	);

	   	 IonCoNum = 0.5*gMax(sumIonFlux/mesh.V().field())*runTime.deltaTValue();

	   	 IonCoNumMean = 0.5*(gSum(sumIonFlux)/gSum(mesh.V().field()))*runTime.deltaTValue();

	//   	 if (gSum(sumIonFlux) < 1.0e-19) { Info << "The ions have exited the internal domain " << endl;  }

	    }
	







          if (IonCoNum >0.25)
	    {
 	  	runTime.setDeltaT(0.9*runTime.deltaT());

		Info<< " New Time Step: " << runTime.deltaT() << "   Mean Courant Number is " <<  IonCoNumMean
    		<< nl <<	endl;
	    }

          if (IonCoNum <0.09)
	    {
 	  	runTime.setDeltaT(1.5*runTime.deltaT());
		Info<< " New Time Step: " << runTime.deltaT()
    		<< nl <<	endl;
	    }


};



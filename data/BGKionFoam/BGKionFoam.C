

////////////////////// (C) Sandilya Garimella edited January 30 2020 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    my_RFPseuod.C

Description
    Time Varying Solver for Static and Dynamic Voltages, Ion Transport and Gas Flow.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "simpleControl.H"
#include "fvOptions.H"
#include "Random.H"
#include "turbulentTransportModel.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "postProcess.H"

    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createControl.H"
    #include "createFields.H"


 

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (simple.loop(runTime))
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

       










/////////////////////// VOLTAGE AND FIELD SOLVER ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////// (C) Sandilya Garimella edited January 30 2020 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
// --- Calculation of DC potential and RF-Effective Potential for the firtst 10 time steps. These are time invariant values------------------------------------------
	
	if (runTime.value()/runTime.deltaT() < NiterVrfMax)   
	{ 
        	{
		    
		    #include "staticVoltage.H"
        	#include "rfVoltage.H"
		    Vpseudo = (zstateIon/DaltonIon) * (0.25*elemQ*avagadroN*1000/(RFfreq*RFfreq)) *  (Vamplitude*Vamplitude)*(fvc::grad(-Vrf) & fvc::grad(-Vrf)) ;

     		}
	}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// --- Calculation of time varying Potentials and Fields -----------------------------------------------------------------------------------------------------------------     
       VrfMicroMotion = 2*( Vpseudo  ) * ( sin(2*22/7*RFfreq*runTime.value()*timedimensionProxy) * sin(2*22/7*RFfreq*runTime.value()*timedimensionProxy) ) ;  // "timedimensionProxy" enables dimension consistency


       #include "dynamicVoltage.H"

       Vnet = Vstatic + (Vpseudo + VrfMicroMotion) + Vdynamic ;	
       EnetField = -fvc::grad(Vnet);
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
      
////////////////// VOLTAGE AND FIELD SOLVER  - END /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////










/////////////////////// ION TRANSPORT SOLVER         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#include "ionTransportRelaxed.H"

/////////////////////// ION TRANSPORT SOLVER   END   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////// GAS FLOW SOLVER         ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

       // this is empty for now. Later on we will append 3 solvers (Navier Stokes, DSMC and MD solvers for neutral flows

           //#include "incompressibleUnsteady.H"
       	  //  #include "incompressibleSteady.H"
       	  //  #include "compressibleUnsteady.H"
       	  //  #include "compressibleSteady.H"
       	  //  #include "dsmcGasFlow.H"
       	  //  #include "mdGasFlow.H"
             // Maxwellian Distributed Gas Uboltz
    //   Info<< "Uboltz  beginning " << endl;
      // 	  #include "maxwellGas.H"
       // --- Pressure-velocity SIMPLE corrector



/////////////////////// GAS FLOW SOLVER   END   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






  
       runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
           << "  ClockTime = " << runTime.elapsedClockTime() << " s"
         << nl << endl;
    
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //

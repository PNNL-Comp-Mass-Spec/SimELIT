Info<< "Reading solverProperties\n" << endl;

IOdictionary solverProperties
(
    IOobject
    (
        "solverProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);


Info<< "Reading elementaryProperties\n" << endl;

IOdictionary elementaryProperties
(
    IOobject
    (
        "elementaryProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);




Info<< "Reading rfProperties\n" << endl;

IOdictionary rfProperties
(
    IOobject
    (
        "rfProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);


Info<< "Reading gasProperties\n" << endl;

IOdictionary gasProperties
(
    IOobject
    (
        "gasProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);



Info<< "Reading ionProperties\n" << endl;

IOdictionary ionProperties
(
    IOobject
    (
        "ionProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);


// SOLVER PROPERTIES //

dimensionedScalar UserChoice_EffPot
(
    "UserChoice_EffPot",
    solverProperties.lookup("UserChoice_EffPot")
);

dimensionedScalar NiterVrfMax
(
    "NiterVrfMax",
    solverProperties.lookup("NiterVrfMax")
);

dimensionedScalar CCSvsK0
(
    "CCSvsK0",
    solverProperties.lookup("CCSvsK0")
);

// RF PROPERTIES RELEVANT IF PSEUDO PTENTIAL IS CALCULATED//

dimensionedScalar timedimensionProxy
(
    "timedimensionProxy",
    rfProperties.lookup("timedimensionProxy")
);

dimensionedScalar RFfreq
(
    "RFfreq",
    rfProperties.lookup("RFfreq")
);

dimensionedScalar Vamplitude
(
    "Vamplitude",
    rfProperties.lookup("Vamplitude")
);



// ELEMENTARY  PROPERTIES //

dimensionedScalar elemQ
(
    "elemQ",
    elementaryProperties.lookup("elemQ")
);


dimensionedScalar avagadroN
(
    "avagadroN",
    elementaryProperties.lookup("avagadroN")
);


dimensionedScalar kBoltz
(
    "kBoltz",
    elementaryProperties.lookup("kBoltz")
);


dimensionedScalar p0
(
    "p0",
    elementaryProperties.lookup("p0")
);



// GAS PROPERTIES //


dimensionedScalar epsilon0
(
    "epsilon0",
    gasProperties.lookup("epsilon0")
);

dimensionedScalar DaltonGas
(
    "DaltonGas",
     gasProperties.lookup("DaltonGas")
);




// ION PROPERTIES //


dimensionedScalar DaltonIon
(
    "DaltonIon",
    ionProperties.lookup("DaltonIon")
);


dimensionedScalar zstateIon
(
    "zstateIon",
    ionProperties.lookup("zstateIon")
);

dimensionedScalar SpHtRatioIon
(
   "SpHtRatioIon",
   ionProperties.lookup("SpHtRatioIon")
);


dimensionedScalar IonCCS
(
   "IonCCS",
   ionProperties.lookup("IonCCS")
);

dimensionedScalar K0reduced
(
   "K0reduced",
   ionProperties.lookup("K0reduced")
);



////////////////////////////////////////// DEFININING FIELD VARIABLES ////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////// (A) NEUTRAL TRANSPORT /////////////////////////////////////////////////////////////
Info<< "Reading field TempGas\n" << endl;
volScalarField TempGas
(
    IOobject
    (
        "TempGas",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
	mesh
);


Info<< "Reading field pGas\n" << endl;
volScalarField pGas
(
    IOobject
    (
        "pGas",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
	mesh
);


Info<< "Reading field nGas\n" << endl;
volScalarField nGas
(
    IOobject
    (
        "nGas",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
	pGas/kBoltz/TempGas
);



Info<< "Reading field Ugas\n" << endl;
volVectorField Ugas
(
    IOobject
    (
        "Ugas",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// (B) VOLTAGES /////////////////////////////////////////////////////////////


Info<< "Reading field Vrf\n" << endl;
volScalarField Vrf
(
    IOobject
    (
        "Vrf",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);



Info<< "Reading field Vstatic\n" << endl;
volScalarField Vstatic
(
    IOobject
    (
        "Vstatic",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


Info<< "Reading field Vdynamic\n" << endl;
volScalarField Vdynamic
(
    IOobject
    (
        "Vdynamic",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);





Info<< "Reading field Vpseudo\n" << endl;
volScalarField Vpseudo
(
    IOobject
    (
        "Vpseudo",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    (zstateIon/DaltonIon) * (0.25*elemQ*avagadroN*1000/(RFfreq*RFfreq)) *  (Vamplitude*Vamplitude)*(fvc::grad(-Vrf) & fvc::grad(-Vrf))
);



Info<< "Reading field VrfMicroMotion\n" << endl;
volScalarField VrfMicroMotion
(
    IOobject
    (
        "VrfMicroMotion",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    2*( Vpseudo  ) * ( sin(2*22/7*RFfreq*runTime.value()*timedimensionProxy) * sin(2*22/7*RFfreq*runTime.value()*timedimensionProxy) ) 
);


Info<< "Reading field Vnet\n" << endl;
volScalarField Vnet
(
    IOobject
    (
        "Vnet",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    Vstatic + (Vpseudo + VrfMicroMotion) + Vdynamic
);


Info<< "Reading field EnetField\n" << endl;
volVectorField EnetField
(
    IOobject
    (
        "EnetField",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    -fvc::grad(Vnet)
);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////..




///////////////////////////////////// (C) ION TRANSPORT /////////////////////////////////////////////////////////////




Info<< "Reading field rhoIon\n" << endl;
volScalarField rhoIon
(
    IOobject
    (
        "rhoIon",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


Info<< "Reading field UIon\n" << endl;
volVectorField UIon
(
    IOobject
    (
        "UIon",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


Info<< "Reading field fluxIon\n" << endl;
surfaceScalarField fluxIon
(
    IOobject
    (
        "fluxIon",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    fvc::interpolate(UIon) & mesh.Sf()
);


Info<< "Reading field pIon\n" << endl;
volScalarField pIon
(
    IOobject
    (
        "pIon",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);



Info<< "Reading field nIon\n" << endl;
volScalarField nIon
(
    IOobject
    (
        "nIon",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
   rhoIon/(DaltonIon/1000/avagadroN)
);



Info<< "Reading field CurrentDensity\n" << endl;
volVectorField CurrentDensity
(
    IOobject
    (
        "CurrentDensity",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
   nIon*zstateIon*elemQ*UIon
);



Info<< "Reading field Current\n" << endl;
surfaceScalarField Current
(
    IOobject
    (
        "Current",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    fvc::interpolate(CurrentDensity) & mesh.Sf()
);



Info<< "Reading field TempIon\n" << endl;
volScalarField TempIon
(
    IOobject
    (
        "TempIon",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
	TempGas
);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////









///////////////////////////////////// (D) ION/NEUTRAL COUPLING VARIABLES /////////////////////////////////////////////////////////////



Info<< "Reading field colFreq\n" << endl;
volScalarField colFreq
(
    IOobject
    (
        "colFreq",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
	zstateIon*elemQ *  (pGas/K0reduced/p0) *  (1000 * avagadroN / DaltonIon)
);


Info<< "Reading field FrandomTerm\n" << endl;
volVectorField FrandomTerm
(
    IOobject
    (
        "FrandomTerm",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    0*(zstateIon/DaltonIon)*(elemQ*avagadroN*1000)*EnetField
);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




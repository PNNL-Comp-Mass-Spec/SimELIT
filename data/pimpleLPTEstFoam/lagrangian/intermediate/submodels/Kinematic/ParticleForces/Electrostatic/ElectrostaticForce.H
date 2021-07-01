/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2019 OpenFOAM Foundation
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

Class
    Foam::ElectrostaticForce

Description
    Calculates particle electrostatic (Electric field) force

SourceFiles
    ElectrostaticForceI.H
    ElectrostaticForce.C

\*---------------------------------------------------------------------------*/

#ifndef ElectrostaticForce_H
#define ElectrostaticForce_H

#include "ParticleForce.H"
#include "interpolation.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class fvMesh;

/*---------------------------------------------------------------------------*\
                       Class ElectrostaticForce Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ElectrostaticForce
:
    public ParticleForce<CloudType>
{
    // Private Data

        //- Name of Electrostatic field strength field - default = "HdotGradH"
        const word HdotGradHName_;

        //- HdotGradH interpolator
        const interpolation<vector>* HdotGradHInterpPtr_;

        //- Electric charge of particle
        const scalar charge_;


public:

    //- Runtime type information
    TypeName("electrostatic");


    // Constructors

        //- Construct from mesh
        ElectrostaticForce
        (
            CloudType& owner,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Construct copy
        ElectrostaticForce(const ElectrostaticForce& gf);

        //- Construct and return a clone
        virtual autoPtr<ParticleForce<CloudType>> clone() const
        {
            return autoPtr<ParticleForce<CloudType>>
            (
                new ElectrostaticForce<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ElectrostaticForce();


    // Member Functions

        // Access

            //- Return the name of electrostatic field strength field
            const word& HdotGradHName() const;

            //- Return the electric charge of particle
            scalar charge() const;


        // Evaluation

            //- Cache fields
            virtual void cacheFields(const bool store);

            //- Calculate the non-coupled force
            virtual forceSuSp calcNonCoupled
            (
                const typename CloudType::parcelType& p,
                const typename CloudType::parcelType::trackingData& td,
                const scalar dt,
                const scalar mass,
                const scalar Re,
                const scalar muc
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "ElectrostaticForceI.H"

#ifdef NoRepository
    #include "ElectrostaticForce.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //

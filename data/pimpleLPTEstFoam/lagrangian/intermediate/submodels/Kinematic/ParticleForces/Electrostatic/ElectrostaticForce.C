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

\*---------------------------------------------------------------------------*/

#include "ElectrostaticForce.H"
#include "demandDrivenData.H"
#include "electromagneticConstants.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
Foam::ElectrostaticForce<CloudType>::ElectrostaticForce
(
    CloudType& owner,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    ParticleForce<CloudType>(owner, mesh, dict, typeName, true),
    HdotGradHName_
    (
        this->coeffs().template lookupOrDefault<word>("E", "E")
    ),
    HdotGradHInterpPtr_(nullptr),
    charge_
    (
        readScalar(this->coeffs().lookup("charge"))
    )
{}


template<class CloudType>
Foam::ElectrostaticForce<CloudType>::ElectrostaticForce
(
    const ElectrostaticForce& pf
)
:
    ParticleForce<CloudType>(pf),
    HdotGradHName_(pf.HdotGradHName_),
    HdotGradHInterpPtr_(pf.HdotGradHInterpPtr_),
    charge_(pf.charge_)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class CloudType>
Foam::ElectrostaticForce<CloudType>::~ElectrostaticForce()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void Foam::ElectrostaticForce<CloudType>::cacheFields(const bool store)
{
    if (store)
    {
        const volVectorField& HdotGradH =
            this->mesh().template lookupObject<volVectorField>(HdotGradHName_);

        HdotGradHInterpPtr_ = interpolation<vector>::New
        (
            this->owner().solution().interpolationSchemes(),
            HdotGradH
        ).ptr();
    }
    else
    {
        deleteDemandDrivenData(HdotGradHInterpPtr_);
    }
}


template<class CloudType>
Foam::forceSuSp Foam::ElectrostaticForce<CloudType>::calcNonCoupled
(
    const typename CloudType::parcelType& p,
    const typename CloudType::parcelType::trackingData& td,
    const scalar dt,
    const scalar mass,
    const scalar Re,
    const scalar muc
) const
{
    forceSuSp value(Zero, 0.0);

    const interpolation<vector>& HdotGradHInterp = *HdotGradHInterpPtr_;

    value.Su()=charge_
       *HdotGradHInterp.interpolate(p.coordinates(), p.currentTetIndices());

    return value;
}


// ************************************************************************* //

// Fill out your copyright notice in the Description page of Project Settings.

#include "RadialActorBase.h"
#include "PolarCollider.h"
#include "Limes.h"
#include "RTSStructureFactory.h"
#include "DrawDebugHelpers.h"

ARadialActorBase::ARadialActorBase() :
	m_bIsCollisionInitialized{ false }
{
}


//Public------------

FVector ARadialActorBase::GetRadialOrigin() const
{
	return m_pOwningFactory->GetActorLocation();

}

bool ARadialActorBase::HasIntersectionsWith(ARadialActorBase *pRadialActor) const
{
	if (!pRadialActor)
	{
		return true;
	}

	if (!m_bIsCollisionInitialized || !pRadialActor->m_bIsCollisionInitialized)
	{
		UE_LOG(RTS_StructurePlacement, Error, TEXT("ARadialActorBase:: intersecton check with uninitalized polar actors"));
		return true;
	}

	return m_PolarCollision.HasIntersectionsWith(pRadialActor->m_PolarCollision);


}

void ARadialActorBase::InitializePolarCollision(ARTSStructureFactory *pNewFactory)
{
	m_pOwningFactory = pNewFactory;
	auto DiscretizedPos{ m_pOwningFactory->Discretize(GetActorLocation()) };

	DrawDebugCrosshairs(GetWorld(), DiscretizedPos, FRotator::ZeroRotator, 50, FColor::Red, true, 100 );
	

	auto CartesianCenter{ PolarMath::SVector2D(DiscretizedPos.X, DiscretizedPos.Y) };
	auto PolarLowrangeCenter{ CartesianCenter.ToPolar() };

	auto HalfBuildingWidth{ m_pOwningFactory->GetCellWidthAngle(PolarLowrangeCenter.Radius) * GetWidthInCells() * .5 };
	PolarMath::CPolarCollider MainHull{ PolarLowrangeCenter,  HalfBuildingWidth, m_pOwningFactory->GetCellDepth() * GetDepthInCells() };

	m_PolarCollision = PolarMath::CPolarCollision{ MainHull, pNewFactory->GetPolarTransform() };
	m_bIsCollisionInitialized = true;


}
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

float ARadialActorBase::GetMainHullDepth() const
{
	return m_bIsCollisionInitialized ? m_PolarCollision.GetMainHullDepth() : 0;


}

float ARadialActorBase::GetMainHullRightAngle() const
{
	return m_bIsCollisionInitialized ? m_PolarCollision.GetMainHullRightAngle() : 0;


}

float ARadialActorBase::GetMainHullLeftAngle() const
{
	return m_bIsCollisionInitialized ? m_PolarCollision.GetMainHullLeftAngle() : 0;


}

float ARadialActorBase::GetMainHullMinRadius() const
{
	return m_bIsCollisionInitialized ? m_PolarCollision.GetMainHullMinRadius() : 0;


}

FVector2D ARadialActorBase::GetCollisionCartesianCenter() const
{
	auto CartesianCenter{ m_PolarCollision.GetCartesianCenter() };
	return m_bIsCollisionInitialized ? FVector2D{ static_cast<float>(CartesianCenter.X), static_cast<float>(CartesianCenter.Y) } : FVector2D::ZeroVector;


}

float ARadialActorBase::GetMainHullHalfWidthAngle() const
{
	return m_bIsCollisionInitialized ? m_PolarCollision.GetMainHullHalfWidthAngle() : 0;


}

float ARadialActorBase::GetMainHullCenterAngle() const
{
	return m_bIsCollisionInitialized ? m_PolarCollision.GetMainHullCenterAngle() : 0;


}

void ARadialActorBase::InitializePolarCollision(ARTSStructureFactory *pNewFactory)
{
	m_pOwningFactory = pNewFactory;
	auto DiscretizedPos{ GetActorLocation() };

	DrawDebugCrosshairs(GetWorld(), DiscretizedPos, FRotator::ZeroRotator, 50, FColor::Red, true, 100 );
	

	auto CartesianCenter{ PolarMath::SVector2D(DiscretizedPos.X, DiscretizedPos.Y) };
	auto PolarLowrangeCenter{ CartesianCenter.ToPolar() };

	auto HalfBuildingWidth{ m_pOwningFactory->GetCellWidthAngle(PolarLowrangeCenter.Radius) * GetWidthInCells() * .5 };
	PolarMath::CPolarCollider MainHull{ PolarLowrangeCenter,  HalfBuildingWidth, m_pOwningFactory->GetCellDepth() * GetDepthInCells() };

	m_PolarCollision = PolarMath::CPolarCollision{ MainHull, pNewFactory->GetPolarTransform() };
	m_bIsCollisionInitialized = true;

	OnCollisionInitialized();


}
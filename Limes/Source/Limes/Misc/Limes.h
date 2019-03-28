// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(RTS_StructureFactory, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(RTS_StructurePlacement, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(RTS_InputDebug, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(RTS_GameInstance, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(RTS_CombatSys, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(RTS_Misc, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(RTS_Events, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(RTS_ResourceSys, Log, All);

template<class T>
UClass *SafeLoadClassPtr(const TSoftClassPtr<T> &ClassPtr)
{
	UClass *pClass = nullptr;
	if ((pClass = ClassPtr.Get()) == nullptr)
	{
		pClass = ClassPtr.LoadSynchronous();

	}

	return ensureMsgf(pClass, TEXT("Could not load class ptr from soft obj ptr")) ? pClass : nullptr;


}

template<class T>
UObject *SafeLoadObjectPtr(const TSoftObjectPtr<T> &ObjectPtr)
{
	UObject *pObject = nullptr;
	if ((pObject = ObjectPtr.Get()) == nullptr)
	{
		pObject = ObjectPtr.LoadSynchronous();

	}

	return ensureMsgf(pObject, TEXT("Could not load class ptr from soft obj ptr")) ? pObject : nullptr;


}

class FLimesGameModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;


};
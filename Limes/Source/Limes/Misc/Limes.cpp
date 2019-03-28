// Fill out your copyright notice in the Description page of Project Settings.

#include "Limes.h"
#include "Modules/ModuleManager.h"
#if WITH_EDITOR
#include "PropertyEditorModule.h"
#endif
#include "Daytime.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FLimesGameModule, Limes, "Limes" );

DEFINE_LOG_CATEGORY(RTS_StructureFactory);
DEFINE_LOG_CATEGORY(RTS_StructurePlacement);
DEFINE_LOG_CATEGORY(RTS_InputDebug);
DEFINE_LOG_CATEGORY(RTS_GameInstance);
DEFINE_LOG_CATEGORY(RTS_CombatSys);
DEFINE_LOG_CATEGORY(RTS_Misc);
DEFINE_LOG_CATEGORY(RTS_Events);
DEFINE_LOG_CATEGORY(RTS_ResourceSys);


void FLimesGameModule::StartupModule()
{
	UE_LOG(RTS_Misc, Log, TEXT("Starting up primary game module"));

	/*
	auto &PropertyModule{ FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor") };

	PropertyModule.RegisterCustomPropertyTypeLayout
	(
		TEXT("Daytime"),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDaytimeCustomization::MakeInstance)
	);
	*/

	UE_LOG(RTS_Misc, Log, TEXT("Start up of primary game module completed"));


}

void FLimesGameModule::ShutdownModule()
{
	UE_LOG(RTS_Misc, Log, TEXT("Shutting down primary game module"));

	/*
	if(FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		auto &PropertyModule{ FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor") };

		PropertyModule.UnregisterCustomPropertyTypeLayout(TEXT("Daytime"));
	}
	*/

	UE_LOG(RTS_Misc, Log, TEXT("Shutt down of primary game module completed"));


}

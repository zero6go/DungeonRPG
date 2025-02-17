// DungeonRPG


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "Player/RPGPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	const ARPGPlayerState *PS = Cast<ARPGPlayerState>(PlayerState);
	OnSpellPointChanged.Broadcast(PS->GetSpellPoint());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	ARPGPlayerState *PS = Cast<ARPGPlayerState>(PlayerState);
	PS->OnSpellPointChanged.AddUObject(this, &USpellMenuWidgetController::SpellPointChanged);
}

void USpellMenuWidgetController::SpellPointChanged(const int32 SpellPoint) const
{
	OnSpellPointChanged.Broadcast(SpellPoint);
}

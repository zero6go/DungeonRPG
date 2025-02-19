// DungeonRPG

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

struct FRPGAbilityInfo;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DUNGEONRPG_API USpellMenuWidgetController : public URPGWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void SpellLevelUp(const FGameplayTag &AbilityTag);

	UFUNCTION(BlueprintCallable)
	int32 GetAbilityLevelByTag(const FGameplayTag &AbilityTag);

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnSpellPointChanged;

	UFUNCTION(BlueprintCallable)
	bool GetDescription(const FGameplayTag &AbilityTag, const int32 AbilityLevel, FString &Description, FString &NextLevelDescription);

protected:
	void SpellPointChanged(const int32 SpellPoint) const;
	
};

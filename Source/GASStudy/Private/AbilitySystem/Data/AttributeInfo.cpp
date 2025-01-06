// Copyright CC Mechanics


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (FAuraAttributeInfo Information : AttributeInformation)
	{
		if (Information.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Information;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("No found info for AttributeTag [%s] on AttributeInformation [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}

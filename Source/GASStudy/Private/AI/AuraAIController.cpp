// Copyright CC Mechanics


#include "AI/AuraAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAuraAIController::AAuraAIController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComp");
	check(BehaviorTreeComp);
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComp");
	check(Blackboard);
}

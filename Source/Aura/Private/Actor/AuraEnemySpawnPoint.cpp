// Copyright Carter Wooton


#include "Actor/AuraEnemySpawnPoint.h"
#include "Character/AuraEnemy.h"

AAuraEnemy* AAuraEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetPlayerLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);

	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
	return Enemy;
}

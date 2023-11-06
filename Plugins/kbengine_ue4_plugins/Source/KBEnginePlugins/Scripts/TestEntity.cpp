#include "TestEntity.h"
#include "Engine/Entity.h"
#include "Engine/KBEngine.h"
#include "Engine/KBDebug.h"

namespace KBEngine
{

    TestEntity::TestEntity() : TestEntityBase()
    {
    }

    TestEntity::~TestEntity()
    {
    }

    void TestEntity::onReqTestProperty(const TEST_ENTITY_PROPS& arg1)
    {}

}
// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Engine/KBECommon.h"

#include "Engine/TestEntityBase.h"

namespace KBEngine
{

    class TestEntity : public TestEntityBase
    {
    public:
        TestEntity();
        virtual ~TestEntity();

        virtual void onReqTestProperty(const TEST_ENTITY_PROPS& arg1) override;
    };

}

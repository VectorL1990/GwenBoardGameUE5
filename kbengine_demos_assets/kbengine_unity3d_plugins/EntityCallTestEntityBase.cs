/*
	Generated by KBEngine!
	Please do not modify this file!
	
	tools = kbcmd
*/

namespace KBEngine
{
	using UnityEngine;
	using System;
	using System.Collections;
	using System.Collections.Generic;

	// defined in */scripts/entity_defs/TestEntity.def
	public class EntityBaseEntityCall_TestEntityBase : EntityCall
	{

		public EntityBaseEntityCall_TestEntityBase(Int32 eid, string ename) : base(eid, ename)
		{
			type = ENTITYCALL_TYPE.ENTITYCALL_TYPE_BASE;
		}

		public void reqTestProperty()
		{
			Bundle pBundle = newCall("reqTestProperty", 0);
			if(pBundle == null)
				return;

			sendCall(null);
		}

	}

	public class EntityCellEntityCall_TestEntityBase : EntityCall
	{

		public EntityCellEntityCall_TestEntityBase(Int32 eid, string ename) : base(eid, ename)
		{
			type = ENTITYCALL_TYPE.ENTITYCALL_TYPE_CELL;
		}

	}
	}

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



	public class DATATYPE_ENTITY_FORBID_COUNTER : DATATYPE_BASE
	{
		public ENTITY_FORBID_COUNTER createFromStreamEx(MemoryStream stream)
		{
			UInt32 size = stream.readUint32();
			ENTITY_FORBID_COUNTER datas = new ENTITY_FORBID_COUNTER();

			while(size > 0)
			{
				--size;
				datas.Add(stream.readInt8());
			};

			return datas;
		}

		public void addToStreamEx(Bundle stream, ENTITY_FORBID_COUNTER v)
		{
			stream.writeUint32((UInt32)v.Count);
			for(int i=0; i<v.Count; ++i)
			{
				stream.writeInt8(v[i]);
			};
		}
	}



	public class DATATYPE_ENTITYID_LIST : DATATYPE_BASE
	{
		public ENTITYID_LIST createFromStreamEx(MemoryStream stream)
		{
			UInt32 size = stream.readUint32();
			ENTITYID_LIST datas = new ENTITYID_LIST();

			while(size > 0)
			{
				--size;
				datas.Add(stream.readInt32());
			};

			return datas;
		}

		public void addToStreamEx(Bundle stream, ENTITYID_LIST v)
		{
			stream.writeUint32((UInt32)v.Count);
			for(int i=0; i<v.Count; ++i)
			{
				stream.writeInt32(v[i]);
			};
		}
	}



	public class DATATYPE_BAG : DATATYPE_BASE
	{
		private DATATYPE__BAG_values22_ArrayType_ChildArray values22_DataType = new DATATYPE__BAG_values22_ArrayType_ChildArray();

		public class DATATYPE__BAG_values22_ArrayType_ChildArray : DATATYPE_BASE
		{
			private DATATYPE___BAG_values22_ArrayType_ArrayType_ChildArray itemType = new DATATYPE___BAG_values22_ArrayType_ArrayType_ChildArray();

			public class DATATYPE___BAG_values22_ArrayType_ArrayType_ChildArray : DATATYPE_BASE
			{
				public List<Int64> createFromStreamEx(MemoryStream stream)
				{
					UInt32 size = stream.readUint32();
					List<Int64> datas = new List<Int64>();

					while(size > 0)
					{
						--size;
						datas.Add(stream.readInt64());
					};

					return datas;
				}

				public void addToStreamEx(Bundle stream, List<Int64> v)
				{
					stream.writeUint32((UInt32)v.Count);
					for(int i=0; i<v.Count; ++i)
					{
						stream.writeInt64(v[i]);
					};
				}
			}

			public List<List<Int64>> createFromStreamEx(MemoryStream stream)
			{
				UInt32 size = stream.readUint32();
				List<List<Int64>> datas = new List<List<Int64>>();

				while(size > 0)
				{
					--size;
					datas.Add(itemType.createFromStreamEx(stream));
				};

				return datas;
			}

			public void addToStreamEx(Bundle stream, List<List<Int64>> v)
			{
				stream.writeUint32((UInt32)v.Count);
				for(int i=0; i<v.Count; ++i)
				{
					itemType.addToStreamEx(stream, v[i]);
				};
			}
		}

		public BAG createFromStreamEx(MemoryStream stream)
		{
			BAG datas = new BAG();
			datas.values22 = values22_DataType.createFromStreamEx(stream);
			return datas;
		}

		public void addToStreamEx(Bundle stream, BAG v)
		{
			values22_DataType.addToStreamEx(stream, v.values22);
		}
	}



	public class DATATYPE_EXAMPLES : DATATYPE_BASE
	{
		public EXAMPLES createFromStreamEx(MemoryStream stream)
		{
			EXAMPLES datas = new EXAMPLES();
			datas.k1 = stream.readInt64();
			datas.k2 = stream.readInt64();
			return datas;
		}

		public void addToStreamEx(Bundle stream, EXAMPLES v)
		{
			stream.writeInt64(v.k1);
			stream.writeInt64(v.k2);
		}
	}



	public class DATATYPE_TEST_ENTITY_PROPS : DATATYPE_BASE
	{
		public TEST_ENTITY_PROPS createFromStreamEx(MemoryStream stream)
		{
			TEST_ENTITY_PROPS datas = new TEST_ENTITY_PROPS();
			datas.test1 = stream.readInt32();
			return datas;
		}

		public void addToStreamEx(Bundle stream, TEST_ENTITY_PROPS v)
		{
			stream.writeInt32(v.test1);
		}
	}



	public class DATATYPE_PLAYER_ACTION : DATATYPE_BASE
	{
		public PLAYER_ACTION createFromStreamEx(MemoryStream stream)
		{
			PLAYER_ACTION datas = new PLAYER_ACTION();
			datas.actionType = stream.readUint8();
			datas.playCardName = stream.readString();
			datas.targetGridNb = stream.readInt32();
			return datas;
		}

		public void addToStreamEx(Bundle stream, PLAYER_ACTION v)
		{
			stream.writeUint8(v.actionType);
			stream.writeString(v.playCardName);
			stream.writeInt32(v.targetGridNb);
		}
	}



	public class DATATYPE_UPDATE_GRID_INFO : DATATYPE_BASE
	{
		public UPDATE_GRID_INFO createFromStreamEx(MemoryStream stream)
		{
			UPDATE_GRID_INFO datas = new UPDATE_GRID_INFO();
			datas.gridNb = stream.readInt32();
			datas.cardUid = stream.readInt32();
			datas.updateType = stream.readString();
			datas.updateValue = stream.readInt32();
			return datas;
		}

		public void addToStreamEx(Bundle stream, UPDATE_GRID_INFO v)
		{
			stream.writeInt32(v.gridNb);
			stream.writeInt32(v.cardUid);
			stream.writeString(v.updateType);
			stream.writeInt32(v.updateValue);
		}
	}



	public class DATATYPE_UPDATE_GRID_INFO_LIST : DATATYPE_BASE
	{
		private DATATYPE__UPDATE_GRID_INFO_LIST_updateGridList_ArrayType_ChildArray updateGridList_DataType = new DATATYPE__UPDATE_GRID_INFO_LIST_updateGridList_ArrayType_ChildArray();

		public class DATATYPE__UPDATE_GRID_INFO_LIST_updateGridList_ArrayType_ChildArray : DATATYPE_BASE
		{
			private DATATYPE_UPDATE_GRID_INFO itemType = new DATATYPE_UPDATE_GRID_INFO();

			public List<UPDATE_GRID_INFO> createFromStreamEx(MemoryStream stream)
			{
				UInt32 size = stream.readUint32();
				List<UPDATE_GRID_INFO> datas = new List<UPDATE_GRID_INFO>();

				while(size > 0)
				{
					--size;
					datas.Add(itemType.createFromStreamEx(stream));
				};

				return datas;
			}

			public void addToStreamEx(Bundle stream, List<UPDATE_GRID_INFO> v)
			{
				stream.writeUint32((UInt32)v.Count);
				for(int i=0; i<v.Count; ++i)
				{
					itemType.addToStreamEx(stream, v[i]);
				};
			}
		}

		public UPDATE_GRID_INFO_LIST createFromStreamEx(MemoryStream stream)
		{
			UPDATE_GRID_INFO_LIST datas = new UPDATE_GRID_INFO_LIST();
			datas.updateId = stream.readInt32();
			datas.updateGridList = updateGridList_DataType.createFromStreamEx(stream);
			return datas;
		}

		public void addToStreamEx(Bundle stream, UPDATE_GRID_INFO_LIST v)
		{
			stream.writeInt32(v.updateId);
			updateGridList_DataType.addToStreamEx(stream, v.updateGridList);
		}
	}



	public class DATATYPE_BATTLE_GRID_INFO : DATATYPE_BASE
	{
		public BATTLE_GRID_INFO createFromStreamEx(MemoryStream stream)
		{
			BATTLE_GRID_INFO datas = new BATTLE_GRID_INFO();
			datas.gridNb = stream.readInt32();
			datas.cardUid = stream.readInt32();
			datas.hp = stream.readInt32();
			datas.defence = stream.readInt32();
			datas.agility = stream.readUint8();
			return datas;
		}

		public void addToStreamEx(Bundle stream, BATTLE_GRID_INFO v)
		{
			stream.writeInt32(v.gridNb);
			stream.writeInt32(v.cardUid);
			stream.writeInt32(v.hp);
			stream.writeInt32(v.defence);
			stream.writeUint8(v.agility);
		}
	}



	public class DATATYPE_CORE_UPDATE_BATLLE_INFO : DATATYPE_BASE
	{
		private DATATYPE__CORE_UPDATE_BATLLE_INFO_updateList_ArrayType_ChildArray updateList_DataType = new DATATYPE__CORE_UPDATE_BATLLE_INFO_updateList_ArrayType_ChildArray();

		public class DATATYPE__CORE_UPDATE_BATLLE_INFO_updateList_ArrayType_ChildArray : DATATYPE_BASE
		{
			private DATATYPE_BATTLE_GRID_INFO itemType = new DATATYPE_BATTLE_GRID_INFO();

			public List<BATTLE_GRID_INFO> createFromStreamEx(MemoryStream stream)
			{
				UInt32 size = stream.readUint32();
				List<BATTLE_GRID_INFO> datas = new List<BATTLE_GRID_INFO>();

				while(size > 0)
				{
					--size;
					datas.Add(itemType.createFromStreamEx(stream));
				};

				return datas;
			}

			public void addToStreamEx(Bundle stream, List<BATTLE_GRID_INFO> v)
			{
				stream.writeUint32((UInt32)v.Count);
				for(int i=0; i<v.Count; ++i)
				{
					itemType.addToStreamEx(stream, v[i]);
				};
			}
		}

		public CORE_UPDATE_BATLLE_INFO createFromStreamEx(MemoryStream stream)
		{
			CORE_UPDATE_BATLLE_INFO datas = new CORE_UPDATE_BATLLE_INFO();
			datas.updateList = updateList_DataType.createFromStreamEx(stream);
			return datas;
		}

		public void addToStreamEx(Bundle stream, CORE_UPDATE_BATLLE_INFO v)
		{
			updateList_DataType.addToStreamEx(stream, v.updateList);
		}
	}



	public class DATATYPE_SYNC_BATTLE_TIME_INFO : DATATYPE_BASE
	{
		public SYNC_BATTLE_TIME_INFO createFromStreamEx(MemoryStream stream)
		{
			SYNC_BATTLE_TIME_INFO datas = new SYNC_BATTLE_TIME_INFO();
			datas.curTime = stream.readInt32();
			datas.playerNb = stream.readUint8();
			return datas;
		}

		public void addToStreamEx(Bundle stream, SYNC_BATTLE_TIME_INFO v)
		{
			stream.writeInt32(v.curTime);
			stream.writeUint8(v.playerNb);
		}
	}



	public class DATATYPE_SYNC_CARD_INFO : DATATYPE_BASE
	{
		private DATATYPE__SYNC_CARD_INFO_tags_ArrayType_ChildArray tags_DataType = new DATATYPE__SYNC_CARD_INFO_tags_ArrayType_ChildArray();

		public class DATATYPE__SYNC_CARD_INFO_tags_ArrayType_ChildArray : DATATYPE_BASE
		{
			public List<string> createFromStreamEx(MemoryStream stream)
			{
				UInt32 size = stream.readUint32();
				List<string> datas = new List<string>();

				while(size > 0)
				{
					--size;
					datas.Add(stream.readString());
				};

				return datas;
			}

			public void addToStreamEx(Bundle stream, List<string> v)
			{
				stream.writeUint32((UInt32)v.Count);
				for(int i=0; i<v.Count; ++i)
				{
					stream.writeString(v[i]);
				};
			}
		}

		public SYNC_CARD_INFO createFromStreamEx(MemoryStream stream)
		{
			SYNC_CARD_INFO datas = new SYNC_CARD_INFO();
			datas.cardKey = stream.readString();
			datas.cardName = stream.readString();
			datas.hp = stream.readUint8();
			datas.defence = stream.readUint8();
			datas.agility = stream.readUint8();
			datas.tags = tags_DataType.createFromStreamEx(stream);
			return datas;
		}

		public void addToStreamEx(Bundle stream, SYNC_CARD_INFO v)
		{
			stream.writeString(v.cardKey);
			stream.writeString(v.cardName);
			stream.writeUint8(v.hp);
			stream.writeUint8(v.defence);
			stream.writeUint8(v.agility);
			tags_DataType.addToStreamEx(stream, v.tags);
		}
	}



	public class DATATYPE_SYNC_PLAYER_BATTLE_INFO : DATATYPE_BASE
	{
		private DATATYPE__SYNC_PLAYER_BATTLE_INFO_cardList_ArrayType_ChildArray cardList_DataType = new DATATYPE__SYNC_PLAYER_BATTLE_INFO_cardList_ArrayType_ChildArray();

		public class DATATYPE__SYNC_PLAYER_BATTLE_INFO_cardList_ArrayType_ChildArray : DATATYPE_BASE
		{
			private DATATYPE_SYNC_CARD_INFO itemType = new DATATYPE_SYNC_CARD_INFO();

			public List<SYNC_CARD_INFO> createFromStreamEx(MemoryStream stream)
			{
				UInt32 size = stream.readUint32();
				List<SYNC_CARD_INFO> datas = new List<SYNC_CARD_INFO>();

				while(size > 0)
				{
					--size;
					datas.Add(itemType.createFromStreamEx(stream));
				};

				return datas;
			}

			public void addToStreamEx(Bundle stream, List<SYNC_CARD_INFO> v)
			{
				stream.writeUint32((UInt32)v.Count);
				for(int i=0; i<v.Count; ++i)
				{
					itemType.addToStreamEx(stream, v[i]);
				};
			}
		}

		private DATATYPE__SYNC_PLAYER_BATTLE_INFO_handCardList_ArrayType_ChildArray handCardList_DataType = new DATATYPE__SYNC_PLAYER_BATTLE_INFO_handCardList_ArrayType_ChildArray();

		public class DATATYPE__SYNC_PLAYER_BATTLE_INFO_handCardList_ArrayType_ChildArray : DATATYPE_BASE
		{
			public List<string> createFromStreamEx(MemoryStream stream)
			{
				UInt32 size = stream.readUint32();
				List<string> datas = new List<string>();

				while(size > 0)
				{
					--size;
					datas.Add(stream.readString());
				};

				return datas;
			}

			public void addToStreamEx(Bundle stream, List<string> v)
			{
				stream.writeUint32((UInt32)v.Count);
				for(int i=0; i<v.Count; ++i)
				{
					stream.writeString(v[i]);
				};
			}
		}

		public SYNC_PLAYER_BATTLE_INFO createFromStreamEx(MemoryStream stream)
		{
			SYNC_PLAYER_BATTLE_INFO datas = new SYNC_PLAYER_BATTLE_INFO();
			datas.cardList = cardList_DataType.createFromStreamEx(stream);
			datas.handCardList = handCardList_DataType.createFromStreamEx(stream);
			return datas;
		}

		public void addToStreamEx(Bundle stream, SYNC_PLAYER_BATTLE_INFO v)
		{
			cardList_DataType.addToStreamEx(stream, v.cardList);
			handCardList_DataType.addToStreamEx(stream, v.handCardList);
		}
	}



	public class DATATYPE_PLAYER_PERSIST_INFO : DATATYPE_BASE
	{
		private DATATYPE__PLAYER_PERSIST_INFO_persistCardList_ArrayType_ChildArray persistCardList_DataType = new DATATYPE__PLAYER_PERSIST_INFO_persistCardList_ArrayType_ChildArray();

		public class DATATYPE__PLAYER_PERSIST_INFO_persistCardList_ArrayType_ChildArray : DATATYPE_BASE
		{
			public List<string> createFromStreamEx(MemoryStream stream)
			{
				UInt32 size = stream.readUint32();
				List<string> datas = new List<string>();

				while(size > 0)
				{
					--size;
					datas.Add(stream.readString());
				};

				return datas;
			}

			public void addToStreamEx(Bundle stream, List<string> v)
			{
				stream.writeUint32((UInt32)v.Count);
				for(int i=0; i<v.Count; ++i)
				{
					stream.writeString(v[i]);
				};
			}
		}

		public PLAYER_PERSIST_INFO createFromStreamEx(MemoryStream stream)
		{
			PLAYER_PERSIST_INFO datas = new PLAYER_PERSIST_INFO();
			datas.persistCardList = persistCardList_DataType.createFromStreamEx(stream);
			datas.campNb = stream.readUint8();
			return datas;
		}

		public void addToStreamEx(Bundle stream, PLAYER_PERSIST_INFO v)
		{
			persistCardList_DataType.addToStreamEx(stream, v.persistCardList);
			stream.writeUint8(v.campNb);
		}
	}


}
#include <stdio.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

int traverse_table(lua_State *L)
{
	lua_pushnil(L);
	    // 现在的栈：-1 => nil; index => table
	int index = -2;
	while (lua_next(L, -2))
	{
			// 现在的栈：-1 => value; -2 => key; index => table
			// 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
		lua_pushvalue(L, -2);
			// 现在的栈：-1 => key; -2 => value; -3 => key; index => table

		const char* key = lua_tostring(L, -1);
		printf("key = %s\n", key);
		lua_pop(L, 1);

		int type = lua_type(L, -1);
		switch (type)
		{
			case LUA_TNUMBER:
				printf("value = %d\n", (int)(lua_tonumber(L, -1)));
				break;
			case LUA_TSTRING:
				printf("value = %s\n", lua_tostring(L, -1));
				break;
			case LUA_TTABLE:
				printf("value = new table\n");
				traverse_table(L);
				break;
			default:
				printf("value = none\n");
				break;
				
		}
		printf(": value type %d %s\n", type, lua_typename(L, type));

//		data[key]=value;
			// 弹出 value 和拷贝的 key，留下原始的 key 作为下一次 lua_next 的参数
		lua_pop(L, 1);
			// 现在的栈：-1 => key; index => table
	}
	    // 现在的栈：index => table （最后 lua_next 返回 0 的时候它已经把上一次留下的 key 给弹出了）
	    // 所以栈已经恢复到进入这个函数时的状态
	return 0;
}

void test(lua_State *n)
{
	int type;
	lua_State *L;
	L = luaL_newstate();
	luaL_openlibs(L);
	
	lua_newtable(L);
	type = lua_type(L, -1);
	printf("%d: %d %s\n", -1, type, lua_typename(L, type));	
	
		//依次将各个元素放入table
	lua_pushnumber(L,1);
	lua_pushstring(L, "AA");
	lua_rawset(L,1);

	lua_pushnumber(L,2);
	lua_pushstring(L, "BB");
	lua_rawset(L,1);

	lua_pushnumber(L,3);
	lua_newtable(L);
	lua_pushstring(L, "CC");
	lua_rawseti(L,-2,1);

	lua_pushstring(L, "DD");
	lua_rawseti(L,-2,2);
	lua_rawset(L,1);

	lua_setglobal(L,"ta");
		//此时栈中为空，此处省略其他操作
		//将ta压入栈顶
	lua_getglobal(L, "ta");
		//获得第一个元素
	lua_rawgeti(L, 1,1);
	if(lua_type(L,-1) == LUA_TSTRING)
		printf("%s", lua_tostring(L,-1));
	lua_pop(L,1);

	lua_rawgeti(L, 1,2);
	if(lua_type(L,-1) == LUA_TSTRING)
		printf("%s", lua_tostring(L,-1));
	lua_pop(L,1);

	lua_rawgeti(L, 1,3);
	if(lua_type(L,-1) == LUA_TTABLE)
	{
			//因为第三个元素是table，所以继续获得它的第一个元素
		lua_rawgeti(L, -1,1);
		if(lua_type(L,-1) == LUA_TSTRING)
			printf("%s", lua_tostring(L,-1));
		lua_pop(L,1);

		lua_rawgeti(L, -1,2);
		if(lua_type(L,-1) == LUA_TSTRING)
			printf("%s", lua_tostring(L,-1));
		lua_pop(L,1);
	}
	lua_pop(L,1); //此时栈顶为ta
}

int main(int argc, char *argv[])
{
    lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, argv[1]);

//	lua_getglobal(L,"ScriptTable");

	int type;
	type = lua_type(L, -1);
	printf("%d: %d %s\n", -1, type, lua_typename(L, type));

//	lua_rawgeti(L, 1, 170000001);
//	type = lua_type(L, -1);
//	printf("%d: %d %s\n", -1, type, lua_typename(L, type));	
	
	traverse_table(L);
    return 0;
}

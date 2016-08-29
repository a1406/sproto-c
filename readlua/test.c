#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "../sproto.h"
#include "../sprotoc_common.h"
#include "test.h"

int traverse_table(lua_State *L, struct sproto_type *sproto_type)
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
				traverse_table(L, sproto_type);
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

int traverse_main_table(lua_State *L, struct sproto_type *sproto_type)
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

		double key = lua_tonumber(L, -1);
		printf("key = %d\n", (int)key);
		lua_pop(L, 1);

		int type = lua_type(L, -1);
		assert(type == LUA_TTABLE);

		traverse_table(L, sproto_type);
		
		lua_pop(L, 1);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	static char buf[4096];
	int fd = open("1.spb", O_RDONLY);
	size_t size =  read(fd, buf, sizeof(buf));
	struct sproto *sp = sproto_create(&buf[0], size);
	close(fd);

	struct sproto_type *type = sproto_type(sp, "ScriptTable");
	assert(type);
	
    lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, argv[1]);

	traverse_main_table(L, type);
    return 0;
}

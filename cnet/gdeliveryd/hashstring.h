#ifndef __HASHSTRING_H
#define __HASHSTRING_H

#include "octets.h"
#include <unordered_map>

namespace std 
{
	template<> 
	struct hash<GNET::Octets>
	{
		size_t operator() (const GNET::Octets &__os) const
		{
			const char *__p = (const char*)__os.begin();
			unsigned long __h = 0;
			for (int len = __os.size(); len-- > 0; )  __h = 5*__h + *__p++;
			return size_t(__h);
		}
		
	};
};

#endif

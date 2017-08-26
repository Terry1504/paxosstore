
/*
* Tencent is pleased to support the open source community by making PaxosStore available.
* Copyright (C) 2017 THL A29 Limited, a Tencent company. All rights reserved.
* Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
* https://opensource.org/licenses/BSD-3-Clause
* Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
*/



#ifndef CERTAIN_AUTOHELPER_H_
#define CERTAIN_AUTOHELPER_H_

#include "utils/Logger.h"
#include "utils/ObjReusedPool.h"
#include "utils/Random.h"

namespace Certain
{

#define NO_COPYING_ALLOWED(cls) \
	cls(const cls &); \
	void operator = (const cls &);

#define TYPE_GET_SET(type, name, tname) \
	const type &Get##name() { return m_##tname; } \
	void Set##name(const type &tname) { m_##tname = tname; }

#define UINT32_GET_SET(var) TYPE_GET_SET(uint32_t, var, i##var)

#define POINTER_GET_SET(type, name, tname) \
	type *Get##name() { return m_##tname; } \
	void Set##name(type *tname) { m_##tname = tname; }

#define BOOLEN_IS_SET(name) \
	const bool &Is##name() { return m_b##name; } \
	void Set##name(const bool &b##name) { m_b##name = b##name; }

#define TEST_CASE_FINISH() \
	fprintf(stderr, "%s finished!\n", __FUNCTION__);

#define RETURN_RANDOM_ERROR_WHEN_IN_DEBUG_MODE() \
	static __thread clsRandom *__poRandom = new clsRandom(uint32_t(pthread_self())); \
	if (__poRandom->Next() % 10 == 0) { \
		return -7999; \
	}

template<typename Type>
class clsAutoDelete
{
private:
	Type *m_pType;

public:
	clsAutoDelete(Type *pType) : m_pType(pType) { }
	~clsAutoDelete() { delete m_pType, m_pType = NULL; }
};

template<typename Type>
class clsAutoMultiDelete
{
private:
	vector<Type *> m_vecType;

public:
	void Add(Type *pType)
	{
		m_vecType.push_back(pType);
	}
	~clsAutoMultiDelete()
	{
		for (auto iter = m_vecType.rbegin(); iter != m_vecType.rend(); ++iter)
		{
			if (*iter != NULL)
			{
				delete *iter, *iter = NULL;
			}
		}
	}
};

template<typename Type>
class clsAutoFreeObjPtr
{
private:
	Type *m_pType;
	clsObjReusedPool<Type> *m_poPool;

public:
	clsAutoFreeObjPtr(Type *pType, clsObjReusedPool<Type> *poPool)
			: m_pType(pType), m_poPool(poPool) { }

	~clsAutoFreeObjPtr()
	{
		if (m_poPool != NULL)
		{
			m_poPool->FreeObjPtr(m_pType);
		}
		else
		{
			delete m_pType, m_pType = NULL;
		}
	}
};

} // namespace Certain

#endif // CERTAIN_AUTOHELPER_H_
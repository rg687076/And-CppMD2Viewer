#pragma warning(disable : 4819)
//
// Created by jun on 2021/06/19.
//
#ifndef ANDCG3DVIEWER_CG3DCOM_H
#define ANDCG3DVIEWER_CG3DCOM_H

#ifdef __ANDROID__
#else /*__ANDROID__*/
#include <Windows.h>
#endif /*__ANDROID__*/
#include <string>
#include <sstream>

#ifndef __ANDROID__
#define ANDROID_LOG_DEBUG 0
#define	__android_log_print(m, n, fmt, ...)	{CG3D::TRACE(fmt, __VA_ARGS__) ; CG3D::TRACE("\n") ;}
#define	__PRETTY_FUNCTION__	__FUNCTION__
#endif /*__ANDROID__*/

namespace CG3D {
	inline void format_internal(std::stringstream &sout) {
	}
	
	/* template関数なので実装もヘッダに書く */
	template<typename First, typename... Rest>
	void format_internal(std::stringstream &sout, const First& first, const Rest&... rest)
	{
		sout << first;
		format_internal(sout, rest...);
	}
	
	/* template関数なので実装もヘッダに書く */
	template<typename... Args>
	std::string format(const Args&... args)
	{
		std::stringstream sout;
		format_internal(sout, args...);
		return sout.str();
	}

	#ifndef __ANDROID__
	void TRACE(LPCSTR pszFormat, ...);
	#endif /*__ANDROID__*/
}	/* namespace CG3D */

#endif //ANDCG3DVIEWER_CG3DCOM_H

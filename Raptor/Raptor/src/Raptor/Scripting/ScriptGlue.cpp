#include "rtpch.h"
#include "ScriptGlue.h"

#include "mono/metadata/object.h"

namespace Raptor {

#define RT_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Raptor.InrenalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int value)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);

		std::cout << str << value << "\n";
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		RT_CORE_WARN(*parameter);

		*outResult = glm::normalize(*parameter);
	}
	
	void ScriptGiue::RegisterFunctions()
	{
		RT_ADD_INTERNAL_CALL(NativeLog);
		RT_ADD_INTERNAL_CALL(NativeLog_Vector);
	}
}

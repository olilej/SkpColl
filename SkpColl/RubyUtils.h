#pragma once

#include <string>
typedef unsigned long VALUE;
typedef unsigned long ID;

class CRubyUtils
{
public:

    //static bool EvaluateRubyExpression(LPCTSTR expr, VALUE* pResult = NULL);
    static int GetRubyInt(VALUE obj);
    static bool GetRubyBool(VALUE obj);
    static double GetRubyDouble(VALUE obj);
    static void RegisterRubyObject(VALUE obj);
    static void UnregisterRubyObject(VALUE obj);
    //static CString GetRubyString(VALUE obj);
	static std::string GetRubystdString(VALUE obj);
    static bool IsRubyString(VALUE arg);
    static bool IsRubyInteger(VALUE arg);
    static bool IsRubyFloat(VALUE arg);
    static VALUE protect_funcall(VALUE obj, ID func, int argc, ...);

	static VALUE GetDAttribute(VALUE, VALUE);

	static bool IsComponentInstance(VALUE);
	static bool IsGroup(VALUE);
	static bool IsFace(VALUE);
	static bool IsEdge(VALUE);

private:
    CRubyUtils(void);
    ~CRubyUtils(void);

    static VALUE GetRubyObjectArray();

    static VALUE s_arrayOfRubyObject;
};

// The signature of a function passed to rb_protect
#define PROTECTFUNC(f) ((VALUE (*)(VALUE)) f)

#ifdef __cplusplus
//#  ifndef RUBY_METHOD_FUNC /* These definitions should work for Ruby 1.4.6 */
//#    define VALUEFUNC(f) ((VALUE (*)()) f)
//#    define VOIDFUNC(f)  ((void (*)()) f)
//#  else
//#    ifndef ANYARGS /* These definitions should work for Ruby 1.6 */
//#      define VALUEFUNC(f) ((VALUE (*)()) f)
//#      define VOIDFUNC(f)  ((RUBY_DATA_FUNC) f)
//#    else /* These definitions should work for Ruby 1.7 */
#      define VALUEFUNC(f) ((VALUE (*)(ANYARGS)) f)
#      define VOIDFUNC(f)  ((RUBY_DATA_FUNC) f)
//#    endif
//#  endif
//#else
//#  define VALUEFUNC(f) (f)
//#  define VOIDFUNC(f) (f)
#endif

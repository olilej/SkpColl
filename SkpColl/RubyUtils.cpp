
#include "StdAfx.h"
#include "rubyutils.h"
#include "ruby.h"

CRubyUtils::CRubyUtils(void)
{
}

CRubyUtils::~CRubyUtils(void)
{
}

CString CRubyUtils::GetRubyString(VALUE obj)
{
    char* name = RSTRING(obj)->ptr;
    CString str = CA2T(name);
    return str;
}

std::string CRubyUtils::GetRubystdString(VALUE obj)
{
    char* name = RSTRING(obj)->ptr;
    std::string str(name);
    return str;
}

VALUE CRubyUtils::GetDAttribute(VALUE obj, VALUE att)
{
    static VALUE getatt = rb_intern("get_attribute");
    static VALUE dicatt = rb_str_new2("dynamic_attributes");
	return rb_funcall(obj, getatt, 2, dicatt, att);
}

// Evaluate a string in Ruby and optionally return the results.
// Returns true if the expression was evaluated OK
// If pResult != NULL, then the result of evaluating the expression
// will be returned there.
bool CRubyUtils::EvaluateRubyExpression(LPCTSTR expr, VALUE* pResult)
{
    if( pResult ) *pResult = Qnil;
    int state = 0;
    VALUE val = rb_eval_string_protect(CT2A(expr), &state);
    if( 0 == state && pResult )
    {
        *pResult = val;
    }

    return (state == 0);
}

//=============================================================================

// These function are used to keep track of Ruby objects that we are keeping
// a reference to in some C++ code so that the garbage collector will know
// about them.

// This is an array of Ruby objects that we want to protect from
// garbage collection.  We will create this as a global object in Ruby
// and then just add objects that we need to keep to it.
VALUE CRubyUtils::s_arrayOfRubyObject = 0;

VALUE CRubyUtils::GetRubyObjectArray()
{
    if( 0 == s_arrayOfRubyObject )
    {
        // Create the Array and make it a global object
        s_arrayOfRubyObject = rb_ary_new();
        rb_global_variable(&s_arrayOfRubyObject);
    }
    return s_arrayOfRubyObject;
}

void CRubyUtils::RegisterRubyObject(VALUE obj)
{
    if( 0 == obj ) return;
    if( Qnil == obj ) return;
    // Add the object to the global array of objects.
    VALUE objArray = GetRubyObjectArray();
    if( objArray )
    {
        //printf("RegisterRubyObject: %u at %d\n", obj, RARRAY(objArray)->len);
        rb_ary_push(objArray, obj);
    }
}

void CRubyUtils::UnregisterRubyObject(VALUE obj)
{
    if( 0 == obj ) return;
    if( Qnil == obj ) return;
    // Remove the object from the global array of objects.
    VALUE objArray = GetRubyObjectArray();
    if( objArray )
    {
        // Look it up by doing a comparison on the object value
        long numberOfObjects = RARRAY(objArray)->len;
        for(long i=0; i<numberOfObjects; i++)
        {
            if( obj != RARRAY(objArray)->ptr[i] ) continue;
            //printf("UnregisterRubyObject %u at %d\n", obj, i);
            rb_ary_delete_at(objArray, i);
            break;
        }
    }
}
// Function for wrap the call to rb_funcall2 so that we will not exit
// SketchUp if there is an error in the Ruby code that we are calling
static VALUE wrap_funcall(VALUE data)
{
    // The data is all wrapped in a single Runy array
    if( !(TYPE(data) == T_ARRAY) ) return Qnil;
    int argc = RARRAY(data)->len;
    if( argc < 2 ) return Qnil;

    VALUE* argv = RARRAY(data)->ptr;
    VALUE obj = argv[0];
    ID func = (ID)NUM2LONG(argv[1]);

    argc -= 2;
    argv += 2;

    return rb_funcall2(obj, func, argc, argv);
}

VALUE CRubyUtils::protect_funcall(VALUE obj, ID func, int argc, ...)
{
    // First check to see if we have a valid object, and if it responds
    // to the desired method
    if( obj == Qnil ) return Qnil;
    if( !rb_respond_to(obj, func) ) return Qnil;

    // rb_protect takes a pointer to a function that only takes a single
    // argument, so we need to package up all of the arguments into
    // a single array.
    // The array will contain the object that the function is called on,
    // the ID of the method to call,
    // and the arguments to pass to the method.

    // Allocate the array and push on the object and the method ID
    VALUE data = rb_ary_new2(argc + 2);
    rb_ary_push(data, obj);
    rb_ary_push(data, LONG2NUM(func));

    // Now push on all of the arguments
    if( argc > 0 )
    {
        va_list ar;
        va_start(ar, argc);
        for(int i=0; i<argc; i++)
        {
            rb_ary_push(data, va_arg(ar, VALUE));
        }
        va_end(ar);
    }

    // Now call the wrapped function inside a protected context
    int error;
    VALUE result = rb_protect(PROTECTFUNC(wrap_funcall), data, &error);

    return result;
}

bool CRubyUtils::GetRubyBool(VALUE obj)
{
    return RTEST(obj);
}

int CRubyUtils::GetRubyInt(VALUE obj)
{
    return NUM2LONG(obj);
}

bool CRubyUtils::IsComponentInstance(VALUE obj)
{
	return Qtrue == rb_obj_is_kind_of(obj, rb_path2class("Sketchup::ComponentInstance"));
}

bool CRubyUtils::IsGroup(VALUE obj)
{
	return Qtrue == rb_obj_is_kind_of(obj, rb_path2class("Sketchup::Group"));
}

bool CRubyUtils::IsFace(VALUE obj)
{
	return Qtrue == rb_obj_is_kind_of(obj, rb_path2class("Sketchup::Face"));
}

bool CRubyUtils::IsEdge(VALUE obj)
{
	return Qtrue == rb_obj_is_kind_of(obj, rb_path2class("Sketchup::Edge"));
}


double CRubyUtils::GetRubyDouble(VALUE obj)
{
    return NUM2DBL(obj);
}

bool CRubyUtils::IsRubyString(VALUE arg)
{
    return TYPE(arg) == T_STRING;
}

bool CRubyUtils::IsRubyInteger(VALUE arg)
{
    return rb_obj_is_kind_of(arg, rb_cInteger) != 0;
}

bool CRubyUtils::IsRubyFloat(VALUE arg)
{
    return rb_obj_is_kind_of(arg, rb_cFloat) != 0;
}
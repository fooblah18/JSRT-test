#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <jsrt.h>

#include <iostream>

using namespace std;

JsValueRef CALLBACK     PrintFormat     (JsValueRef         callee              ,
                                         bool               isConstructCall     ,
                                         JsValueRef*        arguments           ,
                                         unsigned short     argumentCount       ,
                                         void*              callbackState       )
{
    cout << "this is not happening! D:" << endl;

    return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK     Something       (JsValueRef         callee              ,
                                         bool               isConstructCall     ,
                                         JsValueRef*        arugments           ,
                                         unsigned short     argumentCount       ,
                                         void*              callbackState       )
{
    cout << "hello world!" << endl;

    return JS_INVALID_REFERENCE;
}

void                    main            (void                                   )
{
    JsRuntimeHandle     js_handle;
    JsContextRef        js_context;

    auto jscr   = JsCreateRuntime       (JsRuntimeAttributeNone, JsRuntimeVersion11, nullptr, &js_handle);

    if (jscr != JsNoError)
    {
        return;
    }

    auto jscc   = JsCreateContext       (js_handle, nullptr, &js_context);

    if (jscc != JsNoError)
    {
        return;
    }

    auto jssc   = JsSetCurrentContext   (js_context);

    if (jssc != JsNoError)
    {
        return;
    }

    JsValueRef                  js_global;
    JsGetGlobalObject           (&js_global);

    JsPropertyIdRef             js_native_property;
    JsGetPropertyIdFromName     (L"native", &js_native_property);

    JsValueRef                  js_native_object;
    JsCreateObject              (&js_native_object);

    JsPropertyIdRef             js_printf_prop;
    JsGetPropertyIdFromName     (L"printf", &js_printf_prop);

    JsValueRef                  js_function_prop;
    JsCreateFunction            (PrintFormat, nullptr, &js_function_prop);
    
    JsPropertyIdRef             js_something_prop;
    JsGetPropertyIdFromName     (L"something", &js_something_prop);

    JsValueRef                  js_something_func;
    JsCreateFunction            (Something, nullptr, &js_something_func);

    JsPropertyIdRef             js_another_prop;
    JsGetPropertyIdFromName     (L"another", &js_another_prop);

    JsPropertyIdRef             js_finally_prop;
    JsGetPropertyIdFromName     (L"finalli", &js_finally_prop);

    JsValueRef                  js_another_func;
    JsCreateObject              (&js_another_func);

    JsValueRef                  js_finally_func;
    JsCreateObject              (&js_finally_func);

    JsSetProperty               (js_native_object   , js_printf_prop        , js_function_prop  , true);
    JsSetProperty               (js_native_object   , js_something_prop     , js_something_func , true);
    JsSetProperty               (js_native_object   , js_another_prop       , js_another_func   , true);
    JsSetProperty               (js_native_object   , js_finally_prop       , js_finally_func   , true);

    JsSetProperty               (js_global          , js_native_property    , js_native_object  , true);
    
    JsSourceContext             cookie = 0;

    JsValueRef                  result;
    JsRunScript                 (L"function foo() { native.something(); native.printf(); native.printf(); }", cookie, L"source", &result);
    
    JsPropertyIdRef             js_foo_prop;
    JsGetPropertyIdFromName     (L"foo", &js_foo_prop);

    JsValueRef                  js_foo_func;
    JsGetProperty               (js_global, js_foo_prop, &js_foo_func);

    JsCallFunction              (js_foo_func, nullptr, 0, nullptr);
}

#include "chai_stable.h"

#include <QtCore>
#include <QtNetwork>

#include <stdio.h>
#include <stdlib.h>

#include <bitset>
#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

void log(const std::string &msg)
{
    std::cout << "[" << time(nullptr) << "] " << msg << '\n';
}

void log(const std::string &module, const std::string &msg)
{
    std::cout << "[" << time(nullptr) << "] <" << module << "> " << msg << '\n';
}

void bound_log(const std::string &msg)
{
    log(msg);
}

void hello_world(const chaiscript::Boxed_Value & /*o*/)
{
    std::cout << "Hello World\n";
}

void hello_constructor(const chaiscript::Boxed_Value & /*o*/)
{
    std::cout << "Hello Constructor\n";
}


struct System
{
    std::map<std::string, std::function<std::string (const std::string &) > > m_callbacks;

    void add_callback(const std::string &t_name,
                      const std::function<std::string (const std::string &)> &t_func)
    {
        m_callbacks[t_name] = t_func;
    }


    void do_callbacks(const std::string &inp)
    {
        log("Running Callbacks: " + inp);
        for (std::map<std::string, std::function<std::string (const std::string &)> >::iterator itr = m_callbacks.begin();
             itr != m_callbacks.end();
             ++itr)
        {
            log("Callback: " + itr->first, itr->second(inp));
        }
    }
};

void take_shared_ptr(const std::shared_ptr<const std::string> &p)
{
    std::cout << *p << '\n';
}

QString JX9_PROG = R"***(
print(11+22)
print(add2(6, 7))
)***";

// (1) https://codedocs.xyz/ChaiScript/ChaiScript/index.html
double function(int i, double j)
{
  return i * j;
}

// (6) https://codedocs.xyz/ChaiScript/ChaiScript/index.html
class Test
{
  public:
    void function() {}
    std::string function2() { return "Function2"; }
    void function3() {}
    std::string functionOverload(double) { return "double"; }
    std::string functionOverload(int) { return "int"; }
};

int main(int argc, char *argv[]) {

    QCoreApplication a(argc, argv);
    qDebug().noquote() << JX9_PROG.trimmed();

    //using namespace chaiscript;

    //(1) https://codedocs.xyz/ChaiScript/ChaiScript/index.html
    chaiscript::ChaiScript chai;
    chai.add(chaiscript::fun(&function), "function");
    double d = chai.eval<double>("function(3, 4.75);");
    qDebug() << "(1)" << d;

    //(2) https://codedocs.xyz/ChaiScript/ChaiScript/index.html
    chai(R"***(print("(2) hello world") )***");

    //(3) https://codedocs.xyz/ChaiScript/ChaiScript/index.html
    int result = chai.eval<int>("1 + 3");
    qDebug() << "(3)" << result;

    {
        using namespace chaiscript;
        int i = 5;
        chai.add(var(i), "i");
        printf("(4) "); // https://codedocs.xyz/ChaiScript/ChaiScript/index.html
        chai(R"***( print(i) )***");
    }

    {
        using namespace chaiscript;
        int i = 5;
        printf("(5) "); // https://codedocs.xyz/ChaiScript/ChaiScript/index.html
        chai.add(const_var(i), "i");
        try {
          chai("i = 5"); // exception throw, cannot assign const var
        } catch(std::exception &e) {
            qDebug() << e.what();
        }
    }

    {
        using namespace chaiscript;
        printf("(6) "); // https://codedocs.xyz/ChaiScript/ChaiScript/index.html
        chaiscript::ModulePtr m = chaiscript::ModulePtr(new chaiscript::Module());
        chaiscript::utility::add_class<Test>(*m, "Test",
        { constructor<Test()>(),
          constructor<Test(const Test &)>() },
        { {fun(&Test::function), "function"},
          {fun(&Test::function2), "function2"},
          {fun(&Test::function2), "function3"} });
        chai.add(m);
        chai.eval(R"***( var ex6 = Test(); print(ex6.function2()) )***");
    }




    //Create a new system object and share it with the chaiscript engine
    System system;
    chai.add_global(chaiscript::var(&system), "system");

    //Add a bound callback method
    chai.add(chaiscript::fun(&System::add_callback, std::ref(system)), "add_callback_bound");

    //Register the two methods of the System structure.
    chai.add(chaiscript::fun(&System::add_callback), "add_callback");
    chai.add(chaiscript::fun(&System::do_callbacks), "do_callbacks");

    chai.add(chaiscript::fun(&take_shared_ptr), "take_shared_ptr");

    // Let's use chaiscript to add a new lambda callback to our system.
    // The function "{ 'Callback1' + x }" is created in chaiscript and passed into our C++ application
    // in the "add_callback" function of struct System the chaiscript function is converted into a
    // std::function, so it can be handled and called easily and type-safely
    chai.eval("system.add_callback(\"#1\", fun(x) { \"Callback1 \" + x });");

    // Because we are sharing the "system" object with the chaiscript engine we have equal
    // access to it both from within chaiscript and from C++ code
    system.do_callbacks("TestString");
    chai.eval("system.do_callbacks(\"TestString\");");

    // The log function is overloaded, therefore we have to give the C++ compiler a hint as to which
    // version we want to register. One way to do this is to create a typedef of the function pointer
    // then cast your function to that typedef.
    typedef void (*PlainLog)(const std::string &);
    typedef void (*ModuleLog)(const std::string &, const std::string &);
    chai.add(chaiscript::fun(PlainLog(&log)), "log");
    chai.add(chaiscript::fun(ModuleLog(&log)), "log");

    chai.eval("log(\"Test Message\")");

    // A shortcut to using eval is just to use the chai operator()
    chai("log(\"Test Module\", \"Test Message\");");

    //Finally, it is possible to register a lambda as a system function, in this
    //way, we can, for instance add a bound member function to the system
    chai.add(chaiscript::fun([&system](){ return system.do_callbacks("Bound Test"); }), "do_callbacks");

    //Call bound version of do_callbacks
    chai("do_callbacks()");

    std::function<void ()> caller = chai.eval<std::function<void ()> >("fun() { system.do_callbacks(\"From Functor\"); }");
    caller();


    //If we would like a type-safe return value from all call, we can use
    //the templated version of eval:
    int i = chai.eval<int>("5+5");

    std::cout << "5+5: " << i << '\n';

    //Add a new variable
    chai("var scripti = 15");

    //We can even get a handle to the variables in the system
    int &scripti = chai.eval<int &>("scripti");

    std::cout << "scripti: " << scripti << '\n';
    scripti *= 2;
    std::cout << "scripti (updated): " << scripti << '\n';
    chai("print(\"Scripti from chai: \" + to_string(scripti))");

    //To do: Add examples of handling Boxed_Values directly when needed

    //Creating a functor on the stack and using it immediately
    int x = chai.eval<std::function<int (int, int)> >("fun (x, y) { return x + y; }")(5, 6);

    std::stringstream ss;
    ss << x;
    log("Functor test output", ss.str());

    chai.add(chaiscript::var(std::shared_ptr<int>()), "nullvar");
    chai("print(\"This should be true.\"); print(nullvar.is_var_null())");

    // test the global const action
    chai.add_global_const(chaiscript::const_var(1), "constvar");
    chai("def getvar() { return constvar; }");
    chai("print( getvar() )");


    //Ability to create our own container types when needed. std::vector and std::map are
    //mostly supported currently
    chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<int> >("IntVector"));


    // Test ability to register a function that excepts a shared_ptr version of a type
    chai("take_shared_ptr(\"Hello World as a shared_ptr\");");

    chai.add(chaiscript::fun(&bound_log, std::string("Msg")), "BoundFun");

    //Dynamic objects test
    chai.add(chaiscript::Proxy_Function(new chaiscript::dispatch::detail::Dynamic_Object_Function("TestType", chaiscript::fun(&hello_world))), "hello_world");
    chai.add(chaiscript::Proxy_Function(new chaiscript::dispatch::detail::Dynamic_Object_Constructor("TestType", chaiscript::fun(&hello_constructor))), "TestType");

    chai.eval("var x = TestType()");
    chai.eval("x.hello_world()");
}


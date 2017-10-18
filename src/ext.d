import dmdscript.program;
import dmdscript.script;
import dmdscript.extending;

import std.stdio;
import std.typecons;
import std.stdio;

int func(int a,int b){ return a*b;  }
int add2d(int a, int b){ return a+b;  }
string sfunc(string s) { return "<" ~ s ~ ">"; }
                   
struct A{
  int magic;
  this(int m){ magic = m; }
  void f(string a){ 
    writeln(magic,":",a);
  }
  void add2x(int a, int b) {
    //return a + b;
    writeln(a);
    writeln(b);
  }
}
                         

void mainX(){
    Program p = new Program;
    extendGlobal!func(p,"mul");
    extendGlobal!add2d(p,"add2d");
    extendGlobal!sfunc(p,"sfunc");
    //Wrap!(A,"A").methods!(A.f)();
    //Wrap!(A,"A").methods!(A.add2x)();
    Wrap!(A,"A") x;
    //Wrap!(A,"A").methods!(A.f)();
    x.methods!(A.f)();
    x.methods!(A.add2x)();
    
    auto src  = cast(string)std.file.read("ext.ds");
    p.compile("TestExtending",src,null);
    p.execute(null);    
}

unittest {
  mainX();
}

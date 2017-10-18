module sub;

//import std.stdio;
import core.stdc.stdio;
import core.stdc.stdlib;
import core.stdc.string;

import dmdscript.script;
import dmdscript.program;
import dmdscript.errmsgs;

int add2(int a, int b) { return a + b; }

unittest {
  printf("[stdout]\n");
  assert(1==1);
  assert(3==add2(1,2));
  assert(1==1);
}


unittest {
  assert(3==add2(1,2));
  Program program;
  tchar[] buffer;
  program = new Program();
  program.compile("test.ds", "print(1+22); print('\\n')", null);
  program.execute(null);
  program.compile("test.ds", "println(31+22); print('\\n')", null);
  program.execute(null);
}

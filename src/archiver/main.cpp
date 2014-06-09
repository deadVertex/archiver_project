//#include <boost/python.hpp>
#include <python/Python.h>

int main( int argc, char **argv )
{
  // TODO: Setup python.
  Py_Initialize();
  PyRun_SimpleString( "import sys\n" );
  PyRun_SimpleString( "sys.stdout.write( \"Hello from an embedded Python Script\")\n" );
  Py_Finalize();
  //boost::python::exec( "print(\"Swiggity Swooty\")" );
  getchar();

  return 0;
}
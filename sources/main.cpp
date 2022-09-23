#include <core/print.hpp>
#include <core/os/directory.hpp>
#include "application.hpp"

int main(){
	
	Directory::Change("../../../");
	Application().Run();
}

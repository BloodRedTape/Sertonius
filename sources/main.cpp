#include <core/print.hpp>
#include <core/os/directory.hpp>
#include <graphics/api/gpu.hpp>
#include "application.hpp"
#include "sertonius/sertonius_game_mode.hpp"

int main(){
#ifdef SX_OS_WINDOWS
	Directory::Change("../../../");
#endif
	GPU::ForceInit();

	if (!GPU::IsComputePipelineSupported())
		return (Println("compute is not supported"), EXIT_FAILURE);

	SertoniusGameMode mode;
	Application(&mode).Run();
}

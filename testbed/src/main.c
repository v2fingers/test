#include "engine.h"

int main(void) {
	Engine engine;
	engine_init(&engine);
	engine_run(&engine);
	engine_shutdown(&engine);
	return 0;
}

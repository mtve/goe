extern "C" {
#include "picosat.h"
}

class SAT {
	PicoSAT			*sat;

public:
	SAT () : sat (picosat_init ()) { picosat_set_verbosity (sat, 2); }

	~SAT () { picosat_reset (sat); }

	void
	add_lit (int lit) { picosat_add (sat, lit); }

	void
	assume (int lit) { picosat_assume (sat, lit); }

	int
	solve () {
		int ret = picosat_sat (sat, -1);
		return	ret == PICOSAT_SATISFIABLE ? 1 :
			ret == PICOSAT_UNSATISFIABLE ? 0 : -1;
	}

	int
	get_sol (int var) { return picosat_deref (sat, var); }
};

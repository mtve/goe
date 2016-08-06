#include <cryptominisat5/cryptominisat.h>
#include <vector>
#include "solverconf.h"

class SAT {
	CMSat::SolverConf	conf;
	CMSat::SATSolver	solver;
	std::vector<CMSat::Lit>	clause,
				assumptions;

	void *
	Conf () { conf.verbosity = 2; return (void *)&conf; }

	CMSat::Lit
	Lit (int lit) { return CMSat::Lit (abs (lit) - 1, lit > 0); }

	int
	Bool (CMSat::lbool b) {
		return	b == CMSat::l_True ? 1 :
			b == CMSat::l_False ? 0 : -1;
	}
public:
	SAT () : solver (Conf ()) { if (0) solver.set_num_threads (16); }

	void
	add_lit (int lit) {
		if (lit) {
			int add = abs (lit) - solver.nVars ();
			if (add > 0)
				solver.new_vars (add);
			clause.push_back (Lit (lit));
		} else {
			solver.add_clause (clause);
			clause.clear ();
		}
	}

	void
	assume (int lit) { assumptions.push_back (Lit (lit)); }

	int
	solve () {
		int ret = Bool (solver.solve (&assumptions));
		assumptions.clear ();
		return ret;
	}

	int
	get_sol (int var) { return Bool (solver.get_model ()[var - 1]); }
};

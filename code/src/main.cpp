//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#include <iostream>
#include <vector>
#include <goc/goc.h>

#include "preprocess_travel_times.h"
#include "vrp_instance.h"
#include "ttbf.h"
#include "cttbf.h"
#include "gcs.h"

using namespace std;
using namespace goc;
using namespace nlohmann;
using namespace euroalio2018;

// Converts the VRP instance to a time independent one (t_ij = max(\tau_ij(t') : t' \in [0, T]) forall ij \in A).
// Solves the TI model with a small time limit and gets the BKS as the heuristic solution.
bool heuristic_solution(const VRPInstance& vrp, const vector<double>& profits,
						Route* solution, BCExecutionLog* execution_log)
{
	// Convert VRP instance to TI.
	VRPInstance vrp_ti = vrp;
	for (Arc e: vrp.D.Arcs())
	{
		Vertex i = e.tail, j = e.head;
		vrp_ti.tau[i][j] = PWLFunction();
		vrp_ti.tau[i][j].AddPiece(LinearFunction(Point2D(0.0, vrp.tau[i][j].Image().right), Point2D(vrp.T, vrp.tau[i][j].Image().right)));
	}
	
	// Solve model.
	CTTBF model(vrp_ti, profits);
	BCSolver solver;
	solver.time_limit = 5.0_sec;
	*execution_log = solver.Solve(model.f, {BCOption::RootInformation, BCOption::CutInformation, BCOption::BestIntSolution, BCOption::ScreenOutput});
	if (execution_log->best_int_solution.IsSet())
	{
		*solution = model.ParseSolution(execution_log->best_int_solution);
		solution->duration = vrp.ReadyTime(solution->path, solution->t0) - solution->t0; // Update with TD duration.
	}
	return execution_log->best_int_solution.IsSet();
}

// The experiment has the following parameters:
//	- model_type: string (formulation type)
//		* "ttbf": full formulation
//		* "cttbf": compact formulation.
//	- time_limit: number (time limit in seconds)
int main()
{
	json output; // STDOUT output will go into this JSON.
	
	simulate_input_in_debug("instances/euroalio2018", "C101_25_a", "experiments/comparison.json", "TTBF");
	
	json experiment, instance, solutions;
	cin >> experiment >> instance >> solutions;
	
	// Parse instance.
	preprocess_travel_times(instance);
	VRPInstance vrp = instance;
	vector<ProfitUnit> profits = instance["profits"];
	
	// Parse experiment.
	string model_type = value_or_default(experiment, "model_type", "ttbf");
	Duration time_limit = Duration(value_or_default(experiment, "time_limit", 7200), DurationUnit::Seconds);
	
	clog << "Model type: " << model_type << endl;
	clog << "Time limit: " << time_limit << "sec." << endl;
	
	// Create model.
	Model* model = model_type == "ttbf" ? (Model*)new TTBF(vrp, profits) : (Model*)new CTTBF(vrp, profits);
	
	// Create solver.
	BCSolver solver;
	solver.screen_output = &clog; // Show output in clog.
	solver.time_limit = time_limit; // Set time limit.
	solver.config = {{"CPXPARAM_Parallel", 1}, {"CPXPARAM_Threads", 8}, {"CPXPARAM_MIP_Strategy_StartAlgorithm", 2}, {"CPXPARAM_LPMethod", 2}}; // Set CPLEX config parameters.
	
	// Add GCS cuts.
	GCS gcs(vrp, model->x, model->y);
	solver.separation_strategy.AddFamily("gcs");
	solver.separation_strategy.SetSeparationRoutine("gcs", &gcs);
	solver.separation_strategy.node_limit["gcs"] = 1; // Only in root node.
	
	// Execute initial heuristic.
	clog << "Executing initial heuristic" << endl;
	Route heuristic_route;
	BCExecutionLog heuristic_log;
	if (heuristic_solution(vrp, profits, &heuristic_route, &heuristic_log))
	{
		solver.initial_solutions.push_back(model->SerializeSolution(heuristic_route));
		output["Heuristic solution"] = VRPSolution(model->f->EvaluateValuation(model->SerializeSolution(heuristic_route)), {heuristic_route});
	}
	
	// Solve the model.
	auto log = solver.Solve(model->f, {BCOption::BestIntSolution, BCOption::CutInformation, BCOption::RootInformation});
	
	// Show results.
	clog << "Status: " << log.status << endl;
	if (log.best_int_solution.IsSet())
	{
		clog << "Best int: " << log.best_int_value << endl;
		Route r = model->ParseSolution(log.best_int_solution);
		clog << "Best route: " << r.path << ", departing at: " << r.t0 << ", duration: " << r.duration << endl;
		output["Best solution"] = VRPSolution(log.best_int_value, {r});
	}
	
	// Send JSON output to cout.
	output["Heuristic"] = heuristic_log;
	output["Exact"] = log;
	cout << output << endl;
	return 0;
}
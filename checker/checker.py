import sys, os, json, datetime, os, argparse

CHECKER_DIR = os.path.abspath(os.path.dirname(__file__)) # Directory where checker files are located.
CURRENT_DIR = os.path.abspath(os.getcwd()) # Current directory in the command line.
INSTANCES_DIR = os.path.abspath(os.path.join(CHECKER_DIR, "..", "instances/euroalio2018/")) # Directory where instances are stored.

# Util functions
# Returns: JSON content of the file at the specified path.
def read_json_from_file(file_path):
	with open(file_path, "r") as f:
		return json.loads(f.read())

# Returns: text in color blue for console.
def blue(text): return "\033[94m" + text + "\033[0m"
# Returns: text in color red for console.
def red(text): return "\033[91m" + text + "\033[0m"
# Returns: text in color green for console.
def green(text): return "\033[92m" + text + "\033[0m"
# Returns: text in color purple for console.
def purple(text): return "\033[95m" + text + "\033[0m"

INFTY = 10e8; EPS = 10e-6
def epsilon_equal(a, b): return abs(a - b) < EPS
def epsilon_bigger(a, b): return a > b + EPS

# Set command line parameters.
arg_parser = argparse.ArgumentParser(description="Check if the solutions in the output file are correct.")
arg_parser.add_argument("output_files", metavar="OUT_FILE", help="JSON output file(s) to run the checker on.", type=argparse.FileType('r'), nargs='+')

# Read command line parameters.
args = vars(arg_parser.parse_args())
output_files = args["output_files"]

# Show parameters.
print(blue(F"Output files: {[e.name for e in output_files]}"))

# Read dataset index file.
index = read_json_from_file(F"{INSTANCES_DIR}/index.json")

def read_instance(dataset_name, instance_name):
	for entry in index:
		if entry["instance_name"] == instance_name:
			return read_json_from_file(F"{INSTANCES_DIR}/{entry['file_name']}")

	raise "Instance not found"

# Returns: the best known solution (with minimum value) among all with the specific tags.
def best_known_solution(dataset_name, instance_name):
	solutions = read_json_from_file(F"{INSTANCES_DIR}/solutions.json")
	bks = {"value":INFTY}
	for s in solutions:
		if s["instance_name"] == instance_name and s["value"] <= bks["value"]:
			bks = s
	return bks

# Returns: the travel time of arc (i, j) if departing at t0.
def travel_time(instance, i, j, t0):
    c = instance["clusters"][i][j]
    T = instance["speed_zones"]
    v = instance["cluster_speeds"]

    # Find speed slot T_k that includes t0.
    k = min(i for i in range(0,len(T)) if t0 >= T[i][0] and t0 <= T[i][1])

    # Travel time algorithm from Ichoua et al.
    t = t0
    d = instance["distances"][i][j]
    tt = t + d / v[c][k]
    while tt > T[k][1]:
        d = d - v[c][k] * (T[k][1] - t)
        t = T[k][1]
        if epsilon_equal(d, 0): break
        if epsilon_bigger(d, 0) and k+1 == len(T): return INFTY
        tt = t + d / v[c][k+1]
        k = k + 1
    return tt - t0

# Returns: the time when the path finishes if departing at t0.
# If the route is infeasible it returns INFTY.
def ready_time(instance, path, t0):
	n = instance["digraph"]["vertex_count"] # Number of vertices.
	T = instance["horizon"][1] # End of horizon.
	s = instance["service_times"] if "service_times" in instance else [0 for i in range(0, n)] # Service times
	tw = instance["time_windows"] if "time_windows" in instance else [[0, T] for i in range(0, n)] # Time windows
	a = [tw[i][0] for i in range(0, n)] # Release times
	b = [tw[i][1] for i in range(0, n)] # Deadline times
	d = instance["demands"] if "demands" in instance else [0 for i in range(0, n)] # Customer demand
	Q = instance["capacity"] if "capacity" in instance else 1 # Vehicle capacity

	# Check path elementarity.
	if len(path) != len(set(path)): return INFTY

	# Calculate ready time.
	t = max(t0, a[path[0]])+s[path[0]]
	q = d[path[0]]
	for k in range(0, len(path)-1):
		i = path[k] 
		j = path[k+1]
		q += d[j]
		t = max(t+travel_time(instance, i, j, t), a[j]) + s[j]
		if epsilon_bigger(q, Q): return INFTY
		if epsilon_bigger(t, b[j]): return INFTY
	return t

# Returns: true if the routes are valid.
# If routes are not valid, then it adds the error messages to error_messages
def check_routes(instance, solution, error_messages):
	valid = True
	for route in solution["routes"]:
		tf = ready_time(instance, route["path"], route["t0"])
		expected_duration = tf - route["t0"]
		if tf == INFTY: 
			error_messages.append(F"\tInfeasible: {route}")
			valid = False
		elif not epsilon_equal(expected_duration, route["duration"]): 
			error_messages.append(F"\tDifferent duration: {route} - Expected: {expected_duration}")
			valid = False
	return valid

def main():
	ok = 0
	wrong = 0
	suboptimal = 0
	for output_file in output_files:
		output_file_json = json.loads(output_file.read())
		print(F"Checking {output_file.name}")

		for output in output_file_json["outputs"]:
			dataset_name = output["dataset_name"]
			instance_name = output["instance_name"]
			experiment_name = output["experiment_name"]

			instance = read_instance(dataset_name, instance_name)
			bks = best_known_solution(dataset_name, instance_name)

			opt_found = output["stdout"]["Exact"]["status"] == "Optimum" # Indicates if the optimum solution was found.
			if "Best solution" in output["stdout"]: 
				solution = output["stdout"]["Best solution"]
				errors = []
				valid = check_routes(instance, solution, errors)
				if not valid: 
					print(F"Checking {experiment_name} - {dataset_name} {instance_name}")
					for error in errors: print(red(error))
					wrong += 1
				elif opt_found and epsilon_bigger(solution["value"], bks["value"]):
					suboptimal += 1
					print(F"Checking {experiment_name} - {dataset_name} {instance_name}")
					print(blue(F"Suboptimal - BKS: {bks} - Obtained: {solution}"))
				else:
					ok += 1


	print(green(F"ok: {ok}"), red(F"wrong: {wrong}"), blue(F"suboptimal: {suboptimal}"))

if __name__== "__main__":
  main()
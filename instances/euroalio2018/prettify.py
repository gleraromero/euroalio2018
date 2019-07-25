import os, pprint, json, sys

pp = pprint.PrettyPrinter(indent=4, compact=True)
index = json.loads(open("index.json", "r").read())
for entry in index:
	instance = json.loads(open(entry["file_name"], "r").read())
	with open(entry["file_name"], "w") as f:
		f.write(pp.pprint(obj))
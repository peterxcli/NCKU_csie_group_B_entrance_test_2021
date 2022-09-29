import os
import subprocess
import json

with open("./SystemParameters.json", "r") as f:
    para = json.load(f)

""" compile consumer """
s = os.system("make")
assert s == 0

""" system """
p_judg = subprocess.Popen(["python3", ".judge.py"])

""" user defined """
p_cons = []
for _ in range(para["num_of_consumers"]):
    p = subprocess.Popen(["./Consumer"])
    p_cons.append(p)
p_coll = subprocess.Popen(["python3", "ResultCollector.py"])
p_prod = subprocess.Popen(["python3", "Producer.py"])

""" running """
while p_coll.poll() == None:
    continue

""" kill process """
for p in p_cons:
    p.kill()

p_judg.kill()
p_prod.kill()
p_coll.kill()

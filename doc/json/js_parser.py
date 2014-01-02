import json
data = json.load(open('data.txt'))
count = 1
for i in data:
	print "Shot No : ", count
	print i['total_distance']
	print i['total_time']
	print i['velocity']
	print i['angle_error']
	count += 1

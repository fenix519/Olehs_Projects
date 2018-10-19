import re

file = open('data.txt', encoding="utf8")

print (file.readline())
print (file.readline().split("\t"))

data=[]

for line in file :
   m = re.search('(\d+:\d+:\d+)|(\d+:\d+)', line.split("\t")[8] )
   if m.group(0) != None:
       if len(m.group(0).split(":")) == 2:
           value = float(m.group(0).split(":")[0]) + float(m.group(0).split(":")[1])/60
           #print(m.group(0).split(":"))
           #print(value)
           data.append(value)
       if len(m.group(0).split(":")) == 3:
           value = float(m.group(0).split(":")[0])*60 + float(m.group(0).split(":")[1]) + float(m.group(0).split(":")[2])/60
           print(m.group(0).split(":"))
           print(value)
           data.append(value)

#for i in data:
#    print i



file.close()
